from pathlib import Path
import json,zlib
root=Path(__file__).resolve().parents[1]
pages=json.loads((root/'tests/panel_schema.json').read_text())
keys={f['ha'][k].rsplit('/',1)[-1] for p in pages for f in p['fields'] for k in ('state_topic','command_topic') if k in f['ha']}
keys.update(['ClockRtcStatus','LedState','CurrentBrightness','Status','ErrorCount','Hostname','Ssid','MqttServer','WifiStatus','MqttStatus','SerialPartialTimeouts','SerialTxDroppedCommands','SerialCrcErrors','SerialOversizedFrames','SerialMalformedFrames','SerialValidFrames'])
keys.update('Sensor'+str(ch)+part for ch in range(1,9) for part in ('Device','Measurement'))
keys=sorted(keys);fingerprint=zlib.crc32(('AC-WIRE-2 PORTAL-FIELDS-2 CAPABILITIES-1\n'+'\n'.join(keys)+'\npersist='+','.join(sorted(f['ha']['state_topic'].rsplit('/',1)[-1] for page in pages for f in page['fields'] if f['persist']))).encode())
s='#pragma once\n// Generated numeric field dictionary. Both firmwares must use the same schema.\nstatic const uint32_t AC_WIRE_SCHEMA='+str(fingerprint)+'UL;\nstatic const uint32_t AC_CONFIG_SCHEMA=0x41430301UL;\n'
for i,k in enumerate(keys):s+='static const char AC_WIRE_KEY_'+str(i)+'[] PROGMEM = '+json.dumps(k)+';\n'
s+='static const char* const AC_WIRE_KEYS[] PROGMEM={'+','.join('AC_WIRE_KEY_'+str(i) for i in range(len(keys)))+'};\nstatic const uint16_t AC_WIRE_FIELD_COUNT=sizeof(AC_WIRE_KEYS)/sizeof(AC_WIRE_KEYS[0]);\n'
s+='''static String acFieldName(uint16_t id){if(!id||id>AC_WIRE_FIELD_COUNT)return "";const char* p;memcpy_P(&p,&AC_WIRE_KEYS[id-1],sizeof(p));return String(reinterpret_cast<const __FlashStringHelper*>(p));}
static uint16_t acFieldId(const String& name){int lo=0,hi=AC_WIRE_FIELD_COUNT-1;while(lo<=hi){int mid=(lo+hi)/2;const char* candidate;memcpy_P(&candidate,&AC_WIRE_KEYS[mid],sizeof(candidate));int cmp=strcmp_P(name.c_str(),candidate);if(!cmp)return mid+1;if(cmp<0)hi=mid-1;else lo=mid+1;}return 0;}
'''
(root/'WireFields.h').write_text(s)
# Mega contains only numeric identifiers; strings belong exclusively to ESP.
mega_keys=[k.replace('Mqtt','Broker') for k in keys]
t='#pragma once\n// Generated numeric controller identifiers. No field-name dictionary.\n'
t+='static const uint32_t AC_WIRE_SCHEMA='+str(fingerprint)+'UL;\nstatic const uint32_t AC_CONFIG_SCHEMA=0x41430301UL;\nstatic const uint16_t AC_WIRE_FIELD_COUNT='+str(len(keys))+';\n'
t+='namespace FieldId {enum Id : uint16_t {Invalid=0,'+','.join(k+'='+str(i+1) for i,k in enumerate(mega_keys))+'};}\n'
for family,count,func in [('Sensor',8,'sensorField'),('Relay',4,'relayField'),('PWMOutput',6,'pwmField')]:
 props=sorted(k[len(family)+1:] for k in mega_keys if k.startswith(family+'1'))
 first=mega_keys.index(family+'1'+props[0])+1
 assert all(mega_keys.index(family+str(ch)+part)+1==first+(ch-1)*len(props)+j for ch in range(1,count+1) for j,part in enumerate(props))
 t+='namespace '+family+'Field {enum Part : uint8_t {'+','.join(part+'='+str(j) for j,part in enumerate(props))+'};}\n'
 t+='static uint16_t '+func+'(uint8_t channel,'+family+'Field::Part part){return channel>=1&&channel<='+str(count)+'?'+str(first)+'+(channel-1)*'+str(len(props))+'+uint8_t(part):0;}\n'
 t+='static uint8_t '+func+'Channel(uint16_t id){return id>='+str(first)+'&&id<'+str(first+count*len(props))+'?(id-'+str(first)+')/'+str(len(props))+'+1:0;}\n'
