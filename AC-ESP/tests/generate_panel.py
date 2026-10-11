from pathlib import Path
import json
esp=Path(__file__).resolve().parents[1]
def edit(key,label): return dict(key=key,type='edit',name=label,command=key)
def temporal(key,label,kind): return dict(key=key,type=kind,name=label,command=key)
def select(key,label,opts): return dict(key=key,type='select',name=label,command=key,options=opts)
def slider(key,label,lo,hi): return dict(key=key,type='slider',name=label,command=key,min=lo,max=hi,step=1)
groups=[]
def group(id,title,prefix,count,controls):
    defaults={c['key']:c['options'][0] if c['type']=='select' else c['min'] if c['type']=='slider' else '' for c in controls}
    groups.append(dict(id=id,title=title,prefix=prefix,count=count,controls=controls,defaults=defaults))
name=edit('Name','Nazwa (maks. 15 znaków)')
group('UIInputs','Wejścia','Sensor',8,[name,select('Type','Podłączone sterowanie',['notconnected']+['relay'+str(i) for i in range(1,5)]+['pwmoutput'+str(i) for i in range(1,7)])]+[edit(k,l) for k,l in [('CalibValue1','Kalibracja: wartość 1'),('CalibRawValue1','Kalibracja: ADC 1 (0–1023)'),('CalibValue2','Kalibracja: wartość 2'),('CalibRawValue2','Kalibracja: ADC 2 (0–1023)'),('MinValue','Minimum lub off'),('MaxValue','Maksimum lub off'),('CriticalMinValue','Minimum krytyczne lub off'),('CriticalMaxValue','Maksimum krytyczne lub off')]])
def output(pwm):
    return [name,select('ControlMode','Tryb',['manual','partofday','treshold']+(['pid'] if pwm else [])),dict(key='State',type='switch',name='Stan',command='State'),select('ControlDirection','Kierunek',['direct','reverse']),select('Input','Wejście analogowe',['none']+['sensor'+str(i) for i in range(1,9)]),edit('SensorsSetpoint','Wartość zadana'),edit('MaxDeviation','Połowa szerokości histerezy')]+[slider(k,l,0,65535) if pwm else dict(key=k,type='switch',name=l,command=k) for k,l in [('MorningMode','Rano'),('AfternoonMode','Popołudnie'),('EveningMode','Wieczór'),('NightMode','Noc')]]+([slider('ManualMode','Ręczne PWM',0,65535)]+[edit(k,k) for k in ['PidKp','PidKi','PidKd']] if pwm else [])
group('UIRelays','Przekaźniki','Relay',4,output(False)); group('UIPWM','Wyjścia PWM','PWMOutput',6,[c for c in output(True) if c['key']!='State'])
colors=['black','red','green','blue','cyan','magenta','yellow','white','wave','custom']
group('UILed','LED','',0,[dict(key='LedState',type='switch',name='Stan',command='LedState'),select('LedControlMode','Tryb',['manual','partofday']),select('LedManualMode','Kolor ręczny',colors),slider('CurrentBrightness','Jasność ręczna (%)',0,100)]+[edit(k,k+' (RGB 0–16777215)') for k in ['CustomColor']])
groups[3]['controls']+=sum(([select('LedColor'+p,label+' — kolor',colors),slider(p+'Brightness',label+' — jasność (%)',0,100)] for p,label in [('Morning','Rano'),('Afternoon','Popołudnie'),('Evening','Wieczór'),('Night','Noc')]),[])
for c in groups[3]['controls']:
    if c['key'] not in groups[3]['defaults']: groups[3]['defaults'][c['key']]=c['options'][0] if c['type']=='select' else c.get('min',False)
