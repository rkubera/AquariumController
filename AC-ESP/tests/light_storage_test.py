#!/usr/bin/env python3
"""Use real ArdPortal/ArdFS to verify a volatile light plus explicit settings."""
from pathlib import Path
import json,subprocess,tempfile,re
root=Path(__file__).resolve().parents[1]
lib=Path('/Users/radoslawkubera/Documents/Developer/Arduino/libraries/ArdPortal')
fixture=re.split(r'\bint\s+main\s*\(\s*\)\s*\{',(lib/'tests/memory-host/dynamic-patch.cpp').read_text(),maxsplit=1)[0]
fixture=fixture.replace('../../src/',str(lib/'src')+'/')
light=next(f for p in json.loads((root/'tests/panel_schema.json').read_text())for f in p['fields']if f['type']=='light')
page=json.dumps(dict(id='led',name='LED',order=0,fields=[light]),ensure_ascii=False,separators=(',',':'))
code=fixture+'\nconst char* definition=R"PAGE('+page+')PAGE";\nconst char* id="'+light['id']+'";\n'+r'''
void registerLight(ArdPortal& p){assert(p.startAppConfigPageRegistration(String(definition)));for(int i=0;i<1000&&p.appConfigPageRegistrationBusy();i++)p.serviceAppConfigPageRegistration();assert(p._dynamic.count()==1);}
int main(){using V=ArdJSON::JSONVar;ArdPortal p;assert(p.begin());drain(p);registerLight(p);
 V settings=V::object();settings["color"]=16777215;settings["brightness"]=30;
 assert(p.setAppConfigValue("LedSettings",settings));drain(p);
 uint32_t commits=p._storage.commits();
 auto field=p._dynamic.field(id);
 assert(p._appControls.appControl(field,1,V(80),ArdPortal::ChangeSource::Portal));
 assert(p.getAppConfigValue(id)["brightness"].asDouble()==80&&!p._savePending);
 assert(p._appControls.appControl(field,0,V("ON"),ArdPortal::ChangeSource::Portal));
 assert(!p._savePending);drain(p);assert(p._storage.commits()==commits);
 assert(p.getAppConfigValue("LedSettings")["brightness"].asDouble()==30);
 auto mqtt=p.getAppConfigValue(id);mqtt["brightness"]=75;
 assert(p._appControls.acceptMqttState(id,mqtt));p._appControls.prepareMqtt(fakeMillis);
 assert(!p._savePending&&p._appControls._mqttAppQueue.length()==0&&p.getAppConfigValue(id)["brightness"].asDouble()==75);
 mqtt["brightness"]=80;assert(p._appControls.acceptMqttState(id,mqtt));
 V color=V::object();color["r"]=1;color["g"]=2;color["b"]=3;
 assert(p._appControls.appControl(field,2,color,ArdPortal::ChangeSource::Portal));assert(!p._savePending);
 settings["color"]=66051;assert(p.setAppConfigValue("LedSettings",settings));drain(p);
 assert(p.getAppConfigValue(id)["brightness"].asDouble()==80);
 assert(!p._appConfig.hasOwnProperty(id)&&p._appConfig["LedSettings"]["brightness"].asDouble()==30);
 ArdPortal restored;assert(restored.begin());drain(restored);registerLight(restored);
 assert(restored.getAppConfigValue(id)["brightness"].asDouble()==0);
 assert(restored.getAppConfigValue("LedSettings")["brightness"].asDouble()==30&&restored.getAppConfigValue("LedSettings")["color"].asDouble()==66051);
 assert(restored._appControls.appControl(restored._dynamic.field(id),1,V(2),ArdPortal::ChangeSource::Portal));
 settings["brightness"]=2;assert(restored.setAppConfigValue("LedSettings",settings));drain(restored);
 ArdPortal rebooted;assert(rebooted.begin());drain(rebooted);assert(rebooted.getAppConfigValue("LedSettings")["brightness"].asDouble()==2);
}
'''
with tempfile.TemporaryDirectory()as d:
 p=Path(d)/'test.cpp';p.write_text(code);out=Path(d)/'test'
 subprocess.run(['c++','-std=c++17','-fsanitize=address,undefined','-DESP8266','-DARDFS_HOST_TEST','-I'+str(lib/'tests/memory-host'),'-I'+str(lib/'src'),str(p),str(lib/'src/ArdFS.cpp'),'-o',str(out)],check=True)
 subprocess.run([str(out)],check=True)
print('PASS: actual ArdPortal/ArdFS: no auto-save for light; explicit manual/color journal saves; temporary brightness and power excluded across restarts.')
