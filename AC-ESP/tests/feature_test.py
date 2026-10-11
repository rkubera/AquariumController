#!/usr/bin/env python3
"""Check compiled schemas across feature profiles, not only generator defaults."""
from pathlib import Path
import subprocess,re,json,itertools,tempfile
root=Path(__file__).resolve().parents[1]
def pages(overrides):
 defaults={'DHT':1,'RTC':1,'BUZZER':1,'FAN':1}
 defines={**{'AC_ENABLE_'+k:v for k,v in defaults.items()},'AC_RELAY_COUNT':4,'AC_PWM_COUNT':6,'AC_INPUT_COUNT':8,**overrides}
 source='\n'.join('#define '+k+' '+str(v) for k,v in defines.items())+'\n#include "PortalFeatures.h"\n#include "PanelSchema.h"\n'
 text=subprocess.run(['c++','-E','-P','-x','c++','-I',str(root),'-'],input=source,text=True,capture_output=True,check=True).stdout
 return [json.loads(''.join(re.findall(r'R"JSON\((.*?)\)JSON"',body,re.S))) for body in re.findall(r'static const char PANEL_PAGE_\d+\[\].*?=\s*(.*?);',text,re.S)]
profiles=[]
wireKeys=re.findall(r'AC_WIRE_KEY_\d+\[\] PROGMEM = "(.*?)";',(root/"WireFields.h").read_text())
base=pages({});assert base==json.loads((root/'tests/panel_schema.json').read_text())
for bits in itertools.product((0,1),repeat=7):
 opts={'AC_ENABLE_'+k:v for k,v in zip(('DHT','RTC','BUZZER','FAN'),bits[3:])}
 opts.update(AC_RELAY_COUNT=4*bits[0],AC_PWM_COUNT=6*bits[1],AC_INPUT_COUNT=8*bits[2])
 ps=pages(opts);ids={p['id'] for p in ps};fields=[f for p in ps for f in p['fields']]
 assert {'led','schedule','clock'}<=ids
 assert ('relays'in ids)==bool(bits[0]);assert ('pwm'in ids)==bool(bits[1]);assert ('inputs'in ids)==bool(bits[2])
 assert ('fan'in ids)==bool(bits[3]and bits[6]);assert ('dht'in ids)==bool(bits[3]and not bits[6])
 profiles.append(((bits[3]|(bits[4]<<1)|(bits[5]<<2)|((bits[3]and bits[6])<<3),4*bits[0],6*bits[1],8*bits[2]),{f['ha']['state_topic'].rsplit('/',1)[-1]for f in fields},2+sum(f.get('persist',False)for f in fields)))
 assert not any(f['ha']['state_topic'].endswith('/ClockRtcStatus') for f in fields)
 assert ('diagnostics'in ids)==bool(bits[2]or bits[3]or bits[4]);assert ('buzzer'in ids)==bool(bits[5])
 if not bits[2]:
  assert not any(o['value']in ('pid','treshold','threshold') for f in fields for o in f.get('options',[]))
 def leaves(rule):
  if 'field'in rule:return [rule['field']]
  return [v for k in ('and','or') for c in rule.get(k,[]) for v in leaves(c)]
 for p in ps:
  local={f['id']for f in p['fields']}
  assert all(all(v in local for v in leaves(f['visibleWhen']))for f in p['fields']if 'visibleWhen'in f)
ps=pages({'AC_RELAY_COUNT':2,'AC_PWM_COUNT':3,'AC_INPUT_COUNT':2})
for p in ps:
 for f in p['fields']:
  key=f['ha']['state_topic'].rsplit('/',1)[-1]
  for prefix,limit in [('Relay',2),('PWMOutput',3),('Sensor',2)]:
   m=re.match(prefix+r'(\d+)',key)
   if m:assert int(m[1])<=limit
  for o in f.get('options',[]):
   m=re.match(r'(relay|pwmoutput|sensor)(\d+)$',o['value'])
   if m:assert int(m[2])<={'relay':2,'pwmoutput':3,'sensor':2}[m[1]]
# Real shared manifest and generated restore filtering.
code='''#include <cstdint>
#include <cassert>
#define PROGMEM
#define pgm_read_word(p) (*(p))
#include "ControllerFeatures.h"
#include "RestoreFields.h"
#include "FeatureFields.h"
int main(){ControllerFeatures all={15,4,6,8},off={0,0,0,0},out;uint8_t p[5];all.encode(p);assert(ControllerFeatures::decode(p,5,out)&&out.same(all));assert(!ControllerFeatures::decode(p,4,out));p[2]=5;assert(!ControllerFeatures::decode(p,5,out));p[2]=4;p[1]=8;assert(!ControllerFeatures::decode(p,5,out));assert(acActiveRestoreCount(all)==AC_RESTORE_COUNT);assert(acActiveRestoreCount(off)<acActiveRestoreCount(all));for(unsigned i=0;i<acActiveRestoreCount(off);i++)assert(acFieldSupported(acActiveRestoreId(i,off),off));}
'''
panelKeys=[f['ha']['state_topic'].rsplit('/',1)[-1]for p in base for f in p['fields']]
checks='static const uint16_t panelIds[]={'+','.join(str(wireKeys.index(k)+1)for k in panelKeys)+'};\n'
for caps,keys,count in profiles:
 checks+='{ControllerFeatures caps={'+','.join(map(str,caps))+'};const char* expected="'+''.join('1'if k in keys else '0'for k in panelKeys)+'";for(unsigned i=0;i<295;i++)assert(acFieldSupported(panelIds[i],caps)==(expected[i]==\'1\'));assert(acActiveRestoreCount(caps)=='+str(count)+');}\n'
code=code.replace('int main(){','int main(){'+checks)
assert (root/'ControllerFeatures.h').read_bytes()==(root.parent/'AC-ATMEGA/ControllerFeatures.h').read_bytes()
with tempfile.TemporaryDirectory()as tmp:
 src=Path(tmp)/'test.cpp';src.write_text(code);exe=Path(tmp)/'test'
 subprocess.run(['c++','-std=c++17','-I',str(root),str(src),'-o',str(exe)],check=True);subprocess.run([str(exe)],check=True)
print('PASS: all 128 feature combinations, channel limits, dependencies, shared manifest and restore filtering.')
