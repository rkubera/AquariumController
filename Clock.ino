/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/

static uint32_t clockDelta = 0;
static bool clockInited = false;

void clockInit() {
  clockInited = true;

/*
  if (!clockRtc.begin()) {
    Serial.println(F("Couldn't find RTC"));
  }
  if (!clockRtc.isrunning()) {
    Serial.println(F("RTC is NOT running!"));
  }
*/
  uint32_t mytime = configGetUint32Value(EEPROM_unix_timestamp_addr);
  clockDelta = mytime-(millis()/1000);
  if (DEBUG_LEVEL & _DEBUG_NOTICE) {
    Serial.print("Loaded timestamp:");
    Serial.println(mytime);
    
    Serial.print("Clock Delta=");
    Serial.println(clockDelta);
  }

  myDST = TimeChangeRule {"EDT", timezoneRule1Week, timezoneRule1DayOfWeek, timezoneRule1Month, timezoneRule1Hour, timezoneRule1Offset*60}; 
  mySTD = TimeChangeRule {"EST", timezoneRule2Week, timezoneRule1DayOfWeek, timezoneRule2Month, timezoneRule2Hour, timezoneRule1Offset*60}; 
  myTZ.setRules (myDST, mySTD);
  //clockGetActualTimezoneOffset();
  //clockSetLocalTime();
  
}

void clockMqttPublishAll() {
  sprintf(bufferOut,"%+d", timezoneActualOffset);
  mqttElPublish(setBufferFromFlash(getActualTimezoneOffset),String(bufferOut));

  sprintf(bufferOut,"%04d/%02d/%02d", globalYear, globalMonth, globalMonthDay);
  bufferOut[10] = 0;
  mqttElPublish( setBufferFromFlash(getActualDate), bufferOut );
  mqttElPublish( setBufferFromFlash(getActualDayOfWeek), setBufferFromFlash(daysOfTheWeek[globalWeekDay]));

  mqttElPublish(setBufferFromFlash(getTimezoneRule1Week), clockTimezoneCodeToWeek(timezoneRule1Week));
  mqttElPublish(setBufferFromFlash(getTimezoneRule2Week), clockTimezoneCodeToWeek(timezoneRule2Week));

  mqttElPublish(setBufferFromFlash(getTimezoneRule1DayOfWeek), clockTimezoneCodeToDoW(timezoneRule1DayOfWeek));
  mqttElPublish(setBufferFromFlash(getTimezoneRule2DayOfWeek), clockTimezoneCodeToDoW(timezoneRule2DayOfWeek));

  mqttElPublish(setBufferFromFlash(getTimezoneRule1Month), clockTimezoneCodeToMonth(timezoneRule1Month));
  mqttElPublish(setBufferFromFlash(getTimezoneRule2Month), clockTimezoneCodeToMonth(timezoneRule2Month));

  mqttElPublish(setBufferFromFlash(getTimezoneRule1Hour), intToString(timezoneRule1Hour));
  mqttElPublish(setBufferFromFlash(getTimezoneRule2Hour), intToString(timezoneRule2Hour));

  sprintf(bufferOut,"%+d", timezoneRule1Offset);
  mqttElPublish(setBufferFromFlash(getTimezoneRule1Offset), (String)bufferOut);

  sprintf(bufferOut,"%+d", timezoneRule2Offset);
  mqttElPublish(setBufferFromFlash(getTimezoneRule2Offset), (String)bufferOut);
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
    sprintf(bufferOut,"%+d", timezoneRule1Offset);
    mqttElPublish(setBufferFromFlash(getActualTimezoneOffset), (String)bufferOut);
  }
  
  return timezoneActualOffset;
}

void clockUpdateTimezoneRules() {
  myDST = TimeChangeRule {"EDT", timezoneRule1Week, timezoneRule1DayOfWeek, timezoneRule1Month, timezoneRule1Hour, timezoneRule1Offset*60}; 
  mySTD = TimeChangeRule {"EST", timezoneRule2Week, timezoneRule1DayOfWeek, timezoneRule2Month, timezoneRule2Hour, timezoneRule1Offset*60}; 
  myTZ.setRules (myDST, mySTD);
  clockGetActualTimezoneOffset();
  clockSetLocalTime();
  clockMqttPublishHourDate();
}

