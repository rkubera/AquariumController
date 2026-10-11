#!/usr/bin/env python3
"""Exercise the actual LINK parser/transport and every declared LINK setting."""
from pathlib import Path
import re,json
import subprocess
import tempfile
ROOT = Path(__file__).resolve().parents[1]
main = (ROOT/'AC-ATMEGA.ino').read_text().split('void setup()')[0]
constants = '\n'.join(re.findall(r'^#define .+$',main,re.M))
strings = '\n'.join(re.findall(r'^const char .+$',main,re.M))
skip = {'bufferOutSize','partOfDay','ticks'}
globals = []
for line in re.findall(r'^(?:byte|int|long|unsigned long|double|float|bool|boolean|String|char|uint32_t|uint16_t|const uint16_t|const int) \w+[^\n]*;',main,re.M):
    name = re.search(r'(?:byte|int|long|double|float|bool|boolean|String|char|uint32_t|uint16_t) (\w+)',line).group(1)
    if name in skip: continue
    globals.append(line)
support = (ROOT/'tests/host_support.hpp').read_text()
support = support[:support.index('const int bufferOutSize')]
support += '\nint partOfDay=1;\nint schedulerGetActualPartOfDay() { return partOfDay; }\n'
support += '\nconst int bufferOutSize=100;\n'
extra=(ROOT/'tests/link_support.hpp').read_text()
support += '#define A15 15\n#include <deque>\n#include "ReliableUart.h"\nusing __FlashStringHelper=char;\n#define memcpy_P memcpy\n#include "WireFields.h"\n'
# Names are decoded only in the host test, from ESP's external dictionary.
keys=re.findall(r'AC_WIRE_KEY_\d+\[\] PROGMEM = "([^"]+)";', (ROOT.parent/'AC-ESP/WireFields.h').read_text())
keys=[k.replace('Mqtt','Broker') for k in keys]
support+='static const char* testFieldNames[]={'+','.join('"'+k+'"' for k in keys)+'};\n'
support+='String acFieldName(uint16_t id){return id&&id<=AC_WIRE_FIELD_COUNT?testFieldNames[id-1]:"";}\n'
pages=json.loads((ROOT.parent/'AC-ESP/tests/panel_schema.json').read_text())
commands=[f['ha']['command_topic'].rsplit('/',1)[-1] for page in pages for f in page['fields'] if 'command_topic' in f.get('ha',{})]
support+='static const char* testCommandNames[]={'+','.join('"'+c+'"' for c in commands)+'};\n'
support+='static const char* testRestoreRecords[][2]={{"FanMaxSpeedTemp","35"},{"AfternoonBrightness","0"},{"AfternoonTime","10:00:00"},{"BuzzerAfternoon","on"},{"BuzzerEvening","on"},{"BuzzerMorning","on"},{"BuzzerNight","off"},{"BuzzerOnErrors","off"},{"BuzzerOnStart","off"},{"CustomColor","16777215"},{"EveningBrightness","0"},{"EveningTime","19:00:00"},{"LedColorAfternoon","black"},{"LedColorEvening","black"},{"LedColorMorning","black"},{"LedColorNight","black"},{"LedManualMode","black"},{"CurrentBrightness","0"},{"MaxIntTemp","100"},{"MorningBrightness","0"},{"MorningTime","09:00:00"},{"NightBrightness","0"},{"NightTime","22:00:00"},{"PWMOutput1AfternoonMode","0"},{"PWMOutput1ControlDirection","direct"},{"PWMOutput1EveningMode","0"},{"PWMOutput1ManualMode","0"},{"PWMOutput1MaxDeviation","0"},{"PWMOutput1MorningMode","0"},{"PWMOutput1Name",""},{"PWMOutput1NightMode","0"},{"PWMOutput1PidKd","0"},{"PWMOutput1PidKi","0"},{"PWMOutput1PidKp","0"},{"PWMOutput1SensorsSetpoint","0"},{"PWMOutput2AfternoonMode","0"},{"PWMOutput2ControlDirection","direct"},{"PWMOutput2EveningMode","0"},{"PWMOutput2ManualMode","0"},{"PWMOutput2MaxDeviation","0"},{"PWMOutput2MorningMode","0"},{"PWMOutput2Name",""},{"PWMOutput2NightMode","0"},{"PWMOutput2PidKd","0"},{"PWMOutput2PidKi","0"},{"PWMOutput2PidKp","0"},{"PWMOutput2SensorsSetpoint","0"},{"PWMOutput3AfternoonMode","0"},{"PWMOutput3ControlDirection","direct"},{"PWMOutput3EveningMode","0"},{"PWMOutput3ManualMode","0"},{"PWMOutput3MaxDeviation","0"},{"PWMOutput3MorningMode","0"},{"PWMOutput3Name",""},{"PWMOutput3NightMode","0"},{"PWMOutput3PidKd","0"},{"PWMOutput3PidKi","0"},{"PWMOutput3PidKp","0"},{"PWMOutput3SensorsSetpoint","0"},{"PWMOutput4AfternoonMode","0"},{"PWMOutput4ControlDirection","direct"},{"PWMOutput4EveningMode","0"},{"PWMOutput4ManualMode","0"},{"PWMOutput4MaxDeviation","0"},{"PWMOutput4MorningMode","0"},{"PWMOutput4Name",""},{"PWMOutput4NightMode","0"},{"PWMOutput4PidKd","0"},{"PWMOutput4PidKi","0"},{"PWMOutput4PidKp","0"},{"PWMOutput4SensorsSetpoint","0"},{"PWMOutput5AfternoonMode","0"},{"PWMOutput5ControlDirection","direct"},{"PWMOutput5EveningMode","0"},{"PWMOutput5ManualMode","0"},{"PWMOutput5MaxDeviation","0"},{"PWMOutput5MorningMode","0"},{"PWMOutput5Name",""},{"PWMOutput5NightMode","0"},{"PWMOutput5PidKd","0"},{"PWMOutput5PidKi","0"},{"PWMOutput5PidKp","0"},{"PWMOutput5SensorsSetpoint","0"},{"PWMOutput6AfternoonMode","0"},{"PWMOutput6ControlDirection","direct"},{"PWMOutput6EveningMode","0"},{"PWMOutput6ManualMode","0"},{"PWMOutput6MaxDeviation","0"},{"PWMOutput6MorningMode","0"},{"PWMOutput6Name",""},{"PWMOutput6NightMode","0"},{"PWMOutput6PidKd","0"},{"PWMOutput6PidKi","0"},{"PWMOutput6PidKp","0"},{"PWMOutput6SensorsSetpoint","0"},{"Relay1AfternoonMode","off"},{"Relay1ControlDirection","direct"},{"Relay1EveningMode","off"},{"Relay1MaxDeviation","0"},{"Relay1MorningMode","off"},{"Relay1Name",""},{"Relay1NightMode","off"},{"Relay1SensorsSetpoint","0"},{"Relay2AfternoonMode","off"},{"Relay2ControlDirection","direct"},{"Relay2EveningMode","off"},{"Relay2MaxDeviation","0"},{"Relay2MorningMode","off"},{"Relay2Name",""},{"Relay2NightMode","off"},{"Relay2SensorsSetpoint","0"},{"Relay3AfternoonMode","off"},{"Relay3ControlDirection","direct"},{"Relay3EveningMode","off"},{"Relay3MaxDeviation","0"},{"Relay3MorningMode","off"},{"Relay3Name",""},{"Relay3NightMode","off"},{"Relay3SensorsSetpoint","0"},{"Relay4AfternoonMode","off"},{"Relay4ControlDirection","direct"},{"Relay4EveningMode","off"},{"Relay4MaxDeviation","0"},{"Relay4MorningMode","off"},{"Relay4Name",""},{"Relay4NightMode","off"},{"Relay4SensorsSetpoint","0"},{"ScheduleMode","daily"},{"Sensor1CalibRawValue1","0"},{"Sensor1CalibRawValue2","1023"},{"Sensor1CalibValue1","0"},{"Sensor1CalibValue2","1"},{"Sensor1CriticalMaxValue","off"},{"Sensor1CriticalMinValue","off"},{"Sensor1MaxValue","off"},{"Sensor1MinValue","off"},{"Sensor1Name",""},{"Sensor2CalibRawValue1","0"},{"Sensor2CalibRawValue2","1023"},{"Sensor2CalibValue1","0"},{"Sensor2CalibValue2","1"},{"Sensor2CriticalMaxValue","off"},{"Sensor2CriticalMinValue","off"},{"Sensor2MaxValue","off"},{"Sensor2MinValue","off"},{"Sensor2Name",""},{"Sensor3CalibRawValue1","0"},{"Sensor3CalibRawValue2","1023"},{"Sensor3CalibValue1","0"},{"Sensor3CalibValue2","1"},{"Sensor3CriticalMaxValue","off"},{"Sensor3CriticalMinValue","off"},{"Sensor3MaxValue","off"},{"Sensor3MinValue","off"},{"Sensor3Name",""},{"Sensor4CalibRawValue1","0"},{"Sensor4CalibRawValue2","1023"},{"Sensor4CalibValue1","0"},{"Sensor4CalibValue2","1"},{"Sensor4CriticalMaxValue","off"},{"Sensor4CriticalMinValue","off"},{"Sensor4MaxValue","off"},{"Sensor4MinValue","off"},{"Sensor4Name",""},{"Sensor5CalibRawValue1","0"},{"Sensor5CalibRawValue2","1023"},{"Sensor5CalibValue1","0"},{"Sensor5CalibValue2","1"},{"Sensor5CriticalMaxValue","off"},{"Sensor5CriticalMinValue","off"},{"Sensor5MaxValue","off"},{"Sensor5MinValue","off"},{"Sensor5Name",""},{"Sensor6CalibRawValue1","0"},{"Sensor6CalibRawValue2","1023"},{"Sensor6CalibValue1","0"},{"Sensor6CalibValue2","1"},{"Sensor6CriticalMaxValue","off"},{"Sensor6CriticalMinValue","off"},{"Sensor6MaxValue","off"},{"Sensor6MinValue","off"},{"Sensor6Name",""},{"Sensor7CalibRawValue1","0"},{"Sensor7CalibRawValue2","1023"},{"Sensor7CalibValue1","0"},{"Sensor7CalibValue2","1"},{"Sensor7CriticalMaxValue","off"},{"Sensor7CriticalMinValue","off"},{"Sensor7MaxValue","off"},{"Sensor7MinValue","off"},{"Sensor7Name",""},{"Sensor8CalibRawValue1","0"},{"Sensor8CalibRawValue2","1023"},{"Sensor8CalibValue1","0"},{"Sensor8CalibValue2","1"},{"Sensor8CriticalMaxValue","off"},{"Sensor8CriticalMinValue","off"},{"Sensor8MaxValue","off"},{"Sensor8MinValue","off"},{"Sensor8Name",""},{"TimezoneRule1DayOfWeek","Sunday"},{"TimezoneRule1Hour","0"},{"TimezoneRule1Month","January"},{"TimezoneRule1Offset","-12"},{"TimezoneRule1Week","first"},{"TimezoneRule2DayOfWeek","Sunday"},{"TimezoneRule2Hour","0"},{"TimezoneRule2Month","January"},{"TimezoneRule2Offset","-12"},{"TimezoneRule2Week","first"},{"WeekdayAfternoonTime","10:00:00"},{"WeekdayEveningTime","19:00:00"},{"WeekdayMorningTime","09:00:00"},{"WeekdayNightTime","22:00:00"},{"WeekendAfternoonTime","10:00:00"},{"WeekendEveningTime","19:00:00"},{"WeekendMorningTime","09:00:00"},{"WeekendNightTime","22:00:00"},{"Sensor1Type","notconnected"},{"Sensor2Type","notconnected"},{"Sensor3Type","notconnected"},{"Sensor4Type","notconnected"},{"Sensor5Type","notconnected"},{"Sensor6Type","notconnected"},{"Sensor7Type","notconnected"},{"Sensor8Type","notconnected"},{"PWMOutput1Input","none"},{"PWMOutput2Input","none"},{"PWMOutput3Input","none"},{"PWMOutput4Input","none"},{"PWMOutput5Input","none"},{"PWMOutput6Input","none"},{"Relay1Input","none"},{"Relay2Input","none"},{"Relay3Input","none"},{"Relay4Input","none"},{"FanStartTemp","30"},{"LedControlMode","manual"},{"PWMOutput1ControlMode","manual"},{"PWMOutput2ControlMode","manual"},{"PWMOutput3ControlMode","manual"},{"PWMOutput4ControlMode","manual"},{"PWMOutput5ControlMode","manual"},{"PWMOutput6ControlMode","manual"},{"Relay1ControlMode","manual"},{"Relay2ControlMode","manual"},{"Relay3ControlMode","manual"},{"Relay4ControlMode","manual"}};\n'
support+='uint16_t acFieldId(const String& name){for(uint16_t i=1;i<=AC_WIRE_FIELD_COUNT;i++)if(acFieldName(i)==name)return i;return 0;}\n'
support += '#define snprintf_P snprintf\n#define PSTR(x) (x)\n'
support += extra
support += 'struct tmElements_t {uint8_t Year=56,Month=10,Day=6,Hour=12,Minute=34,Second=56;};\nvoid breakTime(time_t,tmElements_t&){}\nint tmYearToCalendar(uint8_t y){return 1970+y;}\n'

