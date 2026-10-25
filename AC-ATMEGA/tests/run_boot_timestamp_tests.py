#!/usr/bin/env python3
"""Actual boot timestamp tracker/formatter and clock-service trust gating."""
from pathlib import Path
import re,subprocess,tempfile
root=Path(__file__).resolve().parents[1]
s=(root/'Clock.ino').read_text()
def function(name):
 a=re.search(r'^void '+name+r'\(',s,re.M).start();b=s.index('{',a);depth=1;c=b+1
 while depth:
  depth+=(s[c]=='{')-(s[c]=='}');c+=1
 return s[a:c]
support=r'''
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <string>
#include "BootTimestamp.h"
using byte=uint8_t;
#define snprintf_P snprintf
#define PSTR(x) (x)
#define CLOCK_MIN_VALID_NTP_TIME 1577836800UL
constexpr int CONFIG_unix_timestamp_addr=0;
BootTimestamp megaBoot;
uint32_t ms=0;uint32_t millis(){return ms;}
time_t utc=0;time_t now(){return utc;}
struct TimeChangeRule{int offset;};
struct TZ {
 TimeChangeRule dst{120},std{60},custom{-330};bool customOffset=false;time_t last=0;
 time_t toLocal(time_t t,TimeChangeRule** rule){last=t;*rule=customOffset?&custom:(t<1792890000?&dst:&std);return t+(*rule)->offset*60;}
}myTZ;
int year(time_t t){return std::gmtime(&t)->tm_year+1900;}
int month(time_t t){return std::gmtime(&t)->tm_mon+1;}
int day(time_t t){return std::gmtime(&t)->tm_mday;}
int hour(time_t t){return std::gmtime(&t)->tm_hour;}
int minute(time_t t){return std::gmtime(&t)->tm_min;}
int second(time_t t){return std::gmtime(&t)->tm_sec;}
time_t clockGetLocalTime(){return utc;}
void clockReportHourDate(){}void configSaveUint32Value(time_t,int){}
bool megaConfigReady=true;uint8_t clockSyncStatus=0;unsigned reports=0;
void clockReportBoot(){++reports;}
'''
main=r'''
int main(){char text[32];
 ms=999;clockFormatBoot(text,sizeof(text));assert(std::string(text)=="999 ms since start");
 ms=12345;clockFormatBoot(text,sizeof(text));assert(std::string(text)=="12345 ms since start");
 megaBoot=BootTimestamp();ms=UINT32_MAX;clockFormatBoot(text,sizeof(text));assert(std::string(text)=="4294967295 ms since start");
 ms+=1001;clockFormatBoot(text,sizeof(text));assert(std::string(text)=="4294968296 ms since start");
 BootTimestamp saved=megaBoot;megaBoot.elapsedSeconds=UINT32_MAX;megaBoot.sampledMillis=ms-23;
 clockFormatBoot(text,sizeof(text));assert(std::string(text)=="4294967295023 ms since start");megaBoot=saved;
 assert(megaBoot.synchronize(1800000000,ms));uint32_t recorded=megaBoot.utc;
 assert(!megaBoot.synchronize(1800000001,ms)&&megaBoot.utc==recorded);
 assert(!megaBoot.synchronize(0,ms)&&!megaBoot.synchronize(UINT32_MAX,ms));
 // A correction changes the estimated boot date, not the elapsed counter.
 assert(megaBoot.synchronize(1800000100,ms)&&megaBoot.utc==recorded+100);
 megaBoot=BootTimestamp();ms=20000;megaBoot.tick(ms);utc=1792899000;
 clockMillisEvent();assert(megaBoot.utc==0&&reports==1);
 ms+=10000;megaBoot.tick(ms);clockSyncStatus=2;clockMillisEvent();assert(megaBoot.utc==0);
 clockSyncStatus=3;clockMillisEvent();assert(megaBoot.utc==0);
 clockSyncStatus=5;clockMillisEvent();assert(megaBoot.utc==uint32_t(utc)-30);
 recorded=megaBoot.utc;clockSyncStatus=1;utc+=1;clockMillisEvent();assert(megaBoot.utc==recorded);
 clockSyncStatus=4;utc+=100;clockMillisEvent();assert(megaBoot.utc==recorded+101);
 // DST fallback: identical local clock readings must carry different offsets.
 megaBoot.utc=1792888200;clockFormatBoot(text,sizeof(text));
 assert(std::string(text)=="2026-10-25 02:30:00 +02:00"&&myTZ.last==megaBoot.utc);
 megaBoot.utc=1792891800;clockFormatBoot(text,sizeof(text));assert(std::string(text)=="2026-10-25 02:30:00 +01:00");
 myTZ.customOffset=true;megaBoot.utc=1792888200;clockFormatBoot(text,sizeof(text));assert(std::string(text)=="2026-10-24 19:00:00 -05:30");
 // Multiple millis wraps with regular servicing do not reset elapsed time.
 BootTimestamp longRun;uint64_t total=0;
 for(unsigned step=0;step<28800;step++){total+=600000;longRun.tick(uint32_t(total));assert(longRun.elapsedSeconds==total/1000);}
 megaBoot=longRun;ms=uint32_t(total);clockFormatBoot(text,sizeof(text));assert(std::string(text)=="17280000000 ms since start");
 // Hardware reset clears the record. An ESP reconnect does not touch this object.
 megaBoot=BootTimestamp();ms=42;clockFormatBoot(text,sizeof(text));assert(std::string(text)=="42 ms since start");
}
'''
with tempfile.TemporaryDirectory(prefix='ac-boot-test-')as d:
 p=Path(d)/'test.cpp';p.write_text(support+function('clockFormatBoot')+'\n'+function('clockMillisEvent')+'\n'+main);exe=Path(d)/'test'
 subprocess.run(['c++','-std=c++11','-fsanitize=address,undefined','-I',str(root),str(p),'-o',str(exe)],check=True)
 subprocess.run([str(exe)],check=True)
print('PASS: boot ms/date fallback, millis wrap, RTC/NTP/manual trust, invalid clock rejection, jitter, correction, historical DST/offset and reset state.')
