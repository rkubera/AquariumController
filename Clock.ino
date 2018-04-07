/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/

static uint32_t clockDelta = 0;

void clockInit() {
  if (!clockRtc.begin()) {
    Serial.println(F("Couldn't find RTC"));
  }
  if (!clockRtc.isrunning()) {
    Serial.println(F("RTC is NOT running!"));
  }
  clockSetLocalTime();
  clockUpdateTimezoneRules();
}

void clockMqttPublishAll() {
  sprintf(buffer,"%+d", timezoneActualOffset);
  mqttElPublish(setBufferFromFlash(getActualTimezoneOffset),String(buffer));

  sprintf(buffer,"%04d/%02d/%02d", globalYear, globalMonth, globalMonthDay);
  buffer[10] = 0;
  mqttElPublish( setBufferFromFlash(getActualDate), buffer );
  mqttElPublish( setBufferFromFlash(getActualDayOfWeek), setBufferFromFlash(daysOfTheWeek[globalWeekDay]));

  mqttElPublish(setBufferFromFlash(getTimezoneRule1Week), clockTimezoneCodeToWeek(timezoneRule1Week));
  mqttElPublish(setBufferFromFlash(getTimezoneRule2Week), clockTimezoneCodeToWeek(timezoneRule2Week));

  mqttElPublish(setBufferFromFlash(getTimezoneRule1DayOfWeek), clockTimezoneCodeToDoW(timezoneRule1DayOfWeek));
  mqttElPublish(setBufferFromFlash(getTimezoneRule2DayOfWeek), clockTimezoneCodeToDoW(timezoneRule2DayOfWeek));

  mqttElPublish(setBufferFromFlash(getTimezoneRule1Month), clockTimezoneCodeToMonth(timezoneRule1Month));
  mqttElPublish(setBufferFromFlash(getTimezoneRule2Month), clockTimezoneCodeToMonth(timezoneRule2Month));

  mqttElPublish(setBufferFromFlash(getTimezoneRule1Hour), intToString(timezoneRule1Hour));
  mqttElPublish(setBufferFromFlash(getTimezoneRule2Hour), intToString(timezoneRule2Hour));

  sprintf(buffer,"%+d", timezoneRule1Offset);
  mqttElPublish(setBufferFromFlash(getTimezoneRule1Offset), (String)buffer);

  sprintf(buffer,"%+d", timezoneRule2Offset);
  mqttElPublish(setBufferFromFlash(getTimezoneRule2Offset), (String)buffer);
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
  static char oldTimezoneActualOffset;

  time_t utcTime = clockGetGlobalDateTime();
  time_t localTime = myTZ.toLocal(utcTime, &tcr);

  DateTime nowUtcTime, nowLocalTime;
  nowUtcTime = DateTime(utcTime);
  nowLocalTime = DateTime(localTime);

  uint32_t nowUtcUnixTime = nowUtcTime.unixtime();
  uint32_t nowLocalUnixTime = nowLocalTime.unixtime();

  uint32_t secs = nowLocalUnixTime-nowUtcUnixTime;
  timezoneActualOffset = (secs/3600);
  if (oldTimezoneActualOffset!=timezoneActualOffset) {
    oldTimezoneActualOffset = timezoneActualOffset;
    sprintf(buffer,"%+d", timezoneRule1Offset);
    mqttElPublish(setBufferFromFlash(getActualTimezoneOffset), (String)buffer);
  }
  
  return timezoneActualOffset;
}

void clockUpdateTimezoneRules() {
  myDST = TimeChangeRule {"EDT", timezoneRule1Week, timezoneRule1DayOfWeek, timezoneRule1Month, timezoneRule1Hour, timezoneRule1Offset*60}; 
  mySTD = TimeChangeRule {"EST", timezoneRule2Week, timezoneRule1DayOfWeek, timezoneRule2Month, timezoneRule2Hour, timezoneRule1Offset*60}; 
  myTZ.setRules (myDST, mySTD);
  clockGetActualTimezoneOffset();
  clockSetLocalTime();

  time_t local = clockGetLocalTime();
   
  clockMqttPublishHourDate();
}

