#!/usr/bin/env python3
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1]
source=r'''
#include "SchedulePlan.h"
#include <cassert>
#include <cstdio>
int main(){
 const uint16_t times[3][4]={{540,600,1140,1320},{360,600,1140,1320},{540,720,1200,1380}};
 for(uint8_t day=1;day<=7;day++)assert(SchedulePlan::select(0,day,540,times).phase==0);
 auto friday=SchedulePlan::select(1,6,360,times);assert(friday.bank==1&&friday.phase==0&&friday.elapsed==0);
 auto saturday=SchedulePlan::select(1,7,60,times);assert(saturday.bank==1&&saturday.phase==3&&saturday.elapsed==180);
 saturday=SchedulePlan::select(1,7,540,times);assert(saturday.bank==2&&saturday.phase==0&&saturday.elapsed==0);
 auto sunday=SchedulePlan::select(1,1,60,times);assert(sunday.bank==2&&sunday.phase==3&&sunday.elapsed==120);
 auto monday=SchedulePlan::select(1,2,60,times);assert(monday.bank==2&&monday.phase==3&&monday.elapsed==120);
 monday=SchedulePlan::select(1,2,360,times);assert(monday.bank==1&&monday.phase==0);
 const uint16_t duplicate[3][4]={{600,600,600,600},{600,600,600,600},{600,600,600,600}};
 assert(SchedulePlan::select(0,2,600,duplicate).phase==3);
 puts("PASS: daily and weekday/weekend schedules, Friday/Saturday and Sunday/Monday carryover, exact boundaries and duplicate starts");
}
'''
with tempfile.TemporaryDirectory() as tmp:
 p=Path(tmp)/'test.cpp';p.write_text(source)
 subprocess.run(['c++','-std=c++11','-I',str(root),'-fsanitize=address,undefined',str(p),'-o',tmp+'/test'],check=True)
 subprocess.run([tmp+'/test'],check=True)
