#!/usr/bin/env python3
"""Test actual DS1307 codec and cooperative TWI state machine with fake registers."""
from pathlib import Path
import subprocess
import tempfile
root=Path(__file__).resolve().parents[1]
support=r'''
#include <ctime>
#define _BV(x) (1<<(x))
enum {TWINT=7,TWEN=2,TWSTA=5,TWSTO=4,TWEA=6};
enum {TW_START=0x08,TW_REP_START=0x10,TW_MT_SLA_ACK=0x18,TW_MT_DATA_ACK=0x28,TW_MR_SLA_ACK=0x40,TW_MR_DATA_ACK=0x50,TW_MR_DATA_NACK=0x58};
#define F_CPU 16000000UL
#define INPUT_PULLUP 2
byte TWCR=0,TWBR=0,TWSR=0,TWDR=0;
byte clockRtcStatus=0,clockSyncStatus=0;
uint32_t clockTimeGeneration=0;
time_t mockUtc=0;
int timeChanges=0;
time_t now() { return mockUtc; }
void setTime(time_t utc) { mockUtc=utc; timeChanges++; }
void clockReportSyncState() {}
String lastRtcDiagnostic;
bool wireDiagnostic(const String& command) { lastRtcDiagnostic=command; return true; }
struct tmElements_t { byte Second,Minute,Hour,Wday,Day,Month,Year; };
time_t makeTime(tmElements_t e) {
  std::tm t={}; t.tm_sec=e.Second; t.tm_min=e.Minute; t.tm_hour=e.Hour;
  t.tm_mday=e.Day; t.tm_mon=e.Month-1; t.tm_year=e.Year+70;
  return timegm(&t);
}
void breakTime(time_t utc,tmElements_t &e) {
  std::tm *t=gmtime(&utc);
  e.Second=t->tm_sec; e.Minute=t->tm_min; e.Hour=t->tm_hour;
  e.Wday=t->tm_wday+1; e.Day=t->tm_mday; e.Month=t->tm_mon+1; e.Year=t->tm_year-70;
}
'''
tests=r'''
void reply(byte status,byte data=0) { TWSR=status; TWDR=data; TWCR=_BV(TWEN)|_BV(TWINT); rtcService(); }
void startRead() { TWCR=_BV(TWEN); rtcService(); assert(rtcState==1); reply(TW_START); reply(TW_MT_SLA_ACK); reply(TW_MT_DATA_ACK); reply(TW_REP_START); reply(TW_MR_SLA_ACK); }
void readData(const byte *data) { for(int i=0;i<7;i++) reply(i==6 ? TW_MR_DATA_NACK : TW_MR_DATA_ACK,data[i]); }
int main() {
  byte data[]={0x56,0x34,0x12,3,0x06,0x10,0x26};
  time_t utc=0; assert(rtcDecodeTime(data,utc));
  byte encoded[7]; assert(rtcEncodeTime(utc,encoded)); assert(!memcmp(data,encoded,7));
  byte bad[7]; memcpy(bad,data,7); bad[0]|=0x80; assert(!rtcDecodeTime(bad,utc));
  memcpy(bad,data,7); bad[1]=0x6a; assert(!rtcDecodeTime(bad,utc));
  memcpy(bad,data,7); bad[4]=0x31; bad[5]=0x02; assert(!rtcDecodeTime(bad,utc));
  memcpy(bad,data,7); bad[4]=0x29; bad[5]=0x02; bad[6]=0x24; assert(rtcDecodeTime(bad,utc));
  bad[6]=0x25; assert(!rtcDecodeTime(bad,utc));
  memcpy(bad,data,7); bad[2]=0x52; assert(rtcDecodeTime(bad,utc)); tmElements_t t; breakTime(utc,t); assert(t.Hour==0);
  bad[2]=0x72; assert(rtcDecodeTime(bad,utc)); breakTime(utc,t); assert(t.Hour==12);
  assert(!rtcEncodeTime(0,encoded));
  rtcInit(); startRead(); readData(data); assert(clockRtcStatus==1 && clockSyncStatus==5 && timeChanges==1);
  //Periodic reads detect CH=0 with a stopped crystal, then recover on progress.
  ticks+=5001; startRead(); readData(data); assert(clockRtcStatus==3);
  ticks+=5001; startRead(); readData(data); assert(clockRtcStatus==3);
  byte advancing[7]; memcpy(advancing,data,7); advancing[0]=0x57;
  ticks+=5001; int restored=timeChanges; startRead(); readData(advancing);
  assert(clockRtcStatus==1 && timeChanges==restored);
  //An accepted NTP/manual correction while a boot read is in flight takes precedence.
  rtcInit(); startRead(); clockTimeGeneration++; setTime(1800000000); readData(data); assert(now()==1800000000);
  //NTP may be pending but the battery-backed clock remains a usable fallback.
  rtcInit(); clockSyncStatus=2; startRead(); readData(data); assert(clockSyncStatus==5);
  //Hardware completion must win over a late cooperative poll.
  rtcInit(); TWCR=_BV(TWEN); rtcService(); ticks+=50; reply(TW_START);
  assert(rtcState==2);
  //No bus response: return promptly; timeout and retry without busy waiting.
  rtcInit(); TWCR=_BV(TWEN); rtcService(); TWCR=_BV(TWEN);
  rtcService(); assert(rtcState==1); ticks+=10; rtcService(); assert(rtcState==0 && clockRtcStatus==2);
  ticks+=5000; startRead(); reply(0x58); assert(clockRtcStatus==2 && rtcState==0);
  rtcInit(); TWCR=_BV(TWEN); rtcService(); reply(TW_START); reply(0x20);
  assert(clockRtcStatus==2 && lastRtcDiagnostic=="no ACK from RTC at 0x68 phase=2 status=32");
  //Queue a newer correction during an active write: it must remain pending.
  rtcInit(); rtcScheduleWrite(); TWCR=_BV(TWEN); rtcService();
  reply(TW_START); reply(TW_MT_SLA_ACK); reply(TW_MT_DATA_ACK);
  rtcScheduleWrite(); for(int i=0;i<7;i++) reply(TW_MT_DATA_ACK);
  assert(rtcWritePending);
  TWCR=_BV(TWEN); rtcService(); reply(TW_START); reply(TW_MT_SLA_ACK); reply(TW_MT_DATA_ACK);
  for(int i=0;i<7;i++) reply(TW_MT_DATA_ACK);
  assert(!rtcWritePending && rtcVerifyPending && clockRtcStatus==0);
  assert(rtcEncodeTime(now(),encoded)); startRead(); readData(encoded); assert(clockRtcStatus==1 && !rtcVerifyPending);
  puts("PASS: DS1307 BCD, calendar/leap years, 12/24h, halted clock, UTC, boot fallback, NTP precedence, cooperative I2C timeout/NACK/retry, pending writes and readback verification.");
}
'''
with tempfile.TemporaryDirectory(prefix='aquarium-rtc-') as tmp:
    tmp=Path(tmp); (tmp/'util').mkdir(); (tmp/'util/twi.h').write_text('')
    (tmp/'test.cpp').write_text((root/'tests/host_support.hpp').read_text()+support+(root/'RTC.ino').read_text()+tests)
    subprocess.run(['c++','-std=c++11','-I',str(tmp),'-fsanitize=address,undefined','-fno-omit-frame-pointer',str(tmp/'test.cpp'),'-o',str(tmp/'test')],check=True)
    subprocess.run([str(tmp/'test')],check=True)
