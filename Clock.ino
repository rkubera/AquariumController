/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/

static bool clockInited = false;

void clockInit() {
  clockInited = true;
  time_t savedTime = configGetUint32Value(EEPROM_unix_timestamp_addr);
  setTime(savedTime);
  myDST = TimeChangeRule {"EDT", timezoneRule1Week, timezoneRule1DayOfWeek, timezoneRule1Month, timezoneRule1Hour, timezoneRule1Offset*60}; 
  mySTD = TimeChangeRule {"EST", timezoneRule2Week, timezoneRule2DayOfWeek, timezoneRule2Month, timezoneRule2Hour, timezoneRule2Offset*60}; 
  myTZ.setRules (myDST, mySTD);
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
  
  time_t utcTime = now();
  time_t localTime = myTZ.toLocal(utcTime, &tcr);

  int secs = localTime- utcTime;
  
  timezoneActualOffset = (secs/3600);
  sprintf(bufferOut,"%+d", timezoneActualOffset);
  mqttElPublish(setBufferFromFlash(getActualTimezoneOffset), (String)bufferOut);
  return timezoneActualOffset;
}

void clockUpdateTimezoneRules() {
  myDST = TimeChangeRule {"EDT", timezoneRule1Week, timezoneRule1DayOfWeek, timezoneRule1Month, timezoneRule1Hour, timezoneRule1Offset*60}; 
  mySTD = TimeChangeRule {"EST", timezoneRule2Week, timezoneRule2DayOfWeek, timezoneRule2Month, timezoneRule2Hour, timezoneRule2Offset*60}; 
  myTZ.setRules (myDST, mySTD);
  clockGetActualTimezoneOffset();
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
  //schedulerGetActualPartOfDay();
  schedulerPublishActualPartOfDay();
}

void clockMinuteEvent() { 
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


void clockMillisEvent() {

  time_t utc = now();
  time_t local = myTZ.toLocal(utc, &tcr);
    
  static double lastMillisSavedTime = -3600000;
  if (abs(millis()-lastMillisSavedTime)>3600000 && wifiStatus == WIFI_STATUS_CONNECTED) {
     lastMillisSavedTime = millis();
     configSaveUint32Value(utc, EEPROM_unix_timestamp_addr);
  }

  static byte clockLastMinute;
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
  setTime(utc);
}

time_t clockGetLocalTime() {

  time_t utcTime = now();
  time_t localTime = myTZ.toLocal(utcTime, &tcr);
  
  globalSecond = second(localTime);
  globalMinute = minute(localTime);
  globalHour = hour(localTime);
  globalWeekDay = weekday(localTime);
  globalMonthDay = day(localTime);
  globalMonth = month(localTime);
  globalYear = year(localTime);
  return localTime;
}

void clockNTPSynchronize_cb(time_t ntpTime) {
  if (ntpTime>0) {
    clockLastSynchro = millis();
    setTime(ntpTime);
    if (DEBUG_LEVEL & _DEBUG_NOTICE) {
      Serial.println(F("Clock NTP Synchro ok"));
    }
    clockUpdateTimezoneRules();
  }
  else {
    if (DEBUG_LEVEL & _DEBUG_WARNING) {
      Serial.println(F("Clock NTP Synchro failed"));
    }
  }
}
