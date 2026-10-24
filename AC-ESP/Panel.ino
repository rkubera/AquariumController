//One scalar field per Mega parameter, shared by the Portal and HA.
#include <utility>
#include "PanelSchema.h"
#include "PanelMetadata.h"
#include "PanelWireIds.h"
std::bitset<PANEL_FIELD_COUNT> panelSeen,panelNeedsRender;
bool panelUpdating=false,panelReady=false;
byte panelRegisteredPages=0;
byte panelRegisteringPage=255;
uint16_t panelRegisteredMask=0;
static const char* panelPageNames[]={"led","relays","schedule","inputs","pwm","clock","fan","buzzer","diagnostics","dht","peripherals"};
static const byte PANEL_PAGE_COUNT=sizeof(panelPageNames)/sizeof(panelPageNames[0]);
static const uint16_t PANEL_PAGE_MASK=(1<<0)|(1<<2)|(1<<5)|(AC_ACTIVE_RELAYS?1<<1:0)|(AC_ACTIVE_INPUTS?1<<3:0)|(AC_ACTIVE_PWM?1<<4:0)|(AC_ACTIVE_FAN?1<<6:0)|(AC_ENABLE_BUZZER?1<<7:0)|(AC_ACTIVE_DIAGNOSTICS?1<<8:0)|(AC_ENABLE_DHT&&!AC_ACTIVE_FAN?1<<9:0)|(AC_ACTIVE_PERIPHERALS?1<<10:0);
static const byte PANEL_ACTIVE_PAGE_COUNT=3+(AC_ACTIVE_RELAYS>0)+(AC_ACTIVE_INPUTS>0)+(AC_ACTIVE_PWM>0)+AC_ACTIVE_FAN+AC_ENABLE_BUZZER+bool(AC_ACTIVE_DIAGNOSTICS)+(AC_ENABLE_DHT&&!AC_ACTIVE_FAN)+bool(AC_ACTIVE_PERIPHERALS);
bool panelFieldEnabled(uint16_t i){return i<PANEL_FIELD_COUNT&&acFieldSupported(panelWireId(i),portalFeatures);}
uint16_t panelActiveFieldCount(){uint16_t count=0;for(uint16_t i=0;i<PANEL_FIELD_COUNT;++i)if(panelFieldEnabled(i)){String key=panelFieldProperty(i);if(key!=F("LedState")&&key!=F("CurrentBrightness")&&key!=F("ClockRtcStatus"))++count;}return count;}
String panelRegistrationErrors[PANEL_PAGE_COUNT];
uint32_t panelLastRx=0,panelRequestAt=0,panelId=0;
bool panelLinked=false,panelRefreshWanted=true;
uint16_t panelPendingIndex=UINT16_MAX;
String panelPendingActual;
static const byte PANEL_SETTING_QUEUE_SIZE=8;
uint16_t panelQueuedIndex[PANEL_SETTING_QUEUE_SIZE];
String panelQueuedValue[PANEL_SETTING_QUEUE_SIZE],panelQueuedActual[PANEL_SETTING_QUEUE_SIZE];
byte panelQueuedCount=0;
bool panelDrainingSetting=false;

