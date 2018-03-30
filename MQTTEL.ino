
static byte mqttSubscribeProbes = 0;

void mqttMinuteEvent() {

}

void mqttElInit() {
  uint32_t lastESPSync;
  Serial.println(F("EL-Client starting!"));
  mqttElesp.wifiCb.attach(mqttElWifiCallback); // wifi status change callback, optional (delete if not desired)
  bool ok;
  lastESPSync = millis();
  do {
    ok = mqttElesp.Sync();      // sync up with esp-link, blocks for up to 2 seconds
    if (!ok) {
      Serial.println(F("EL-Client sync failed!"));
    }
    else {
      Serial.println(F("EL-Client synced!"));    
    }
    if (abs(millis()-lastESPSync)>10000) {
      ok = true;
    }
  } while(!ok);
  

  // Set-up callbacks for events and initialize with es-link.
  mqttEl.connectedCb.attach(mqttElConnected);
  mqttEl.disconnectedCb.attach(mqttElDisconnected);
  mqttEl.publishedCb.attach(mqttElPublished);
  mqttEl.dataCb.attach(mqttElData);
  mqttEl.setup();
  Serial.println(F("EL-MQTT ready"));
}

// Callback made from esp-link to notify of wifi status changes
void mqttElWifiCallback(void* response) {
  ELClientResponse *res = (ELClientResponse*)response;
  if (res->argc() == 1) {
    uint8_t status;
    res->popArg(&status, 1);
    if(status == STATION_GOT_IP) {
      Serial.println(F("WIFI CONNECTED"));
      wifiStatus = WIFI_STATUS_CONNECTED;
      eventWifiConnected();
    } else {
      Serial.print(F("WIFI NOT READY: "));
      wifiStatus = WIFI_STATUS_DISCONNECTED;
      Serial.println(status);
      eventWifiDisconnected();
    }
  }
}

void mqttElPublish(String topic, String value) {
  if (mqttStatus==MQTT_STATUS_CONNECTED) {
    String fullTopic = String(mqttElDeviceName)+'/'+topic;
    //char buffer[100];
    fullTopic.toCharArray( buffer, 100 );
    char ebuffer[100];
    value.toCharArray( ebuffer, 100 );
    mqttEl.publish( buffer, ebuffer, strlen(ebuffer), true );
  }
}

void mqttElPublishFull (String topic, String value) {
  if (mqttStatus==MQTT_STATUS_CONNECTED) {
    String fullTopic = topic;
    //char buffer[100];
    fullTopic.toCharArray( buffer, 100 );
    char ebuffer[100];
    value.toCharArray( ebuffer, 100 );
    mqttEl.publish( buffer, ebuffer, strlen(ebuffer), true );
    Serial.print(topic);
    Serial.print(F(" "));
    Serial.println(value);
  }
}

void mqttElSubscribe(String topic) {
  if (mqttStatus==MQTT_STATUS_CONNECTED) {
    String fullTopic = String(mqttElDeviceName)+'/'+topic;
    fullTopic.toCharArray( buffer, 100 );
    mqttEl.subscribe(buffer);
  }
}

void mqttElSubscribe(String topic, int qos) {
  if (mqttStatus==MQTT_STATUS_CONNECTED) {
    String fullTopic = String(mqttElDeviceName)+'/'+topic;
    fullTopic.toCharArray( buffer, 100 );
    mqttEl.subscribe(buffer, qos);
  }
}

void mqttElSubscribeFull(String topic) {
  if (mqttStatus==MQTT_STATUS_CONNECTED) {
   String fullTopic = topic;
    fullTopic.toCharArray( buffer, 100 );
    mqttEl.subscribe(buffer);
  }
}

void mqttElSubscribeFull(String topic, int qos) {
  if (mqttStatus==MQTT_STATUS_CONNECTED) {
    String fullTopic = topic;
    fullTopic.toCharArray( buffer, 100 );
    mqttEl.subscribe(buffer, qos);
  }
}

void mqttSubscribe() {
  if (mqttStatus==MQTT_STATUS_CONNECTED) {
    mqttElSubscribe("set/+");
    mqttElSubscribeFull("stat/sonoff/+",1);
  }
}

// Callback when MQTT is connected
void mqttElConnected(void* response) {
  Serial.println(F("MQTT connected!"));
  mqttSubscribe();
  mqttElconnected = true;
  mqttStatus = MQTT_STATUS_CONNECTED;
  eventMqttConnected();
  mqttSubscribe();
}

// Callback when MQTT is disconnected
void mqttElDisconnected(void* response) {
  Serial.println(F("MQTT disconnected"));
  mqttStatus = MQTT_STATUS_DISCONNECTED;
  mqttElconnected = false;
  eventMqttDisconnected();
}

void mqttElPublished(void* response) {
  Serial.println(F("MQTT published"));
}

void mqttElCheck() {
  mqttElesp.Process();
}

