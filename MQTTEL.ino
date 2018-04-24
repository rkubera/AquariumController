/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/
 
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
    wdt_reset();
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
    wdt_reset();
  }
}

void mqttElSubscribe(String topic) {
  if (mqttStatus==MQTT_STATUS_CONNECTED) {
    String fullTopic = String(mqttElDeviceName)+'/'+topic;
    fullTopic.toCharArray( buffer, 100 );
    mqttEl.subscribe(buffer);
    wdt_reset();
  }

}

void mqttElSubscribe(String topic, int qos) {
  if (mqttStatus==MQTT_STATUS_CONNECTED) {
    String fullTopic = String(mqttElDeviceName)+'/'+topic;
    fullTopic.toCharArray( buffer, 100 );
    mqttEl.subscribe(buffer, qos);
    wdt_reset();
  }
}

void mqttElSubscribeFull(String topic) {
  if (mqttStatus==MQTT_STATUS_CONNECTED) {
   String fullTopic = topic;
    fullTopic.toCharArray( buffer, 100 );
    mqttEl.subscribe(buffer);
    wdt_reset();
  }
}

void mqttElSubscribeFull(String topic, int qos) {
  if (mqttStatus==MQTT_STATUS_CONNECTED) {
    String fullTopic = topic;
    fullTopic.toCharArray( buffer, 100 );
    mqttEl.subscribe(buffer, qos);
    wdt_reset();
  }
}

