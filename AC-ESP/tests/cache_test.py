#!/usr/bin/env python3
"""Run actual shared cache and generated ID mapping, without Portal hardware."""
from pathlib import Path
import subprocess,tempfile,re,json
root=Path(__file__).resolve().parents[1]
support=(root/'tests/host_support.hpp').read_text().split('const int bufferOutSize')[0]
source=(root/'Panel.ino').read_text();cache=source[source.index('void panelClearCache()'):source.index('String panelCached(')]
panel_keys=json.loads((root/'tests/panel_metadata.json').read_text())
count=len(panel_keys)
pages=json.loads((root/'tests/panel_schema.json').read_text())
control_hashes={f['ha']['state_topic'].rsplit('/',1)[-1]:int(f['id'][2:],16) for page in pages for f in page['fields']}
for key in panel_keys:
 if key not in control_hashes:
  h=2166136261
  for b in key.encode():h=((h^b)*16777619)&0xffffffff
  control_hashes[key]=h
wire_count=len(re.findall(r'AC_WIRE_KEY_\d+\[\]',(root/'WireFields.h').read_text()))
fixture=f'#define AC_ACTIVE_FAN 1\n#define AC_ENABLE_DHT 1\n#include <bitset>\nconstexpr uint16_t AC_WIRE_FIELD_COUNT={wire_count},PANEL_FIELD_COUNT={count};\n#define memcpy_P memcpy\n#include "PanelWireIds.h"\nString bridgeTelemetry[AC_WIRE_FIELD_COUNT];std::bitset<PANEL_FIELD_COUNT> panelSeen,panelNeedsRender;\n'
fixture+='byte panelQueuedCount=0;String panelQueuedValue[8],panelQueuedActual[8];\n'
fixture+='static const uint32_t expectedHashes[]={'+','.join(str(control_hashes[k])+'UL' for k in panel_keys)+'};\n'
main=r'''
int main(){
 for(uint16_t i=0;i<PANEL_FIELD_COUNT;i++){
  assert(panelControlHash(i)==expectedHashes[i]);uint16_t id=panelWireId(i);assert(id&&id<=AC_WIRE_FIELD_COUNT&&wirePanelIndex(id)==i);
  assert(panelValue(i)=="?");panelStore(i,"long value with spaces: 12345678901234567890");
  assert(bridgeTelemetry[id-1]==panelValue(i)&&panelSeen[i]&&panelNeedsRender[i]);
  panelNeedsRender[i]=false;panelStore(i,"long value with spaces: 12345678901234567890");assert(!panelNeedsRender[i]);
  panelStore(i,"changed");assert(panelNeedsRender[i]&&bridgeTelemetry[id-1]=="changed");
 }
 assert(wirePanelIndex(0)==65535&&wirePanelIndex(65535)==65535);
 panelQueuedCount=1;panelQueuedValue[0]="queued";panelQueuedActual[0]="old";
 panelClearCache();assert(panelQueuedCount==0 && !panelQueuedValue[0].length() && !panelQueuedActual[0].length());for(uint16_t i=0;i<PANEL_FIELD_COUNT;i++){assert(panelValue(i)=="?"&&!panelSeen[i]&&!panelNeedsRender[i]);assert(bridgeTelemetry[panelWireId(i)-1]=="changed");}
 puts("PASS: actual shared panel/MQTT cache; generated ID round-trips, unchanged values, render flags and session invalidation.");
}
'''
with tempfile.TemporaryDirectory(prefix='ac-cache-test-') as tmp:
 tmp=Path(tmp);(tmp/'test.cpp').write_text(support+fixture+cache+main)
 subprocess.run(['c++','-std=c++11','-I',str(root),'-fsanitize=address,undefined','-fno-omit-frame-pointer',str(tmp/'test.cpp'),'-o',str(tmp/'test')],check=True)
 subprocess.run([str(tmp/'test')],check=True)