support += '#include "SchedulePlan.h"\n'
support += r'''
void usbDiagnosticStage(byte,uint32_t){}
void usbDiagnosticSetting(uint32_t,uint16_t,const String&){}
void usbDiagnosticAck(uint32_t,uint16_t,const String&){}
bool megaConfigReady=true;
#define pgm_read_word(p) (*(p))
#include "RestoreFields.h"
#include "FeatureFields.h"
unsigned long micros(){return ticks*1000;}
void megaConfigMessage(uint8_t,const uint8_t*,uint8_t){}
void megaConfigSession(bool){}
void configRestoreService(){}
void nullReceive(uint8_t,const uint8_t*,uint8_t){}void nullConnection(bool){}
struct TestLink {AcWire::Link::Stats stats;bool connected(){return true;}void begin(uint32_t,uint32_t,uint32_t){}void input(uint8_t,uint32_t){}template<class S>void service(S&,uint32_t){}} wireLink;
struct CaptureQueue {
 struct Msg{byte kind;std::vector<byte> data;};std::deque<Msg> messages;uint16_t bytes=0;
 uint16_t free(){return 512-bytes;}
 void clear(){messages.clear();bytes=0;}
 bool push(byte kind,const byte* p,byte n){if(n+2>free())return false;messages.push_back({kind,std::vector<byte>(p,p+n)});bytes+=n+2;return true;}
 void drain(TestLink&) {
  if(messages.empty()||!Serial3.writable)return;
  auto m=messages.front();messages.pop_front();bytes-=m.data.size()+2;auto p=m.data.data();String text;
  if(m.kind==AcWire::Value){text="value "+acFieldName(AcWire::u16(p))+" ";text.value+=std::string((char*)p+2,m.data.size()-2);}
  if(m.kind==AcWire::Result){text="result "+String(AcWire::u32(p))+" "+acFieldName(AcWire::u16(p+4))+" ";text.value+=std::string((char*)p+6,m.data.size()-6);}
  if(m.kind==AcWire::Request)text="query timestamp";
  if(text.length())Serial3.output+=text.value+"\r\n";
 }
} linkWireQueue;
'''


