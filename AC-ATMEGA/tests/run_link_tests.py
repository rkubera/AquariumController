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
support += '\nconst int bufferOutSize='+re.search(r'const int bufferOutSize\s*=\s*(\d+)',main).group(1)+';\n'
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
defaults={f['ha']['state_topic'].rsplit('/',1)[-1]:f['default'] for page in pages for f in page['fields'] if f.get('persist')}
defaults.update(CurrentBrightness='0',CustomColor='16777215')
restore_text=(ROOT/'RestoreFields.h').read_text()
restore_ids=[int(n) for n in re.search(r'AC_RESTORE_IDS\[\] PROGMEM=\{([^}]+)',restore_text).group(1).split(',')]
records=[]
for id in restore_ids:
    key=keys[id-1];value=defaults[key]
    value=('on' if value else 'off') if isinstance(value,bool) else str(value)
    records.append('{'+json.dumps(key)+','+json.dumps(value)+'}')
support+='static const char* testRestoreRecords[][2]={'+','.join(records)+'};\n'
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


files = ['PwmHardware.ino','Wire.ino','Tools.ino','Control.ino','Sensors.ino','PWMOutputs.ino','Relays.ino','PeripheralConfig.ino','LED.ino','DHT.ino','ControllerLink.ino','Errors.ino','Events.ino','Menu.ino','Keyboard.ino','Buzzer.ino','Fan.ino']
texts = [(ROOT/file).read_text() for file in files]
support += '#include "PeripheralDevices.h"\n#include "ReportField.h"\n#include "TemporaryState.h"\n#include "LcdMenuLayout.h"\n'
#Some publications live alongside hardware calls irrelevant to the LINK test.
def function(file,name):
    text=(ROOT/file).read_text()
    at=re.search(r'^(?:void|int|String|bool|const char\*) '+name+r'\(',text,re.M).start()
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
        r'^(?:void|bool|float|double|byte|int|String|unsigned long|uint32_t|uint16_t|const char\*) \w+\s*\([^\n{]*\)\s*\{',text,re.M)]
config=(ROOT/'Config.ino').read_text()
name_save=config[config.index('void configSaveString '):config.index('void configSaveLedBrightness')]
with tempfile.TemporaryDirectory(prefix='aquarium-link-tests-') as temp:
    temp=Path(temp)
    (temp/'Arduino.h').write_text('#pragma once\n')
    code='\n'.join([support,constants,strings,'\n'.join(globals),'\n'.join(prototypes), 'void pwmWrite16(byte,uint16_t);', (ROOT/'tests/peripheral_support.hpp').read_text(),
                     (ROOT/'tests/link_stubs.cpp').read_text(),*texts,name_save,
                     (ROOT/'tests/link_tests.cpp').read_text()])
    (temp/'test.cpp').write_text(code)
    subprocess.run(['c++','-std=c++11','-DARDUINO=100','-I',str(temp),'-I',str(ROOT),
                    '-Wno-deprecated-declarations','-Wno-constant-logical-operand','-fsanitize=address,undefined','-fno-omit-frame-pointer','-g',
                    str(temp/'test.cpp'),'-o',str(temp/'tests')],check=True)
    subprocess.run([str(temp/'tests')],check=True)
