#!/usr/bin/env python3
"""Current UART v2, persistence, boot synchronization and shared-schema checks."""
from pathlib import Path
import subprocess,sys,json
root=Path(__file__).resolve().parents[1]
for name in ['feature_test.py','active_cache_test.py','decoder_memory_test.py','run_wire_tests.py','run_replication_tests.py','registration_test.py','cache_test.py','pacing_test.py','optimistic_test.py','native_persistence_test.py','light_test.py','light_storage_test.py','peripheral_profiles_test.py']:
 subprocess.run([sys.executable,str(root/'tests'/name)],check=True)
pages=json.loads((root/'tests/panel_schema.json').read_text())
assert len(pages)==10 and sum(len(p['fields']) for p in pages)==397
all_ids=[f['id'] for p in pages for f in p['fields']];assert len(all_ids)==len(set(all_ids))
for page in pages:
 ids={f['id'] for f in page['fields']}
 for f in page['fields']:
  key=f['ha']['state_topic'].rsplit('/',1)[-1]
  expected='command_topic' in f['ha'] and key not in ('LedState','CustomColor','ActualTime','ActualDate') and not key.endswith('State')
  assert f.get('persist') is expected, key
  if expected: assert 'default' in f, key
  assert f['ha']['state_topic'].startswith('stat/$device/')
  if 'command_topic' in f['ha']: assert f['ha']['command_topic'].startswith('cmnd/$device/')
  def leaves(rule):
   if 'field' in rule: return [rule]
   return [leaf for group in ('and','or') for child in rule.get(group,[]) for leaf in leaves(child)]
  if 'visibleWhen' in f: assert all(leaf['field'] in ids for leaf in leaves(f['visibleWhen']))
 assert len(json.dumps(page,ensure_ascii=False,separators=(',',':')).replace('$device','mqtt-clnt-68D3CFE6E2E0').encode())<=65535
led=next(p for p in pages if p['id']=='led');keys=[f['ha']['state_topic'].rsplit('/',1)[-1] for f in led['fields']]
assert 'LedState' not in keys and 'CurrentBrightness' not in keys and keys[0]=='CustomColor'
light=led['fields'][0];assert light['type']=='light' and light['ha']['brightness_scale']==100
assert [c['key'] for c in light['controls']]==['state','brightness','color']
assert light['controls'][1]['name']=='LED: Current brightness (%)'
brightness=[f for f in led['fields'] if f['ha']['state_topic'].rsplit('/',1)[-1].endswith('Brightness')]
assert len(brightness)==4 and all(f['min']==0 and f['max']==100 for f in brightness)
assert not any('LedPowerState' in f['ha']['state_topic'] or 'LedActual' in f['ha']['state_topic'] for p in pages for f in p['fields'])
print('PASS: shared Portal/HA schema, field uniqueness, dependency references, page limits and LED order.')

pwm=next(p for p in pages if p['id']=='pwm')
def visible(rule,mode):
 if 'field' in rule:return rule['equals']==mode
 if 'or' in rule:return any(visible(child,mode) for child in rule['or'])
 return all(visible(child,mode) for child in rule['and'])
for channel in range(1,7):
 for part in ('ControlDirection','Input','SensorsSetpoint','SensorsValue'):
  field=next(f for f in pwm['fields'] if f['ha']['state_topic'].endswith('/PWMOutput'+str(channel)+part))
  assert [visible(field['visibleWhen'],mode) for mode in ('manual','partofday','treshold','pid')]==[False,False,True,True]
print('PASS: common regulation fields for all six PWM channels are visible only in threshold/PID.')

assert all("optimistic" not in f["ha"] if f["type"] in ("switch","light") else f["ha"].get("optimistic") is True for p in pages for f in p["fields"] if "command_topic" in f["ha"])
pwm_fields=[f for f in pwm["fields"] if f.get("type")=="slider"]
assert len(pwm_fields)==30 and all(f["max"]==65535 for f in pwm_fields)

assert not any("Aggregation" in f["ha"]["state_topic"] for p in pages for f in p["fields"])
inputs=[f for p in pages for f in p["fields"] if f["ha"]["state_topic"].endswith("Input")]
assert len(inputs)==10 and all(len(f["options"])==9 for f in inputs)

states=[f for p in pages for f in p["fields"] if f["ha"]["state_topic"].endswith("State")]
assert len(states)==4 and all(f["type"]=="switch" and "command_topic" in f["ha"] and not f.get("readonly") for f in states)
assert not any("ManualOverride" in f["ha"]["state_topic"] for p in pages for f in p["fields"])

assert pages[0]["id"]=="led" and not any(p["id"]=="home" for p in pages)
clock=next(p for p in pages if p["id"]=="clock")
assert all(any(f["ha"]["state_topic"].endswith(k) for f in clock["fields"]) for k in ("ActualDate","ActualTime"))
assert all(f["name"].startswith("Timezone: ") for f in clock["fields"] if "TimezoneRule" in f["ha"]["state_topic"])

relay=next(p for p in pages if p['id']=='relays')
phase_fields=[f for f in relay['fields'] if f['ha']['state_topic'].endswith(('MorningMode','AfternoonMode','EveningMode','NightMode'))]
assert len(phase_fields)==16 and all(f['type']=='switch' and f['ha']['payload_on']=='on' and f['ha']['payload_off']=='off' and 'options' not in f for f in phase_fields)

for key in ('MorningTime','AfternoonTime','EveningTime','NightTime','ActualTime','ActualDate'):
 field=next(f for p in pages for f in p['fields'] if f['ha']['state_topic'].endswith('/'+key))
 assert field['type']==('date' if key=='ActualDate' else 'time')

schedule=next(p for p in pages if p['id']=='schedule')
assert len([f for f in schedule['fields'] if f['type']=='time'])==12
assert all('visibleWhen' in f for f in schedule['fields'] if f['type']=='time')

assert not any(f["ha"]["state_topic"].endswith("State") for f in pwm["fields"])

diag=next(p for p in pages if p['id']=='diagnostics')
assert len(diag['fields'])==4 and all(f['type']=='text' and f['readonly'] for f in diag['fields'])
assert {f['ha']['state_topic'].rsplit('/',1)[-1] for f in diag['fields']}=={'ErrorCount','AlarmLast1','AlarmLast2','AlarmLast3'}
print('PASS: diagnostic page has four readonly Text fields shared with HA.')

# Every descriptor used by the schema must be enabled in this firmware.
features=(root/'AC-ESP.ino').read_text()
for kind in {f['type'] for page in pages for f in page['fields']}:
 if kind=='edit':
  assert '#define ARDPORTAL_ENABLE_CONTROL_EDIT (AC_ACTIVE_RELAYS > 0 || AC_ACTIVE_INPUTS > 0 || AC_ACTIVE_PWM > 0)' in features
  continue
 assert '#define ARDPORTAL_ENABLE_CONTROL_'+kind.upper()+' 1' in features,kind
print('PASS: all schema descriptor types are explicitly enabled in the ESP sketch.')