files = ['PwmHardware.ino','Wire.ino','Tools.ino','Control.ino','Sensors.ino','PWMOutputs.ino','Relays.ino','LED.ino','DHT.ino','ControllerLink.ino','Errors.ino','Events.ino','Menu.ino','Keyboard.ino','Buzzer.ino','Fan.ino']
texts = [(ROOT/file).read_text() for file in files]
support += '#include "ReportField.h"\n#include "TemporaryState.h"\n#include "LcdMenuLayout.h"\n'
#Some publications live alongside hardware calls irrelevant to the LINK test.
def function(file,name):
    text=(ROOT/file).read_text()
    at=re.search(r'^(?:void|int|String|bool) '+name+r'\(',text,re.M).start()
    start=text.index('{',at)
    depth=1; end=start+1
    while depth:
        if text[end]=='{': depth+=1
        elif text[end]=='}': depth-=1
        end+=1
    return text[at:end]
texts += [
          (ROOT/'Scheduler.ino').read_text().replace(function('Scheduler.ino','schedulerGetActualPartOfDay'),'').replace(function('Scheduler.ino','schedulerPublishActualPartOfDay'),''),
          function('Clock.ino','clockReportAll'),function('Clock.ino','clockReportHourDate'),function('Clock.ino','clockReportSyncState'),function('Clock.ino','clockSyncStatusString'),function('Clock.ino','clockNTPSynchronize_cb'),function('Clock.ino','clockSyncEvent')]
