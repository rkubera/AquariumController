//Tiny RTC DS1307, address 0x68. Owns the otherwise unused Mega TWI peripheral.
//One hardware step per loop; no Wire waits or ISR. RTC stores UTC, never local time.
#include <util/twi.h>
static byte rtcState=0,rtcIndex=0,rtcRegisters[7];
static byte rtcReportedFault=255,rtcFaultState=0,rtcFaultStatus=0;
static unsigned long rtcLastFaultReport=0;
static bool rtcWriting=false,rtcWritePending=false,rtcBootRead=true;
static unsigned long rtcStepMillis=0,rtcRetryMillis=0,rtcStopMillis=0;
static uint32_t rtcWriteGeneration=0,rtcActiveGeneration=0,rtcBootTimeGeneration=0;
static bool rtcVerifyPending=false,rtcPreviousSample=false;
static uint32_t rtcPreviousUtc=0;
static unsigned long rtcPreviousMillis=0;

bool rtcDecodeBcd(byte encoded,byte maximum,byte &decoded) {
  if ((encoded&15)>9 || (encoded>>4)>9) return false;
  decoded=(encoded>>4)*10+(encoded&15);
  return decoded<=maximum;
}

bool rtcDecodeTime(const byte *data,time_t &utc) {
  if (data[0]&0x80) return false; //CH: oscillator halted, time cannot be trusted.
  tmElements_t tm;
  byte years,hours;
  if (!rtcDecodeBcd(data[0],59,tm.Second) || !rtcDecodeBcd(data[1],59,tm.Minute)) return false;
  if (data[2]&0x40) {
    if (!rtcDecodeBcd(data[2]&0x1f,12,hours) || hours==0 || (data[2]&0x80)) return false;
    tm.Hour=hours%12+((data[2]&0x20) ? 12 : 0);
  } else if (!rtcDecodeBcd(data[2],23,tm.Hour)) return false;
  if (data[3]<1 || data[3]>7 || !rtcDecodeBcd(data[4],31,tm.Day) || tm.Day==0 ||
      !rtcDecodeBcd(data[5],12,tm.Month) || tm.Month==0 || !rtcDecodeBcd(data[6],99,years) || years<20) return false;
  tm.Year=years+30; //TimeLib counts years from 1970; DS1307 from 2000.
  utc=makeTime(tm);
  tmElements_t checked;
  breakTime(utc,checked);
  return checked.Year==tm.Year && checked.Month==tm.Month && checked.Day==tm.Day;
}

byte rtcEncodeBcd(byte value) { return (value/10)*16+value%10; }

bool rtcEncodeTime(time_t utc,byte *data) {
  tmElements_t tm; breakTime(utc,tm);
  if (tm.Year<50 || tm.Year>129) return false; //2020..2099
  data[0]=rtcEncodeBcd(tm.Second); //Clears CH, starts oscillator.
  data[1]=rtcEncodeBcd(tm.Minute); data[2]=rtcEncodeBcd(tm.Hour); //24-hour mode
  data[3]=tm.Wday; data[4]=rtcEncodeBcd(tm.Day);
  data[5]=rtcEncodeBcd(tm.Month); data[6]=rtcEncodeBcd(tm.Year-30);
  return true;
}

void rtcScheduleWrite() {
  if(!megaFeatures.has(ControllerFeatures::Rtc))return;
  rtcWriteGeneration++;
  rtcWritePending=true;
}

void rtcInit() {
  pinMode(20,INPUT_PULLUP); pinMode(21,INPUT_PULLUP);
  TWSR=0; TWBR=(F_CPU/100000UL-16)/2; TWCR=_BV(TWEN);
  rtcState=0; rtcBootRead=true; rtcWritePending=false;
  rtcReportedFault=255; rtcLastFaultReport=0;
  clockRtcStatus=0; rtcRetryMillis=millis(); rtcPreviousSample=false;
  rtcBootTimeGeneration=clockTimeGeneration; rtcVerifyPending=false;
}

void rtcAdvance(byte state,byte control) {
  rtcState=state; rtcStepMillis=millis();
  TWCR=_BV(TWINT)|_BV(TWEN)|control;
}

void rtcReportFault(byte fault) {
  if (fault==rtcReportedFault && (fault==0 || millis()-rtcLastFaultReport<30000UL)) return;
  String reason=fault==0 ? String("bus ok") : fault==1 ? String("I2C timeout") :
    fault==3 ? String("I2C STOP stuck") :
    rtcFaultStatus==0x20 || rtcFaultStatus==0x48 ? String("no ACK from RTC at 0x68") : String("I2C unexpected status");
  if (fault) reason+=" phase="+String(rtcFaultState)+" status="+String(rtcFaultStatus);
  if (wireDiagnostic(reason)) { rtcReportedFault=fault; rtcLastFaultReport=millis(); }
}