group('UISchedule','Harmonogram','',0,[select('ScheduleMode','Harmonogram: Tryb',['daily','weekdayweekend'])]+[temporal(prefix+p+'Time','Harmonogram: '+group_label+' — '+label,'time') for prefix,group_label in [('', 'Każdego dnia'),('Weekday','Pn–Pt'),('Weekend','Weekend')] for p,label in [('Morning','Rano'),('Afternoon','Popołudnie'),('Evening','Wieczór'),('Night','Noc')]])
group('UIClock','Zegar','',0,[temporal('ActualDate','Data lokalna','date'),temporal('ActualTime','Czas lokalny','time')]+sum(([select('TimezoneRule'+str(i)+'Week','Strefa czasowa: Reguła '+str(i)+' — tydzień',['first','second','third','fourth','last']),select('TimezoneRule'+str(i)+'DayOfWeek','Strefa czasowa: Reguła '+str(i)+' — dzień',['Sunday','Monday','Tuesday','Wednesday','Thursday','Friday','Saturday']),select('TimezoneRule'+str(i)+'Month','Strefa czasowa: Reguła '+str(i)+' — miesiąc',['January','February','March','April','May','June','July','August','September','October','November','December']),slider('TimezoneRule'+str(i)+'Hour','Strefa czasowa: Reguła '+str(i)+' — godzina',0,23),slider('TimezoneRule'+str(i)+'Offset','Strefa czasowa: Reguła '+str(i)+' — offset UTC',-12,14)] for i in (1,2)),[]))
group('UIFan','Wentylator','',0,[slider(k,'Fan: '+l,0,100) for k,l in [('FanStartTemp','Temperatura startu'),('FanMaxSpeedTemp','Temperatura pełnej prędkości'),('MaxIntTemp','Alarm temperatury')]])
group('UIBuzzer','Dźwięki','',0,[select('BuzzerOnStart','Dźwięk startu',['off','on']),select('BuzzerOnErrors','Dźwięk błędów',['off','on'])]+[dict(key='Buzzer'+key,type='switch',name='Dźwięki: '+label,command='Buzzer'+key) for key,label in [('Morning','Rano'),('Afternoon','Popołudnie'),('Evening','Wieczór'),('Night','Noc')]])
group('UIDiagnostics','Diagnostyka','',0,[])
# Metadata: persistent settings and telemetry cache, all keys are canonical Mega fields.
readouts={0:['Value','RawValue'],1:['State','SensorsValue'],2:['Value','SensorsValue'],3:['LedState'],4:['ActualPartOfDay'],5:['ClockRtcStatus','ActualTimezoneOffset','ActualDayOfWeek'],6:['InternalTemperature','InternalHumidity','FanPWMValue'],7:[],8:['ErrorCount','AlarmLast1','AlarmLast2','AlarmLast3']}
meta=[]; cache=[]
for n,g in enumerate(groups):
    keys=[c['key'] for c in g['controls'] if c['key']!='channel']
    suffixes=list(dict.fromkeys(keys+readouts[n]))
    for ch in range(1,(g['count'] or 1)+1):
        for s in suffixes: cache.append((n,ch,(g['prefix']+str(ch) if g['count'] else '')+s,s))
    meta.append((g['id'],g['count'],','.join(keys)))
extras=['ActualDate','ActualTime']
for k in extras:
    if not any(c[2]==k for c in cache): cache.append((255,1,k,k))