// Reconcile validated settings into the same native fields, never a second record.
// Only these two settings are durable; the light itself is runtime-only.
uint32_t panelLedSavedColor=0xFFFFFF;
uint8_t panelLedSavedBrightness=0;
bool panelLedSettingsLoaded=false,panelLedSettingsDirty=false,panelLedBrightnessManualWanted=false;
std::bitset<PANEL_FIELD_COUNT> panelSaveConfirmed;
bool panelPersistenceBatch=false;
bool panelAllPagesReady(){return panelRegisteredMask==PANEL_PAGE_MASK;}
String panelNativeRaw(const ArdJSON::JSONVar& value){
 if(value.type()==ArdJSON::JSONVar::Type::Boolean)return value.asBool()?"on":"off";
 return value.type()==ArdJSON::JSONVar::Type::String?value.asString():ArdJSON::JSON.stringify(value);
}
bool panelValuesEqual(const String& actual,const String& wanted){
 if(actual.equalsIgnoreCase(wanted))return true;
 char *a=nullptr,*b=nullptr;double av=strtod(actual.c_str(),&a),bv=strtod(wanted.c_str(),&b);
 return actual.length()&&wanted.length()&&a&&b&&!*a&&!*b&&isfinite(av)&&isfinite(bv)&&fabs(av-bv)<=0.00001*fmax(1.0,fabs(bv));
}
ArdJSON::JSONVar panelNativeValue(uint16_t i,const String& raw){
 auto current=portal.getAppConfigValue(panelControlIdAt(i).c_str());
 if(current.type()==ArdJSON::JSONVar::Type::Boolean)return ArdJSON::JSONVar(raw==F("on"));
 if(current.type()==ArdJSON::JSONVar::Type::Number)return ArdJSON::JSONVar(raw.toFloat());
 return ArdJSON::JSONVar(raw);
}
void panelConfirmSetting(uint16_t i,const String& raw,bool force=false){
 if(i<PANEL_FIELD_COUNT && (panelFieldProperty(i)==F("CurrentBrightness")||panelFieldProperty(i)==F("CustomColor"))){panelLedConfirm(i,raw);return;}
 if(i>=PANEL_FIELD_COUNT||!panelFieldPersistent(i)||!configMegaReady||!(panelRegisteredMask&(uint16_t(1)<<panelFieldPage(i))))return;
 if(force||!panelValuesEqual(raw,panelNativeRaw(portal.getAppConfigValue(panelControlIdAt(i).c_str()))))panelSaveConfirmed[i]=true;
}
void panelPersistenceService(){
 // Wait for native HTTP/MQTT persistence; then merge confirmed corrections into
 // the same pending application save. Never serialize a separate configuration.
 if(panelSaveConfirmed.none())return;
 if(!configMegaReady||(!panelPersistenceBatch&&portal.portalAndAppConfigBusy())||panelPendingField.length()||panelQueuedCount)return;
 uint32_t started=millis();byte applied=0;
 for(uint16_t i=0;i<PANEL_FIELD_COUNT;i++)if(panelSaveConfirmed[i]&&panelSeen[i]){
  panelUpdating=true;bool ok=portal.setAppConfigValue(panelControlIdAt(i).c_str(),panelNativeValue(i,panelValue(i)));panelUpdating=false;
  if(!ok)return;
  panelPersistenceBatch=true;panelSaveConfirmed[i]=false;
  bridgeSerialService();if(++applied>=8||millis()-started>=4)return;
 }
}

int panelQueuedSlot(uint16_t i){
  for(byte slot=0;slot<panelQueuedCount;slot++)if(panelQueuedIndex[slot]==i)return slot;
  return -1;
}

String panelPendingField,panelPendingValue,panelOperation="Waiting for Mega",panelDefinitionHostname;

String panelControlIdAt(uint16_t i){
 char id[11];snprintf(id,sizeof(id),"M_%08lx",static_cast<unsigned long>(panelControlHash(i)));return String(id);
}

