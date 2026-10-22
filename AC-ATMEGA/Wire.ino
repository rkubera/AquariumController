// Decode atomically: never pass a partially allocated numeric value to a setter.
String wireText(const uint8_t* p,uint8_t n){
 String s;
 for(uint8_t i=0;i<n;i++)if(!p[i]||p[i]=='\r'||p[i]=='\n')return s;
 if(n && !s.reserve(n))return s;
 for(uint8_t i=0;i<n;i++)if(!s.concat(char(p[i]))){s.remove(0);break;}
 return s;
}
bool wirePacket(uint8_t kind,uint16_t id,const String& value,uint32_t operation){
 uint8_t header=kind==AcWire::Result?6:2;
 if(!id||value.length()>static_cast<unsigned int>(AcWire::Link::MaxMessage-header))return false;
 uint8_t p[AcWire::Link::MaxMessage];if(header==6)AcWire::put32(p,operation);AcWire::put16(p+header-2,id);memcpy(p+header,value.c_str(),value.length());
 if(linkWireQueue.push(kind,p,value.length()+header))return true;
 serialTxDroppedCommands++;linkSnapshotRetry=true;return false;
}
bool wireValue(uint16_t id,const String& value){return wirePacket(AcWire::Value,id,value,0);}
bool wireResult(uint32_t operation,uint16_t id,const String& value){return wirePacket(AcWire::Result,id,value,operation);}
bool wireRequest(uint8_t query){return linkWireQueue.push(AcWire::Request,&query,1);}
bool wireDiagnostic(const String& value){if(value.length()>AcWire::Link::MaxMessage)return false;return linkWireQueue.push(AcWire::Diagnostic,(const uint8_t*)value.c_str(),value.length());}
void megaWireConnection(bool connected){megaConfigSession(connected);if(connected){linkLocalPanel=true;hostnameReceived=false;}}
void megaWireReceive(uint8_t kind,const uint8_t* p,uint8_t n){
 if(kind>=AcWire::ConfigBegin&&kind<=AcWire::Capabilities){megaConfigMessage(kind,p,n);return;}
 if(kind==AcWire::Setting&&n>=6&&megaConfigReady){
  uint32_t operation=AcWire::u32(p);uint16_t field=AcWire::u16(p+4);
  if(!operation||!acFieldSupported(field,megaFeatures)||linkPanelField)return;
  String value=wireText(p+6,n-6);
  if(value.length()!=size_t(n-6))return;
#ifdef ARDUINO
  usbDiagnosticSetting(operation,field,value);
#endif
  linkLocalPanel=true;linkPanelId=operation;applySetting(field,value);linkPanelField=field;
  return;
 }
 if(kind==AcWire::Request&&n==1&&p[0]==AcWire::Snapshot&&megaConfigReady){linkRequestSnapshot();return;}
 if(kind!=AcWire::Info||n<1)return;
 String value=wireText(p+1,n-1);
 if(value.length()!=size_t(n-1))return;
 switch(p[0]){
 case AcWire::Timestamp:{uint32_t utc;if(linkParseUint32(value,utc))clockNTPSynchronize_cb(utc);else serialMalformedFrames++;break;}
 case AcWire::Wifi:if(value=="connected")eventWifiConnected();else eventWifiDisconnected();break;
 case AcWire::Broker:if(value=="connected")eventBrokerConnected();else eventBrokerDisconnected();break;
 case AcWire::Hostname:
  if(!value.length()||value.length()>32||value.indexOf('/')>=0||value.indexOf(' ')>=0||value.indexOf('+')>=0||value.indexOf('#')>=0)return;
  deviceName=value;eventHostnameReceived();break;
 case AcWire::Ssid:wifi_ssid=value;reportField(FieldId::Ssid,value,false);break;
 case AcWire::BrokerServer:brokerServer=value;eventBrokerServerReceived();break;
 }
}
