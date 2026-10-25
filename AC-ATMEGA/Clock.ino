/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/

static bool clockInited = false;
static bool clockLocalCacheValid=false;
static time_t clockLocalCacheUtc=0,clockLocalCacheTime=0;
static tmElements_t clockLocalCacheParts;
void clockInvalidateLocalTime(){clockLocalCacheValid=false;}


void clockInit() {
  clockInited = true;
  time_t savedTime = configGetUint32Value(CONFIG_unix_timestamp_addr);
  if (savedTime < CLOCK_MIN_VALID_NTP_TIME || savedTime == UINT32_MAX) savedTime = 0;
  setTime(savedTime);
  myDST = TimeChangeRule {"EDT", timezoneRule1Week, timezoneRule1DayOfWeek, timezoneRule1Month, timezoneRule1Hour, timezoneRule1Offset*60}; 
  mySTD = TimeChangeRule {"EST", timezoneRule2Week, timezoneRule2DayOfWeek, timezoneRule2Month, timezoneRule2Hour, timezoneRule2Offset*60}; 
  myTZ.setRules (myDST, mySTD);
  clockInvalidateLocalTime();
}

void clockReportAll() {
  clockReportSyncState();
  clockReportBoot();
  clockGetLocalTime();
  clockReportHourDate();
  sprintf(bufferOut,"%+d", timezoneActualOffset);
  reportBuffer(FieldId::ActualTimezoneOffset);

  sprintf(bufferOut,"%04d-%02d-%02d", globalYear, globalMonth, globalMonthDay);
  bufferOut[10] = 0;
  reportBuffer(FieldId::ActualDate);
  reportField( FieldId::ActualDayOfWeek, setBufferFromFlash(daysOfTheWeek[globalWeekDay-1]));

  reportField(FieldId::TimezoneRule1Week, clockTimezoneCodeToWeek(timezoneRule1Week));
  reportField(FieldId::TimezoneRule2Week, clockTimezoneCodeToWeek(timezoneRule2Week));

  reportField(FieldId::TimezoneRule1DayOfWeek, clockTimezoneCodeToDoW(timezoneRule1DayOfWeek));
  reportField(FieldId::TimezoneRule2DayOfWeek, clockTimezoneCodeToDoW(timezoneRule2DayOfWeek));

  reportField(FieldId::TimezoneRule1Month, clockTimezoneCodeToMonth(timezoneRule1Month));
  reportField(FieldId::TimezoneRule2Month, clockTimezoneCodeToMonth(timezoneRule2Month));

  reportField(FieldId::TimezoneRule1Hour, intToString(timezoneRule1Hour));
  reportField(FieldId::TimezoneRule2Hour, intToString(timezoneRule2Hour));

  sprintf(bufferOut,"%+d", timezoneRule1Offset);
  reportBuffer(FieldId::TimezoneRule1Offset);

  sprintf(bufferOut,"%+d", timezoneRule2Offset);
  reportBuffer(FieldId::TimezoneRule2Offset);
}

String clockTimezoneCodeToWeek(int value) {
  if (value == TIMEZONE_WEEK_FIRST) {
      return setBufferFromFlash(timezoneWeekFirst);
    }
    else if (value == TIMEZONE_WEEK_SECOND) {
      return setBufferFromFlash(timezoneWeekSecond);
    }
    else if (value == TIMEZONE_WEEK_THIRD) {
      return setBufferFromFlash(timezoneWeekThird);
    }
    else if (value == TIMEZONE_WEEK_FOURTH) {
      return setBufferFromFlash(timezoneWeekFourth);
    }
    return setBufferFromFlash(timezoneWeekLast);
}

String clockTimezoneCodeToDoW (int value) {
  if (value>=1 && value <=7) {
    return setBufferFromFlash(daysOfTheWeek[value-1]);
  }
  return "";
}

String clockTimezoneCodeToMonth (int value) {
  if (value>=1 && value <=12) {
    return setBufferFromFlash(monthsOfYear[value-1]);
  }
  return "";
}