void panelClearCache() {
  for(byte slot=0;slot<panelQueuedCount;slot++){panelQueuedValue[slot]="";panelQueuedActual[slot]="";}
  panelQueuedCount=0;
  for (uint16_t i=0;i<PANEL_FIELD_COUNT;i++) { panelSeen[i]=false; panelNeedsRender[i]=false; }
}
String panelValue(uint16_t i) { return panelSeen[i] ? bridgeTelemetry[panelWireId(i)-1] : String(F("?")); }
void panelStore(uint16_t i,const String& value) {
  if (!panelSeen[i] || bridgeTelemetry[panelWireId(i)-1]!=value) panelNeedsRender[i]=true;
  bridgeTelemetry[panelWireId(i)-1]=value; panelSeen[i]=true;
}
String panelCached(const String& key) {
  uint16_t i=wirePanelIndex(acFieldId(key));
  if(i<PANEL_FIELD_COUNT)return panelValue(i);
  return "";
}
uint32_t panelLargestHeapBlock() { return ArdHeap::sample().block8; }
String panelMemoryDiagnostic() {
  const auto memory=ArdHeap::sample();
#if defined(ESP32)
  return String(F(" heap8="))+String(memory.free8)+String(F(" dma="))+String(memory.freeDma)+String(F(" block8="))+String(memory.block8);
#else
  return String(F(" heap="))+String(memory.free8)+String(F(" block="))+String(memory.block8);
#endif
}
void panelState(const char* id,const ArdJSON::JSONVar& value) {
  panelUpdating=true;
  if (!portal.setAppConfigStateValue(id,value,false)) AC_PORTAL_LOG(String(F("Panel: state rejected "))+String(id));
  panelUpdating=false;
}
// UART keeps scalar values; the shared native light combines them without
// introducing duplicate configuration fields or additional HA entities.
bool panelLedComponent(uint16_t i){String key=panelFieldProperty(i);return key==F("CustomColor")||key==F("LedState")||key==F("CurrentBrightness");}
String panelLedId(){return panelControlIdAt(wirePanelIndex(acFieldId("CustomColor")));}
ArdJSON::JSONVar panelLedValue(){
 auto value=portal.getAppConfigValue(panelLedId().c_str());
 if(value.type()!=ArdJSON::JSONVar::Type::Object)value=ArdJSON::JSONVar::object();
 String state=panelCached("LedState"),brightness=panelCached("CurrentBrightness"),color=panelCached("CustomColor");
 if(state!="?"&&state.length())value["state"]=state=="on"?"ON":"OFF";
 if(brightness!="?"&&brightness.length())value["brightness"]=brightness.toInt();
 if(color!="?"&&color.length()){uint32_t rgb=strtoul(color.c_str(),nullptr,10);value["color"]["r"]=int((rgb>>16)&255);value["color"]["g"]=int((rgb>>8)&255);value["color"]["b"]=int(rgb&255);}
 value["color_mode"]="rgb";return value;
}
void panelLedLoadSettings(){
 if(panelLedSettingsLoaded)return;
 auto saved=portal.getAppConfigValue("LedSettings");
 if(saved.type()==ArdJSON::JSONVar::Type::Object){
  auto color=saved["color"],brightness=saved["brightness"];
  double c=color.asDouble(),v=brightness.asDouble();
  if(color.type()==ArdJSON::JSONVar::Type::Number&&isfinite(c)&&c>=0&&c<=16777215&&floor(c)==c)panelLedSavedColor=uint32_t(c);
  if(brightness.type()==ArdJSON::JSONVar::Type::Number&&isfinite(v)&&v>=0&&v<=100&&floor(v)==v)panelLedSavedBrightness=uint8_t(v);
 }else{
  // Before registering the light, existing native scalar values remain readable.
  // Seed only these two settings; no second controller configuration is copied.
  auto color=portal.getAppConfigValue(panelLedId().c_str());
  if(color.type()==ArdJSON::JSONVar::Type::String||color.type()==ArdJSON::JSONVar::Type::Number){
   String raw=panelNativeRaw(color);char* end=nullptr;double n=strtod(raw.c_str(),&end);
   if(raw.length()&&end&&!*end&&isfinite(n)&&n>=0&&n<=16777215&&floor(n)==n){panelLedSavedColor=uint32_t(n);panelLedSettingsDirty=true;}
  }
  auto brightness=portal.getAppConfigValue("M_fbd624a8");double n=brightness.asDouble();
  if(brightness.type()==ArdJSON::JSONVar::Type::Number&&isfinite(n)&&n>=0&&n<=100&&floor(n)==n){panelLedSavedBrightness=uint8_t(n);panelLedSettingsDirty=true;}
 }
 panelLedSettingsLoaded=true;
}
String panelLedRestoreRaw(uint16_t id){
 panelLedLoadSettings();return id==acFieldId("CurrentBrightness")?String(panelLedSavedBrightness):String(panelLedSavedColor);
}
void panelLedConfirm(uint16_t i,const String& raw){
 if(!configMegaReady||!panelLedSettingsLoaded)return;
 if(panelFieldProperty(i)==F("CustomColor")){
  uint32_t color=strtoul(raw.c_str(),nullptr,10);if(color<=16777215&&color!=panelLedSavedColor){panelLedSavedColor=color;panelLedSettingsDirty=true;}
 }else if(panelFieldProperty(i)==F("CurrentBrightness")&&((i==panelPendingIndex)?panelLedBrightnessManualWanted:panelCached("LedControlMode")==F("manual"))){
  long brightness=raw.toInt();if(brightness>=0&&brightness<=100&&brightness!=panelLedSavedBrightness){panelLedSavedBrightness=brightness;panelLedSettingsDirty=true;}
 }
}
void panelLedPersistenceService(){
 if(!panelLedSettingsDirty||!configMegaReady||panelPendingField.length()||panelQueuedCount||portal.portalAndAppConfigBusy())return;
 auto saved=ArdJSON::JSONVar::object();saved["color"]=double(panelLedSavedColor);saved["brightness"]=int(panelLedSavedBrightness);
 if(portal.setAppConfigValue("LedSettings",saved))panelLedSettingsDirty=false;
}
void panelRenderLed(){if(!(panelRegisteredMask&1))return;panelState(panelLedId().c_str(),panelLedValue());}
bool panelSendLed(const ArdJSON::JSONVar& value){
 if(value.type()!=ArdJSON::JSONVar::Type::Object)return false;
 auto color=value["color"],brightness=value["brightness"],state=value["state"];
 if(!brightness.isUndefined()&&(brightness.type()!=ArdJSON::JSONVar::Type::Number||!isfinite(brightness.asDouble())||brightness.asDouble()<0||brightness.asDouble()>100||floor(brightness.asDouble())!=brightness.asDouble()))return false;
 if(!state.isUndefined()&&(state.type()!=ArdJSON::JSONVar::Type::String||(!state.asString().equalsIgnoreCase("ON")&&!state.asString().equalsIgnoreCase("OFF"))))return false;
 if(!color.isUndefined()){
  if(color.type()!=ArdJSON::JSONVar::Type::Object)return false;
  const char* channels[]={"r","g","b"};for(const char* channel:channels){auto v=color[channel];double n=v.asDouble();if(v.type()!=ArdJSON::JSONVar::Type::Number||!isfinite(n)||n<0||n>255||floor(n)!=n)return false;}
 }
 if(!color.isUndefined()){
  uint32_t rgb=(uint32_t(color["r"].asDouble())<<16)|(uint32_t(color["g"].asDouble())<<8)|uint32_t(color["b"].asDouble());
  uint16_t i=wirePanelIndex(acFieldId("CustomColor"));if(!panelValuesEqual(panelValue(i),String(rgb)))panelSendSetting(i,String(rgb));
 }
 if(!brightness.isUndefined()){
  uint16_t i=wirePanelIndex(acFieldId("CurrentBrightness"));String raw(int(brightness.asDouble()));
  bool changed=!panelValuesEqual(panelValue(i),raw);
  // Full light updates include the previous power state: a brightness edit wins.
  if(changed||state.isUndefined())state=brightness.asDouble()>0?"ON":"OFF";
  if(changed)panelSendSetting(i,raw);
 }
 if(!state.isUndefined()){uint16_t i=wirePanelIndex(acFieldId("LedState"));String raw=state.asString().equalsIgnoreCase("ON")?"on":"off";if(panelValue(i)!=raw)panelSendSetting(i,raw);}
 return true;
}
void panelRenderField(uint16_t i) {
  if (!panelFieldEnabled(i) || !panelSeen[i] || !(panelRegisteredMask&(uint16_t(1)<<panelFieldPage(i)))) return;
  if(panelFieldProperty(i)==F("ClockRtcStatus"))return;
  if(panelLedComponent(i)){panelRenderLed();return;}
  String id=panelControlIdAt(i);
  auto definition=portal.getAppConfigValue(id.c_str());
  if (definition.isUndefined()) return;
  String raw=panelValue(i);
  // Native configuration already supplies unchanged settings. Avoid allocating
  // a second runtime JSON member for every field in the initial snapshot.
  String current=panelNativeRaw(definition);
  if (definition.type()==ArdJSON::JSONVar::Type::Number ? panelValuesEqual(raw,current) : raw==current) return;
  if (definition.type()==ArdJSON::JSONVar::Type::Boolean) panelState(id.c_str(),ArdJSON::JSONVar(raw==F("on")));
  else if (definition.type()==ArdJSON::JSONVar::Type::Number) panelState(id.c_str(),ArdJSON::JSONVar(raw.toFloat()));
  else panelState(id.c_str(),ArdJSON::JSONVar(raw));
}
void panelReceiveWireValue(uint16_t id,const String& value){
 uint16_t i=wirePanelIndex(id);
 if(!panelFieldEnabled(i))return;
 static const uint16_t rtcId=acFieldId("ClockRtcStatus");
 if(id==rtcId&&panelValue(i)!=value)AC_PORTAL_LOG("RTC status: "+value);
 panelLastRx=millis();panelLinked=true;
 if(i==panelPendingIndex && panelPendingField.length()){panelPendingActual=value;return;}
 int queued=panelQueuedSlot(i);
 if(queued>=0){panelQueuedActual[queued]=value;return;}
 panelConfirmSetting(i,value);panelStore(i,value);
}
void panelReceiveValue(const String& topic,const String& value){
 String prefix="stat/"+bridgeHostname()+"/";
 if(topic.startsWith(prefix))panelReceiveWireValue(acFieldId(topic.substring(prefix.length())),value);
}
// ACK fields are already decoded by UART: do not rebuild/parse a text frame.
void panelReplyValue(uint32_t operation,uint16_t wireId,const String& actual){
 if(operation!=panelId||!panelPendingField.length()||panelPendingIndex>=PANEL_FIELD_COUNT||panelWireId(panelPendingIndex)!=wireId)return;
 String field=panelPendingField;uint16_t acknowledged=panelPendingIndex;
  if(panelQueuedSlot(acknowledged)<0)panelConfirmSetting(acknowledged,actual,true);
  panelPendingIndex=UINT16_MAX;
  panelReceiveWireValue(wireId,actual);
  if(wireId)bridgeTelemetryDirty[wireId-1]=true;
  uint16_t confirmed=acknowledged;
  if(confirmed<PANEL_FIELD_COUNT){panelNeedsRender[confirmed]=false;panelRenderField(confirmed);}
  bool same=actual.equalsIgnoreCase(panelPendingValue);
  char *a=nullptr,*b=nullptr;
  double av=strtod(actual.c_str(),&a),bv=strtod(panelPendingValue.c_str(),&b);
  if (actual.length() && panelPendingValue.length() && a && b && !*a && !*b && isfinite(av) && isfinite(bv)) same=fabs(av-bv)<=0.00001*fmax(1.0,fabs(bv));
  panelOperation=(same ? "Mega confirmed: " : "Mega returned a different value: ")+field+" = "+actual;
  AC_PORTAL_LOG(panelOperation);
  panelPendingField=""; panelPendingValue="";
}
// Compatibility adapter for local callers/tests; UART uses panelReplyValue.
void panelReply(String payload){
 int first=payload.indexOf(' '),second=payload.indexOf(' ',first+1);
 if(first<1||second<0||payload.substring(0,first)!=String(panelId))return;
 panelReplyValue(panelId,acFieldId(payload.substring(first+1,second)),payload.substring(second+1));
}
void panelRollbackPending(){
  if(panelPendingIndex<PANEL_FIELD_COUNT){
    uint16_t i=panelPendingIndex;
    int queued=panelQueuedSlot(i);
    if(queued>=0)panelQueuedActual[queued]=panelPendingActual;
    else {panelConfirmSetting(i,panelPendingActual,true);panelStore(i,panelPendingActual);panelRenderField(i);}
    bridgeTelemetryDirty[panelWireId(i)-1]=true;
  }
  panelPendingIndex=UINT16_MAX;panelPendingActual="";
}