metadata='#pragma once\n#include "PanelWireIds.h"\nstruct PanelField { byte group,channel,property; };\n'
props=list(dict.fromkeys(c[3] for c in cache))
assert len(props)<256
for i,k in enumerate(props): metadata+='static const char PANEL_PROPERTY_'+str(i)+'[] PROGMEM = '+json.dumps(k)+';\n'
metadata+='static const char* const PANEL_PROPERTIES[] PROGMEM={'+','.join('PANEL_PROPERTY_'+str(i) for i in range(len(props)))+'};\n'
metadata+='static const PanelField panelFields[] PROGMEM = {\n'+''.join('{'+str(n)+','+str(ch)+','+str(props.index(prop))+'},\n' for n,ch,key,prop in cache)+'};\n'
metadata+='static PanelField panelField(uint16_t i) { PanelField f; memcpy_P(&f,&panelFields[i],sizeof(f)); return f; }\nstatic String panelFieldName(uint16_t i) { return acFieldName(panelWireId(i)); }\nstatic String panelFieldProperty(uint16_t i) { const char* property; memcpy_P(&property,PANEL_PROPERTIES+panelField(i).property,sizeof(property)); return String(FPSTR(property)); }\n'
# Host generator metadata only; the firmware reuses its existing UART dictionary.
(esp/'tests/panel_metadata.json').write_text(json.dumps([c[2] for c in cache]))
# Separate native HA definitions use canonical Mega topics; UI composites stay local.
profiles=[]; records=[]
for index,(n,ch,field,prop) in enumerate(cache):
    if field in ('ClockSyncStatus','ClockLastNtpTime'): continue
    control=next((c for c in groups[n]['controls'] if c['key']==prop),None) if n<len(groups) else None
    label=control['name'] if control else {'ErrorCount':'Aktywne alarmy','AlarmLast1':'Aktywny alarm 1','AlarmLast2':'Aktywny alarm 2','AlarmLast3':'Aktywny alarm 3'}.get(prop,prop) if n==8 else 'Harmonogram: Aktualna pora' if n==4 and prop=='ActualPartOfDay' else prop
    profile=dict(type='text',name=label)
    if control:
        if control['type']=='select': profile.update(type='select',options=[dict(value=v,name=v) for v in control['options']])
        elif control['type']=='slider': profile.update(type='slider',min=control['min'],max=control['max'],step=control['step'])
        elif control['type'] in ('switch','time','date'): profile.update(type=control['type'])
        else: profile.update(type='edit')
        profile['command']=True
        if prop=='ScheduleMode': profile['options']=[dict(value='daily',name='Każdego dnia'),dict(value='weekdayweekend',name='Pn–Pt i Weekend')]
    if prop in ('State','LedState'):
        profile=dict(type='switch',name='Stan',command=True)
    if field=='InternalTemperature': profile.update(unit_of_measurement='°C',device_class='temperature')
    if field=='InternalHumidity': profile.update(unit_of_measurement='%',device_class='humidity')
    if profile not in profiles: profiles.append(profile)
    records.append((index,profiles.index(profile)))

# The same scalar fields are rendered by the Portal and discovered by HA.
page_defs=[('led','LED',0),('relays','Przekaźniki',10),('schedule','Harmonogram',20),('inputs','Wejścia',30),('pwm','Wyjścia PWM',40),('clock','Zegar',50),('fan','Wentylator',60),('buzzer','Dźwięki',70),('diagnostics','Diagnostyka',80)]
pages=[dict(id=id,name=name,order=order,fields=[]) for id,name,order in page_defs]
group_page=[3,1,4,0,2,5,6,7,8]
commands=[]
for index,profile_index in records:
    n,ch,key,prop=cache[index]
    f=dict(profiles[profile_index])
    command=f.pop('command',False)
    command_key=f.pop('commandField',key)
    hashed=2166136261
    for b in key.encode(): hashed=((hashed^b)*16777619)&0xffffffff
    f['id']='M_'+format(hashed,'08x')
    f['persist']=bool(command and prop not in ('State','LedState','ActualTime','ActualDate'))
    if f['persist']:
        control=next(c for c in groups[n]['controls'] if c['key']==prop)
        default=control['options'][0] if control['type']=='select' else control['min'] if control['type']=='slider' else False if control['type']=='switch' else '0'
        if prop=='Name': default=''
        if prop in ('MinValue','MaxValue','CriticalMinValue','CriticalMaxValue'): default='off'
        if prop in ('CalibValue2',): default='1'
        if prop=='CalibRawValue2': default='1023'
        if prop.startswith('Buzzer') and control['type']=='switch': default=prop!='BuzzerNight'
        if prop in ('FanStartTemp','FanMaxSpeedTemp','MaxIntTemp'): default={'FanStartTemp':30,'FanMaxSpeedTemp':35,'MaxIntTemp':100}[prop]
        if prop in ('CustomColor',): default='16777215'
        if control['type']=='time':
            part=next(part for part in ('Morning','Afternoon','Evening','Night') if part in prop)
            default={'Morning':'09:00:00','Afternoon':'10:00:00','Evening':'19:00:00','Night':'22:00:00'}[part]
        f['default']=default
    if prop=='Name': f['name']='Nazwa'
    if n in (0,1,2): f['name']=['S','R','PWM'][n]+str(ch)+': '+f['name']
    if n==3: f['name']='LED: '+f['name']
    if not command: f['readonly']=True
    ha=dict(state_topic='stat/$device/'+key)
    if command:
        ha['command_topic']='cmnd/$device/'+command_key
        if f['type']!='switch': ha['optimistic']=True
    if f['type'] in ('binary_sensor','switch'): ha.update(payload_on='on',payload_off='off')
    if f['type']=='edit' and prop=='Name': ha.update(min=0,max=15)
    f['ha']=ha
    if n==4 and prop!='ScheduleMode' and command:
        mode_hash=2166136261
        for b in b'ScheduleMode': mode_hash=((mode_hash^b)*16777619)&0xffffffff
        f['visibleWhen']=dict(field='M_'+format(mode_hash,'08x'),equals='weekdayweekend' if key.startswith(('Weekday','Weekend')) else 'daily')
    if n==3:
        mode='manual' if key in ('LedManualMode','CurrentBrightness') else 'partofday' if key in ['LedColor'+part for part in ('Morning','Afternoon','Evening','Night')] + [part+'Brightness' for part in ('Morning','Afternoon','Evening','Night')] else None
        if mode:
            dependency_hash=2166136261
            for b in b'LedControlMode': dependency_hash=((dependency_hash^b)*16777619)&0xffffffff
            f['visibleWhen']=dict(field='M_'+format(dependency_hash,'08x'),equals=mode)
    if n in (1,2):
        mode=None
        if prop in ('MorningMode','AfternoonMode','EveningMode','NightMode'): mode='partofday'
        elif prop=='ManualMode': mode='manual'
        elif prop=='MaxDeviation': mode='treshold'
        elif prop in ('PidKp','PidKi','PidKd'): mode='pid'
        elif prop in ('ControlDirection','Input','SensorsSetpoint','SensorsValue'): mode=['treshold','pid'] if n==2 else 'treshold'
        if mode:
            mode_key=groups[n]['prefix']+str(ch)+'ControlMode'
            dependency_hash=2166136261
            for b in mode_key.encode(): dependency_hash=((dependency_hash^b)*16777619)&0xffffffff
            dependency_id='M_'+format(dependency_hash,'08x')
            f['visibleWhen']={'or':[dict(field=dependency_id,equals=value) for value in mode]} if isinstance(mode,list) else dict(field=dependency_id,equals=mode)
    page=group_page[n] if n<len(group_page) else 5
    pages[page]['fields'].append(f)
    commands.append(command_key if command else '')
