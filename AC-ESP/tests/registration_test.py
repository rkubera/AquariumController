#!/usr/bin/env python3
"""Test actual panel registration flow with inline/deferred API completion."""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1]
support=(root/'tests/host_support.hpp').read_text().split('const int bufferOutSize')[0]
support=support.replace('  const char* c_str()', '  void replace(const char*,const String&) {}\n  const char* c_str()')
source=(root/'Panel.ino').read_text()
flow=source[source.index('void panelRegistrationFinished'):source.index('// Coalesce revision')] + source[source.index('void panelService()'):source.index('  if (!panelReady) return;',source.index('void panelService'))]+'}\n'
fixture=r'''
#define strlen_P strlen
#include <functional>
uint32_t testClock=0;
uint32_t millis(){return testClock;}
constexpr byte PANEL_PAGE_COUNT=11;
constexpr uint16_t PANEL_FIELD_COUNT=1,PANEL_PAGE_MASK=1535;
uint16_t panelActiveFieldCount(){return PANEL_FIELD_COUNT;}
const char* panelPageNames[]={"led","relays","schedule","inputs","pwm","clock","fan","buzzer","diagnostics","dht","peripherals"};
byte panelRegisteringPage=255,panelRegisteredPages=0;
uint16_t panelRegisteredMask=0;
bool panelReady=false,panelSeen[1]={true},panelNeedsRender[1]={false};
bool panelSaveConfirmed[1]={};
bool panelPersistenceBatch=false;
bool panelFieldPersistent(uint16_t){return true;}
uint16_t panelWireId(uint16_t i){return i+1;}
String panelRegistrationErrors[11],panelDefinitionHostname;
String panelMemoryDiagnostic(){return "";}
String panelFieldName(uint16_t){return "Sensor1Name";}
String panelControlIdAt(uint16_t){return "M_test";}
String bridgeHostname(){return "test";}
const char* panelPageDefinition(byte p){return panelPageNames[p];}
void panelClearCache(){}
void panelLedLoadSettings(){}
uint8_t panelFieldPage(uint16_t){return 0;}
String panelValue(uint16_t){return "test";}
void panelConfirmSetting(uint16_t,const String&,bool=false){}
void panelChanged(){}
void panelRenderService(){}
struct Portal {
 struct Error {const char* stage="field";String fieldId="M_test",reason="test rejection";} error;
 std::function<void(bool)> callback;
 bool busy=false;int mode=0,starts=0;
 std::vector<String> logs;
 void onPortalAndAppConfigSaved(std::function<void(bool,const String&)>) {}
 void log(String s){logs.push_back(s);}
 const Error& appConfigRegistrationError(){return error;}
 bool portalAndAppConfigReady(){return true;}
 bool appConfigPageRegistrationBusy(){return busy;}
 void onAppConfigValueChanged(void(*)()){}
 void onAppConfigPageRegistrationFinished(std::function<void(bool)> c){callback=c;}
 bool startAppConfigPageRegistration(const String&)=delete;
 bool startAppConfigPageRegistration(const char*) {
  assert(!busy);++starts;
  if(mode==1){callback(true);return true;}
  if(mode==2){callback(false);return false;}
  if(mode==3)return false;
  busy=true;return true;
 }
 void finish(bool success){assert(busy);busy=false;callback(success);}
} portal;
'''
main=r'''
int main(){
 panelSetup();panelService();assert(portal.starts==1 && panelRegisteredPages==0 && !panelReady);
 for(int i=0;i<20;i++)panelService();assert(portal.starts==1);
 portal.finish(true);assert(panelRegisteredPages==1 && panelReady && panelNeedsRender[0]);
 portal.mode=1;panelService();assert(panelRegisteredPages==2 && panelRegisteringPage==255);
 portal.mode=2;panelService();assert(panelRegisteredPages==2 && panelRegisteringPage==255 && panelRegistrationErrors[3].length());
 portal.mode=3;panelService();assert(panelRegisteredPages==2 && panelRegisteringPage==255 && panelRegistrationErrors[1].length());
 portal.mode=0;panelService();portal.finish(false);assert(panelRegisteredPages==2 && panelRegisteringPage==255 && panelRegistrationErrors[5].length());
 portal.mode=1;for(int i=0;i<5;i++)panelService();
 assert(panelRegisteredPages==7);
 testClock=30001;for(int i=0;i<12;i++)panelService();
 assert(panelRegisteredPages==10 && panelRegisteredMask==PANEL_PAGE_MASK);
 assert(!panelRegistrationErrors[5].length() && !panelRegistrationErrors[1].length() && !panelRegistrationErrors[3].length());
 puts("Registration: inline/deferred success, failures, busy gate and retry passed");
}
'''
# The full support provides millis(), retain only its Arduino String definitions.
support=support[:support.index('unsigned long ticks') ] if 'unsigned long ticks' in support else support
# Remove unrelated hardware helpers after the String class.
end=support.index('\n};',support.index('class String'))+4
support=support[:end]+"\ninline String operator+(const String& a,const String& b){return String(a.value+b.value); }\n"
with tempfile.TemporaryDirectory() as tmp:
 p=Path(tmp)/'test.cpp';p.write_text(support+fixture+flow+main)
 subprocess.run(['c++','-std=c++17','-fsanitize=address,undefined',str(p),'-o',tmp+'/test'],check=True)
 subprocess.run([tmp+'/test'],check=True)
