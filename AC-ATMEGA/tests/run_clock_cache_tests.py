from pathlib import Path
import re,tempfile,subprocess
root=Path(__file__).resolve().parents[1]
s=(root/'Clock.ino').read_text()
a=s.index('static bool clockLocalCacheValid=');b=s.index('void clockInit()',a)
c=s.index('time_t clockGetLocalTime() {');d=s.index('\nvoid clockNTPSynchronize_cb',c)
support='''#include <cassert>
#include <cstdint>
using time_t=uint32_t;
struct tmElements_t {uint8_t Second,Minute,Hour,Wday,Day,Month,Year;};
int globalSecond,globalMinute,globalHour,globalWeekDay,globalMonthDay,globalMonth,globalYear;
uint32_t utc=100;time_t now(){return utc;}
int converts=0,breaks=0,offset=3600;void* tcr=nullptr;
struct TZ {time_t toLocal(time_t t,void**){++converts;return t+offset;}}myTZ;
void breakTime(time_t t,tmElements_t& p){++breaks;p={uint8_t(t%60),uint8_t(t/60%60),uint8_t(t/3600%24),5,9,10,56};}
int tmYearToCalendar(int y){return y+1970;}
'''
main='''int main(){
 assert(clockGetLocalTime()==3700 && converts==1 && breaks==1);
 for(int i=0;i<100;i++){globalHour=99;assert(clockGetLocalTime()==3700 && globalHour==1);}
 assert(converts==1 && breaks==1 && globalYear==2026);
 ++utc;assert(clockGetLocalTime()==3701 && converts==2);
 offset=7200;clockInvalidateLocalTime();assert(clockGetLocalTime()==7301 && converts==3 && globalHour==2);
 utc=50;assert(clockGetLocalTime()==7250 && converts==4);
 clockInvalidateLocalTime();clockGetLocalTime();assert(converts==5);
}'''
with tempfile.TemporaryDirectory() as td:
 p=Path(td)/'test.cpp';p.write_text(support+s[a:b]+s[c:d]+main);exe=Path(td)/'test'
 subprocess.run(['c++','-std=c++11','-fsanitize=address,undefined',str(p),'-o',str(exe)],check=True);subprocess.run([str(exe)],check=True)
print('PASS: actual local clock cache; repeated reads, component restoration, second tick, timezone invalidation and backwards clock jump')