char clockGetActualTimezoneOffset() {
  
  time_t utcTime = now();
  time_t localTime = clockGetLocalTime();

  long secs = (long)(localTime-utcTime); //An offset can exceed AVR int16 range.
  
  timezoneActualOffset = (secs/3600);
  sprintf(bufferOut,"%+d", timezoneActualOffset);
  reportBuffer(FieldId::ActualTimezoneOffset);
  return timezoneActualOffset;
}

void clockUpdateTimezoneRules() {
  time_t previousBootLocal=megaBoot.utc?myTZ.toLocal(time_t(megaBoot.utc)):0;
  myDST = TimeChangeRule {"EDT", timezoneRule1Week, timezoneRule1DayOfWeek, timezoneRule1Month, timezoneRule1Hour, timezoneRule1Offset*60}; 
  mySTD = TimeChangeRule {"EST", timezoneRule2Week, timezoneRule2DayOfWeek, timezoneRule2Month, timezoneRule2Hour, timezoneRule2Offset*60}; 
  myTZ.setRules (myDST, mySTD);
  clockInvalidateLocalTime();
  clockGetActualTimezoneOffset();
  if(megaBoot.utc&&previousBootLocal!=myTZ.toLocal(time_t(megaBoot.utc)))clockReportBoot();
  clockReportHourDate();
}

void clockReportHourDate() {
  clockGetLocalTime();
  sprintf(bufferOut,"%02d:%02d:%02d", globalHour, globalMinute,globalSecond);
  bufferOut[8] = 0;
  reportBuffer(FieldId::ActualTime);
  sprintf(bufferOut,"%04d-%02d-%02d", globalYear, globalMonth, globalMonthDay);
  bufferOut[10] = 0;
  reportBuffer(FieldId::ActualDate);
  reportField( FieldId::ActualDayOfWeek, setBufferFromFlash(daysOfTheWeek[globalWeekDay-1]));
  //schedulerGetActualPartOfDay();
  schedulerPublishActualPartOfDay();
}

void clockMinuteEvent() {
  clockSyncEvent();
}

void clockSyncEvent() {
  static unsigned long lastRequest = millis()-CLOCK_SYNC_INTERVAL_MILLIS;
  if (!clockInited || wifiStatus != WIFI_STATUS_CONNECTED) return;
  if (millis()-lastRequest >= CLOCK_SYNC_INTERVAL_MILLIS) {
    //A full queue is temporary: do not postpone a command that was never queued.
    if (wireRequest(AcWire::Timestamp)) lastRequest=millis();
  }
}

void clockMillisEvent() {

  time_t utc = now();
  uint32_t ms=millis();
  bool bootChanged=false;
  if(megaConfigReady&&(clockSyncStatus==1||clockSyncStatus==4||clockSyncStatus==5))bootChanged=megaBoot.synchronize(uint32_t(utc),ms);
  static uint32_t lastBootFallback=0;
  if(megaConfigReady&&(bootChanged||(!megaBoot.utc&&uint32_t(ms-lastBootFallback)>=10000UL))){lastBootFallback=ms;clockReportBoot();}
  time_t local = clockGetLocalTime();
    
  static unsigned long lastMillisSavedTime = millis();
  if (millis()-lastMillisSavedTime >= 3600000UL &&
      (clockSyncStatus==1 || clockSyncStatus==4) && utc>=CLOCK_MIN_VALID_NTP_TIME) {
     lastMillisSavedTime = millis();
     configSaveUint32Value(utc, CONFIG_unix_timestamp_addr);
  }

  static byte clockLastMinute;
  byte mymin = minute(local);
  
  if (clockLastMinute!=mymin) {
    clockLastMinute = mymin;
    clockReportHourDate();
  }
}

void clockSetLocalTime() {
  time_t tSet;
  tmElements_t tmSet;
  tmSet.Year = globalYear-1970;
  tmSet.Month = globalMonth;
  tmSet.Day = globalMonthDay;
  tmSet.Hour = globalHour;
  tmSet.Minute = globalMinute;
  tmSet.Second = globalSecond;
  tSet = makeTime(tmSet);
  
  //convert to utc
  time_t utc = myTZ.toUTC(tSet);
  setTime(utc);
  clockTimeGeneration++;
  clockSyncStatus = 4;
  rtcScheduleWrite();
  configSaveUint32Value(utc,CONFIG_unix_timestamp_addr);
  clockReportSyncState();
}