led_order={'CustomColor':0}
pages[0]['fields'].sort(key=lambda f: led_order.get(f['ha']['state_topic'].rsplit('/',1)[-1],2))
# Three Mega parameters share one native light entity in both Portal and HA.
led=pages[0]['fields']
pages[5]['fields'][:]=[f for f in pages[5]['fields'] if not f['ha']['state_topic'].endswith('/ClockRtcStatus')]
led[:]=[f for f in led if f['ha']['state_topic'].rsplit('/',1)[-1] not in ('LedState','CurrentBrightness')]
f=next(f for f in led if f['ha']['state_topic'].endswith('/CustomColor'))
f.update(type='light',name='LED: Status',colorLabel='Custom Color',persist=False,
 default=dict(state='OFF',brightness=0,color=dict(r=255,g=255,b=255),color_mode='rgb'),
 controls=[dict(key='state',type='switch',name='LED: Stan',command='',on='ON',off='OFF'),
 dict(key='brightness',type='slider',name='LED: Aktualna jasność (%)',command='',min=0,max=100,step=1),
 dict(key='color',type='color',name='LED: CustomColor',command='')])
f['ha'].update(schema='json',brightness=True,brightness_scale=100,supported_color_modes=['rgb'])
f['ha'].pop('optimistic',None)
# Compile-time selections affect exactly the shared definitions used by Portal/HA.
import re
page_conditions=['1','AC_ACTIVE_RELAYS > 0','1','AC_ACTIVE_INPUTS > 0','AC_ACTIVE_PWM > 0','1','AC_ACTIVE_FAN','AC_ENABLE_BUZZER','AC_ACTIVE_DIAGNOSTICS','AC_ENABLE_DHT && !AC_ACTIVE_FAN']
regulation={'ControlDirection','Input','SensorsSetpoint','SensorsValue','MaxDeviation','PidKp','PidKi','PidKd'}
def field_condition(f):
    key=f['ha']['state_topic'].rsplit('/',1)[-1]
    m=re.match(r'(Relay|PWMOutput|Sensor)([1-8])(.*)',key)
    if m:
        cond={'Relay':'AC_ACTIVE_RELAYS','PWMOutput':'AC_ACTIVE_PWM','Sensor':'AC_ACTIVE_INPUTS'}[m[1]]+' >= '+m[2]
        if m[1]!='Sensor' and m[3] in regulation:cond+=' && AC_ACTIVE_INPUTS > 0'
        return cond
    if key=='ClockRtcStatus':return 'AC_ENABLE_RTC'
    if key=='BuzzerOnErrors':return 'AC_ACTIVE_DIAGNOSTICS'
    return '1'