void clockMqttPublishHourDate() {
  sprintf(buffer,"%02d:%02d", globalHour, globalMinute);
  buffer[5] = 0;
  mqttElPublish( setBufferFromFlash(getActualTime), buffer );
  sprintf(buffer,"%04d/%02d/%02d", globalYear, globalMonth, globalMonthDay);
  buffer[10] = 0;
  mqttElPublish( setBufferFromFlash(getActualDate), buffer );
  mqttElPublish( setBufferFromFlash(getActualDayOfWeek), setBufferFromFlash(daysOfTheWeek[globalWeekDay]));
}
void clockMinuteEvent() { 
  if (boot_time>0) {
    DateTime btime = DateTime(boot_time);
    Serial.print(F("Boot time="));
    Serial.print(btime.year());
    Serial.print(F("/"));
    Serial.print(btime.month());
    Serial.print(F("/"));
    Serial.print(btime.day());
    Serial.print(F(" "));    
    Serial.print(btime.hour());
    Serial.print(F(":")); 
    Serial.print(btime.minute());
    Serial.print(F(":")); 
    Serial.println(btime.second());
  }
  else {
    Serial.println(F("Wrong Boot time"));
  }
}

void clockMillisEvent() {
  static byte clockLastMinute;
  time_t local = clockGetLocalTime();
   
  byte mymin = minute(local);
  byte myhour = hour(local);

  if (clockLastMinute!=mymin) {
    clockLastMinute = mymin;
    clockMqttPublishHourDate();
  }
}

time_t clockSetLocalTime() {
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
  clockSetGlobalDateTime(utc);
}

time_t clockGetLocalTime() {
  time_t utcTime = clockGetGlobalDateTime();
  time_t localTime = myTZ.toLocal(utcTime, &tcr);
  DateTime btime = DateTime(localTime);
  
  globalSecond = second(localTime);
  globalMinute = minute(localTime);
  globalHour = hour(localTime);
  globalWeekDay = btime.dayOfTheWeek();
  globalMonthDay = day(localTime);
  globalMonth = month(localTime);
  globalYear = year(localTime);

  /*
  Serial.print(F("Actual time="));
  Serial.print(btime.year());
  Serial.print(F("/"));
  Serial.print(btime.month());
  Serial.print(F("/"));
  Serial.print(btime.day());
  Serial.print(F(" "));    
  Serial.print(btime.hour());
  Serial.print(F(":")); 
  Serial.print(btime.minute());
  Serial.print(F(":")); 
  Serial.print(btime.second());
  Serial.print(F(" ")); 
  Serial.println(setBufferFromFlash(daysOfTheWeek[globalWeekDay]));
  */
  
  return localTime;
}

void clockNTPSynchronize() {
  Serial.println(F("Clock NTP Synchro start"));
  uint32_t ntpTime = mqttElcmd.GetTime();
  if (ntpTime>0) {
    Serial.println(F("Clock NTP Synchro ok"));
    clockLastSynchro = millis();
    clockDelta = ntpTime-(millis()/1000);
    if (boot_time==0) {
      boot_time = ntpTime;
    }
  }
  else {
    Serial.println(F("Clock NTP Synchro failed"));
  }
}

time_t clockGetGlobalDateTime(){
  static uint32_t clockLastMillisGet;
  DateTime now;
  if (abs(millis()-clockLastSynchro)>3600000 && wifiStatus == WIFI_STATUS_CONNECTED) {
    if (abs(millis()-clockLastMillisGet)>10000) {
      clockLastMillisGet = millis();
      clockNTPSynchronize();
    }
  }
    
  if (clockRtc.isrunning()) {
    now = clockRtc.now();
    if (boot_time==0) {
      boot_time = now.unixtime();
    }
  }
  else {
    uint32_t mytime = (millis()/1000)+clockDelta;
    now = DateTime(mytime);
  }
 
  time_t tSet;
  tmElements_t tmSet;
  tmSet.Year = now.year()-1970;
  tmSet.Month = now.month();
  tmSet.Day = now.day();
  tmSet.Hour = now.hour();
  tmSet.Minute = now.minute();
  tmSet.Second = now.second();
  tSet = makeTime(tmSet);
  return tSet;
}

void clockSetGlobalDateTime(time_t tSet){
  if (clockRtc.isrunning()) {
    clockRtc.adjust(DateTime(year(tSet), month(tSet), day(tSet), hour(tSet), minute(tSet), second(tSet)));
  }
  DateTime now = DateTime(tSet);
  uint32_t mytime = now.unixtime();
  clockDelta = mytime-(millis()/1000);
}


