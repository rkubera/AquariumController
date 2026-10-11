#!/usr/bin/env python3
"""Exercise actual RGB light adapters with ArdJSON, including partial commands."""
from pathlib import Path
import re,subprocess,tempfile
root=Path(__file__).resolve().parents[1]
library=Path('/Users/radoslawkubera/Documents/Developer/Arduino/libraries/ArdPortal')
source=(root/'Panel.ino').read_text()
def function(name):
 a=re.search(r'^(?:void|bool|String|ArdJSON::JSONVar) '+name+r'\(',source,re.M).start();b=source.index('{',a)+1;depth=1
 while depth:depth+=(source[b]=='{')-(source[b]=='}');b+=1
 return source[a:b]
code=r'''
#include <cassert>
#include <cmath>
#include <map>
#include "ArdJSON.h"
uint32_t fakeMillis=0;
std::map<std::string,String> values;
uint32_t panelLedSavedColor=0xFFFFFF;uint8_t panelLedSavedBrightness=0;
uint16_t panelPendingIndex=UINT16_MAX;bool panelLedBrightnessManualWanted=false;
bool panelLedSettingsLoaded=false,panelLedSettingsDirty=false,configMegaReady=true;
String panelPendingField;uint8_t panelQueuedCount=0;
String panelFieldProperty(uint16_t i){return i==0?"CustomColor":i==1?"CurrentBrightness":"LedState";}

std::vector<std::pair<uint16_t,String>> commands;
uint16_t acFieldId(const String& key){return key=="CustomColor"?1:key=="CurrentBrightness"?2:key=="LedState"?3:0;}
uint16_t wirePanelIndex(uint16_t id){return id-1;}
String panelControlIdAt(uint16_t){return "light";}
String panelCached(const String& key){return values[key.c_str()];}
String panelValue(uint16_t i){return panelCached(i==0?"CustomColor":i==1?"CurrentBrightness":"LedState");}
bool panelValuesEqual(const String& a,const String& b){return a==b;}
bool panelSendSetting(uint16_t i,String value){commands.emplace_back(i,value);values[i==0?"CustomColor":i==1?"CurrentBrightness":"LedState"]=value;return true;}
struct Portal {
 ArdJSON::JSONVar saved,durable,legacyBrightness;bool busy=false;int writes=0;
 ArdJSON::JSONVar getAppConfigValue(const char* key){return String(key)=="LedSettings"?durable:String(key)=="light"?saved:legacyBrightness;}
 bool portalAndAppConfigBusy(){return busy;}
 bool setAppConfigValue(const char* key,const ArdJSON::JSONVar& value){assert(String(key)=="LedSettings");durable=value;++writes;return true;}
}portal;
'''
code+='\n'.join(function(n)for n in ('panelNativeRaw','panelLedId','panelLedValue','panelLedLoadSettings','panelLedRestoreRaw','panelLedConfirm','panelLedPersistenceService','panelSendLed'))
code+=r'''
int main(){
 portal.saved=ArdJSON::JSON.parse(R"({"state":"OFF","brightness":30,"color":{"r":255,"g":255,"b":255}})");
 values={{"CustomColor","8323327"},{"CurrentBrightness","80"},{"LedState","on"}};
 auto runtime=panelLedValue();assert(runtime["brightness"].asDouble()==80);assert(runtime["color"]["r"].asDouble()==127&&runtime["color"]["b"].asDouble()==255);assert(runtime["state"].asString()=="ON");
 assert(portal.saved["brightness"].asDouble()==30); // Readback does not rewrite persisted settings.
 portal.durable=ArdJSON::JSON.parse(R"({"color":16777215,"brightness":30})");
 assert(panelLedRestoreRaw(1)=="16777215"&&panelLedRestoreRaw(2)=="30");
 values["LedControlMode"]="partofday";panelLedConfirm(1,"80");panelLedPersistenceService();
 assert(portal.writes==0&&panelLedSavedBrightness==30);
 panelLedConfirm(0,"66051");panelLedPersistenceService();
 assert(portal.writes==1&&portal.durable["color"].asDouble()==66051&&portal.durable["brightness"].asDouble()==30);
 values["LedControlMode"]="manual";panelLedConfirm(1,"2");panelPendingField="CurrentBrightness";panelLedPersistenceService();assert(portal.writes==1);
 panelPendingField="";portal.busy=true;panelLedPersistenceService();assert(portal.writes==1);portal.busy=false;panelLedPersistenceService();
 assert(portal.writes==2&&portal.durable["brightness"].asDouble()==2);
 // A manual brightness ACK must remain durable when a later mode command is queued.
 values["LedControlMode"]="partofday";panelPendingIndex=1;panelLedBrightnessManualWanted=true;panelLedConfirm(1,"7");panelPendingIndex=UINT16_MAX;panelLedPersistenceService();assert(portal.writes==3&&portal.durable["brightness"].asDouble()==7);
 panelLedConfirm(2,"off");panelLedPersistenceService();assert(portal.writes==3); // power is never durable
 panelLedSettingsLoaded=false;panelLedSavedColor=0;panelLedSavedBrightness=0;
 assert(panelLedRestoreRaw(1)=="66051"&&panelLedRestoreRaw(2)=="7");
 values["CurrentBrightness"]="80";
 assert(panelSendLed(ArdJSON::JSON.parse(R"({"brightness":2})")));assert(commands.size()==1&&commands[0].first==1&&commands[0].second=="2");
 assert(panelSendLed(ArdJSON::JSON.parse(R"({"color":{"r":1,"g":2,"b":3},"state":"OFF"})")));assert(commands.size()==3&&commands[1].second=="66051"&&commands[2].second=="off");
 auto n=commands.size();assert(!panelSendLed(ArdJSON::JSON.parse(R"({"color":{"r":256,"g":0,"b":0},"brightness":90})")));assert(commands.size()==n);
 assert(!panelSendLed(ArdJSON::JSON.parse(R"({"brightness":101})")));assert(!panelSendLed(ArdJSON::JSON.parse(R"({"brightness":0.5})")));assert(!panelSendLed(ArdJSON::JSON.parse(R"({"state":"AUTO"})")));assert(commands.size()==n);
 // Full native updates carry old state; brightness must override it.
 values["LedState"]="off";values["CurrentBrightness"]="0";commands.clear();
 assert(panelSendLed(ArdJSON::JSON.parse(R"({"brightness":25,"state":"OFF"})")));
 assert(values["CurrentBrightness"]=="25"&&values["LedState"]=="on");
 assert(panelSendLed(ArdJSON::JSON.parse(R"({"brightness":0,"state":"ON"})")));
 assert(values["CurrentBrightness"]=="0"&&values["LedState"]=="off");
 // Existing scalar settings seed the new record before light registration.
 portal.durable=ArdJSON::JSONVar();portal.saved=ArdJSON::JSONVar(String("8323327"));portal.legacyBrightness=12;
 panelLedSettingsLoaded=false;panelLedSettingsDirty=false;panelLedSavedColor=0;panelLedSavedBrightness=0;
 panelLedLoadSettings();assert(panelLedSavedColor==8323327&&panelLedSavedBrightness==12&&panelLedSettingsDirty);
}
'''
with tempfile.TemporaryDirectory()as d:
 host=(library/'tests/host/Arduino.h').read_text().replace('  String() = default;', '  long toInt()const{return strtol(c_str(),nullptr,10);}'+'\n'+'  bool equalsIgnoreCase(const String& other)const{return strcasecmp(c_str(),other.c_str())==0;}'+'\n'+'  String() = default;')
 (Path(d)/'Arduino.h').write_text('#include <strings.h>\n'+host)
 p=Path(d)/'test.cpp';p.write_text(code);out=Path(d)/'test'
 subprocess.run(['c++','-std=c++17','-fsanitize=address,undefined','-I'+d,'-I'+str(library/'src'),str(p),'-o',str(out)],check=True)
 subprocess.run([str(out)],check=True)
print('PASS: light RGB packing, current state, manual restore, partial commands and atomic validation.')