(root.parent/'AC-ATMEGA'/'WireFields.h').write_text(t)
print('Wire dictionary:',len(keys),'fields, fingerprint',hex(fingerprint))

# Direct panel-index <-> wire-ID mapping; generated once, no runtime names scan.
import re
panel_keys=json.loads((root/'tests/panel_metadata.json').read_text())
wire_ids={k:i+1 for i,k in enumerate(keys)}
ids=[wire_ids[k] for k in panel_keys]
reverse=[65535]*len(keys)
for i,id in enumerate(ids):reverse[id-1]=i
mapping='#pragma once\nstatic const uint16_t PANEL_WIRE_IDS[] PROGMEM={'+','.join(map(str,ids))+'};\n'
mapping+='static const uint16_t WIRE_PANEL_INDEX[] PROGMEM={'+','.join(map(str,reverse))+'};\n'
mapping+='static uint16_t panelWireId(uint16_t i){uint16_t id;memcpy_P(&id,&PANEL_WIRE_IDS[i],sizeof(id));return id;}\n'
mapping+='static uint16_t wirePanelIndex(uint16_t id){if(!id||id>AC_WIRE_FIELD_COUNT)return 65535;uint16_t i;memcpy_P(&i,&WIRE_PANEL_INDEX[id-1],sizeof(i));return i;}\n'
hashes=[]
for key in panel_keys:
 h=2166136261
 for byte in key.encode():h=((h^byte)*16777619)&0xffffffff
 hashes.append(h)
mapping+='static const uint32_t PANEL_CONTROL_HASHES[] PROGMEM={'+','.join(str(h)+'UL' for h in hashes)+'};\n'
mapping+='static uint32_t panelControlHash(uint16_t i){uint32_t hash;memcpy_P(&hash,&PANEL_CONTROL_HASHES[i],sizeof(hash));return hash;}\n'
page_for_key={f['ha']['state_topic'].rsplit('/',1)[-1]:page for index,p in enumerate(pages) for page in [10 if p['id']=='peripherals' else index] for f in p['fields']}
mapping+='static const uint8_t PANEL_FIELD_PAGES[] PROGMEM={'+','.join(str(page_for_key.get(k,5 if k=="ClockRtcStatus" else 0)) for k in panel_keys)+'};\n'
mapping+='static uint8_t panelFieldPage(uint16_t i){uint8_t page;memcpy_P(&page,&PANEL_FIELD_PAGES[i],sizeof(page));return page==6&&!AC_ACTIVE_FAN&&AC_ENABLE_DHT?9:page;}\n'
(root/'PanelWireIds.h').write_text(mapping)

# Only native persistent fields may be used in the boot transaction.
persistent={f['ha']['state_topic'].rsplit('/',1)[-1] for page in pages for f in page['fields'] if f['persist']}
persistent.update(('CurrentBrightness','CustomColor'))
# Calibration before assignments; output modes last. Fan full-speed before start.
def restore_order(key):
 if key.endswith('Model'): return (-4,key)
 if key.endswith('Function'): return (-3,key)
 if key.endswith('Device'): return (-2,key)
 if key.endswith('Measurement'): return (-1,key)
 if key=='FanMaxSpeedTemp': return (0,key)
 if key=='FanStartTemp': return (4,key)
 if key.endswith('ControlMode'): return (5,key)
 if key.endswith('Type'): return (2,key)
 if key.endswith('Input'): return (3,key)
 return (1,key)
