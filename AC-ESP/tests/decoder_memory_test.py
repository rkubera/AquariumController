#!/usr/bin/env python3
"""Actual ESP UART/MQTT decoding under reserve/concat allocation failures."""
from pathlib import Path
import re,subprocess,tempfile
root=Path(__file__).resolve().parents[1]
support=(root.parent/'AC-ATMEGA/tests/host_support.hpp').read_text().split('unsigned long ticks')[0]
source=(root/'parser.ino').read_text();mqtt=(root/'commands.ino').read_text()
def function(text,name):
 a=re.search(r'^(?:void|bool|String) '+name+r'\(',text,re.M).start();b=text.index('{',a)+1;depth=1
 while depth:depth+=(text[b]=='{')-(text[b]=='}');b+=1
 return text[a:b]
fixture=r'''
#include <bitset>
#define AC_PORTAL_LOG(...) portal.log(__VA_ARGS__)
#define memcpy_P memcpy
#define strcmp_P strcmp
using __FlashStringHelper=char;
#include "ReliableUart.h"
#include "WireFields.h"
ControllerFeatures portalFeatures={15,4,6,8};bool configMegaReady=true;
bool acFieldSupported(uint16_t id,const ControllerFeatures&){return id>0&&id<=AC_WIRE_FIELD_COUNT;}
constexpr uint16_t PANEL_FIELD_COUNT=AC_WIRE_FIELD_COUNT;
uint16_t wirePanelIndex(uint16_t id){return id-1;}
String bridgeTelemetry[AC_WIRE_FIELD_COUNT];std::bitset<AC_WIRE_FIELD_COUNT> bridgeTelemetryDirty;
unsigned values=0,replies=0,configReplies=0,mqttCommands=0;String last;
struct Portal {void log(const String&) {}}portal;
void espConfigMessage(uint8_t,const uint8_t*,uint8_t){}
void getCommand(const String&){}
bool espConfigReply(uint32_t,uint16_t,const String& value){++configReplies;last=value;return false;}
void panelReplyValue(uint32_t,uint16_t,const String& value){++replies;last=value;}
void panelReceiveWireValue(uint16_t,const String& value){++values;last=value;}
String bridgeHostname(){return "test";}bool panelRefreshWanted=false;
bool panelMqttSetting(const String&,const String& value){++mqttCommands;last=value;return true;}
std::vector<uint8_t> packet;uint8_t packetKind=0;
bool bridgePush(uint8_t kind,const uint8_t* p,uint8_t n){packetKind=kind;packet.assign(p,p+n);return true;}
'''
main=r'''
int main(){
 String key="CurrentBrightness";unsigned before=acTestStringConstructions;assert(acFieldId(key));assert(acTestStringConstructions==before); // No temporary Strings during dictionary lookup.
 uint8_t p[160]={};AcWire::put16(p,acFieldId("CurrentBrightness"));memcpy(p+2,"100",3);
 espWireReceive(AcWire::Value,p,5);assert(values==1&&last=="100");
 acTestStringReserveFailures=1;espWireReceive(AcWire::Value,p,5);assert(values==1);
 acTestStringConcatBudget=2;espWireReceive(AcWire::Value,p,5);assert(values==1);acTestStringConcatBudget=-1;
 p[3]=0;espWireReceive(AcWire::Value,p,5);assert(values==1);p[3]='0';
 AcWire::put32(p,7);AcWire::put16(p+4,acFieldId("CurrentBrightness"));memcpy(p+6,"100",3);
 acTestStringReserveFailures=1;espWireReceive(AcWire::Result,p,9);assert(!configReplies&&!replies);
 acTestStringConcatBudget=1;espWireReceive(AcWire::Result,p,9);assert(!configReplies&&!replies);acTestStringConcatBudget=-1;
 espWireReceive(AcWire::Result,p,9);assert(configReplies==1&&replies==1&&last=="100");
 const uint8_t payload[]={'1','0','0'};
 acTestStringReserveFailures=1;bridgeMqttMessage("cmnd/test/CurrentBrightness",payload,3);assert(!mqttCommands);
 acTestStringConcatBudget=2;bridgeMqttMessage("cmnd/test/CurrentBrightness",payload,3);assert(!mqttCommands);acTestStringConcatBudget=-1;
 bridgeMqttMessage("cmnd/test/CurrentBrightness",payload,3);assert(mqttCommands==1&&last=="100");
 assert(bridgeQueueSetting(17,23,"with spaces"));assert(packetKind==AcWire::Setting&&AcWire::u32(packet.data())==17&&AcWire::u16(packet.data()+4)==23&&std::string((char*)packet.data()+6,packet.size()-6)=="with spaces");
 assert(!bridgeQueueSetting(0,23,"value")&&!bridgeQueueSetting(17,0,"value"));
 for(int i=0;i<20000;i++){assert(wireText(payload,3)=="100");assert(acFieldId("CurrentBrightness"));assert(!acFieldId("invalid_field"));}
 puts("PASS: actual ESP decoders reject partial/OOM UART values, replies and MQTT commands; typed setting packets; repeated decode/lookup under sanitizers.");
}
'''
code=support+fixture+'\n'+function(source,'wireText')+'\n'+function(source,'espWireReceive')+'\n'+function(source,'bridgeQueueSetting')+'\n'+function(mqtt,'bridgeMqttMessage')+main
with tempfile.TemporaryDirectory(prefix='ac-decoder-memory-')as tmp:
 p=Path(tmp)/'test.cpp';p.write_text(code)
 subprocess.run(['c++','-std=c++11','-fsanitize=address,undefined','-I',str(root),'-I',str(root.parent/'AC-ATMEGA'),str(p),'-o',tmp+'/test'],check=True)
 subprocess.run([tmp+'/test'],check=True)
