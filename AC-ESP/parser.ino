// Local adapters retain the existing business handlers; only typed binary
// messages and numeric field IDs cross the UART. No MQTT topic text on the wire.
uint16_t bridgeTxFree(){return bridgeWireQueue.free();}
bool bridgePush(uint8_t kind,const uint8_t* p,uint8_t n){if(bridgeWireQueue.push(kind,p,n))return true;bridgeDroppedFrames++;return false;}
// Allocation failure must discard the entire value, never accept a prefix.
String wireText(const uint8_t* p,uint8_t n){
 String s;for(uint8_t i=0;i<n;i++)if(!p[i]||p[i]=='\r'||p[i]=='\n')return s;
 if(n&&!s.reserve(n))return s;
 for(uint8_t i=0;i<n;i++)if(!s.concat(char(p[i]))){s.remove(0);break;}
 return s;
}
bool bridgeQueueFrame(String message){
 uint8_t p[AcWire::Link::MaxMessage];
 if(message=="[panelping]")return wireLink.connected();
 if(message=="[panelrefresh]"){p[0]=AcWire::Snapshot;return bridgePush(AcWire::Request,p,1);}
 if(message.startsWith("[hostname ")&&message.endsWith("]")){String name=message.substring(10,message.length()-1);p[0]=AcWire::Hostname;uint8_t n=name.length();if(n>32)return false;memcpy(p+1,name.c_str(),n);return bridgePush(AcWire::Info,p,n+1);}

 return false;
}
bool bridgeQueueSetting(uint32_t operation,uint16_t field,const String& value){
 if(!operation||!field||value.length()>AcWire::Link::MaxMessage-6)return false;
 uint8_t p[AcWire::Link::MaxMessage];AcWire::put32(p,operation);AcWire::put16(p+4,field);
 memcpy(p+6,value.c_str(),value.length());return bridgePush(AcWire::Setting,p,value.length()+6);
}
void sendCommand(String command){
 int at=command.indexOf(' ');String name=at<0?command:command.substring(0,at),value=at<0?"":command.substring(at+1);uint8_t kind=name=="timestamp"?AcWire::Timestamp:name=="wifi"?AcWire::Wifi:name=="mqtt"?AcWire::Broker:name=="hostname"?AcWire::Hostname:name=="ssid"?AcWire::Ssid:name=="mqttserver"?AcWire::BrokerServer:name=="echo"?AcWire::Ping:0;if(!kind)return;
 uint8_t p[AcWire::Link::MaxMessage];if(value.length()>AcWire::Link::MaxMessage-1)return;p[0]=kind;memcpy(p+1,value.c_str(),value.length());bridgePush(AcWire::Info,p,value.length()+1);
}
void espWireConnection(bool connected){bridgeWireQueue.clear();espConfigSession(connected);bridgeLastHostname="";bridgeWifiWasConnected=!portal.wifiConnected();bridgeMqttWasConnected=!portal.mqttConnected();}
void espWireReceive(uint8_t kind,const uint8_t* p,uint8_t n){
 if(kind>=AcWire::ConfigBegin&&kind<=AcWire::ConfigReady){espConfigMessage(kind,p,n);return;}
 if(kind==AcWire::Diagnostic){AC_PORTAL_LOG("Mega: "+wireText(p,n));return;}
 if(kind==AcWire::Request&&n==1){const char* request=p[0]==AcWire::Timestamp?"timestamp":p[0]==AcWire::Wifi?"wifistatus":p[0]==AcWire::Broker?"mqttstatus":p[0]==AcWire::Hostname?"hostname":p[0]==AcWire::Ssid?"ssid":p[0]==AcWire::BrokerServer?"mqttserver":p[0]==AcWire::Ping?"echo":nullptr;if(request)getCommand(request);return;}
 if(kind==AcWire::Result&&n>=6){String actual=wireText(p+6,n-6);if(actual.length()!=size_t(n-6))return;uint32_t operation=AcWire::u32(p);uint16_t field=AcWire::u16(p+4);if(!espConfigReply(operation,field,actual))panelReplyValue(operation,field,actual);return;}
 if(kind==AcWire::Value&&n>=2&&configMegaReady){uint16_t id=AcWire::u16(p);if(!acFieldSupported(id,portalFeatures))return;String value=wireText(p+2,n-2);if(value.length()!=size_t(n-2))return;panelReceiveWireValue(id,value);if(wirePanelIndex(id)>=PANEL_FIELD_COUNT)bridgeTelemetry[id-1]=value;bridgeTelemetryDirty[id-1]=true;return;}
}
void bridgeSerialService(){static bool driving=false;if(driving)return;driving=true;byte budget=64;while(budget--&&megaSerial.available()>0){bridgeRxBytes++;wireLink.input((uint8_t)megaSerial.read(),millis());}wireLink.service(megaSerial,millis());driving=false;}
