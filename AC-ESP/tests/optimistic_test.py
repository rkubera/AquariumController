#!/usr/bin/env python3
"""Actual optimistic command/cache handlers: acceptance, echo suppression, rollback and ACK."""
from pathlib import Path
import subprocess,tempfile,re
root=Path(__file__).resolve().parents[1]
source=(root/'Panel.ino').read_text().replace('  if(key==F("CustomColor")){auto parsed=ArdJSON::JSON.parse(value);return panelSendLed(parsed);}', '')
def function(name):
 start=re.search(r'^(?:void|bool|int|String) '+name+r'\(',source,re.M).start();end=source.index('{',start)+1;depth=1
 while depth:
  depth+=(source[end]=='{')-(source[end]=='}');end+=1
 return source[start:end]
support=(root/'tests/host_support.hpp').read_text().split('const int bufferOutSize')[0]
support=support.replace('  int indexOf(char ch)', '  bool equalsIgnoreCase(const String& o) const {return strcasecmp(value.c_str(),o.value.c_str())==0;}\n  int indexOf(char ch,int from) const {auto n=value.find(ch,from);return n==std::string::npos?-1:int(n);}\n  int indexOf(char ch)')
fixture=r'''#include <strings.h>
const uint16_t PANEL_FIELD_COUNT=2;
bool panelLedBrightnessManualWanted=false;
String panelCached(const String&){return "manual";}
bool panelFieldEnabled(uint16_t){return true;}
String bridgeTelemetry[3],panelPendingField,panelPendingValue,panelPendingActual,panelOperation;
bool panelSeen[2]={},panelNeedsRender[2]={},bridgeTelemetryDirty[3]={},panelLinked=true,configMegaReady=true,queueOk=true;
uint16_t panelPendingIndex=UINT16_MAX;
static const byte PANEL_SETTING_QUEUE_SIZE=8;
uint16_t panelQueuedIndex[PANEL_SETTING_QUEUE_SIZE];
String panelQueuedValue[PANEL_SETTING_QUEUE_SIZE],panelQueuedActual[PANEL_SETTING_QUEUE_SIZE];
byte panelQueuedCount=0;bool panelDrainingSetting=false;
int bridgeTxFree(){return queueOk?512:0;}

uint32_t panelLastRx=0,panelRequestAt=0,panelId=0;
struct Portal {void log(const String&) {}} portal;
String panelCommandField(uint16_t i){return i==0?"CurrentBrightness":"Sensor1Name";}
struct MockField{byte group;}; MockField panelField(uint16_t){return {0};}
String panelFieldProperty(uint16_t i){return i==0?"CurrentBrightness":"Name";}
uint16_t panelWireId(uint16_t i){return i+1;}
uint16_t wirePanelIndex(uint16_t id){return id>=1 && id<=2?id-1:UINT16_MAX;}
uint16_t acFieldId(const String& f){return f=="CurrentBrightness"?1:f=="Sensor1Name"?2:3;}
String bridgeHostname(){return "test";}
bool bridgeQueueFrame(const String&){return queueOk;}
void panelRenderField(uint16_t){}
void panelConfirmSetting(uint16_t,const String&,bool=false){}
'''
# Declare handlers before combining the actual source functions.
names=['panelQueuedSlot','panelValue','panelStore','panelReceiveWireValue','panelReceiveValue','panelReply','panelRollbackPending','panelSendSetting','panelMqttSetting','panelFlushQueuedSetting']
funcs=[function(n) for n in names]
protos='\n'.join(f[:f.index('{')].strip()+';' for f in funcs)
main=r'''int main(){
 ticks=2000;panelLastRx=ticks;panelStore(0,"20");panelStore(1,"old");
 assert(panelMqttSetting("CurrentBrightness","70"));assert(panelValue(0)=="70" && panelPendingIndex==0 && bridgeTelemetryDirty[0]);
 panelReceiveWireValue(1,"20");assert(panelValue(0)=="70");
 panelRollbackPending();assert(panelValue(0)=="20");panelPendingField="";panelPendingValue="";
 assert(panelSendSetting(0,"80"));panelReply(String(panelId)+" CurrentBrightness 80");
 assert(panelValue(0)=="80" && panelPendingField.length()==0 && panelPendingIndex==UINT16_MAX);
 assert(panelSendSetting(0,"90"));panelReply(String(panelId)+" CurrentBrightness 80");assert(panelValue(0)=="80" && panelPendingField.length()==0);
 queueOk=false;assert(panelSendSetting(0,"30"));assert(panelValue(0)=="80" && panelPendingField.length()==0);
 queueOk=true;assert(panelSendSetting(0,"50"));assert(panelSendSetting(1,"new"));assert(panelValue(1)=="new" && panelQueuedCount==1);
 assert(panelSendSetting(0,"60"));assert(panelSendSetting(0,"70"));assert(panelQueuedCount==2 && panelValue(0)=="70");
 panelReceiveWireValue(2,"old");assert(panelValue(1)=="new");
 panelReply(String(panelId)+" CurrentBrightness 50");assert(panelValue(0)=="70");
 panelFlushQueuedSetting();assert(panelPendingIndex==1 && panelValue(1)=="new");
 panelReply(String(panelId)+" Sensor1Name new");panelFlushQueuedSetting();assert(panelPendingIndex==0 && panelPendingValue=="70");
 panelReceiveWireValue(1,"50");assert(panelValue(0)=="70");
 panelReply(String(panelId)+" CurrentBrightness 70");assert(panelValue(0)=="70" && panelQueuedCount==0);
 // A failed latest request rolls back to the acknowledged intermediate value.
 panelSendSetting(0,"80");panelSendSetting(0,"90");panelReply(String(panelId)+" CurrentBrightness 80");
 panelFlushQueuedSetting();panelRollbackPending();panelPendingField="";panelPendingValue="";assert(panelValue(0)=="80");
 // Invalid values never replace a queued valid request.
 panelSendSetting(0,"30");panelSendSetting(1,"valid");panelSendSetting(1,"invalid\nvalue");assert(panelValue(1)=="valid" && panelQueuedCount==1);
 panelReply(String(panelId)+" CurrentBrightness 30");panelFlushQueuedSetting();panelReply(String(panelId)+" Sensor1Name valid");
}'''
with tempfile.TemporaryDirectory() as d:
 p=Path(d);(p/'test.cpp').write_text('#include <strings.h>\n'+support+fixture+protos+'\n'.join(funcs)+main)
 subprocess.run(['c++','-std=c++11','-fsanitize=address,undefined','-I',str(root),str(p/'test.cpp'),'-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True)
print('PASS: optimistic Portal/HA commands, stale echo suppression, ACK/correction, timeout rollback and coalesced FIFO edits across controls and rollback to confirmed state.')