bool panelSendSetting(uint16_t i,String value) {
  if(i>=PANEL_FIELD_COUNT || !panelFieldEnabled(i))return false;
  String field=panelCommandField(i),property=panelFieldProperty(i);
  if (!field.length()) return false;
  if(property==F("Inverted") || property==F("Pullup") || property==F("State") || property==F("LedState") || property==F("BuzzerMorning") || property==F("BuzzerAfternoon") || property==F("BuzzerEvening") || property==F("BuzzerNight") || (panelField(i).group==1 && (property==F("MorningMode") || property==F("AfternoonMode") || property==F("EveningMode") || property==F("NightMode")))){
    value.trim();value.toLowerCase();if(value==F("true"))value="on";else if(value==F("false"))value="off";
    if(value!=F("on")&&value!=F("off"))return false;
  }
  if (!configMegaReady || !panelSeen[i] || !panelLinked || millis()-panelLastRx>20000UL) {
    panelOperation="No current data from Mega"; AC_PORTAL_LOG(panelOperation); panelRenderField(i);bridgeTelemetryDirty[panelWireId(i)-1]=true;return true;
  }
  if ((property==F("Name") && value.length()>15) || value.length()>AcWire::Link::MaxMessage-6 || !value.length() || value.indexOf(']')>=0 || value.indexOf('\r')>=0 || value.indexOf('\n')>=0) {
    AC_PORTAL_LOG("Panel: rejected value for "+field);panelRenderField(i);bridgeTelemetryDirty[panelWireId(i)-1]=true;return true;
  }
  if(panelPendingField.length() || (panelQueuedCount && !panelDrainingSetting)){
    int slot=panelQueuedSlot(i);
    if(slot<0){
      if(panelQueuedCount>=PANEL_SETTING_QUEUE_SIZE){
        AC_PORTAL_LOG("Panel: setting queue full for "+field);panelRenderField(i);bridgeTelemetryDirty[panelWireId(i)-1]=true;return true;
      }
      slot=panelQueuedCount++;panelQueuedIndex[slot]=i;
      panelQueuedActual[slot]=(i==panelPendingIndex)?panelPendingActual:panelValue(i);
    }
    if(property==F("CurrentBrightness")&&!panelDrainingSetting)panelLedBrightnessManualWanted=panelCached("LedControlMode")==F("manual");
    panelQueuedValue[slot]=value;
    panelStore(i,value);panelRenderField(i);bridgeTelemetryDirty[panelWireId(i)-1]=true;
    return true;
  }
  if (++panelId==0 || panelId==UINT32_MAX) panelId=1;
  if (!bridgeQueueSetting(panelId,panelWireId(i),value)) { AC_PORTAL_LOG(F("Panel: RS queue full"));panelRenderField(i);bridgeTelemetryDirty[panelWireId(i)-1]=true;return true; }
  if(property==F("CurrentBrightness")&&!panelDrainingSetting)panelLedBrightnessManualWanted=panelCached("LedControlMode")==F("manual");
  panelPendingField=field; panelPendingValue=value; panelRequestAt=millis();
  panelPendingIndex=i;panelPendingActual=panelValue(i);
  panelStore(i,value);panelRenderField(i);
  bridgeTelemetryDirty[panelWireId(i)-1]=true;
  AC_PORTAL_LOG("Panel: send "+field+" = "+value);
  return true;
}
void panelFlushQueuedSetting(){
  if(!panelQueuedCount || panelPendingField.length() || !configMegaReady || !panelLinked || millis()-panelLastRx>20000UL || bridgeTxFree()<96)return;
  uint16_t i=panelQueuedIndex[0];String value=std::move(panelQueuedValue[0]),actual=std::move(panelQueuedActual[0]);
  for(byte slot=1;slot<panelQueuedCount;slot++){
    panelQueuedIndex[slot-1]=panelQueuedIndex[slot];panelQueuedValue[slot-1]=std::move(panelQueuedValue[slot]);panelQueuedActual[slot-1]=std::move(panelQueuedActual[slot]);
  }
  --panelQueuedCount;panelQueuedValue[panelQueuedCount]="";panelQueuedActual[panelQueuedCount]="";
  // Preserve the confirmed readback for rollback; the cache already holds the latest request.
  panelDrainingSetting=true;panelSendSetting(i,value);panelDrainingSetting=false;
  if(panelPendingIndex==i)panelPendingActual=actual;
  else {panelStore(i,actual);panelRenderField(i);bridgeTelemetryDirty[panelWireId(i)-1]=true;}
}
void panelChanged(const String& id,const ArdJSON::JSONVar& value,ArdPortal::ChangeSource source) {
  if (panelUpdating || source==ArdPortal::ChangeSource::Application || !panelReady) return;
  panelPersistenceBatch=false;
  if(!configMegaReady)espConfigEdited();
  if(id.length()!=10||!id.startsWith("M_"))return;
  char* end=nullptr;uint32_t hash=strtoul(id.c_str()+2,&end,16);
  if(end!=id.c_str()+id.length())return;
  for (uint16_t i=0;i<PANEL_FIELD_COUNT;i++) if(hash==panelControlHash(i)) {
    if(panelLedComponent(i)){panelSendLed(value);return;}
    String raw=value.type()==ArdJSON::JSONVar::Type::String ? value.asString() : ArdJSON::JSON.stringify(value);
    panelSendSetting(i,raw); return;
  }
}
bool panelMqttSetting(const String& key,const String& value) {
  if(key==F("CustomColor")){auto parsed=ArdJSON::JSON.parse(value);return panelSendLed(parsed);}
  for (uint16_t i=0;i<PANEL_FIELD_COUNT;i++) if (key==panelCommandField(i)) return panelSendSetting(i,value);
  return false;
}
String panelMissingPages() {
  String missing;
  for (byte i=0;i<PANEL_PAGE_COUNT;i++) if ((PANEL_PAGE_MASK&(uint16_t(1)<<i))&&!(panelRegisteredMask&(uint16_t(1)<<i))) { if (missing.length()) missing+=", "; missing+=panelPageNames[i]; }
  return missing.length() ? missing : String(F("none"));
}
// Completion can happen inside start() for small pages or later from loop().
void panelRegistrationFinished(bool success) {
  byte page=panelRegisteringPage;
  if (page>=PANEL_PAGE_COUNT) return;
  panelRegisteringPage=255;
  if (success) {
    if (!(panelRegisteredMask&(uint16_t(1)<<page))) {
      panelRegisteredMask|=uint16_t(1)<<page; ++panelRegisteredPages;
    }
    panelReady=true;
    panelRegistrationErrors[page]="";
    AC_PORTAL_LOG(String(F("Panel: "))+String(panelPageNames[page])+" registered"+panelMemoryDiagnostic());
    for (uint16_t i=0;i<PANEL_FIELD_COUNT;i++) if (panelSeen[i]) {
      if(panelFieldPage(i)==page)panelConfirmSetting(i,panelValue(i));
      panelNeedsRender[i]=true;
    }
    if (panelRegisteredMask==PANEL_PAGE_MASK) AC_PORTAL_LOG(String(F("Panel/HA: "))+String(panelActiveFieldCount())+" shared Mega fields registered");
  } else {
    const auto& error=portal.appConfigRegistrationError();
    String field=error.fieldId;
    for (uint16_t i=0;i<PANEL_FIELD_COUNT;i++) if (field==panelControlIdAt(i)) { field+=" ("+panelFieldName(i)+")"; break; }
    panelRegistrationErrors[page]=String(F("Panel: registration FAILED "))+String(panelPageNames[page])+String(F(" stage="))+String(error.stage)+" field="+field+" reason="+error.reason;
    AC_PORTAL_LOG(panelRegistrationErrors[page]+panelMemoryDiagnostic());
  }
}
void panelSetup() {
  panelClearCache();
  portal.onAppConfigValueChanged(panelChanged);
  portal.onPortalAndAppConfigSaved([](bool ok,const String& error){
    panelPersistenceBatch=false;
    if(!ok){AC_PORTAL_LOG("Native config: ArdFS save failed: "+error);for(uint16_t i=0;i<PANEL_FIELD_COUNT;i++)if(panelSeen[i]&&panelFieldPersistent(i))panelSaveConfirmed[i]=true;}
  });
  portal.onAppConfigPageRegistrationFinished(panelRegistrationFinished);
}
// Coalesce revision notifications so a snapshot does not trigger hundreds of HTTP refreshes.
void panelRenderService(){
 static uint16_t render=0;static uint32_t nextAt=0;
 uint32_t start=millis();if(int32_t(start-nextAt)<0)return;nextAt=start+200;
 byte applied=0;
 if(panelNeedsRender.none())return;
 for(uint16_t scanned=0;scanned<PANEL_FIELD_COUNT;scanned++){
  uint16_t i=render;render=(render+1)%PANEL_FIELD_COUNT;
  if(!panelNeedsRender[i])continue;
  panelNeedsRender[i]=false;panelRenderField(i);bridgeSerialService();
  if(++applied>=8||millis()-start>=4)break;
 }
}

