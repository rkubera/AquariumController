uint32_t megaFeatureDigest(){uint8_t p[5];megaFeatures.encode(p);return AcWire::crc(p,5);}
// Native ArdPortal fields are the only persistent configuration. This is a
// RAM-only boot transaction; no image is uploaded or saved by the controller.
bool megaConfigReady=false,megaReceivingConfig=false,megaConfigRequestPending=true,megaConfigReadyPending=false;
uint16_t megaConfigReceived=0;
uint32_t megaConfigToken=0,megaConfigDigest=0,megaConfigReceiveAt=0,megaConfigLastRequest=0;
void megaFeatureInterlock(){
 megaConfigReady=false;megaReceivingConfig=false;linkPanelField=0;linkWireQueue.clear();
 relaysInit();pwmOutputsInit();initFan();ledInit();buzzerDisable();dhtDisable();rtcDisable();
}
void megaConfigSession(bool connected){
 linkWireQueue.clear();linkPanelField=0;megaReceivingConfig=false;
 megaConfigRequestPending=!megaConfigReady;megaConfigReadyPending=connected&&megaConfigReady;
 if(connected){megaConfigLastRequest=millis()-2000UL;linkSnapshotRetry=true;}
 else {wifiStatus=WIFI_STATUS_DISCONNECTED;brokerStatus=BROKER_STATUS_DISCONNECTED;}
}
void megaConfigActivate(){
 configLoad();
 if(megaFeatures.has(ControllerFeatures::Dht))dhtInit();
 if(megaFeatures.has(ControllerFeatures::Rtc))rtcInit();
 sensorsResetAcquisition();
 // Pin initialization is performed while output interlocks are still closed.
 relaysInit();pwmOutputsInit();clockUpdateTimezoneRules();clockInvalidateLocalTime();clockGetLocalTime();
 megaConfigReady=true;megaConfigRequestPending=false;megaConfigReadyPending=true;
 errorReport(true);buzzerInit();linkLocalPanel=true;linkRequestSnapshot();
}
void megaConfigMessage(uint8_t kind,const uint8_t* p,uint8_t n){
 if(kind==AcWire::Capabilities){ControllerFeatures next;if(!ControllerFeatures::decode(p,n,next))return;
  if(megaFeaturesKnown&&megaFeatures.same(next)&&megaConfigReady){megaConfigReadyPending=true;return;}
  megaFeatureInterlock();megaFeatures=next;megaFeaturesKnown=true;
  megaConfigRequestPending=true;megaConfigReadyPending=false;megaConfigLastRequest=millis()-2000UL;return;
 }
 if(kind==AcWire::ConfigBegin){
  if(!megaFeaturesKnown||megaConfigReady||n!=10||!AcWire::u32(p)||AcWire::u16(p+4)!=acActiveRestoreCount(megaFeatures)||AcWire::u32(p+6)!=AC_CONFIG_SCHEMA)return;
  configStorage.defaults();configLoad();
  // Fan settings are restored full-speed first, then start. Allow low thresholds.
  fanStartTemperature=0;configSaveValue(byte(0),CONFIG_fanStartTemperature_addr);
  megaConfigToken=AcWire::u32(p);megaConfigDigest=megaFeatureDigest();megaConfigReceived=0;
  megaReceivingConfig=true;megaConfigRequestPending=false;megaConfigReceiveAt=millis();return;
 }
 if(kind==AcWire::ConfigChunk){
  if(!megaReceivingConfig||n<4||linkPanelField||AcWire::u16(p)!=megaConfigReceived||AcWire::u16(p+2)!=acActiveRestoreId(megaConfigReceived,megaFeatures))return;
  uint16_t field=AcWire::u16(p+2);String value=wireText(p+4,n-4);
  if(value.length()!=size_t(n-4))return;
  megaConfigDigest=AcWire::configDigest(megaConfigDigest,p,n);++megaConfigReceived;megaConfigReceiveAt=millis();
  linkLocalPanel=true;linkPanelId=UINT32_MAX;applySetting(field,value);linkPanelField=field;return;
 }
 if(kind==AcWire::ConfigCommit){
  if(!megaReceivingConfig||n!=8||linkPanelField||AcWire::u32(p)!=megaConfigToken||AcWire::u32(p+4)!=megaConfigDigest||megaConfigReceived!=acActiveRestoreCount(megaFeatures))return;
  megaReceivingConfig=false;megaConfigActivate();return;
 }
}
void configRestoreService(){
 if(megaReceivingConfig&&millis()-megaConfigReceiveAt>5000UL){megaReceivingConfig=false;megaConfigRequestPending=true;}
 if(!wireLink.connected()||!wireLink.idle()||linkPanelField)return;
 if(megaConfigReadyPending){uint8_t p[12];AcWire::put32(p,megaConfigToken);AcWire::put32(p+4,megaConfigDigest);AcWire::put32(p+8,megaFeatureDigest());if(wireLink.send(AcWire::ConfigReady,p,12))megaConfigReadyPending=false;return;}
 if(megaConfigRequestPending&&millis()-megaConfigLastRequest>=2000UL){uint8_t p=0;if(wireLink.send(AcWire::ConfigRequest,&p,1))megaConfigLastRequest=millis();}
}
