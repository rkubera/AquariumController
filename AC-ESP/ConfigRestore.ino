uint32_t espFeatureDigest(){uint8_t p[5];portalFeatures.encode(p);return AcWire::crc(p,5);}
bool configFeaturesSent=false;
// Restore the native fields loaded by ArdPortal from ArdFS. No second store,
// hidden keys, binary configuration image, or independent filesystem owner.
bool configMegaReady=false,configRestoreWanted=false,configRestoreWaiting=false;
bool configRestoreRestart=false;
byte configRestoreStep=0;
void espConfigEdited(){if(configRestoreWanted)configRestoreRestart=true;}
uint16_t configRestoreIndex=0,configRestoreField=0;
uint32_t configRestoreToken=0,configRestoreDigest=0,configRestoreAt=0;
String configRestoreValue;
void espConfigSetup(){AC_PORTAL_LOG(F("Mega config: native ArdPortal fields in ArdFS"));}
void espConfigSession(bool connected){
 configFeaturesSent=false;configMegaReady=false;configRestoreWanted=false;configRestoreWaiting=false;configRestoreStep=0;
 panelLinked=false;panelClearCache();panelRefreshWanted=true;
 if(panelPendingField.length()){AC_PORTAL_LOG(F("Panel: link changed; checking setting after synchronization"));panelPendingField="";panelPendingValue="";panelPendingIndex=UINT16_MAX;}
 AC_PORTAL_LOG(connected?"UART v2: session established":"UART v2: disconnected; Mega continues its last configuration");
}
void espConfigMessage(uint8_t kind,const uint8_t* p,uint8_t n){
 if(kind==AcWire::ConfigRequest&&n==1&&p[0]==0&&configFeaturesSent){
  if(!configRestoreWanted){configRestoreWanted=true;configRestoreWaiting=false;configRestoreStep=0;configRestoreIndex=0;configRestoreDigest=espFeatureDigest();if(++configRestoreToken==0)++configRestoreToken;}
  return;
 }
 if(kind==AcWire::ConfigReady&&n==12&&configFeaturesSent&&AcWire::u32(p+8)==espFeatureDigest()){
  // A running Mega announces readiness after an ESP-only restart, keeping RAM.
  if(configRestoreWanted&&(configRestoreStep!=3||AcWire::u32(p)!=configRestoreToken||AcWire::u32(p+4)!=configRestoreDigest))return;
  configMegaReady=true;configRestoreWanted=false;panelLinked=true;panelLastRx=millis();panelRefreshWanted=true;
  AC_PORTAL_LOG(F("Mega config: native fields applied; outputs enabled"));
 }
}
bool espConfigReply(uint32_t operation,uint16_t field,const String& actual){
 if(operation!=UINT32_MAX)return false;
 if(!configRestoreWanted||!configRestoreWaiting||field!=configRestoreField)return true;
 if(actual=="unsupported"){AC_PORTAL_LOG("Mega config: unsupported native field "+acFieldName(field));configRestoreWanted=false;return true;}
 if(!panelValuesEqual(actual,configRestoreValue))AC_PORTAL_LOG("Mega config: validated "+acFieldName(field)+" = "+actual);
 configRestoreWaiting=false;++configRestoreIndex;return true;
}
void espConfigService(){
 if(!wireLink.connected())return;
 // Registration supplies validation/defaults. Never restore a partial panel.
 if(!portal.portalAndAppConfigReady()||!panelAllPagesReady())return;
 if(!configFeaturesSent){if(!wireLink.idle()||bridgeWireQueue.free()!=512)return;uint8_t p[5];portalFeatures.encode(p);if(wireLink.send(AcWire::Capabilities,p,5)){configFeaturesSent=true;AC_PORTAL_LOG(F("Mega: capabilities sent"));}return;}
 if(!configRestoreWanted)return;
 if(configRestoreWaiting){
  if(millis()-configRestoreAt>5000UL){AC_PORTAL_LOG(F("Mega config: readback timeout; restarting restore"));configRestoreWaiting=false;configRestoreStep=0;configRestoreIndex=0;configRestoreDigest=espFeatureDigest();if(++configRestoreToken==0)++configRestoreToken;}
  return;
 }
 if(!wireLink.idle()||bridgeWireQueue.free()!=512)return;
 if(configRestoreRestart){configRestoreRestart=false;configRestoreStep=0;configRestoreIndex=0;configRestoreDigest=espFeatureDigest();if(++configRestoreToken==0)++configRestoreToken;}
 uint8_t p[AcWire::Link::MaxMessage];
 if(configRestoreStep==0){AcWire::put32(p,configRestoreToken);AcWire::put16(p+4,acActiveRestoreCount(portalFeatures));AcWire::put32(p+6,AC_CONFIG_SCHEMA);if(wireLink.send(AcWire::ConfigBegin,p,10)){configRestoreStep=1;AC_PORTAL_LOG(String(F("Mega config: restoring "))+String(acActiveRestoreCount(portalFeatures))+" native fields");}return;}
 if(configRestoreStep==1){
  if(configRestoreIndex==acActiveRestoreCount(portalFeatures)){configRestoreStep=2;return;}
  configRestoreField=acActiveRestoreId(configRestoreIndex,portalFeatures);uint16_t i=wirePanelIndex(configRestoreField);
  bool led=configRestoreField==acFieldId("CustomColor")||configRestoreField==acFieldId("CurrentBrightness");
  auto value=portal.getAppConfigValue((led?panelLedId():panelControlIdAt(i)).c_str());
  if(value.isUndefined()){AC_PORTAL_LOG("Mega config: missing field "+acFieldName(configRestoreField));return;}
  configRestoreValue=led?panelLedRestoreRaw(configRestoreField):panelNativeRaw(value);
  if(configRestoreValue.length()>AcWire::Link::MaxMessage-4){AC_PORTAL_LOG("Mega config: oversized field "+acFieldName(configRestoreField));return;}
  AcWire::put16(p,configRestoreIndex);AcWire::put16(p+2,configRestoreField);memcpy(p+4,configRestoreValue.c_str(),configRestoreValue.length());
  uint8_t n=configRestoreValue.length()+4;
  if(wireLink.send(AcWire::ConfigChunk,p,n)){configRestoreDigest=AcWire::configDigest(configRestoreDigest,p,n);configRestoreWaiting=true;configRestoreAt=millis();}return;
 }
 if(configRestoreStep==2){AcWire::put32(p,configRestoreToken);AcWire::put32(p+4,configRestoreDigest);if(wireLink.send(AcWire::ConfigCommit,p,8)){configRestoreStep=3;configRestoreAt=millis();}return;}
 if(configRestoreStep==3&&millis()-configRestoreAt>5000UL){configRestoreStep=2;}
}