void panelService() {
  if (!portal.portalAndAppConfigReady()) return;
  panelLedLoadSettings();
  if (panelRegisteredMask!=PANEL_PAGE_MASK && panelRegisteringPage==255 && !portal.appConfigPageRegistrationBusy()) {
    //Largest pages first, before smaller schemas fragment the heap.
    //A rejected page must not prevent the other pages or UART from working.
    static const byte order[]={0,4,3,1,5,6,9,2,7,8,10};
    static byte nextPage=0;
    static uint32_t retryAt=0;
    if ((long)(millis()-retryAt)>=0) {
      while (nextPage<PANEL_PAGE_COUNT && (!(PANEL_PAGE_MASK&(uint16_t(1)<<order[nextPage]))||(panelRegisteredMask&(uint16_t(1)<<order[nextPage])))) ++nextPage;
      if (nextPage>=PANEL_PAGE_COUNT) { nextPage=0; retryAt=millis()+30000; }
      else {
        byte page=order[nextPage++];
        if (!panelDefinitionHostname.length()) panelDefinitionHostname=bridgeHostname();
        const auto* definition=panelPageDefinition(page);
        AC_PORTAL_LOG(String(F("Panel: register "))+String(panelPageNames[page])+String(F(" bytes="))+String(strlen_P(reinterpret_cast<const char*>(definition)))+panelMemoryDiagnostic());
        // Set the owner before start(): the completion callback may run inline.
        panelRegisteringPage=page;
        if (!portal.startAppConfigPageRegistration(definition) && panelRegisteringPage==page) {
          // Immediate validation errors may return without a completion callback.
          panelRegistrationFinished(false);
        }
      }
    }
  }
  if (!panelReady) return;
  panelLedLoadSettings();
  uint32_t now=millis();
  if (panelDefinitionHostname!=bridgeHostname()) {
    AC_PORTAL_LOG(F("Panel/HA: MQTT device name changed; restart ESP to rebuild topics")); panelDefinitionHostname=bridgeHostname();
  }
  if (panelPendingField.length() && now-panelRequestAt>10000UL) {
    AC_PORTAL_LOG("Panel: configuration/save confirmation timeout for "+panelPendingField);
    panelRollbackPending();
    panelPendingField=""; panelPendingValue=""; panelRefreshWanted=true;
  }
  if (panelLinked && now-panelLastRx>20000UL) { panelLinked=false; panelClearCache(); panelRefreshWanted=true; }
  panelFlushQueuedSetting();
  // Binary link heartbeats and boot sessions own liveness.
  if (panelRefreshWanted && !panelPendingField.length() && !panelQueuedCount && bridgeTxFree()>96) {
    if (bridgeQueueFrame("[hostname "+bridgeHostname()+"]") && bridgeQueueFrame("[panelrefresh]")) panelRefreshWanted=false;
  }
  panelRenderService();
  panelPersistenceService();
  panelLedPersistenceService();
  static uint32_t lastDiagnostic=0;
  if (now-lastDiagnostic>=30000UL) {
    //Keep the latest registration failure next to the periodic summary.
    for (byte page=0;page<PANEL_PAGE_COUNT;page++) if (panelRegistrationErrors[page].length()) AC_PORTAL_LOG(panelRegistrationErrors[page]);
    AC_PORTAL_LOG(String(F("Panel/HA: fields="))+String(panelActiveFieldCount())+String(F(", pages="))+String(panelRegisteredPages)+String(F("/"))+String(PANEL_ACTIVE_PAGE_COUNT)+", missing="+panelMissingPages()+String(F(", Mega="))+String(panelLinked ? "online" : "waiting")+panelMemoryDiagnostic());
    lastDiagnostic=now;
  }
}
