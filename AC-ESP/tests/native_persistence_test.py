#!/usr/bin/env python3
"""Actual native-field reconciliation; no serialized copy or telemetry storage."""
from pathlib import Path
import re,subprocess,tempfile
root=Path(__file__).resolve().parents[1]
source=(root/'Panel.ino').read_text()
def function(name):
 a=re.search(r'^(?:void|bool|String|ArdJSON::JSONVar) '+name+r'\(',source,re.M).start();b=source.index('{',a)+1;depth=1
 while depth:depth+=(source[b]=='{')-(source[b]=='}');b+=1
 return source[a:b]
support=(root/'tests/host_support.hpp').read_text().split('const int bufferOutSize')[0]
support=support.replace('  const char* c_str()', '  float toFloat() const {return strtof(value.c_str(),nullptr);}\n  const char* c_str()')
support=support.replace('  int indexOf(char ch)', '  bool equalsIgnoreCase(const String& o) const {return strcasecmp(value.c_str(),o.value.c_str())==0;}\n  int indexOf(char ch)')
fixture=r'''
#include <bitset>
#include <strings.h>
#include <map>
namespace ArdJSON {
class JSONVar {
public:
 enum class Type {Number,Boolean,String};Type kind;double number=0;bool boolean=false;::String text;
 JSONVar():kind(Type::String){}JSONVar(double n):kind(Type::Number),number(n){}JSONVar(bool b):kind(Type::Boolean),boolean(b){}JSONVar(const ::String& s):kind(Type::String),text(s){}
 bool isUndefined()const{return false;}Type type()const{return kind;}bool asBool()const{return boolean;}::String asString()const{return text;}
};
struct Json {::String stringify(const JSONVar& v){return v.type()==JSONVar::Type::Boolean?::String(v.boolean?"true":"false"):v.type()==JSONVar::Type::Number?::String(v.number):v.text;}}JSON;
}
const uint16_t PANEL_FIELD_COUNT=3;
bool panelFieldEnabled(uint16_t){return true;}
std::bitset<3> panelSaveConfirmed;bool panelSeen[3]={true,true,true},configMegaReady=true,panelUpdating=false,panelPersistenceBatch=false;
uint16_t panelRegisteredMask=511;String panelPendingField;byte panelQueuedCount=0;
String actual[3];
uint8_t panelFieldPage(uint16_t){return 0;}
bool panelFieldPersistent(uint16_t i){return i<2;}
String panelControlIdAt(uint16_t i){return String(i);}
String panelValue(uint16_t i){return actual[i];}
void bridgeSerialService(){}
String panelFieldProperty(uint16_t i){return i==1?String("Inverted"):String("Name");}
void panelLedConfirm(uint16_t,const String&){}
bool panelLedComponent(uint16_t){return false;}void panelRenderLed(){}
struct Field{uint8_t group=0;};Field panelField(uint16_t){return Field();}
struct Portal {
 bool busy=false,writeAllowed=true;int scheduled=0,rendered=0;
 void log(const String&){}
 bool setAppConfigStateValue(const char* id,const ArdJSON::JSONVar& v,bool){state[id]=v;++rendered;return true;}
 std::map<std::string,ArdJSON::JSONVar> state,pending;
 ArdJSON::JSONVar getAppConfigValue(const char* id){return state[id];}
 bool portalAndAppConfigBusy(){return busy;}
 bool setAppConfigValue(const char* id,const ArdJSON::JSONVar& v){if(!writeAllowed)return false;pending[id]=v;busy=true;++scheduled;return true;}
}portal;
'''
names=['panelNativeRaw','panelValuesEqual','panelNativeValue','panelConfirmSetting','panelPersistenceService','panelState','panelRenderField']
funcs=[function(n) for n in names];protos='\n'.join(f[:f.index('{')].strip().replace('bool force=false','bool force')+';' for f in funcs)
main=r'''
int main(){
 portal.state["0"]=ArdJSON::JSONVar(30.0);portal.state["1"]=ArdJSON::JSONVar(false);portal.state["2"]=ArdJSON::JSONVar(String("alarm"));actual[0]="30";actual[1]="off";actual[2]="telemetry";
 panelConfirmSetting(0,"30.00",false);assert(!panelSaveConfirmed[0]); // Canonical equality doesn't schedule a flash write.
 panelConfirmSetting(2,"other alarm",true);assert(!panelSaveConfirmed[2]); // Never persist readings/alarms.
 configMegaReady=false;panelConfirmSetting(0,"35",true);assert(!panelSaveConfirmed[0]);configMegaReady=true;
 // Controller rejects an optimistic value: wait for the original native save,
 // then correct the same field, using the actual value and its native JSON type.
 portal.state["0"]=ArdJSON::JSONVar(10.0);portal.busy=true;actual[0]="30";panelConfirmSetting(0,"30",false);
 panelPersistenceService();assert(portal.scheduled==0&&panelSaveConfirmed[0]);portal.busy=false;panelPersistenceService();assert(portal.pending["0"].number==30&&!panelSaveConfirmed[0]);
 // Dependent assignment/settings merge into the same pending native write.
 actual[1]="on";panelConfirmSetting(1,"on",true);panelPersistenceService();assert(portal.pending["1"].asBool()&&portal.scheduled==2);
 // Older readbacks cannot save over a newer optimistic request in flight/queue.
 actual[0]="60";panelSaveConfirmed[0]=true;panelPendingField="setting";panelPersistenceService();assert(portal.scheduled==2);
 panelPendingField="";panelQueuedCount=1;panelPersistenceService();assert(portal.scheduled==2);
 panelQueuedCount=0;actual[0]="70";panelPersistenceService();assert(portal.pending["0"].number==70);
 portal.writeAllowed=false;panelSaveConfirmed[0]=true;panelPersistenceService();assert(panelSaveConfirmed[0]);portal.writeAllowed=true;panelPersistenceService();assert(!panelSaveConfirmed[0]);
 // The initial snapshot must not duplicate unchanged native fields in runtime JSON.
 portal.state["0"]=ArdJSON::JSONVar(30.0);actual[0]="30.00";panelRenderField(0);assert(portal.rendered==0);
 portal.state["1"]=ArdJSON::JSONVar(false);actual[1]="off";panelRenderField(1);assert(portal.rendered==0);
 portal.state["2"]=ArdJSON::JSONVar(String("Pump"));actual[2]="Pump";panelRenderField(2);assert(portal.rendered==0);
 actual[0]="31";panelRenderField(0);assert(portal.rendered==1&&portal.state["0"].number==31);
 actual[1]="on";panelRenderField(1);assert(portal.rendered==2&&portal.state["1"].asBool());
 actual[2]="pump";panelRenderField(2);assert(portal.rendered==3&&portal.state["2"].asString()=="pump");
 panelRenderField(2);assert(portal.rendered==3);
 puts("PASS: unchanged runtime-state suppression, case-sensitive names; native-only persistence, telemetry exclusion, numeric/boolean types, rejection rollback, dependent-field batching, pending edit protection and retry.");
}
'''
with tempfile.TemporaryDirectory(prefix='ac-native-persistence-') as tmp:
 f=Path(tmp)/'test.cpp';f.write_text(support+fixture+protos+'\n'+'\n'.join(funcs)+main);out=str(Path(tmp)/'test')
 subprocess.run(['c++','-std=c++17','-fsanitize=address,undefined','-fno-omit-frame-pointer','-g',str(f),'-o',out],check=True);subprocess.run([out],check=True)