void mqttSubscribe() {
  if (mqttStatus==MQTT_STATUS_CONNECTED) {
    mqttElSubscribe("set/+");
    mqttElSubscribe("stat/+",1);
    wdt_reset();
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
  wdt_reset();
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
  String RawValue = res->popString();
  String Value = RawValue;
  Value.toLowerCase();
  Serial.println(Value);

  String endpoint = topic;
  endpoint = endpoint.substring(strlen(mqttElDeviceName)+1);

  if (endpoint==setBufferFromFlash(setLedState)) {
    if (Value == setBufferFromFlash(charOn)) {
      ledManualOnOff = LED_MANUAL_ONOFF_ON;
      mqttElPublish("cmnd/POWER1","ON");
      mqttElPublish( setBufferFromFlash(getLedState), setBufferFromFlash(charOn));
    }
    else if (Value == setBufferFromFlash(charOff)) {  
      ledManualOnOff = LED_MANUAL_ONOFF_OFF;
      mqttElPublish("cmnd/POWER1","OFF");
      mqttElPublish( setBufferFromFlash(getLedState), setBufferFromFlash(charOff));
    }
    else if (Value == setBufferFromFlash(charWave)) {
      ledManualOnOff = LED_MANUAL_ONOFF_AUTO;
    }
  }

  if (String(topic)=="ac/stat/RESULT") {
    if (Value=="{\"power1\":\"on\"}") {   
      ledManualOnOff = LED_MANUAL_ONOFF_ON;
      mqttElPublish( setBufferFromFlash(getLedState), setBufferFromFlash(charOn));
    }
    if (Value=="{\"power1\":\"off\"}") {
      ledManualOnOff = LED_MANUAL_ONOFF_OFF;
      mqttElPublish( setBufferFromFlash(getLedState), setBufferFromFlash(charOff));
    }


    if (Value=="{\"power2\":\"on\"}") {
      relaysSetRelay(3, RELAY_MANUAL_ONOFF, RELAY_MANUAL_ONOFF_ON);
      mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(3)+setBufferFromFlash(charState),setBufferFromFlash(charOn));
    }
    if (Value=="{\"power2\":\"off\"}") {
      relaysSetRelay(3, RELAY_MANUAL_ONOFF, RELAY_MANUAL_ONOFF_OFF);
      mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(3)+setBufferFromFlash(charState),setBufferFromFlash(charOff));
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
        ledMorningBrightness = stringToInt(Value);
      }
      else if (actualPartOfDay==SCHEDULER_MODE_AFTERNOON) {
        ledAfternoonBrightness = stringToInt(Value);
      }
      else if (actualPartOfDay==SCHEDULER_MODE_EVENING) {
        ledEveningBrightness = stringToInt(Value);
      }
      else {
        ledNightBrightness = stringToInt(Value);
      }
    }
    else {
      ledManualBrightness = stringToInt(Value);
    }
    configSaveLedBrightness(actualPartOfDay, stringToInt(Value));
    ledSetBrightness(stringToInt(Value));
    mqttElPublish(setBufferFromFlash(getBrightness), (String)stringToInt(Value));
    
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
  for (byte j = 0; j<SENSORS_COUNT; j++) {
    int i = j+1;
    sensorsEndpoint = setBufferFromFlash(charSetSensor)+intToString(i)+setBufferFromFlash(charName);
    if (endpoint == sensorsEndpoint) {
      sensorsSaveSensorName(i,RawValue);
      mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(i)+setBufferFromFlash(charName),RawValue.substring(0,NAME_LENGTH));
    }
    sensorsEndpoint = setBufferFromFlash(charSetSensor)+intToString(i)+setBufferFromFlash(charSensorType);
    if (endpoint == sensorsEndpoint) {
      byte bValue = sensorsGetByteSensorType(Value);
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
  }
  
  //Relays
  String relaysEndpoint;
  for (byte j = 0; j<RELAYS_COUNT; j++) {
    int i = j+1;
    relaysEndpoint = setBufferFromFlash(charSetRelay)+intToString(i)+setBufferFromFlash(charName);
    if (endpoint == relaysEndpoint) {
      relaysSaveRelayName(i,RawValue);
      mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(i)+setBufferFromFlash(charName),RawValue.substring(0,NAME_LENGTH));
    }
    
    relaysEndpoint = setBufferFromFlash(charSetRelay)+intToString(i)+setBufferFromFlash(charControlMode);
    if (endpoint == relaysEndpoint) {
      byte bValue;
      bValue=255;
      if (Value==setBufferFromFlash(charManual)) {
        bValue = CONTROL_MODE_MANUAL;
      }
      if (Value==setBufferFromFlash(charPartofday)) {
        bValue = CONTROL_MODE_PART_OF_DAY;
      }
      if (Value==setBufferFromFlash(charTreshold)) {
        bValue = CONTROL_MODE_TRESHOLD;
      }
      if (bValue!=255) {
        relaysSetRelay(i, RELAY_CONTROL_MODE, bValue);
        mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(i)+setBufferFromFlash(charControlMode),Value);
      }
    }

    relaysEndpoint = setBufferFromFlash(charSetRelay)+intToString(i)+setBufferFromFlash(charMaxDeviation);
    if (endpoint == relaysEndpoint) {
      relaysSetRelayDouble(i, RELAY_MAX_DEVIATION, stringToDouble(Value));
      Value = floatToString(relaysGetRelayDouble(i,RELAY_MAX_DEVIATION));
      mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(i)+setBufferFromFlash(charMaxDeviation),Value);
      relaysCheckTresholdDirection(i, OUTPUT_TYPE_RELAY);
    }

    relaysEndpoint = setBufferFromFlash(charSetRelay)+intToString(i)+setBufferFromFlash(charSensorsSetpoint);
    if (endpoint == relaysEndpoint) {
      relaysSetRelayDouble(i, RELAY_SENSORS_SETPOINT, stringToDouble(Value));
      Value = floatToString(relaysGetRelayDouble(i,RELAY_SENSORS_SETPOINT));
      mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(i)+setBufferFromFlash(charSensorsSetpoint),Value);
      relaysCheckTresholdDirection(i, OUTPUT_TYPE_RELAY);
    }
    
    relaysEndpoint = setBufferFromFlash(charSetRelay)+intToString(i)+setBufferFromFlash(charControlDirection);
    if (endpoint == relaysEndpoint) {
      if (Value == setBufferFromFlash(charDirect)) {
        relaysSetRelay(i, RELAY_CONTROL_DIRECTION,CONTROL_DIRECT);
        mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(i)+setBufferFromFlash(charControlDirection),setBufferFromFlash(charDirect));
      }
      if (Value == setBufferFromFlash(charReverse)) {
        relaysSetRelay(i, RELAY_CONTROL_DIRECTION,PID_REVERSE);
        mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(i)+setBufferFromFlash(charControlDirection),setBufferFromFlash(charReverse));
      }
    }
    
    relaysEndpoint = setBufferFromFlash(charSetRelay)+intToString(i)+setBufferFromFlash(charState);
    if (endpoint == relaysEndpoint) {
      if (Value == setBufferFromFlash(charOn)) {
        relaysSetRelay(i, RELAY_MANUAL_ONOFF, RELAY_MANUAL_ONOFF_ON);
        mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(i)+setBufferFromFlash(charState),setBufferFromFlash(charOn));
      }
      if (Value == setBufferFromFlash(charOff)) {
        relaysSetRelay(i, RELAY_MANUAL_ONOFF, RELAY_MANUAL_ONOFF_OFF);
        mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(i)+setBufferFromFlash(charState),setBufferFromFlash(charOff));
      }
    }

    relaysEndpoint = setBufferFromFlash(charSetRelay)+intToString(i);
    if (endpoint == relaysEndpoint+setBufferFromFlash(charMorningMode) || endpoint == relaysEndpoint+setBufferFromFlash(charAfternoonMode) || endpoint == relaysEndpoint+setBufferFromFlash(charEveningMode) || endpoint == relaysEndpoint+setBufferFromFlash(charNightMode)) {
      byte tempRelayMode;
      if (Value == setBufferFromFlash(charOn)) {    
        tempRelayMode = RELAY_MODE_ON;
      }
      else if (Value == setBufferFromFlash(charOff)) {  
        tempRelayMode = RELAY_MODE_OFF;
      }
      else {
        tempRelayMode = RELAY_MODE_NONE;
      }

      if (endpoint == relaysEndpoint+setBufferFromFlash(charMorningMode)) {
        if (tempRelayMode!= RELAY_MODE_NONE) {
          relaysSetRelay(i, RELAY_MODE_MORNING, tempRelayMode);
          mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(i)+setBufferFromFlash(charMorningMode),Value);
        }
      }

      if (endpoint == relaysEndpoint+setBufferFromFlash(charAfternoonMode)) {
        if (tempRelayMode!= RELAY_MODE_NONE) {
          relaysSetRelay(i, RELAY_MODE_AFTERNOON, tempRelayMode);
          mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(i)+setBufferFromFlash(charAfternoonMode),Value);
        }
      }

      if (endpoint == relaysEndpoint+setBufferFromFlash(charEveningMode)) {
        if (tempRelayMode!= RELAY_MODE_NONE) {
          relaysSetRelay(i, RELAY_MODE_EVENING, tempRelayMode);
          mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(i)+setBufferFromFlash(charEveningMode),Value);
        }
      }

      if (endpoint == relaysEndpoint+setBufferFromFlash(charNightMode)) {
        if (tempRelayMode!= RELAY_MODE_NONE) {
          relaysSetRelay(i, RELAY_MODE_NIGHT, tempRelayMode);
          mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(i)+setBufferFromFlash(charNightMode),Value);
        }
      }
    }
  }
  
  //PWMOutputs
  String pwmOutputsEndpoint;
  for (byte j = 0; j<PWMOUTPUTS_COUNT; j++) {
    int i = j+1;
    pwmOutputsEndpoint = setBufferFromFlash(charSetPwmOutput)+intToString(i)+setBufferFromFlash(charName);
    if (endpoint == pwmOutputsEndpoint) {
      pwmOutputsSavePwmOutputName(i,RawValue);
      mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charName),RawValue.substring(0,NAME_LENGTH));
    }
    
    pwmOutputsEndpoint = setBufferFromFlash(charSetPwmOutput)+intToString(i)+setBufferFromFlash(charControlMode);
    if (endpoint == pwmOutputsEndpoint) {
      byte bValue;
      bValue=255;
      if (Value==setBufferFromFlash(charManual)) {
        bValue = CONTROL_MODE_MANUAL;
      }
      if (Value==setBufferFromFlash(charPartofday)) {
        bValue = CONTROL_MODE_PART_OF_DAY;
      }
      if (Value==setBufferFromFlash(charTreshold)) {
        bValue = CONTROL_MODE_TRESHOLD;
      }
      if (Value==setBufferFromFlash(charPid)) {
        bValue = CONTROL_MODE_PID;
      }
      if (bValue!=255) {
        pwmOutputsSetPwmOutput(i, PWMOUTPUT_CONTROL_MODE, bValue);
        mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charControlMode),Value);
        pwmOutputsCheckTresholdDirection(i,OUTPUT_TYPE_PWM);
      }
    }

    pwmOutputsEndpoint = setBufferFromFlash(charSetPwmOutput)+intToString(i)+setBufferFromFlash(charControlDirection);
    if (endpoint == pwmOutputsEndpoint) {
      if (Value == setBufferFromFlash(charDirect)) {
        pwmOutputsSetPwmOutput(i, PWMOUTPUT_CONTROL_DIRECTION,CONTROL_DIRECT);
        mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charControlDirection),setBufferFromFlash(charDirect));
      }
      if (Value == setBufferFromFlash(charReverse)) {
        pwmOutputsSetPwmOutput(i, PWMOUTPUT_CONTROL_DIRECTION,PID_REVERSE);
        mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charControlDirection),setBufferFromFlash(charReverse));
      }
    }

    pwmOutputsEndpoint = setBufferFromFlash(charSetPwmOutput)+intToString(i)+setBufferFromFlash(charMaxDeviation);
    if (endpoint == pwmOutputsEndpoint) {
      pwmOutputsSetPwmOutputDouble(i, PWMOUTPUT_MAX_DEVIATION, stringToDouble(Value));
      Value = floatToString(pwmOutputsGetPwmOutputDouble(i,PWMOUTPUT_MAX_DEVIATION));
      mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charMaxDeviation),Value);
      pwmOutputsCheckTresholdDirection(i, OUTPUT_TYPE_PWM);
    }
    
    pwmOutputsEndpoint = setBufferFromFlash(charSetPwmOutput)+intToString(i)+setBufferFromFlash(charSensorsSetpoint);
    if (endpoint == pwmOutputsEndpoint) {
      pwmOutputsSetPwmOutputDouble(i, PWMOUTPUT_SENSORS_SETPOINT, stringToDouble(Value));
      Value = floatToString(pwmOutputsGetPwmOutputDouble(i,PWMOUTPUT_SENSORS_SETPOINT));
      mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charSensorsSetpoint),Value);
      pwmOutputsCheckTresholdDirection(i, OUTPUT_TYPE_PWM);
    }

    pwmOutputsEndpoint = setBufferFromFlash(charSetPwmOutput)+intToString(i)+setBufferFromFlash(charPidKp);
    if (endpoint == pwmOutputsEndpoint) {
      pwmOutputsSetPwmOutputDouble(i, PWMOUTPUT_PID_KP, stringToDouble(Value));
      Value = doubleToString(pwmOutputsGetPwmOutputDouble(i,PWMOUTPUT_PID_KP));
      mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charPidKp),Value);
    }

    pwmOutputsEndpoint = setBufferFromFlash(charSetPwmOutput)+intToString(i)+setBufferFromFlash(charPidKi);
    if (endpoint == pwmOutputsEndpoint) {
      pwmOutputsSetPwmOutputDouble(i, PWMOUTPUT_PID_KI, stringToDouble(Value));
      Value = doubleToString(pwmOutputsGetPwmOutputDouble(i,PWMOUTPUT_PID_KI));
      mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charPidKi),Value);
    }

    pwmOutputsEndpoint = setBufferFromFlash(charSetPwmOutput)+intToString(i)+setBufferFromFlash(charPidKd);
    if (endpoint == pwmOutputsEndpoint) {
      pwmOutputsSetPwmOutputDouble(i, PWMOUTPUT_PID_KD, stringToDouble(Value));
      Value = doubleToString(pwmOutputsGetPwmOutputDouble(i,PWMOUTPUT_PID_KD));
      mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charPidKd),Value);
    }
    
    pwmOutputsEndpoint = setBufferFromFlash(charSetPwmOutput)+intToString(i)+setBufferFromFlash(charState);
    if (endpoint == pwmOutputsEndpoint) {
      if (pwmOutputsGetPwmOutput(i, PWMOUTPUT_CONTROL_MODE) == CONTROL_MODE_PID || pwmOutputsGetPwmOutput(i, PWMOUTPUT_CONTROL_MODE) == CONTROL_MODE_TRESHOLD) {
        if (pwmOutputsGetPwmOutput(i, PWMOUTPUT_OUTPUT_VALUE)==0) {
          pwmOutputsSetPwmOutput(i, PWMOUTPUT_MANUAL_ONOFF,PWMOUTPUT_MANUAL_ONOFF_OFF);
          mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charState),setBufferFromFlash(charOff));
        }
        else {
          pwmOutputsSetPwmOutput(i, PWMOUTPUT_MANUAL_ONOFF,PWMOUTPUT_MANUAL_ONOFF_ON);
          mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charState),setBufferFromFlash(charOn));
        }
      }
      else {
        if (Value == setBufferFromFlash(charOn)) {
          pwmOutputsSetPwmOutput(i, PWMOUTPUT_MANUAL_ONOFF,PWMOUTPUT_MANUAL_ONOFF_ON);
          mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charState),setBufferFromFlash(charOn));
        }
        if (Value == setBufferFromFlash(charOff)) {
          pwmOutputsSetPwmOutput(i, PWMOUTPUT_MANUAL_ONOFF,PWMOUTPUT_MANUAL_ONOFF_OFF);
          mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charState),setBufferFromFlash(charOff));
        }
      }
    }

    pwmOutputsEndpoint = setBufferFromFlash(charSetPwmOutput)+intToString(i);
    if (endpoint == pwmOutputsEndpoint+setBufferFromFlash(charMorningMode) || endpoint == pwmOutputsEndpoint+setBufferFromFlash(charAfternoonMode) || endpoint == pwmOutputsEndpoint+setBufferFromFlash(charEveningMode) || endpoint == pwmOutputsEndpoint+setBufferFromFlash(charNightMode)  || endpoint == pwmOutputsEndpoint+setBufferFromFlash(charManualMode)) {
      
      if (endpoint == pwmOutputsEndpoint+setBufferFromFlash(charMorningMode)) {
        pwmOutputsSetPwmOutput(i, PWMOUTPUT_MODE_MORNING, stringToInt(Value));
        Value = intToString(pwmOutputsGetPwmOutput(i,PWMOUTPUT_MODE_MORNING));
        mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charMorningMode),Value);
      }

      if (endpoint == pwmOutputsEndpoint+setBufferFromFlash(charAfternoonMode)) {
        pwmOutputsSetPwmOutput(i, PWMOUTPUT_MODE_AFTERNOON, stringToInt(Value));
        Value = intToString(pwmOutputsGetPwmOutput(i,PWMOUTPUT_MODE_AFTERNOON));
        mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charAfternoonMode),Value);
      }

      if (endpoint == pwmOutputsEndpoint+setBufferFromFlash(charEveningMode)) {
        pwmOutputsSetPwmOutput(i, PWMOUTPUT_MODE_EVENING, stringToInt(Value));
        Value = intToString(pwmOutputsGetPwmOutput(i,PWMOUTPUT_MODE_EVENING));
        mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charEveningMode),Value);
      }

      if (endpoint == pwmOutputsEndpoint+setBufferFromFlash(charNightMode)) {
        pwmOutputsSetPwmOutput(i, PWMOUTPUT_MODE_NIGHT, stringToInt(Value));
        Value = intToString(pwmOutputsGetPwmOutput(i,PWMOUTPUT_MODE_NIGHT));
        mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charNightMode),Value);
      }

      if (endpoint == pwmOutputsEndpoint+setBufferFromFlash(charManualMode)) {
        pwmOutputsSetPwmOutput(i, PWMOUTPUT_MANUAL_MODE, stringToInt(Value));
        Value = intToString(pwmOutputsGetPwmOutput(i,PWMOUTPUT_MANUAL_MODE));
        mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charManualMode),Value);
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