void rtcFinish(bool success,byte fault=2) {
  rtcFaultState=rtcState; rtcFaultStatus=TWSR&0xf8;
  TWCR=_BV(TWINT)|_BV(TWEN)|_BV(TWSTO);
  rtcStopMillis=millis();
  rtcState=0; rtcRetryMillis=millis()+5000UL;
  if (!success) { clockRtcStatus=2; rtcReportFault(fault); clockReportSyncState(); return; }
  rtcReportFault(0);
  if (rtcWriting) {
    if (rtcActiveGeneration==rtcWriteGeneration) rtcWritePending=false;
    rtcVerifyPending=true; rtcBootRead=true; rtcPreviousSample=false;
    rtcRetryMillis=millis();
    clockRtcStatus=0; //Verify the registers before reporting success.
  } else {
    time_t utc;
    bool valid=rtcDecodeTime(rtcRegisters,utc);
    bool decoded=valid;
    if (valid && rtcVerifyPending) {
      uint32_t current=now(),stored=utc;
      valid=(current>stored ? current-stored : stored-current)<=2;
    }
    if (valid && !rtcVerifyPending && rtcPreviousSample && millis()-rtcPreviousMillis>=2000UL) {
      //CH=0 alone does not prove that the crystal is ticking.
      valid=uint32_t(utc)>rtcPreviousUtc;
    }
    if (valid) { rtcPreviousSample=true; rtcPreviousUtc=utc; rtcPreviousMillis=millis(); }
    if (!valid) {
      if (!decoded) rtcPreviousSample=false;
      clockRtcStatus=3;
      if (rtcVerifyPending) rtcWritePending=true;
      //Only trusted NTP/manual time can repair invalid RTC registers. Never
      //overwrite battery-backed time with an old CONFIG fallback at boot.
      if ((clockSyncStatus==1 || clockSyncStatus==4) && now()>=1577836800UL) rtcScheduleWrite();
    } else {
      clockRtcStatus=1;
      //A late RTC reply must never undo a manual setting or accepted NTP time.
      if (!rtcVerifyPending && rtcBootTimeGeneration==clockTimeGeneration) {
        setTime(utc); clockSyncStatus=5; clockTimeGeneration++;
      }
    }
    rtcBootRead=false; rtcVerifyPending=false;
  }
  clockReportSyncState();
}

void rtcService() {
  if(!megaFeatures.has(ControllerFeatures::Rtc))return;
  if (rtcState==0) {
    if ((long)(millis()-rtcRetryMillis)<0) return;
    if (!rtcWritePending && !rtcBootRead) rtcBootRead=true; //Periodic read/progress check.
    if (TWCR&_BV(TWSTO)) {
      //Allow STOP to complete; recover a stuck bus without waiting in this call.
      if (millis()-rtcStopMillis<10UL) return;
      rtcFaultState=0; rtcFaultStatus=TWSR&0xf8; rtcReportFault(3);
      clockRtcStatus=2; clockReportSyncState();
      TWCR=0; TWCR=_BV(TWEN); rtcRetryMillis=millis()+5000UL; return;
    }
    rtcWriting=rtcWritePending;
    if (rtcWriting) {
      if (!rtcEncodeTime(now(),rtcRegisters)) {
        rtcWritePending=false; clockRtcStatus=3; return;
      }
      rtcActiveGeneration=rtcWriteGeneration;
    }
    rtcIndex=0; rtcAdvance(1,_BV(TWSTA)); return;
  }
  //A completed hardware step remains valid even if the main loop polls late.
  if (!(TWCR&_BV(TWINT))) {
    if (millis()-rtcStepMillis>=10UL) {
      rtcFinish(false,1);
    }
    return;
  }
  byte status=TWSR&0xf8;
  switch (rtcState) {
    case 1:
      if (status!=TW_START) { rtcFinish(false); return; }
      TWDR=0x68<<1; rtcAdvance(2,0); break;
    case 2:
      if (status!=TW_MT_SLA_ACK) { rtcFinish(false); return; }
      TWDR=0; rtcAdvance(3,0); break;
    case 3:
      if (status!=TW_MT_DATA_ACK) { rtcFinish(false); return; }
      if (rtcWriting) { TWDR=rtcRegisters[rtcIndex++]; rtcAdvance(6,0); }
      else rtcAdvance(4,_BV(TWSTA));
      break;
    case 4:
      if (status!=TW_REP_START) { rtcFinish(false); return; }
      TWDR=(0x68<<1)|1; rtcAdvance(5,0); break;
    case 5:
      if (status!=TW_MR_SLA_ACK) { rtcFinish(false); return; }
      rtcAdvance(7,_BV(TWEA)); break;
    case 6:
      if (status!=TW_MT_DATA_ACK) { rtcFinish(false); return; }
      if (rtcIndex==7) rtcFinish(true);
      else { TWDR=rtcRegisters[rtcIndex++]; rtcAdvance(6,0); }
      break;
    case 7:
      if (status!=(rtcIndex==6 ? TW_MR_DATA_NACK : TW_MR_DATA_ACK)) { rtcFinish(false); return; }
      rtcRegisters[rtcIndex++]=TWDR;
      if (rtcIndex==7) rtcFinish(true);
      else rtcAdvance(7,rtcIndex==6 ? 0 : _BV(TWEA));
      break;
    default: rtcFinish(false);
  }
}

String rtcStatusString() {
  switch (clockRtcStatus) {
    case 1: return "ok";
    case 2: return "missing";
    case 3: return "invalid";
    default: return "waiting";
  }
}

void rtcDisable(){rtcState=0;rtcBootRead=false;rtcWritePending=false;rtcVerifyPending=false;rtcPreviousSample=false;TWCR=0;}