def raw(text):return 'R"JSON('+text+')JSON"\n'
def encode_field(f):
    if 'options' not in f:return raw(json.dumps(f,ensure_ascii=False,separators=(',',':')))
    data=dict(f);options=data.pop('options');out=raw(json.dumps(data,ensure_ascii=False,separators=(',',':'))[:-1]+',"options":[')
    for j,option in enumerate(options):
        value=option['value'];cond='1';m=re.fullmatch(r'(relay|pwmoutput|sensor)([1-8])',value)
        if m:cond={'relay':'AC_ACTIVE_RELAYS','pwmoutput':'AC_ACTIVE_PWM','sensor':'AC_ACTIVE_INPUTS'}[m[1]]+' >= '+m[2]
        elif value in ('treshold','pid'):cond='AC_ACTIVE_INPUTS > 0'
        if cond!='1':out+='#if '+cond+'\n'
        out+=raw((',' if j else '')+json.dumps(option,ensure_ascii=False,separators=(',',':')))
        if cond!='1':out+='#endif\n'
    return out+raw(']}')
compiled_pages=pages+[dict(id='dht',name='DHT',order=60,fields=[f for f in pages[6]['fields'] if f['ha']['state_topic'].rsplit('/',1)[-1] in ('MaxIntTemp','InternalTemperature','InternalHumidity')])]
header='// Generated shared Portal/HA definitions, filtered by ESP feature defines.\n'
for i,page in enumerate(compiled_pages):
    encoded=json.dumps(page,ensure_ascii=False,separators=(',',':'))
    assert len(encoded.encode())<32768,(page['id'],len(encoded.encode()))
    header+='#if '+page_conditions[i]+'\nstatic const char PANEL_PAGE_'+str(i)+'[] PROGMEM =\n'
    head={k:v for k,v in page.items() if k!='fields'}
    header+=raw(json.dumps(head,ensure_ascii=False,separators=(',',':'))[:-1]+',"fields":[')
    for j,f in enumerate(page['fields']):
        cond=field_condition(f)
        if cond!='1':header+='#if '+cond+'\n'
        if j:header+=raw(',')
        header+=encode_field(f)
        if cond!='1':header+='#endif\n'
    header+=raw(']}')+';\n#endif\n'
header+='static const __FlashStringHelper* panelPageDefinition(byte index) { switch(index) {\n'
for i,cond in enumerate(page_conditions):header+='#if '+cond+'\ncase '+str(i)+': return FPSTR(PANEL_PAGE_'+str(i)+');\n#endif\n'
header+='default: return nullptr; }}\n'
(esp/'PanelSchema.h').write_text(header)
metadata+='static const uint16_t PANEL_FIELD_COUNT=sizeof(panelFields)/sizeof(panelFields[0]);\n'
assert all(not command or command==cache[i][2] for i,command in enumerate(commands))
metadata+='static const uint8_t PANEL_COMMAND_ENABLED[] PROGMEM={'+','.join('1' if key else '0' for key in commands)+'};\n'
metadata+='static String panelCommandField(uint16_t i) { return i<PANEL_FIELD_COUNT && pgm_read_byte(PANEL_COMMAND_ENABLED+i)?panelFieldName(i):String(); }\n'
(esp/'PanelMetadata.h').write_text(metadata)
(esp/'tests/panel_schema.json').write_text(json.dumps(pages,ensure_ascii=False,indent=2))
print('Shared Portal/HA:',len(pages),'pages,',len(cache),'fields,',sum(len(json.dumps(p,ensure_ascii=False,separators=(',',':')).encode()) for p in pages),'schema bytes')

# Keep the UART dictionary synchronized with the shared Portal/HA fields.
import subprocess,sys
subprocess.run([sys.executable,str(esp/"tests/generate_wire.py")],check=True)