restore=sorted(persistent,key=restore_order)
ids=[wire_ids[k] for k in restore]
h='#pragma once\nstatic const uint16_t AC_RESTORE_COUNT='+str(len(ids))+';\n'
h+='static const uint16_t AC_RESTORE_IDS[] PROGMEM={'+','.join(map(str,ids))+'};\n'
h+='static uint16_t acRestoreId(uint16_t step){return step<AC_RESTORE_COUNT?pgm_read_word(AC_RESTORE_IDS+step):0;}\n'
h+='static inline bool acPersistentField(uint16_t id){for(uint16_t i=0;i<AC_RESTORE_COUNT;i++)if(acRestoreId(i)==id)return true;return false;}\n'
(root/'RestoreFields.h').write_text(h)
(root.parent/'AC-ATMEGA/RestoreFields.h').write_text(h)

mapping+='static const uint8_t PANEL_FIELD_PERSISTENT[] PROGMEM={'+','.join('1' if k in persistent else '0' for k in panel_keys)+'};\n'
mapping+='static bool panelFieldPersistent(uint16_t i){return i<sizeof(PANEL_FIELD_PERSISTENT)&&pgm_read_byte(PANEL_FIELD_PERSISTENT+i); }\n'
(root/'PanelWireIds.h').write_text(mapping)

# Numeric capability descriptors: the same filter drives boot restore and telemetry.
import re
peripheral_keys={f['ha']['state_topic'].rsplit('/',1)[-1] for p in pages if p['id']=='peripherals' for f in p['fields']}
peripheral_keys.update('Sensor'+str(ch)+part for ch in range(1,9) for part in ('Device','Measurement'))
regulation={'ControlDirection','Input','SensorsSetpoint','SensorsValue','MaxDeviation','PidKp','PidKi','PidKd'}
def feature(key):
 m=re.match(r'(Relay|PWMOutput|Sensor)([1-8])(.*)',key)
 if m:
  group={'Relay':1,'PWMOutput':2,'Sensor':3}[m[1]]
  return group+(int(m[2])<<4)+(256 if m[1]!='Sensor' and m[3] in regulation else 0)+(1024 if key in peripheral_keys else 0)
 if key in ('InternalTemperature','InternalHumidity','MaxIntTemp'):return 4
 if key=='ClockRtcStatus':return 5
 if key.startswith('Buzzer'):return 6+(512 if key=='BuzzerOnErrors' else 0)
 if key in ('FanStartTemp','FanMaxSpeedTemp','FanPWMValue'):return 7
 if key in ('Status','ErrorCount','AlarmLast1','AlarmLast2','AlarmLast3','MegaLastBoot'):return 8
 return 0
h='#pragma once\n#include "ControllerFeatures.h"\nstatic const uint16_t AC_FIELD_FEATURES[] PROGMEM={'+','.join(str(feature(k)) for k in keys)+'};\n'
h+=r'''static bool acFieldSupported(uint16_t id,const ControllerFeatures& caps){
 if(!id||id>sizeof(AC_FIELD_FEATURES)/sizeof(AC_FIELD_FEATURES[0]))return false;
 uint16_t descriptor=pgm_read_word(AC_FIELD_FEATURES+id-1);uint8_t group=descriptor&15,channel=(descriptor>>4)&15;
 if((descriptor&1024)&&caps.has(ControllerFeatures::NoPeripherals))return false;
 if((descriptor&256)&&!caps.inputs)return false;
 if((descriptor&512)&&!caps.diagnostics())return false;
 switch(group){case 1:return channel<=caps.relays;case 2:return channel<=caps.pwm;case 3:return channel<=caps.inputs;case 4:return caps.has(ControllerFeatures::Dht);case 5:return caps.has(ControllerFeatures::Rtc);case 6:return caps.has(ControllerFeatures::Buzzer);case 7:return caps.has(ControllerFeatures::Fan);case 8:return caps.diagnostics();default:return true;}
}
static uint16_t acActiveRestoreCount(const ControllerFeatures& caps){uint16_t count=0;for(uint16_t i=0;i<AC_RESTORE_COUNT;++i)if(acFieldSupported(acRestoreId(i),caps))++count;return count;}
static uint16_t acActiveRestoreId(uint16_t step,const ControllerFeatures& caps){for(uint16_t i=0;i<AC_RESTORE_COUNT;++i){uint16_t id=acRestoreId(i);if(acFieldSupported(id,caps)){if(!step)return id;--step;}}return 0;}
'''
(root/'FeatureFields.h').write_text(h)
(root.parent/'AC-ATMEGA/FeatureFields.h').write_text(h)

