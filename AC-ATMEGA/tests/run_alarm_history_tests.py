#!/usr/bin/env python3
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1]
code=r'''
#include "AlarmHistory.h"
#include <cassert>
int main(){
 AlarmHistory h;
 assert(!h.update(0,100));assert(h.count==0);
 assert(h.update(1,101));assert(h.codes[0]==1 && h.timestamps[0]==101);
 assert(!h.update(1,102));assert(h.timestamps[0]==101);
 assert(h.update(3,103));assert(h.codes[0]==2 && h.codes[1]==1);
 assert(h.update(2,104));assert(h.codes[0]==2 && h.codes[1]==0 && h.timestamps[0]==103);
 assert(h.update(3,105));assert(h.codes[0]==1 && h.timestamps[0]==105 && h.codes[1]==2 && h.codes[2]==0);
 assert(h.update(0,106));assert(h.codes[0]==0 && h.count==0);
 assert(h.update(uint32_t(1)<<25,107));assert(h.codes[0]==26 && h.timestamps[0]==107);
 assert(h.update((uint32_t(1)<<25)|(uint32_t(1)<<24),0));assert(h.codes[0]==25 && h.timestamps[0]==0);
 assert(!h.update(h.active,108));
 // Four active causes: oldest returns with its original time when a newer one clears.
 h.update(0,110);h.update(1,111);h.update(3,112);h.update(7,113);h.update(15,114);
 assert(h.count==4 && h.codes[0]==4 && h.codes[2]==2);
 h.update(7,115);assert(h.count==3 && h.codes[2]==1 && h.timestamps[2]==111);
 // A clock correction does not reorder existing alarms.
 h.update(15,90);assert(h.codes[0]==4 && h.timestamps[0]==90);
 h.update(3,120);assert(h.codes[0]==2 && h.codes[1]==1 && h.codes[2]==0);
 h.update(0,121);assert(h.count==0);
 h.update((uint32_t(1)<<27)-1,122);assert(h.count==27 && h.codes[0]==27);
 h.update(1,123);assert(h.count==1 && h.codes[0]==1 && h.timestamps[0]==122);

}
'''
with tempfile.TemporaryDirectory() as d:
 p=Path(d);(p/'test.cpp').write_text(code)
 subprocess.run(['c++','-std=c++11','-fsanitize=address,undefined','-I',str(root),str(p/'test.cpp'),'-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True)
print('PASS: active-only alarms, clearing, hidden-alarm recovery, stable timestamps, capacity and recurrence.')
