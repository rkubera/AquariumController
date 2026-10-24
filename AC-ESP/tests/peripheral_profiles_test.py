#!/usr/bin/env python3
"""Register the complete >32 KiB Peripherals page through real ArdPortal/ArdFS."""
from pathlib import Path
import json,re,subprocess,tempfile
root=Path(__file__).resolve().parents[1]
lib=Path('/Users/radoslawkubera/Documents/Developer/Arduino/libraries/ArdPortal')
fixture=re.split(r'\bint\s+main\s*\(\s*\)\s*\{',(lib/'tests/memory-host/dynamic-patch.cpp').read_text(),maxsplit=1)[0]
fixture=fixture.replace('../../src/',str(lib/'src')+'/')
page=next(p for p in json.loads((root/'tests/panel_schema.json').read_text()) if p['id']=='peripherals')
encoded=json.dumps(page,ensure_ascii=False,separators=(',',':'))
fields={f['ha']['state_topic'].rsplit('/',1)[-1]:f for f in page['fields']}
code=fixture+'\nstatic const char definition[]=R"PAGE('+encoded+')PAGE";\n'
code+='static const char* phId="'+fields['Sensor1PhModel']['id']+'";\n'
code+='static const char* leakId="'+fields['Sensor2LeakModel']['id']+'";\n'
code+='void registerPage(ArdPortal& p){assert(p.startAppConfigPageRegistration(reinterpret_cast<const __FlashStringHelper*>(definition)));for(unsigned i=0;i<100000&&p.appConfigPageRegistrationBusy();i++)p.serviceAppConfigPageRegistration();assert(!p.appConfigPageRegistrationBusy());assert(p._dynamic.count()=='+str(len(page['fields']))+');}\n'
code+=r'''
int main(){ArdPortal p;assert(p.begin());drain(p);registerPage(p);
 assert(p.getAppConfigValue(phId).asString()=="generic");
 assert(p.setAppConfigValue(phId,ArdJSON::JSONVar("sen0169v2")));
 assert(p.setAppConfigValue(leakId,ArdJSON::JSONVar("grove101020018")));drain(p);
 ArdPortal reboot;assert(reboot.begin());drain(reboot);registerPage(reboot);
 assert(reboot.getAppConfigValue(phId).asString()=="sen0169v2");
 assert(reboot.getAppConfigValue(leakId).asString()=="grove101020018");
}
'''
with tempfile.TemporaryDirectory(prefix='ac-profile-native-')as d:
 p=Path(d)/'test.cpp';p.write_text(code);out=Path(d)/'test'
 subprocess.run(['c++','-std=c++17','-fsanitize=address,undefined','-DESP8266','-DARDFS_HOST_TEST','-I'+str(lib/'tests/memory-host'),'-I'+str(lib/'src'),str(p),str(lib/'src/ArdFS.cpp'),'-o',str(out)],check=True)
 subprocess.run([str(out)],check=True)
print('PASS: full Peripherals flash registration ('+str(len(encoded.encode()))+' bytes), concrete model defaults and ArdFS restore.')