// Callback when an MQTT message arrives for one of our subscriptions
void mqttElData(void* response) {
  ELClientResponse *res = (ELClientResponse *)response;

  Serial.print(F("Received: topic="));
  String topic = res->popString();
  Serial.println(topic);

  Serial.print(F("data="));
  String Value = res->popString();
  Value.toLowerCase();
  Serial.println(Value);

  String endpoint = topic;
  endpoint = endpoint.substring(strlen(mqttElDeviceName)+1);


  if (endpoint==setBufferFromFlash(setLedState)) {
    if (Value == setBufferFromFlash(charOn)) {
      
      ledManualOnOff = LED_MANUAL_ONOFF_ON;
      mqttElPublishFull("cmnd/sonoff/POWER1","ON");
      mqttElPublish( setBufferFromFlash(getLedState), setBufferFromFlash(charOn));
    }
    else if (Value == setBufferFromFlash(charOff)) {  
      ledManualOnOff = LED_MANUAL_ONOFF_OFF;
      mqttElPublishFull("cmnd/sonoff/POWER1","OFF");
      mqttElPublish( setBufferFromFlash(getLedState), setBufferFromFlash(charOff));
    }
    else if (Value == setBufferFromFlash(charWave)) {
      ledManualOnOff = LED_MANUAL_ONOFF_AUTO;
    }
  }


  //if (String(topic)=="stat/sonoff/POWER1") {\

   if (String(topic)=="stat/sonoff/RESULT") {
    if (Value=="{\"power1\":\"on\"}") {

    //if (Value == setBufferFromFlash(charOn)) {    
      ledManualOnOff = LED_MANUAL_ONOFF_ON;
      mqttElPublish( setBufferFromFlash(getLedState), setBufferFromFlash(charOn));
    }
    if (Value=="{\"power1\":\"off\"}") {
    //else if (Value == setBufferFromFlash(charOff)) {  
      ledManualOnOff = LED_MANUAL_ONOFF_OFF;
      mqttElPublish( setBufferFromFlash(getLedState), setBufferFromFlash(charOff));
    }
    else if (Value == setBufferFromFlash(charWave)) {
      ledManualOnOff = LED_MANUAL_ONOFF_AUTO;
    }
  }
  
  /*
    if (Value == setBufferFromFlash(charOn)) {    
      relay1ManualOnOff = RELAY_MANUAL_ONOFF_ON;
    }
    else if (Value == setBufferFromFlash(charOff)) {  
      relay1ManualOnOff = RELAY_MANUAL_ONOFF_OFF;
    }
    else if (Value == setBufferFromFlash(charWave)) {  
      relay1ManualOnOff = RELAY_MANUAL_ONOFF_AUTO;
    }
  }
  */

  if (String(topic)=="stat/sonoff/POWER3") {

    if (Value == setBufferFromFlash(charOn)) {    
      relay2ManualOnOff = RELAY_MANUAL_ONOFF_ON;
    }
    else if (Value == setBufferFromFlash(charOff)) {  
      relay2ManualOnOff = RELAY_MANUAL_ONOFF_OFF;
    }
    else if (Value == setBufferFromFlash(charWave)) {  
      relay2ManualOnOff = RELAY_MANUAL_ONOFF_AUTO;
    }
  }

  //Buzzer
  if (endpoint==setBufferFromFlash(setBuzzerOnStart)) {
    if (Value == setBufferFromFlash(charOff)) {    
      buzzerOnStart = BUZZER_OFF;
      mqttElPublish( setBufferFromFlash(getBuzzerOnStart), setBufferFromFlash(charOff));
    }
    else {  
      buzzerOnStart = BUZZER_ON;
      mqttElPublish( setBufferFromFlash(getBuzzerOnStart), setBufferFromFlash(charOn));
    }
    configSaveValue(buzzerOnStart, EEPROM_buzzerOnStart_addr);
  }

  if (endpoint==setBufferFromFlash(setBuzzerOnErrors)) {
    if (Value == setBufferFromFlash(charOff)) {    
      buzzerOnErrors = BUZZER_OFF;
      mqttElPublish( setBufferFromFlash(getBuzzerOnErrors), setBufferFromFlash(charOff));
    }
    else {  
      buzzerOnErrors = BUZZER_ON;
      mqttElPublish( setBufferFromFlash(getBuzzerOnErrors), setBufferFromFlash(charOn));
    }
    configSaveValue(buzzerOnErrors, EEPROM_buzzerOnErrors_addr);
  }

  //Brigtness
  if (endpoint==setBufferFromFlash(setBrightness)) {
    int actualPartOfDay = schedulerGetActualPartOfDay();
    if (ledControlMode==CONTROL_MODE_PART_OF_DAY) {
      
      if (actualPartOfDay==SCHEDULER_MODE_MORNING) {
        ledMorningBrightness = Value.toInt();
      }
      else if (actualPartOfDay==SCHEDULER_MODE_AFTERNOON) {
        ledAfternoonBrightness = Value.toInt();
      }
      else if (actualPartOfDay==SCHEDULER_MODE_EVENING) {
        ledEveningBrightness = Value.toInt();
      }
      else {
        ledNightBrightness = Value.toInt();
      }
    }
    else {
      ledManualBrightness = Value.toInt();
    }
    configSaveLedBrightness(actualPartOfDay, Value.toInt());
    ledSetBrightness(Value.toInt());
    mqttElPublish(setBufferFromFlash(getBrightness), (String)Value.toInt());
    
  }

  //Date
  if (endpoint == setBufferFromFlash(setActualDate)) {
    globalYear = parseYearFromString(Value);
    globalMonth = parseMonthFromString(Value);
    globalMonthDay = parseDayFromString(Value);
    sprintf(buffer,"%04d/%02d/%02d", globalYear, globalMonth, globalMonthDay);
    buffer[10] = 0;
    mqttElPublish( setBufferFromFlash(getActualDate), buffer );
    clockSetLocalTime();
    clockGetLocalTime();
    mqttElPublish(setBufferFromFlash(getActualDayOfWeek), setBufferFromFlash(daysOfTheWeek[globalWeekDay]));
  }
  
  if (endpoint == setBufferFromFlash(setActualTime)) {
    globalHour = parseHourFromString(Value);
    globalMinute = parseMinuteFromString(Value);
    clockSetLocalTime();
  
    time_t local = clockGetLocalTime();
    int mymin = minute(local);
    int myhour = hour(local);
    sprintf(buffer,"%02d:%02d", myhour, mymin);
    buffer[5] = 0;
    mqttElPublish(setBufferFromFlash(getActualTime), buffer );
  }

  //Timezone
  /*
  if (endpoint == setBufferFromFlash(setTimezone)) {
      char tZone = Value.toInt();
      clockSetTimezone(tZone);
  }
  */
  if (endpoint==setBufferFromFlash(setTimezoneRule1Week) || endpoint==setBufferFromFlash(setTimezoneRule2Week)) {
    byte val;
    if (endpoint==setBufferFromFlash(setTimezoneRule1Week)) {
      val = timezoneRule1Week;
    }
    else {
      val = timezoneRule2Week;
    }

    if (Value == setBufferFromFlash(timezoneWeekFirst)) {
      val = TIMEZONE_WEEK_FIRST;
    }
    else if (Value == setBufferFromFlash(timezoneWeekSecond)) {
      val = TIMEZONE_WEEK_SECOND;
    }
    else if (Value == setBufferFromFlash(timezoneWeekThird)) {
      val = TIMEZONE_WEEK_THIRD;
    }
    else if (Value == setBufferFromFlash(timezoneWeekFourth)) {
      val = TIMEZONE_WEEK_FOURTH;
    }
    else if (Value == setBufferFromFlash(timezoneWeekLast)) {
      val = TIMEZONE_WEEK_LAST;
    }

    if (endpoint==setBufferFromFlash(setTimezoneRule1Week)) {
      configSaveValue(val, EEPROM_timezoneRule1Week_addr);
      timezoneRule1Week = val;
      mqttElPublish(setBufferFromFlash(getTimezoneRule1Week), clockTimezoneCodeToWeek(val));
    }
    else {
      configSaveValue(val, EEPROM_timezoneRule2Week_addr);
      timezoneRule2Week = val;
      mqttElPublish(setBufferFromFlash(getTimezoneRule2Week), clockTimezoneCodeToWeek(val));
    }
    clockUpdateTimezoneRules();
  }

  if (endpoint==setBufferFromFlash(setTimezoneRule1DayOfWeek) || endpoint==setBufferFromFlash(setTimezoneRule2DayOfWeek)) {
    byte val;
    if (endpoint==setBufferFromFlash(setTimezoneRule1Week)) {
      val = timezoneRule1DayOfWeek;
    }
    else {
      val = timezoneRule1DayOfWeek;
    }
    String DoW;
    for (int i=0; i<7; i++) {
      DoW = setBufferFromFlash(daysOfTheWeek[i]);
      DoW.toLowerCase();
      if (Value == DoW) {
        switch(i) {
          case 0: val = TIMEZONE_DOW_SUNDAY;
                  break;
          case 1: val = TIMEZONE_DOW_MONDAY;
                  break;
          case 2: val = TIMEZONE_DOW_TUESDAY;
                  break;
          case 3: val = TIMEZONE_DOW_WEDNESDAY;
                  break;
          case 4: val = TIMEZONE_DOW_FOURSDAY;
                  break;
          case 5: val = TIMEZONE_DOW_FRIDAY;
                  break;
          case 6: val = TIMEZONE_DOW_SATURDAY;
                  break;
        }
      }
    }
    
    if (endpoint==setBufferFromFlash(timezoneRule1DayOfWeek)) {
      configSaveValue(val, EEPROM_timezoneRule1DayOfWeek_addr);
      timezoneRule1Week = val;
      mqttElPublish(setBufferFromFlash(getTimezoneRule1DayOfWeek), clockTimezoneCodeToDoW(val));
    }
    else {
      configSaveValue(val, EEPROM_timezoneRule2DayOfWeek_addr);
      timezoneRule2Week = val;
      mqttElPublish(setBufferFromFlash(getTimezoneRule2DayOfWeek), clockTimezoneCodeToDoW(val));
    }
    clockUpdateTimezoneRules();
  }

  if (endpoint==setBufferFromFlash(setTimezoneRule1Month) || endpoint==setBufferFromFlash(setTimezoneRule2Month)) {
    byte val;
    if (endpoint==setBufferFromFlash(setTimezoneRule1Month)) {
      val = timezoneRule1Month;
    }
    else {
      val = timezoneRule2Month;
    }
    String DoW;
    for (int i=0; i<12; i++) {
      DoW = setBufferFromFlash(monthsOfYear[i]);
      DoW.toLowerCase();
      if (Value == DoW) {
        switch(i) {
          case 0: val = TIMEZONE_MONTH_JANUARY;
                  break;
          case 1: val = TIMEZONE_MONTH_FEBRUARY;
                  break;
          case 2: val = TIMEZONE_MONTH_MARCH;
                  break;
          case 3: val = TIMEZONE_MONTH_APRIL;
                  break;
          case 4: val = TIMEZONE_MONTH_MAY;
                  break;
          case 5: val = TIMEZONE_MONTH_JUNE;
                  break;
          case 6: val = TIMEZONE_MONTH_JULY;
                  break;
          case 7: val = TIMEZONE_MONTH_AUGUST;
                  break;
          case 8: val = TIMEZONE_MONTH_SEPTEMBER;
                  break;
          case 9: val = TIMEZONE_MONTH_OCTOBER;
                  break;
          case 10: val = TIMEZONE_MONTH_NOVEMBER;
                  break;
          case 11: val = TIMEZONE_MONTH_DECEMBER;
                  break;
        }
      }
    }
    
    if (endpoint==setBufferFromFlash(setTimezoneRule1Month)) {
      configSaveValue(val, EEPROM_timezoneRule1Month_addr);
      timezoneRule1Month = val;
      mqttElPublish(setBufferFromFlash(getTimezoneRule1Month), clockTimezoneCodeToMonth(val));
    }
    else {
      configSaveValue(val, EEPROM_timezoneRule2Month_addr);
      timezoneRule2Month = val;
      mqttElPublish(setBufferFromFlash(getTimezoneRule2Month), clockTimezoneCodeToMonth(val));
    }
    clockUpdateTimezoneRules();
  }
  
  if (endpoint == setBufferFromFlash(setTimezoneRule1Hour)) {
    int temp = stringToInt(Value);
    if (temp>=0 && temp<=23) {
      timezoneRule1Hour = temp;
      configSaveValue(timezoneRule1Hour, EEPROM_timezoneRule1Hour_addr);
    }
    mqttElPublish(setBufferFromFlash(getTimezoneRule1Hour), intToString(timezoneRule1Hour));
    clockUpdateTimezoneRules();
  }

  if (endpoint == setBufferFromFlash(setTimezoneRule2Hour)) {
    int temp = stringToInt(Value);
    if (temp>=0 && temp<=23) {
      timezoneRule2Hour = temp;
      configSaveValue(timezoneRule2Hour, EEPROM_timezoneRule2Hour_addr);
    }
    mqttElPublish(setBufferFromFlash(getTimezoneRule2Hour), intToString(timezoneRule2Hour));
    clockUpdateTimezoneRules();
  }

  if (endpoint == setBufferFromFlash(setTimezoneRule1Offset)) {
    char temp = stringToInt(Value);
    timezoneRule1Offset = temp;
    configSaveValue(timezoneRule1Offset, EEPROM_timezoneRule1Offset_addr);
    sprintf(buffer,"%+d", temp);
    mqttElPublish(setBufferFromFlash(getTimezoneRule1Offset), (String)buffer);
    clockUpdateTimezoneRules();
  }

  if (endpoint == setBufferFromFlash(setTimezoneRule2Offset)) {
    char temp = stringToInt(Value);
    timezoneRule2Offset = temp;
    configSaveValue(timezoneRule2Offset, EEPROM_timezoneRule2Offset_addr);
    sprintf(buffer,"%+d", temp);
    mqttElPublish(setBufferFromFlash(getTimezoneRule2Offset), (String)buffer);
    clockUpdateTimezoneRules();
  }
  
  //PH
  if (endpoint == setBufferFromFlash(setPHMin)) {
    float temp = stringToFloat(Value);
    if (temp>=0 && temp<=14) {
      phMin = temp;
      configSaveFloatValue(phMin, EEPROM_phMin_addr);
    }
    mqttElPublish(setBufferFromFlash(getPHMin), floatToString(phMin));
  }

  if (endpoint == setBufferFromFlash(setPHMax)) {
    float temp = stringToFloat(Value);
    if (temp>=0 && temp<=14) {
      phMax = temp;
      configSaveFloatValue(phMax, EEPROM_phMax_addr);
    }
    mqttElPublish(setBufferFromFlash(getPHMax), floatToString(phMax));
  }

  if (endpoint == setBufferFromFlash(setPH1Calib)) {
    float temp = stringToInt(Value);
    if (temp>0) {
      phValue1 = temp;
      configSaveFloatValue(phValue1, EEPROM_phValue1_addr);
    }
    mqttElPublish(setBufferFromFlash(getPH1Calib), floatToString(phValue1));
  }

  if (endpoint == setBufferFromFlash(setPH2Calib)) {
    float temp = stringToInt(Value);
    if (temp>0) {
      phValue2 = temp;
      configSaveFloatValue(phValue2, EEPROM_phValue2_addr);
    }
    mqttElPublish(setBufferFromFlash(getPH2Calib), floatToString(phValue2));
  }

  if (endpoint == setBufferFromFlash(setPH1RawCalib)) {
    float temp = stringToInt(Value);
    if (temp>0) {
      phRawRead1 = temp;
      configSaveFloatValue(phRawRead1, EEPROM_phRawRead1_addr);
    }
    mqttElPublish(setBufferFromFlash(getPH1RawCalib), intToString((int)phRawRead1));
  }

  if (endpoint == setBufferFromFlash(setPH2RawCalib)) {
    float temp = stringToInt(Value);
    if (temp>0) {
      phRawRead2 = temp;
      configSaveFloatValue(phRawRead2, EEPROM_phRawRead2_addr);
    }
    mqttElPublish(setBufferFromFlash(getPH2RawCalib), intToString((int)phRawRead2));
  }

  //Temperature
  if (endpoint == setBufferFromFlash(setTempMin)) {
    float temp = stringToFloat(Value);
    tempMin = temp;
    configSaveFloatValue(tempMin, EEPROM_tempMin_addr);
    mqttElPublish(setBufferFromFlash(getTempMin), floatToString(tempMin));
  }

  if (endpoint == setBufferFromFlash(setTempMax)) {
    float temp = stringToFloat(Value);
    tempMax = temp;
    configSaveFloatValue(tempMax, EEPROM_tempMax_addr);
    mqttElPublish(setBufferFromFlash(getTempMax), floatToString(tempMax));
  }

  if (endpoint == setBufferFromFlash(setTemp1Calib)) {
    float temp = stringToInt(Value);
    if (temp>0) {
      tempValue1 = temp;
      configSaveFloatValue(tempValue1, EEPROM_tempValue1_addr);
    }
    mqttElPublish(setBufferFromFlash(getTemp1Calib), floatToString(tempValue1));
  }

  if (endpoint == setBufferFromFlash(setTemp2Calib)) {
    float temp = stringToInt(Value);
    if (temp>0) {
      tempValue2 = temp;
      configSaveFloatValue(tempValue2, EEPROM_tempValue2_addr);
    }
    mqttElPublish(setBufferFromFlash(getTemp2Calib), floatToString(tempValue2));
  }

  if (endpoint == setBufferFromFlash(setTemp1RawCalib)) {
    float temp = stringToInt(Value);
    if (temp>0) {
      tempRawRead1 = temp;
      configSaveFloatValue(tempRawRead1, EEPROM_tempRawRead1_addr);
    }
    mqttElPublish(setBufferFromFlash(getTemp1RawCalib), intToString((int)tempRawRead1));
  }

  if (endpoint == setBufferFromFlash(setTemp2RawCalib)) {
    float temp = stringToInt(Value);
    if (temp>0) {
      tempRawRead2 = temp;
      configSaveFloatValue(tempRawRead2, EEPROM_tempRawRead2_addr);
    }
    mqttElPublish(setBufferFromFlash(getTemp2RawCalib), intToString((int)tempRawRead2));
  }

  //Fan
  if (endpoint == setBufferFromFlash(setFanStartTemperature)) {
    float temp = stringToInt(Value);
    if (temp>0) {
      fanStartTemperature = temp;
      configSaveValue(fanStartTemperature, EEPROM_fanStartTemperature_addr);
    }
    mqttElPublish( setBufferFromFlash(getFanStartTemperature), intToString(fanStartTemperature));
  }

  if (endpoint == setBufferFromFlash(setFanMaxSpeedTemperature)) {
    float temp = stringToInt(Value);
    if (temp>0) {
      fanMaxSpeedTemperature = temp;
      configSaveValue(fanMaxSpeedTemperature, EEPROM_fanMaxSpeedTemperature_addr);
    }
    mqttElPublish( setBufferFromFlash(getFanMaxSpeedTemperature), intToString(fanMaxSpeedTemperature));
  }
  
  if (endpoint == setBufferFromFlash(setMaxInternalTemperature)) {
    float temp = stringToInt(Value);
    if (temp>0) {
      maxInternalTemperature = temp;
      configSaveValue(maxInternalTemperature, EEPROM_maxInternalTemperature_addr);
    }
    mqttElPublish( setBufferFromFlash(getMaxInternalTemperature), intToString(maxInternalTemperature));
  }

  //Daily settings
  if (endpoint == setBufferFromFlash(setMorningTime)) {
      schedulerStartMorningHour = parseHourFromString(Value);
      schedulerStartMorningMinute = parseMinuteFromString(Value);
      configSaveSchedulerTimers();
      sprintf(buffer,"%02d:%02d", schedulerStartMorningHour, schedulerStartMorningMinute);
      buffer[5] = 0;
      mqttElPublish( setBufferFromFlash(getMorningTime), buffer );
  }

  if (endpoint == setBufferFromFlash(setAfternoonTime)) {
      schedulerStartAfternoonHour = parseHourFromString(Value);
      schedulerStartAfternoonMinute = parseMinuteFromString(Value);
      configSaveSchedulerTimers();
      sprintf(buffer,"%02d:%02d", schedulerStartAfternoonHour, schedulerStartAfternoonMinute);
      buffer[5] = 0;
      mqttElPublish( setBufferFromFlash(getAfternoonTime), buffer );
  }

  if (endpoint == setBufferFromFlash(setEveningTime)) {
      schedulerStartEveningHour = parseHourFromString(Value);
      schedulerStartEveningMinute = parseMinuteFromString(Value);
      configSaveSchedulerTimers();
      sprintf(buffer,"%02d:%02d", schedulerStartEveningHour, schedulerStartEveningMinute);
      buffer[5] = 0;
      mqttElPublish( setBufferFromFlash(getEveningTime), buffer );
  }

  if (endpoint == setBufferFromFlash(setNightTime)) {
      schedulerStartNightHour = parseHourFromString(Value);
      schedulerStartNightMinute = parseMinuteFromString(Value);
      configSaveSchedulerTimers();
      sprintf(buffer,"%02d:%02d", schedulerStartNightHour, schedulerStartNightMinute);
      buffer[5] = 0;
      mqttElPublish( setBufferFromFlash(getNightTime), buffer );
  }


  //Sensors
  String sensorsEndpoint;
  for (byte i = 0; i<SENSORS_COUNT; i++) {
    sensorsEndpoint = setBufferFromFlash(charSetSensor)+intToString(i)+setBufferFromFlash(charSensorType);
    if (endpoint == sensorsEndpoint) {
      byte bValue = 255;
      if (Value==setBufferFromFlash(charSensornone)) {
        bValue = SENSOR_TYPE_NONE;
      }
      if (Value==setBufferFromFlash(charPhsensor)) {
        bValue = SENSOR_TYPE_PH;
      }
      if (Value==setBufferFromFlash(charThermometer)) {
        bValue = SENSOR_TYPE_THERMOMETER;
      }
      if (Value==setBufferFromFlash(charAquawaterlevel)) {
        bValue = SENSOR_TYPE_AQUA_WATER_LEVEL;
      }
      if (Value==setBufferFromFlash(charTankwaterlevel)) {
        bValue = SENSOR_TYPE_TANK_WATER_LEVEL;
      }
      if (bValue!=255) {
        sensorsSetSensor(i, SENSORS_VALUE_TYPE, bValue);
        mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(i)+setBufferFromFlash(charSensorType),Value);
      }
    }
    sensorsEndpoint = setBufferFromFlash(charSetSensor)+intToString(i)+setBufferFromFlash(charSensorCalibValue1);
    if (endpoint == sensorsEndpoint) {
      sensorsSetSensor(i, SENSORS_VALUE_CALIB_VALUE1, stringToFloat(Value));
      float myValue = sensorsGetSensor(i, SENSORS_VALUE_CALIB_VALUE1);
      mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(i)+setBufferFromFlash(charSensorCalibValue1),floatToString(myValue));
    }
    sensorsEndpoint = setBufferFromFlash(charSetSensor)+intToString(i)+setBufferFromFlash(charSensorCalibRawValue1);
    if (endpoint == sensorsEndpoint) {
      sensorsSetSensor(i, SENSORS_VALUE_CALIB_RAW_VALUE1, stringToFloat(Value));
      int myValue = sensorsGetSensor(i, SENSORS_VALUE_CALIB_RAW_VALUE1);
      mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(i)+setBufferFromFlash(charSensorCalibRawValue1),intToString(myValue));
    }
    sensorsEndpoint = setBufferFromFlash(charSetSensor)+intToString(i)+setBufferFromFlash(charSensorCalibValue2);
    if (endpoint == sensorsEndpoint) {
      sensorsSetSensor(i, SENSORS_VALUE_CALIB_VALUE2, stringToFloat(Value));
      float myValue = sensorsGetSensor(i, SENSORS_VALUE_CALIB_VALUE2);
      mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(i)+setBufferFromFlash(charSensorCalibValue2),floatToString(myValue));
    }
    sensorsEndpoint = setBufferFromFlash(charSetSensor)+intToString(i)+setBufferFromFlash(charSensorCalibRawValue2);
    if (endpoint == sensorsEndpoint) {
      sensorsSetSensor(i, SENSORS_VALUE_CALIB_RAW_VALUE2, stringToFloat(Value));
      int myValue = sensorsGetSensor(i, SENSORS_VALUE_CALIB_RAW_VALUE2);
      mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(i)+setBufferFromFlash(charSensorCalibRawValue2),intToString(myValue));
    }
    sensorsEndpoint = setBufferFromFlash(charSetSensor)+intToString(i)+setBufferFromFlash(charSensorMinValue);
    if (endpoint == sensorsEndpoint) {
      sensorsSetSensor(i, SENSORS_VALUE_MIN, stringToFloat(Value));
      float myValue = sensorsGetSensor(i, SENSORS_VALUE_MIN);
      mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(i)+setBufferFromFlash(charSensorMinValue),floatToString(myValue));
    }
    sensorsEndpoint = setBufferFromFlash(charSetSensor)+intToString(i)+setBufferFromFlash(charSensorMaxValue);
    if (endpoint == sensorsEndpoint) {
      sensorsSetSensor(i, SENSORS_VALUE_MAX, stringToFloat(Value));
      float myValue = sensorsGetSensor(i, SENSORS_VALUE_MAX);
      mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(i)+setBufferFromFlash(charSensorMaxValue),floatToString(myValue));
    }
    sensorsEndpoint = setBufferFromFlash(charSetSensor)+intToString(i)+setBufferFromFlash(charSensorCriticalMinValue);
    if (endpoint == sensorsEndpoint) {
      sensorsSetSensor(i, SENSORS_VALUE_CRITICAL_MIN, stringToFloat(Value));
      float myValue = sensorsGetSensor(i, SENSORS_VALUE_CRITICAL_MIN);
      mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(i)+setBufferFromFlash(charSensorCriticalMinValue),floatToString(myValue));
    }
    sensorsEndpoint = setBufferFromFlash(charSetSensor)+intToString(i)+setBufferFromFlash(charSensorCriticalMaxValue);
    if (endpoint == sensorsEndpoint) {
      sensorsSetSensor(i, SENSORS_VALUE_CRITICAL_MAX, stringToFloat(Value));
      float myValue = sensorsGetSensor(i, SENSORS_VALUE_CRITICAL_MAX);
      mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(i)+setBufferFromFlash(charSensorCriticalMaxValue),floatToString(myValue));
    }
  }
  
  //Relays
  if (endpoint == setBufferFromFlash(setRelay1ControlMode) || endpoint == setBufferFromFlash(setRelay2ControlMode)) {
    byte bValue;
    if (Value==setBufferFromFlash(charManual)) {
      bValue = CONTROL_MODE_MANUAL;
    }
    if (Value==setBufferFromFlash(charPartofday)) {
      bValue = CONTROL_MODE_PART_OF_DAY;
    }
    if (Value==setBufferFromFlash(charTemperature)) {
      bValue = CONTROL_MODE_TEMPERATURE;
    }
    if (Value==setBufferFromFlash(charPh)) {
      bValue = CONTROL_MODE_PH;
    }
    if (Value==setBufferFromFlash(charAquawaterlevel)) {
      bValue = CONTROL_MODE_AQUAWATERLEVEL;
    }
    if (Value==setBufferFromFlash(charTankwaterlevel)) {
      bValue = CONTROL_MODE_TANKWATERLEVEL;
    }
    if (Value==setBufferFromFlash(charPhsensor1)) {
      bValue = CONTROL_MODE_PHSENSOR1;
    }
    if (Value==setBufferFromFlash(charPhsensor2)) {
      bValue = CONTROL_MODE_PHSENSOR2;
    }
    if (Value==setBufferFromFlash(charThermometer1)) {
      bValue = CONTROL_MODE_THERMOMETER1;
    }
    if (Value==setBufferFromFlash(charThermometer2)) {
      bValue = CONTROL_MODE_THERMOMETER2;
    }
    if (Value==setBufferFromFlash(charWaterlevelsensor1)) {
      bValue = CONTROL_MODE_WATERLEVELSENSOR1;
    }
    if (Value==setBufferFromFlash(charWaterlevelsensor2)) {
      bValue = CONTROL_MODE_WATERLEVELSENSOR2;
    }
    if (Value==setBufferFromFlash(charWaterlevelsensor3)) {
      bValue = CONTROL_MODE_WATERLEVELSENSOR3;
    }
    if (Value==setBufferFromFlash(charWaterlevelsensor4)) {
      bValue = CONTROL_MODE_WATERLEVELSENSOR4;
    }

    if (endpoint == setBufferFromFlash(setRelay1ControlMode)) {
      relay1ControlMode = bValue;
      configSaveValue(bValue,EEPROM_relay1ControlMode_addr);
      mqttElPublish(setBufferFromFlash(getRelay1ControlMode), Value);
    }
    else {
      relay2ControlMode = bValue;
      configSaveValue(bValue,EEPROM_relay2ControlMode_addr);
      mqttElPublish(setBufferFromFlash(getRelay2ControlMode), Value);
    }
  }