void clockMqttPublishHourDate() {
  sprintf(bufferOut,"%02d:%02d", globalHour, globalMinute);
  bufferOut[5] = 0;
  mqttElPublish( setBufferFromFlash(getActualTime), bufferOut );
  sprintf(bufferOut,"%04d/%02d/%02d", globalYear, globalMonth, globalMonthDay);
  bufferOut[10] = 0;
  mqttElPublish( setBufferFromFlash(getActualDate), bufferOut );
  mqttElPublish( setBufferFromFlash(getActualDayOfWeek), setBufferFromFlash(daysOfTheWeek[globalWeekDay]));
}
void clockMinuteEvent() { 
  static bool bootTimeSet = false;

  DateTime btime = DateTime(boot_time);
  if (bootTimeSet==false && MQTT_STATUS_CONNECTED && hostnameReceived == true && mqttElDeviceName!="") {
    String bootTimeStr = String(btime.year())+"/"+String(btime.month())+"/"+String(btime.day())+" "+String(btime.hour())+":"+String(btime.minute());
    mqttElPublish( setBufferFromFlash(setBootTime), bootTimeStr);
    bootTimeSet = true;
  }
  clockNTPSynchronize();
}


void clockMillisEvent() {
  static double lastMillisSavedTime = -3600000;

  time_t my_time = clockGetGlobalDateTime();
  DateTime btime = DateTime(my_time);
  uint32_t mytime = btime.unixtime();
  if (abs(millis()-lastMillisSavedTime)>3600000 && wifiStatus == WIFI_STATUS_CONNECTED) {
    lastMillisSavedTime = millis();
    configSaveUint32Value(mytime, EEPROM_unix_timestamp_addr);
    if (DEBUG_LEVEL & _DEBUG_NOTICE) {
      Serial.print("Saved timestamp:");
      Serial.print(millis()-lastMillisSavedTime);
      Serial.print(" ");
      Serial.println(mytime);
    }
  }
  static byte clockLastMinute;
  time_t local = clockGetLocalTime();
   
  byte mymin = minute(local);
  
  if (clockLastMinute!=mymin) {
    clockLastMinute = mymin;
    clockMqttPublishHourDate();
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
  return localTime;
}

void clockNTPSynchronize() {
  if (clockInited==false) return;
  static double lastSyncMillis = -10000;
  if (wifiStatus == WIFI_STATUS_CONNECTED) {
    if (abs(millis()-lastSyncMillis)>10000) {
      lastSyncMillis = millis();
      if (DEBUG_LEVEL & _DEBUG_NOTICE) {
        Serial.println(F("Clock NTP Synchro start"));
      }
      String command = "get timestamp";
      mqttSendCommand(command);
    }
  }
}

void clockNTPSynchronize_cb(long ntpTime) {
  if (ntpTime>0) {
    clockLastSynchro = millis();
    clockDelta = ntpTime-(millis()/1000);
    
    if (DEBUG_LEVEL & _DEBUG_NOTICE) {
      Serial.println(F("Clock NTP Synchro ok"));
      Serial.print("Clock Delta=");
      Serial.println(clockDelta);
    }
    
    if (boot_time==0) {
      boot_time = ntpTime;
    }
    clockUpdateTimezoneRules();
  }
  else {
    if (DEBUG_LEVEL & _DEBUG_WARNING) {
      Serial.println(F("Clock NTP Synchro failed"));
    }
  }
}

time_t clockGetGlobalDateTime(){
  DateTime now;
  if (abs(millis()-clockLastSynchro)>60000 && wifiStatus == WIFI_STATUS_CONNECTED) {
    clockNTPSynchronize();
  }
  if (1==0 && clockRtc.isrunning()) {
    now = clockRtc.now();
    if (boot_time==0) {
      if (now.year()>2000 && now.year()<2100) {
        boot_time = now.unixtime();
      }
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
  /*
  DateTime now = DateTime(tSet);
  uint32_t mytime = now.unixtime();
  clockDelta = mytime-(millis()/1000);

  if (DEBUG_LEVEL & _DEBUG_NOTICE) {
    Serial.print("Clock Delta=");
    Serial.println(clockDelta);
  }
  */
}
