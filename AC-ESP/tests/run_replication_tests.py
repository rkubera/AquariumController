#!/usr/bin/env python3
"""Actual boot state machines and reliable transport: native fields, no storage copy."""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1]
s=(root/'tests/host_support.hpp').read_text().split('unsigned long ticks')[0]
s+='''
#include <deque>
#include <map>
#include "ReliableUart.h"
#include "ConfigStorage.h"
#include "ControllerFeatures.h"
uint16_t acActiveRestoreCount(const ControllerFeatures&){return 4;}
uint16_t acActiveRestoreId(uint16_t i,const ControllerFeatures&){return i<4?i+1:0;}
uint32_t testTime=0;uint32_t millis(){return testTime;}
const uint32_t AC_CONFIG_SCHEMA=123;const uint16_t AC_RESTORE_COUNT=4;
uint16_t acRestoreId(uint16_t i){return i<4?i+1:0;}
struct Port{std::deque<uint8_t> bytes;int availableForWrite(){return 64;}void write(uint8_t b){bytes.push_back(b);}};
bool truncateNextWireText=false;
String wireText(const uint8_t* p,uint8_t n){if(truncateNextWireText&&n){truncateNextWireText=false;--n;}return String(std::string((const char*)p,n));}
namespace Mega {
ControllerFeatures megaFeatures={0,0,0,0};bool megaFeaturesKnown=false;
ConfigStorage configStorage;
uint16_t linkPanelField=0;uint32_t linkPanelId=0;
bool linkSnapshotRetry=false,linkLocalPanel=false;
int wifiStatus=0,brokerStatus=0;
const int WIFI_STATUS_DISCONNECTED=0,BROKER_STATUS_DISCONNECTED=0,CONFIG_fanStartTemperature_addr=3;
byte fanStartTemperature=30;
struct Queue{void clear(){}} linkWireQueue;
void megaConfigSession(bool);void megaConfigMessage(uint8_t,const uint8_t*,uint8_t);
void received(uint8_t k,const uint8_t* p,uint8_t n){megaConfigMessage(k,p,n);}
void connection(bool c){megaConfigSession(c);}
AcWire::Link wireLink(received,connection);Port port;
int activations=0,beeps=0;std::map<uint16_t,String> actual;
void configLoad(){configStorage.begin();}
void configSaveValue(byte v,int a){configStorage.update(a,v);}
void initFan(){}void ledInit(){}void buzzerDisable(){}void dhtDisable(){}void rtcDisable(){}void dhtInit(){}void rtcInit(){}void errorReport(bool){}
void sensorsResetAcquisition(){}void relaysInit(){}void pwmOutputsInit(){}
void clockUpdateTimezoneRules(){}void clockInvalidateLocalTime(){}void clockGetLocalTime(){}
void buzzerInit(){beeps++;}void linkRequestSnapshot(){activations++;}
void applySetting(uint16_t id,String v){actual[id]=v;}
'''+(root.parent/'AC-ATMEGA/ConfigRestore.ino').read_text()+'''
void readback(){if(linkPanelField&&wireLink.idle()){uint8_t p[160];AcWire::put32(p,linkPanelId);AcWire::put16(p+4,linkPanelField);String v=actual[linkPanelField];memcpy(p+6,v.c_str(),v.length());if(wireLink.send(AcWire::Result,p,v.length()+6))linkPanelField=0;}}
}
namespace Esp {
ControllerFeatures portalFeatures={15,4,6,8};
struct Value {String text;bool isUndefined(){return false;}};
struct Portal {
 std::map<std::string,String> saved;bool ready=false;std::vector<String> logs;
 void log(String s){logs.push_back(s);}
 bool portalAndAppConfigReady(){return ready;}
 Value getAppConfigValue(const char* id){return {saved[id]};}
}portal;
bool panelLinked=false,panelRefreshWanted=false,pagesReady=false;
String panelPendingField,panelPendingValue;uint32_t panelLastRx=0;uint16_t panelPendingIndex=UINT16_MAX;
void panelClearCache(){}
bool panelAllPagesReady(){return pagesReady;}
uint16_t acFieldId(const char*){return 65535;}
String panelLedId(){return "light";}
String panelLedRestoreRaw(uint16_t){return "0";}
String panelNativeRaw(Value v){return v.text;}
String panelControlIdAt(uint16_t i){return String(i+1);}
String acFieldName(uint16_t i){return String(i);}
uint16_t wirePanelIndex(uint16_t i){return i-1;}
bool panelValuesEqual(const String& a,const String& b){return a==b;}
struct Queue{int free(){return 512;}}bridgeWireQueue;
void espConfigSession(bool);void espConfigMessage(uint8_t,const uint8_t*,uint8_t);bool espConfigReply(uint32_t,uint16_t,const String&);
void received(uint8_t k,const uint8_t* p,uint8_t n){if(k==AcWire::Result){espConfigReply(AcWire::u32(p),AcWire::u16(p+4),wireText(p+6,n-6));return;}espConfigMessage(k,p,n);}
void connection(bool c){espConfigSession(c);}
AcWire::Link wireLink(received,connection);Port port;
'''+(root/'ConfigRestore.ino').read_text()+'''
}
void transfer(Port& p,AcWire::Link& link){while(!p.bytes.empty()){auto b=p.bytes.front();p.bytes.pop_front();link.input(b,testTime);}}
void step(){testTime++;Mega::configRestoreService();Mega::readback();Esp::espConfigService();Mega::wireLink.service(Mega::port,testTime);Esp::wireLink.service(Esp::port,testTime);transfer(Mega::port,Esp::wireLink);transfer(Esp::port,Mega::wireLink);}
void pump(int n){while(n--)step();}
int main(){
 uint8_t manifest[5],before[10]={};AcWire::put32(before,1);AcWire::put16(before+4,4);AcWire::put32(before+6,123);
 Mega::megaConfigMessage(AcWire::ConfigBegin,before,10);assert(!Mega::megaReceivingConfig);
 Esp::portal.saved={{"1","sensor name"},{"2","0"},{"3","65535"},{"4","on"}};
 Mega::wireLink.begin(11,44,testTime);Esp::wireLink.begin(22,44,testTime);pump(3000);
 assert(!Mega::megaConfigReady);assert(Mega::activations==0);
 Esp::portal.ready=true;pump(3000);assert(!Mega::megaConfigReady); // All pages must finish registration.
 Esp::pagesReady=true;pump(3000);
 assert(Mega::megaConfigReady&&Esp::configMegaReady&&Mega::activations==1&&Mega::beeps==1);
 for(int i=1;i<=4;i++)assert(Mega::actual[i]==Esp::portal.saved[std::to_string(i)]);
 // Reset only ESP: no defaults or boot transaction are sent to a running Mega.
 Esp::portal.saved["1"]="stale";Esp::wireLink.begin(55,44,testTime);pump(4000);
 assert(Mega::actual[1]=="sensor name"&&Mega::activations==1&&Esp::configMegaReady);
 // Bad count/schema/order/CRC and incomplete transactions cannot activate outputs.
 Mega::megaConfigReady=false;Mega::actual.clear();Mega::megaConfigSession(true);
 uint8_t p[20];AcWire::put32(p,100);AcWire::put16(p+4,3);AcWire::put32(p+6,123);
 Mega::megaConfigMessage(AcWire::ConfigBegin,p,10);assert(!Mega::megaReceivingConfig);
 AcWire::put16(p+4,4);Mega::megaConfigMessage(AcWire::ConfigBegin,p,10);assert(Mega::megaReceivingConfig);
 AcWire::put32(p,100);AcWire::put32(p+4,0);Mega::megaConfigMessage(AcWire::ConfigCommit,p,8);assert(!Mega::megaConfigReady);
 AcWire::put16(p,0);AcWire::put16(p+2,2);p[4]='x';Mega::megaConfigMessage(AcWire::ConfigChunk,p,5);assert(Mega::megaConfigReceived==0);
 // A partially decoded configuration value must not advance count or CRC.
 AcWire::put16(p,0);AcWire::put16(p+2,1);memcpy(p+4,"12345",5);
 uint32_t unchangedDigest=Mega::megaConfigDigest;
 truncateNextWireText=true;Mega::megaConfigMessage(AcWire::ConfigChunk,p,9);
 assert(!truncateNextWireText&&Mega::megaConfigReceived==0&&Mega::megaConfigDigest==unchangedDigest&&!Mega::linkPanelField&&Mega::actual.empty());
 uint32_t digest=Mega::megaFeatureDigest();
 for(int i=0;i<4;i++){AcWire::put16(p,i);AcWire::put16(p+2,i+1);p[4]='x';digest=AcWire::configDigest(digest,p,5);Mega::megaConfigMessage(AcWire::ConfigChunk,p,5);Mega::linkPanelField=0;}
 AcWire::put32(p,100);AcWire::put32(p+4,digest^1);Mega::megaConfigMessage(AcWire::ConfigCommit,p,8);assert(!Mega::megaConfigReady);
 AcWire::put32(p+4,digest);Mega::megaConfigMessage(AcWire::ConfigCommit,p,8);assert(Mega::megaConfigReady);
 // A new native configuration restores after Mega reset; pending edits restart boot.
 Mega::megaConfigReady=false;Mega::wireLink.begin(77,44,testTime);pump(2000);
 if(Esp::configRestoreWanted){Esp::portal.saved["3"]="1234";Esp::espConfigEdited();}
 pump(7000);assert(Mega::megaConfigReady&&Esp::configMegaReady);
 for(int i=1;i<=4;i++)assert(Mega::actual[i]==Esp::portal.saved[std::to_string(i)]);
 // Changed capabilities close output interlocks and require a new boot transaction.
 ControllerFeatures changed={0,0,0,0};changed.encode(manifest);
 Mega::megaConfigMessage(AcWire::Capabilities,manifest,4);assert(Mega::megaConfigReady);
 manifest[2]=5;Mega::megaConfigMessage(AcWire::Capabilities,manifest,5);assert(Mega::megaConfigReady);
 changed.encode(manifest);Mega::megaConfigMessage(AcWire::Capabilities,manifest,5);
 assert(!Mega::megaConfigReady&&Mega::megaFeatures.same(changed)&&Mega::megaConfigRequestPending);
 Esp::portalFeatures=changed;Esp::wireLink.begin(88,44,testTime);pump(7000);
 assert(Mega::megaConfigReady&&Esp::configMegaReady&&Mega::megaFeatures.same(changed));
 // Loss of ESP does not disable a configured controller.
 for(int i=0;i<9000;i++){testTime++;Mega::wireLink.service(Mega::port,testTime);Mega::port.bytes.clear();}
 assert(!Mega::wireLink.connected()&&Mega::megaConfigReady);
 puts("PASS: native field restore, registration gate, readback, CRC/count/order rejection, Mega/ESP resets, edits during restore and offline autonomy.");
}
'''
with tempfile.TemporaryDirectory(prefix='ac-native-restore-') as tmp:
 f=Path(tmp)/'test.cpp';f.write_text(s);out=str(Path(tmp)/'test')
 subprocess.run(['c++','-std=c++17','-fsanitize=address,undefined','-fno-omit-frame-pointer','-g','-I',str(root),'-I',str(root.parent/'AC-ATMEGA'),str(f),'-o',out],check=True)
 subprocess.run([out],check=True)
