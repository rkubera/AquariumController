#!/usr/bin/env python3
from pathlib import Path
import tempfile,subprocess
root=Path(__file__).resolve().parents[1]
support=(root/'tests/host_support.hpp').read_text().split('const int bufferOutSize')[0]
panel=(root/'Panel.ino').read_text();batch=panel[panel.index('void panelRenderService()'):panel.index('void panelService()')]
commands=(root/'commands.ino').read_text();mqtt=commands[commands.index('void bridgeMqttService()'):commands.index('void bridgeMqttMessage(')]
fixture=r'''
#include <bitset>
#define memcpy_P memcpy
using __FlashStringHelper=char;
#include "WireFields.h"
namespace ArdJSON{struct Json{String stringify(const String& s){return s;}}JSON;}
String panelLedValue(){return "{}";}
struct TestFeatures{};TestFeatures portalFeatures;bool acFieldSupported(uint16_t,const TestFeatures&){return true;}
constexpr uint16_t PANEL_FIELD_COUNT=299;
std::bitset<PANEL_FIELD_COUNT> panelNeedsRender;std::vector<uint16_t> rendered;
int uartCalls=0,renderCost=1;
void panelRenderField(uint16_t i){rendered.push_back(i);ticks+=renderCost;}
void bridgeSerialService(){uartCalls++;}
String bridgeTelemetry[AC_WIRE_FIELD_COUNT];std::bitset<AC_WIRE_FIELD_COUNT> bridgeTelemetryDirty,bridgeTelemetrySeen;
bool bridgeDhtSent[2]={};uint32_t bridgeDhtLastSent[2]={};
byte bridgePowerValue[2]={};bool bridgePowerPending[2]={};
String bridgeHostname(){return "test";}
struct Portal{bool accept=true;std::vector<std::string> topics;bool mqttConnected(){return true;}bool publish(const char* topic,const char*,bool){topics.push_back(topic);return accept;}}portal;
'''
main=r'''
void serviceMqtt(){for(unsigned i=0;i<AC_WIRE_FIELD_COUNT/16+2;i++)bridgeMqttService();}
int main(){
 ticks=1000;panelNeedsRender.set();
 panelRenderService();assert(rendered.size()==4&&uartCalls==4);
 panelRenderService();assert(rendered.size()==4);
 ticks=1200;panelRenderService();assert(rendered.size()==8);
 renderCost=0;ticks=1400;panelRenderService();assert(rendered.size()==16);
 while(rendered.size()<299){ticks+=200;panelRenderService();}
 for(uint16_t i=0;i<299;i++)assert(rendered[i]==i&&!panelNeedsRender[i]);
 assert(uartCalls==299);
 panelNeedsRender[0]=true;panelRenderService();assert(panelNeedsRender[0]);ticks+=200;panelRenderService();assert(!panelNeedsRender[0]&&rendered.back()==0);
 ticks=100000;bridgeTelemetryDirty[0]=bridgeTelemetryDirty[1]=true;
 serviceMqtt();assert(portal.topics.size()==1&&!bridgeTelemetryDirty[0]);
 serviceMqtt();assert(portal.topics.size()==1&&bridgeTelemetryDirty[1]);ticks+=25;serviceMqtt();assert(portal.topics.size()==2&&!bridgeTelemetryDirty[1]);
 bridgeTelemetryDirty[2]=true;portal.accept=false;ticks+=25;serviceMqtt();assert(bridgeTelemetryDirty[2]&&portal.topics.size()==3);
 serviceMqtt();assert(portal.topics.size()==3);portal.accept=true;ticks+=25;serviceMqtt();assert(!bridgeTelemetryDirty[2]);
 uint16_t temperature=acFieldId("InternalTemperature")-1;bridgeTelemetryDirty[temperature]=true;bridgeDhtSent[0]=true;bridgeDhtLastSent[0]=ticks;
 size_t count=portal.topics.size();ticks+=1000;serviceMqtt();assert(portal.topics.size()==count&&bridgeTelemetryDirty[temperature]);
 ticks=bridgeDhtLastSent[0]+60000;serviceMqtt();assert(portal.topics.size()==count+1&&!bridgeTelemetryDirty[temperature]);
 puts("PASS: actual bounded render batches, UART service between updates, coalescing, MQTT pacing/retry, one-minute DHT interval.");
}
'''
with tempfile.TemporaryDirectory(prefix='ac-pacing-') as tmp:
 tmp=Path(tmp);(tmp/'test.cpp').write_text(support+fixture+batch+mqtt+main)
 subprocess.run(['c++','-std=c++11','-I',str(root),'-fsanitize=address,undefined','-fno-omit-frame-pointer',str(tmp/'test.cpp'),'-o',str(tmp/'test')],check=True)
 subprocess.run([str(tmp/'test')],check=True)