prototypes=[]
for text in texts:
    prototypes += [line[:line.index('{')].rstrip()+';' for line in re.findall(
        r'^(?:void|bool|float|double|byte|int|String|unsigned long|uint32_t|uint16_t) \w+\s*\([^\n{]*\)\s*\{',text,re.M)]
config=(ROOT/'Config.ino').read_text()
name_save=config[config.index('void configSaveString '):config.index('void configSaveLedBrightness')]
with tempfile.TemporaryDirectory(prefix='aquarium-link-tests-') as temp:
    temp=Path(temp)
    (temp/'Arduino.h').write_text('#pragma once\n')
    code='\n'.join([support,constants,strings,'\n'.join(globals),'\n'.join(prototypes),
                     (ROOT/'tests/link_stubs.cpp').read_text(),*texts,name_save,
                     (ROOT/'tests/link_tests.cpp').read_text()])
    (temp/'test.cpp').write_text(code)
    subprocess.run(['c++','-std=c++11','-DARDUINO=100','-I',str(temp),'-I',str(ROOT),
                    '-Wno-deprecated-declarations','-Wno-constant-logical-operand','-fsanitize=address,undefined','-fno-omit-frame-pointer','-g',
                    str(temp/'test.cpp'),'-o',str(temp/'tests')],check=True)
    subprocess.run([str(temp/'tests')],check=True)