/*
  if (endpoint == setBufferFromFlash(setRelay1State)) {
    if (Value == setBufferFromFlash(charOn)) {
      relay1ManualOnOff = RELAY_MANUAL_ONOFF_ON;
      mqttElPublish(setBufferFromFlash(getRelay1State), setBufferFromFlash(charOn));
    }
    else if (Value == setBufferFromFlash(charOff)) {  
      relay1ManualOnOff = RELAY_MANUAL_ONOFF_OFF;
      mqttElPublish(setBufferFromFlash(getRelay1State), setBufferFromFlash(charOff));
    }
  }
*/

  if (String(topic)=="stat/sonoff/RESULT") {
    if (Value=="{\"power2\":\"on\"}") {
      relay1ManualOnOff = RELAY_MANUAL_ONOFF_ON;
      mqttElPublish(setBufferFromFlash(getRelay1State), setBufferFromFlash(charOn));
      relay1Up();
    }
    if (Value=="{\"power2\":\"off\"}") {
      relay1ManualOnOff = RELAY_MANUAL_ONOFF_OFF;
      mqttElPublish(setBufferFromFlash(getRelay1State), setBufferFromFlash(charOff));
      relay1Down();
    }
  }
  
  if (endpoint == setBufferFromFlash(setRelay1State)) {
    if (Value == setBufferFromFlash(charOn)) {
      relay1ManualOnOff = RELAY_MANUAL_ONOFF_ON;
      mqttElPublish(setBufferFromFlash(getRelay1State), setBufferFromFlash(charOn));
      mqttElPublishFull("cmnd/sonoff/POWER2","ON");
      relay1Up();
    }
    else if (Value == setBufferFromFlash(charOff)) {
      relay1ManualOnOff = RELAY_MANUAL_ONOFF_OFF;
      mqttElPublish(setBufferFromFlash(getRelay1State), setBufferFromFlash(charOff));
      mqttElPublishFull("cmnd/sonoff/POWER2","OFF");
      relay1Down();
    }
  }
  
  if (endpoint == setBufferFromFlash(setRelay2State)) {
    if (Value == setBufferFromFlash(charOn)) {
      relay2ManualOnOff = RELAY_MANUAL_ONOFF_ON;
      mqttElPublish(setBufferFromFlash(getRelay2State), setBufferFromFlash(charOn));
      relay2Up();
    }
    else if (Value == setBufferFromFlash(charOff)) {
      relay2ManualOnOff = RELAY_MANUAL_ONOFF_OFF;
      mqttElPublish(setBufferFromFlash(getRelay2State), setBufferFromFlash(charOff));
      relay2Down();
    }
  }
  
  if (endpoint == setBufferFromFlash(setRelay1MorningMode) || endpoint == setBufferFromFlash(setRelay1AfternoonMode) || endpoint == setBufferFromFlash(setRelay1EveningMode) || endpoint == setBufferFromFlash(setRelay1NightMode)) {
    int tempRelayMode;
    if (Value == setBufferFromFlash(charOn)) {    
      tempRelayMode = RELAY_MODE_ON;
    }
    else if (Value == setBufferFromFlash(charOff)) {  
      tempRelayMode = RELAY_MODE_OFF;
    }
    else {
      tempRelayMode = RELAY_MODE_NONE;
    }
    
    if (endpoint == setBufferFromFlash(setRelay1MorningMode)) {
      if (relay1ModeMorning!=tempRelayMode) {
        configSaveValue(tempRelayMode,EEPROM_relay1ModeMorning_addr);
        mqttElPublish(setBufferFromFlash(getRelay1MorningMode), Value);
        relay1ModeMorning = tempRelayMode;
      }
    }
    if (endpoint == setBufferFromFlash(setRelay1AfternoonMode)) {
      if (relay1ModeAfternoon!=tempRelayMode) {
        configSaveValue(tempRelayMode,EEPROM_relay1ModeAfternoon_addr);
        mqttElPublish( setBufferFromFlash(getRelay1AfternoonMode), Value);
        relay1ModeAfternoon = tempRelayMode;
      }
    }
    if (endpoint == setBufferFromFlash(setRelay1EveningMode)) {
      if (relay1ModeEvening!=tempRelayMode) {
        configSaveValue(tempRelayMode,EEPROM_relay1ModeEvening_addr);
        mqttElPublish( setBufferFromFlash(getRelay1EveningMode), Value);
        relay1ModeEvening = tempRelayMode;
      }
    }
    if (endpoint == setBufferFromFlash(setRelay1NightMode)) {
      if (relay1ModeNight!=tempRelayMode) {
        configSaveValue(tempRelayMode,EEPROM_relay1ModeNight_addr);
        mqttElPublish(setBufferFromFlash(getRelay1NightMode), Value);
        relay1ModeNight = tempRelayMode;
      }
    }
  }

  if (endpoint == setBufferFromFlash(setRelay2MorningMode) || endpoint == setBufferFromFlash(setRelay2AfternoonMode) || endpoint == setBufferFromFlash(setRelay2EveningMode) || endpoint == setBufferFromFlash(setRelay2NightMode)) {
    int tempRelayMode;
    if (Value == setBufferFromFlash(charOn)) {    
      tempRelayMode = RELAY_MODE_ON;
    }
    else if (Value == setBufferFromFlash(charOff)) {  
      tempRelayMode = RELAY_MODE_OFF;
    }
    else {
      tempRelayMode = RELAY_MODE_NONE;
    }

    if (endpoint == setBufferFromFlash(setRelay2MorningMode)) {
      if (relay2ModeMorning!=tempRelayMode) {
        configSaveValue(tempRelayMode,EEPROM_relay2ModeMorning_addr);
        mqttElPublish(setBufferFromFlash(getRelay2MorningMode), Value);
        relay2ModeMorning = tempRelayMode;
      }
    }
    if (endpoint == setBufferFromFlash(setRelay2AfternoonMode)) {
      if (relay2ModeAfternoon!=tempRelayMode) {
        configSaveValue(tempRelayMode,EEPROM_relay2ModeAfternoon_addr);
        mqttElPublish(setBufferFromFlash(getRelay2AfternoonMode), Value);
        relay2ModeAfternoon = tempRelayMode;
      }
    }
    if (endpoint == setBufferFromFlash(setRelay2EveningMode)) {
      if (relay2ModeEvening!=tempRelayMode) {
        configSaveValue(tempRelayMode,EEPROM_relay2ModeEvening_addr);
        mqttElPublish( setBufferFromFlash(getRelay2EveningMode), Value);
        relay2ModeEvening = tempRelayMode;
      }
    }
    if (endpoint == setBufferFromFlash(setRelay2NightMode)) {
      if (relay2ModeNight!=tempRelayMode) {
        configSaveValue(tempRelayMode,EEPROM_relay2ModeNight_addr);
        mqttElPublish(setBufferFromFlash(getRelay2NightMode), Value);
        relay2ModeNight = tempRelayMode;
      }
    }
  }

  //LED
  if (endpoint == setBufferFromFlash(setLedControlMode)) {
    int tempRelayMode;
    if (Value == setBufferFromFlash(charManual)) {
      ledSetBrightness(LED_BRIGHTNESS_AUTO);
      ledControlMode = CONTROL_MODE_MANUAL;
      configSaveValue(ledControlMode,EEPROM_ledControlMode_addr);
      mqttElPublish(setBufferFromFlash(getLedControlMode), Value);
    }
    else if (Value == setBufferFromFlash(charPartofday)) {
      ledSetBrightness(LED_BRIGHTNESS_AUTO);
      ledControlMode = CONTROL_MODE_PART_OF_DAY;
      configSaveValue(ledControlMode,EEPROM_ledControlMode_addr);
      mqttElPublish(setBufferFromFlash(getLedControlMode), Value);
    }
  }
  
  if (endpoint == setBufferFromFlash(setLedManualMode) || endpoint == setBufferFromFlash(setLedColorMorning) || endpoint == setBufferFromFlash(setLedColorAfternoon) || endpoint == setBufferFromFlash(setLedColorEvening) || endpoint == setBufferFromFlash(setLedColorNight)) {

    int tempLedMode;

    if (Value == setBufferFromFlash(charBlack) && endpoint != setBufferFromFlash(setLedManualMode)) {
      tempLedMode = LED_MODE_BLACK;
    }
    if (Value == setBufferFromFlash(charRed)) {
      tempLedMode = LED_MODE_RED;
    }
    if (Value == setBufferFromFlash(charGreen)) {
      tempLedMode = LED_MODE_GREEN;
    }
    if (Value == setBufferFromFlash(charBlue)) {
      tempLedMode = LED_MODE_BLUE;
    }
    if (Value == setBufferFromFlash(charCyan)) {
      tempLedMode = LED_MODE_CYAN;
    }
    if (Value == setBufferFromFlash(charMagenta)) {
      tempLedMode = LED_MODE_MAGENTA;
    }
    if (Value == setBufferFromFlash(charYellow)) {
      tempLedMode = LED_MODE_YELLOW;
    }
    if (Value == setBufferFromFlash(charWhite)) {
      tempLedMode = LED_MODE_WHITE;
    }
    if (Value == setBufferFromFlash(charWave)) {
      tempLedMode = LED_MODE_WAVE;
    }
    if (endpoint == setBufferFromFlash(setLedManualMode)) {
      if (ledManualMode!=tempLedMode) {
        configSaveValue(tempLedMode,EEPROM_ledManualMode_addr);
        mqttElPublish(setBufferFromFlash(getLedManualMode), Value);
        ledManualMode = tempLedMode;
      }
    }
    else if (endpoint == setBufferFromFlash(setLedColorMorning)) {
      if (ledModeMorning!=tempLedMode) {
        configSaveValue(tempLedMode,EEPROM_ledModeMorning_addr);
        mqttElPublish(setBufferFromFlash(getLedColorMorning), Value);
        ledModeMorning = tempLedMode;
      }
    }
    else if (endpoint == setBufferFromFlash(setLedColorAfternoon)) {
      if (ledModeAfternoon!=tempLedMode) {
        configSaveValue(tempLedMode,EEPROM_ledModeAfternoon_addr);
        mqttElPublish(setBufferFromFlash(getLedColorAfternoon), Value);
        ledModeAfternoon = tempLedMode;
      }
    }
    else if (endpoint == setBufferFromFlash(setLedColorEvening)) {
      if (ledModeEvening!=tempLedMode) {
        configSaveValue(tempLedMode,EEPROM_ledModeEvening_addr);
        mqttElPublish(setBufferFromFlash(getLedColorEvening), Value);
        ledModeEvening = tempLedMode;
      }
    }
    else {
      if (ledModeNight!=tempLedMode) {
        configSaveValue(tempLedMode,EEPROM_ledModeNight_addr);
        mqttElPublish(setBufferFromFlash(getLedColorNight), Value);
        ledModeNight = tempLedMode;
      }
    }
    ledSetActualMode();
  }
}