time_t clockGetLocalTime() {
  time_t utcTime=now();
  if (!clockLocalCacheValid || utcTime!=clockLocalCacheUtc) {
    clockLocalCacheUtc=utcTime;
    clockLocalCacheTime=myTZ.toLocal(utcTime,&tcr);
    breakTime(clockLocalCacheTime,clockLocalCacheParts);
    clockLocalCacheValid=true;
  }
  globalSecond=clockLocalCacheParts.Second;
  globalMinute=clockLocalCacheParts.Minute;
  globalHour=clockLocalCacheParts.Hour;
  globalWeekDay=clockLocalCacheParts.Wday;
  globalMonthDay=clockLocalCacheParts.Day;
  globalMonth=clockLocalCacheParts.Month;
  globalYear=tmYearToCalendar(clockLocalCacheParts.Year);
  return clockLocalCacheTime;
}

void clockNTPSynchronize_cb(time_t ntpTime) {
  static uint32_t pendingTime = 0;
  static unsigned long pendingMillis = 0;
  uint32_t received = ntpTime;
  if (received < CLOCK_MIN_VALID_NTP_TIME || received == UINT32_MAX) {
    pendingTime = 0;
    clockSyncStatus = 3;
    clockReportSyncState();
    return;
  }
  uint32_t current = now();
  uint32_t difference = current > received ? current-received : received-current;
  if (current < CLOCK_MIN_VALID_NTP_TIME || difference > CLOCK_MAX_NTP_STEP) {
    uint32_t elapsed = (millis()-pendingMillis)/1000UL;
    uint32_t expected = pendingTime+elapsed;
    uint32_t delta = received > expected ? received-expected : expected-received;
    if (pendingTime == 0 || elapsed > 120UL || delta > 5UL) {
      pendingTime = received;
      pendingMillis = millis();
      clockSyncStatus = 2;
      clockReportSyncState();
      return;
    }
  }
  bool saveCorrection=clockLastNtpTime==0 || difference>CLOCK_MAX_NTP_STEP;
  pendingTime = 0;
  clockLastSynchro = millis();
  clockLastNtpTime = received;
  clockSyncStatus = 1;
  setTime(ntpTime);
  clockTimeGeneration++;
  if (saveCorrection) configSaveUint32Value(received,CONFIG_unix_timestamp_addr);
  if (saveCorrection || difference>1 || clockRtcStatus!=1) rtcScheduleWrite();
  clockUpdateTimezoneRules();
  clockReportSyncState();
}

const char* clockSyncStatusString() {
  switch (clockSyncStatus) {
    case 1: return "synchronized";
    case 2: return "pending";
    case 3: return "rejected";
    case 4: return "manual";
    case 5: return "rtc";
    default: return "waiting";
  }
}

void clockReportSyncState() {
  reportField(FieldId::ClockRtcStatus,rtcStatusString());
}

// Use the offset at the boot instant, including historical DST, not today's offset.
void clockFormatBoot(char* output,size_t capacity){
 uint32_t ms=millis();megaBoot.tick(ms);
 if(!megaBoot.utc){
  if(megaBoot.elapsedSeconds)snprintf_P(output,capacity,PSTR("%lu%03u ms since start"),(unsigned long)megaBoot.elapsedSeconds,unsigned(megaBoot.remainder(ms)));
  else snprintf_P(output,capacity,PSTR("%u ms since start"),unsigned(megaBoot.remainder(ms)));
  return;
 }
 TimeChangeRule* rule=nullptr;time_t local=myTZ.toLocal(time_t(megaBoot.utc),&rule);
 int offset=rule?rule->offset:0;unsigned minutes=offset<0?unsigned(-offset):unsigned(offset);
 snprintf_P(output,capacity,PSTR("%04d-%02d-%02d %02d:%02d:%02d %c%02u:%02u"),year(local),month(local),day(local),hour(local),minute(local),second(local),offset<0?'-':'+',minutes/60,minutes%60);
}
void clockReportBoot(){
 if(!linkFieldWanted(FieldId::MegaLastBoot))return;
 clockFormatBoot(bufferOut,bufferOutSize);reportSelectedField(FieldId::MegaLastBoot,String(bufferOut));
}