# Empty String objects also cost RAM. Omit caches for compiled-out hardware,
# while keeping every external numeric ID unchanged.
h='''#pragma once
#if AC_ENABLE_PERIPHERALS && AC_ACTIVE_RELAYS==4 && AC_ACTIVE_PWM==6 && AC_ACTIVE_INPUTS==8 && AC_ENABLE_DHT && AC_ENABLE_RTC && AC_ENABLE_BUZZER && AC_ACTIVE_FAN
static const uint16_t AC_TELEMETRY_COUNT=AC_WIRE_FIELD_COUNT;
static uint16_t acTelemetryIndex(uint16_t offset){return offset<AC_TELEMETRY_COUNT?offset:UINT16_MAX;}
#else
static const uint16_t AC_ACTIVE_TELEMETRY_IDS[] PROGMEM={
'''
for index,key in enumerate(keys):
 descriptor=feature(key);group=descriptor&15;channel=(descriptor>>4)&15
 condition={0:'1',1:f'AC_ACTIVE_RELAYS >= {channel}',2:f'AC_ACTIVE_PWM >= {channel}',3:f'AC_ACTIVE_INPUTS >= {channel}',4:'AC_ENABLE_DHT',5:'AC_ENABLE_RTC',6:'AC_ENABLE_BUZZER',7:'AC_ACTIVE_FAN',8:'AC_ACTIVE_DIAGNOSTICS'}[group]
 if descriptor&1024:condition+=' && AC_ENABLE_PERIPHERALS'
 if descriptor&256:condition+=' && AC_ACTIVE_INPUTS > 0'
 if descriptor&512:condition+=' && AC_ACTIVE_DIAGNOSTICS'
 h+='#if '+condition+'\n'+str(index+1)+',\n#endif\n'
h+='''};
static const uint16_t AC_TELEMETRY_COUNT=sizeof(AC_ACTIVE_TELEMETRY_IDS)/sizeof(AC_ACTIVE_TELEMETRY_IDS[0]);
static uint16_t acTelemetryIndex(uint16_t offset){
 uint16_t id=offset+1;int lo=0,hi=AC_TELEMETRY_COUNT-1;
 while(lo<=hi){int mid=(lo+hi)/2;uint16_t candidate=pgm_read_word(AC_ACTIVE_TELEMETRY_IDS+mid);
  if(candidate==id)return mid;if(candidate<id)lo=mid+1;else hi=mid-1;
 }
 return UINT16_MAX;
}
#endif
class ActiveTelemetryCache {
 String values[AC_TELEMETRY_COUNT];
#if !(AC_ENABLE_PERIPHERALS && AC_ACTIVE_RELAYS==4 && AC_ACTIVE_PWM==6 && AC_ACTIVE_INPUTS==8 && AC_ENABLE_DHT && AC_ENABLE_RTC && AC_ENABLE_BUZZER && AC_ACTIVE_FAN)
 String unsupported;
#endif
public:
 String& operator[](uint16_t offset){
#if AC_ENABLE_PERIPHERALS && AC_ACTIVE_RELAYS==4 && AC_ACTIVE_PWM==6 && AC_ACTIVE_INPUTS==8 && AC_ENABLE_DHT && AC_ENABLE_RTC && AC_ENABLE_BUZZER && AC_ACTIVE_FAN
  return values[offset];
#else
  uint16_t i=acTelemetryIndex(offset);return i==UINT16_MAX?unsupported:values[i];
#endif
 }
};
'''
(root/'ActiveTelemetryCache.h').write_text(h)
