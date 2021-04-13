/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/

void mqttSendCommand(String command) {
  mqttSendCommand(command,true);
}

void mqttSendCommand(String command, bool callCriticalEvent) {
  int i;
  int len = command.length();
  for (i=0; i<len; i++) {
    Serial3.print(command[i]);
    criticalEvent();
  }
  Serial3.println();
  //Serial3.println(command+char(0));
  Serial3.flush();
  
  if ((DEBUG_LEVEL & _DEBUG_MQTT) || (DEBUG_LEVEL & _DEBUG_NOTICE)) {
    for (i=0; i<len; i++) {
      Serial.print(command[i]);
      criticalEvent();
    }
    Serial.println();
    //Serial.println(command+char(0));
  }
  if (callCriticalEvent==true) {
    criticalEvent();
  }
}

void mqttSubscribe() {
  if (mqttStatus==MQTT_STATUS_CONNECTED && hostnameReceived == true && mqttElDeviceName!="") {
    mqttElSubscribe("set/+");
    mqttElSubscribe("stat/+");
    mqttElSubscribe("cmnd/+");
  }
}

void mqttElSubscribe(String topic) {
  if (mqttStatus==MQTT_STATUS_CONNECTED && hostnameReceived == true && mqttElDeviceName!="") {
    String command = setBufferFromFlash(mqttSubscribeCommand)+String(mqttElDeviceName)+'/'+topic;
    mqttSendCommand(command);
    criticalEvent();
  }
}

void mqttElSubscribeFull(String topic) {
  String command = setBufferFromFlash(mqttSubscribeCommand)+topic;
  mqttSendCommand(command);
  criticalEvent();
}

void mqttElPublish(String topic, String value) {
  mqttElPublish(topic,value,true);
}
void mqttElPublish(String topic, String value, bool callCriticalEvent) {
  if (mqttStatus==MQTT_STATUS_CONNECTED && hostnameReceived == true && mqttElDeviceName!="") {
    String command = setBufferFromFlash(mqttPublishretainedCommand)+String(mqttElDeviceName)+'/'+topic+" "+value;
    mqttSendCommand(command);
  }
  if (callCriticalEvent==true) {
    criticalEvent();
  }
}

void mqttElPublishFull (String topic, String value) {
  mqttElPublishFull(topic,value,true);
}
void mqttElPublishFull (String topic, String value, bool callCriticalEvent) {
  if (mqttStatus==MQTT_STATUS_CONNECTED && hostnameReceived == true && mqttElDeviceName!="") {
    String command = setBufferFromFlash(mqttPublishCommand)+topic+" "+value;
    mqttSendCommand(command);
  }
  if (callCriticalEvent==true) {
    criticalEvent();
  }
}

void mqttSecondsEvent() {
  static double lastMqttStatusEvent = 0;
  static bool lastWifiQuery = false;
  
  if (abs(millis()-lastMqttStatusEvent)>7000) {
    lastMqttStatusEvent = millis();
    if (lastWifiQuery==false) {
      String command = setBufferFromFlash(getWifiStatus);
      mqttSendCommand(command);
      lastWifiQuery = true;
    }
    else {
      String command = setBufferFromFlash(getMqttStatus);
      mqttSendCommand(command);
      lastWifiQuery = false;
    }
  }
}

void mqttConnectToServer() {
  wifiStatus = WIFI_STATUS_DISCONNECTED;
  mqttStatus = MQTT_STATUS_DISCONNECTED;
  eventWifiDisconnected();
}

void mqttInit() {
  Serial3.begin(9600);
  delay(1000);
  Serial3.flush();
  mqttConnectToServer();
}

void mqttCheckCriticalEvent() {
  static char inBuffer [100];
  static int inBufferIdx=0;
   while(true) {
    if(Serial3.available()) {
      criticalEventNoMqtt();
      char readed = Serial3.read();
      if (readed=='\n') {
        if (inBufferIdx>0) {
          mqttCheckCRCCommand((String)inBuffer+char(0));
          inBufferIdx = 0;
          inBuffer[0] = 0;
          inBuffer[1] = 0;
        }
      }
      else {
        inBuffer[inBufferIdx] = readed;
        inBuffer[inBufferIdx+1] = 0;
        inBufferIdx = inBufferIdx+1;
        if (inBufferIdx==98) {
          if ((DEBUG_LEVEL & _DEBUG_MQTT) || (DEBUG_LEVEL & _DEBUG_WARNING)) {
            Serial.print("Msg too long:");
            Serial.println((String)inBuffer+char(0));
          }
          inBufferIdx = 0;
          inBuffer[0] = 0;
          inBuffer[1] = 0;
        }
      }
    } else break;
  }
}

void mqttCheckCRCCommand(String line) {
   //Check CRC
   int endIdx = line.indexOf(' ');
   int lenIdx = line.indexOf(char(0));
   if (endIdx>-1) {
      String crcMsgString = line.substring(0, endIdx);
      String message = line.substring(endIdx+1, lenIdx-1)+char(0);
      //message.trim();
      lenIdx = message.indexOf(char(0));
      uint32_t crcMsg = crcMsgString.toInt();
      CRC32_reset();
      
      for (size_t i = 0; i < lenIdx; i++) {
        CRC32_update(message[i]);
      }
      uint32_t checksum = CRC32_finalize();
      if (crcMsg!=checksum) {
        if ((DEBUG_LEVEL & _DEBUG_MQTT) || (DEBUG_LEVEL & _DEBUG_WARNING)) {
          Serial.print("Wrong msg. Calculated crc:");
          Serial.print(checksum);
          Serial.print(" Message: ");
          Serial.println(line);
        }
        return;
      }
      //Command
      if (message[0]=='[') {
        mqttParseCommand(message);
      }
      //Topic
      else {
        endIdx = message.indexOf(' ');
        lenIdx = message.indexOf(char(0));
        String topic = message.substring(0, endIdx);
        String payload = message.substring(endIdx+1);   
        mqttElData(topic,payload); 
      }
    }
    else {
      if ((DEBUG_LEVEL & _DEBUG_MQTT) || (DEBUG_LEVEL & _DEBUG_WARNING)) {
        Serial.print("Wrong msg:");
        Serial.println(line);
      }
    }
}

void mqttParseCommand(String line) {
  if (line[0]=='[') {
    int endIdx = line.indexOf(']');
    if (endIdx>-1) {
      String command = line.substring(1, endIdx);
      if ((DEBUG_LEVEL & _DEBUG_MQTT) || (DEBUG_LEVEL & _DEBUG_NOTICE)) {
        Serial.print("Command:");
        Serial.println(command);
      }
      endIdx = command.indexOf(' ');
      if (endIdx>-1) {
        String cmd = command.substring(0, endIdx);
        String payload = command.substring(endIdx+1);
        if (cmd=="wifi") {
          if (payload == "connected") {
            eventWifiConnected();
          }
          else {
            eventWifiDisconnected();
          }
        }
        else if (cmd=="mqtt") {
          if (payload == "connected") {
            eventMqttConnected();
          }
          else {
            eventMqttDisconnected();
          }
        }
        else if (cmd=="subscription") {
          if (payload=="exists") {
            subscriptionSent = true;
          }
        }
        else if (cmd=="timestamp") {
          long timestamp = payload.toInt();
          clockNTPSynchronize_cb(timestamp);   
        }
        else if(cmd=="hostname") {
          mqttElDeviceName = payload;   
          eventHostnameReceived();
        }
        else if(cmd=="ssid") {
          wifi_ssid = payload;   
        }
        else if(cmd=="mqttserver") {
          mqtt_server = payload;
          eventMqttServerReceived();
        }
      }
    }
    else {
      if ((DEBUG_LEVEL & _DEBUG_MQTT) || (DEBUG_LEVEL & _DEBUG_WARNING)) {
        Serial.print("Wrong msg:");
        Serial.println(line);
      }
    }
  }
}

void mqttElData(String topic, String RawValue) {
  criticalEventNoMqtt();
  String Value = RawValue;
  Value.trim();
  Value.toLowerCase();
  if ((DEBUG_LEVEL & _DEBUG_MQTT) || (DEBUG_LEVEL & _DEBUG_NOTICE)) {
    Serial.print("received topic:");
    Serial.print(topic);
    Serial.print(" ");
    Serial.println(RawValue);
  }

  String endpoint = topic;
  endpoint = endpoint.substring(strlen(mqttElDeviceName.c_str())+1); 
  if (endpoint==setBufferFromFlash(setLedState)) {
    if (Value == setBufferFromFlash(charOn)) {
      ledManualOnOff = LED_MANUAL_ONOFF_ON;
      mqttElPublish("cmnd/POWER1","ON", false);
      mqttElPublish( setBufferFromFlash(getLedState), setBufferFromFlash(charOn), false);
    }
    else if (Value == setBufferFromFlash(charOff)) {  
      ledManualOnOff = LED_MANUAL_ONOFF_OFF;
      mqttElPublish("cmnd/POWER1","OFF", false);
      mqttElPublish( setBufferFromFlash(getLedState), setBufferFromFlash(charOff), false);
    }
    else if (Value == setBufferFromFlash(charWave)) {
      ledManualOnOff = LED_MANUAL_ONOFF_AUTO;
    }
    return;
  }

  if (String(topic)=="ac/stat/RESULT") {
    if (Value=="{\"power1\":\"on\"}") {   
      ledManualOnOff = LED_MANUAL_ONOFF_ON;
      mqttElPublish( setBufferFromFlash(getLedState), setBufferFromFlash(charOn), false);
    }
    if (Value=="{\"power1\":\"off\"}") {
      ledManualOnOff = LED_MANUAL_ONOFF_OFF;
      mqttElPublish( setBufferFromFlash(getLedState), setBufferFromFlash(charOff), false);
    }

    if (Value=="{\"power2\":\"on\"}") {
      relaysSetRelay(3, RELAY_MANUAL_ONOFF, RELAY_MANUAL_ONOFF_ON);
      mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(3)+setBufferFromFlash(charState),setBufferFromFlash(charOn), false);
    }
    if (Value=="{\"power2\":\"off\"}") {
      relaysSetRelay(3, RELAY_MANUAL_ONOFF, RELAY_MANUAL_ONOFF_OFF);
      mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(3)+setBufferFromFlash(charState),setBufferFromFlash(charOff), false);
    }
    return;
  }

  //Buzzer
  if (endpoint==setBufferFromFlash(setBuzzerOnStart)) {
    if (Value == setBufferFromFlash(charOff)) {    
      buzzerOnStart = BUZZER_OFF;
      mqttElPublish( setBufferFromFlash(getBuzzerOnStart), setBufferFromFlash(charOff), false);
    }
    else {  
      buzzerOnStart = BUZZER_ON;
      mqttElPublish( setBufferFromFlash(getBuzzerOnStart), setBufferFromFlash(charOn), false);
    }
    configSaveValue(buzzerOnStart, EEPROM_buzzerOnStart_addr);
    return;
  }

  if (endpoint==setBufferFromFlash(setBuzzerOnErrors)) {
    if (Value == setBufferFromFlash(charOff)) {    
      buzzerOnErrors = BUZZER_OFF;
      mqttElPublish( setBufferFromFlash(getBuzzerOnErrors), setBufferFromFlash(charOff), false);
    }
    else {  
      buzzerOnErrors = BUZZER_ON;
      mqttElPublish( setBufferFromFlash(getBuzzerOnErrors), setBufferFromFlash(charOn), false);
    }
    configSaveValue(buzzerOnErrors, EEPROM_buzzerOnErrors_addr);
    return;
  }

    //Brigtness
  if (endpoint==setBufferFromFlash(setBrightness)) {
    ledMorningBrightness = stringToInt(Value);
    int actualPartOfDay = schedulerGetActualPartOfDay();
    configSaveLedBrightness(actualPartOfDay, stringToInt(Value));

    ledSetBrightness(stringToInt(Value));
    mqttElPublish(setBufferFromFlash(getBrightness), (String)stringToInt(Value), false);
    return;
  }

  //Date
  if (endpoint == setBufferFromFlash(setActualDate)) {
    globalYear = parseYearFromString(Value);
    globalMonth = parseMonthFromString(Value);
    globalMonthDay = parseDayFromString(Value);
    sprintf(bufferOut,"%04d/%02d/%02d", globalYear, globalMonth, globalMonthDay);
    bufferOut[10] = 0;
    mqttElPublish( setBufferFromFlash(getActualDate), bufferOut, false );
    clockSetLocalTime();
    clockGetLocalTime();
    mqttElPublish(setBufferFromFlash(getActualDayOfWeek), setBufferFromFlash(daysOfTheWeek[globalWeekDay]), false);
    return;
  }
  
  if (endpoint == setBufferFromFlash(setActualTime)) {
    globalHour = parseHourFromString(Value);
    globalMinute = parseMinuteFromString(Value);
    clockSetLocalTime();
  
    time_t local = clockGetLocalTime();
    int mymin = minute(local);
    int myhour = hour(local);
    sprintf(bufferOut,"%02d:%02d", myhour, mymin);
    bufferOut[5] = 0;
    mqttElPublish(setBufferFromFlash(getActualTime), bufferOut, false );
    return;
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
      mqttElPublish(setBufferFromFlash(getTimezoneRule1Week), clockTimezoneCodeToWeek(val), false);
    }
    else {
      configSaveValue(val, EEPROM_timezoneRule2Week_addr);
      timezoneRule2Week = val;
      mqttElPublish(setBufferFromFlash(getTimezoneRule2Week), clockTimezoneCodeToWeek(val), false);
    }
    clockUpdateTimezoneRules();
    return;
  }

  if (endpoint==setBufferFromFlash(setTimezoneRule1DayOfWeek) || endpoint==setBufferFromFlash(setTimezoneRule2DayOfWeek)) {
    byte val;
    String DoW;
    bool found = false;
    for (int i=0; i<7; i++) {
      DoW = setBufferFromFlash(daysOfTheWeek[i]);
      DoW.toLowerCase();
      if (Value == DoW) {
        switch(i) {
          case 0: val = TIMEZONE_DOW_SUNDAY;
                  found = true;
                  break;
          case 1: val = TIMEZONE_DOW_MONDAY;
                  found = true;
                  break;
          case 2: val = TIMEZONE_DOW_TUESDAY;
                  found = true;
                  break;
          case 3: val = TIMEZONE_DOW_WEDNESDAY;
                  found = true;
                  break;
          case 4: val = TIMEZONE_DOW_FOURSDAY;
                  found = true;
                  break;
          case 5: val = TIMEZONE_DOW_FRIDAY;
                  found = true;
                  break;
          case 6: val = TIMEZONE_DOW_SATURDAY;
                  found = true;
                  break;
        }
        break;
      }
    }

    if (found == true) {
      if (endpoint==setBufferFromFlash(setTimezoneRule1DayOfWeek)) {
        configSaveValue(val, EEPROM_timezoneRule1DayOfWeek_addr);
        timezoneRule1DayOfWeek = val;
        mqttElPublish(setBufferFromFlash(getTimezoneRule1DayOfWeek), clockTimezoneCodeToDoW(val), false);
      }
      else {
        configSaveValue(val, EEPROM_timezoneRule2DayOfWeek_addr);
        timezoneRule2DayOfWeek = val;
        mqttElPublish(setBufferFromFlash(getTimezoneRule2DayOfWeek), clockTimezoneCodeToDoW(val), false);
      }
      clockUpdateTimezoneRules();
    }
    return;
  }

  if (endpoint==setBufferFromFlash(setTimezoneRule1Month) || endpoint==setBufferFromFlash(setTimezoneRule2Month)) {
    byte val;
    String DoM;
    bool found = false;
    for (int i=0; i<12; i++) {
      DoM = setBufferFromFlash(monthsOfYear[i]);
      DoM.toLowerCase();
      if (Value == DoM) {
        switch(i) {
          case 0: val = TIMEZONE_MONTH_JANUARY;
                  found = true;
                  break;
          case 1: val = TIMEZONE_MONTH_FEBRUARY;
                  found = true;
                  break;
          case 2: val = TIMEZONE_MONTH_MARCH;
                  found = true;
                  break;
          case 3: val = TIMEZONE_MONTH_APRIL;
                  found = true;
                  break;
          case 4: val = TIMEZONE_MONTH_MAY;
                  found = true;
                  break;
          case 5: val = TIMEZONE_MONTH_JUNE;
                  found = true;
                  break;
          case 6: val = TIMEZONE_MONTH_JULY;
                  found = true;
                  break;
          case 7: val = TIMEZONE_MONTH_AUGUST;
                  found = true;
                  break;
          case 8: val = TIMEZONE_MONTH_SEPTEMBER;
                  found = true;
                  break;
          case 9: val = TIMEZONE_MONTH_OCTOBER;
                  found = true;
                  break;
          case 10: val = TIMEZONE_MONTH_NOVEMBER;
                  found = true;
                  break;
          case 11: val = TIMEZONE_MONTH_DECEMBER;
                  found = true;
                  break;
        }
        break;
      }
    }

    if (found == true) {
      if (endpoint==setBufferFromFlash(setTimezoneRule1Month)) {
        configSaveValue(val, EEPROM_timezoneRule1Month_addr);
        timezoneRule1Month = val;
        mqttElPublish(setBufferFromFlash(getTimezoneRule1Month), clockTimezoneCodeToMonth(val), false);
      }
      else {
        configSaveValue(val, EEPROM_timezoneRule2Month_addr);
        timezoneRule2Month = val;
        mqttElPublish(setBufferFromFlash(getTimezoneRule2Month), clockTimezoneCodeToMonth(val), false);
      }
      clockUpdateTimezoneRules();
    }
    return;
  }
  
  if (endpoint == setBufferFromFlash(setTimezoneRule1Hour)) {
    int temp = stringToInt(Value);
    if (temp>=0 && temp<=23) {
      timezoneRule1Hour = temp;
      configSaveValue(timezoneRule1Hour, EEPROM_timezoneRule1Hour_addr);
    }
    mqttElPublish(setBufferFromFlash(getTimezoneRule1Hour), intToString(timezoneRule1Hour), false);
    clockUpdateTimezoneRules();
    return;
  }

  if (endpoint == setBufferFromFlash(setTimezoneRule2Hour)) {
    int temp = stringToInt(Value);
    if (temp>=0 && temp<=23) {
      timezoneRule2Hour = temp;
      configSaveValue(timezoneRule2Hour, EEPROM_timezoneRule2Hour_addr);
    }
    mqttElPublish(setBufferFromFlash(getTimezoneRule2Hour), intToString(timezoneRule2Hour), false);
    clockUpdateTimezoneRules();
    return;
  }

  if (endpoint == setBufferFromFlash(setTimezoneRule1Offset)) {
    char temp = stringToInt(Value);
    timezoneRule1Offset = temp;
    configSaveValue(timezoneRule1Offset, EEPROM_timezoneRule1Offset_addr);
    sprintf (bufferOut,"%+d", temp);
    mqttElPublish(setBufferFromFlash(getTimezoneRule1Offset), (String)bufferOut, false);
    clockUpdateTimezoneRules();
    return;
  }

  if (endpoint == setBufferFromFlash(setTimezoneRule2Offset)) {
    char temp = stringToInt(Value);
    timezoneRule2Offset = temp;
    configSaveValue(timezoneRule2Offset, EEPROM_timezoneRule2Offset_addr);
    sprintf(bufferOut,"%+d", temp);
    mqttElPublish(setBufferFromFlash(getTimezoneRule2Offset), (String)bufferOut, false);
    clockUpdateTimezoneRules();
    return;
  }
  
  //Fan
  if (endpoint == setBufferFromFlash(setFanStartTemperature)) {
    float temp = stringToInt(Value);
    if (temp>0) {
      fanStartTemperature = temp;
      configSaveValue(fanStartTemperature, EEPROM_fanStartTemperature_addr);
    }
    mqttElPublish( setBufferFromFlash(getFanStartTemperature), intToString(fanStartTemperature), false);
    return;
  }

  if (endpoint == setBufferFromFlash(setFanMaxSpeedTemperature)) {
    float temp = stringToInt(Value);
    if (temp>0) {
      fanMaxSpeedTemperature = temp;
      configSaveValue(fanMaxSpeedTemperature, EEPROM_fanMaxSpeedTemperature_addr);
    }
    mqttElPublish( setBufferFromFlash(getFanMaxSpeedTemperature), intToString(fanMaxSpeedTemperature), false);
    return;
  }
  
  if (endpoint == setBufferFromFlash(setMaxInternalTemperature)) {
    float temp = stringToInt(Value);
    if (temp>0) {
      maxInternalTemperature = temp;
      configSaveValue(maxInternalTemperature, EEPROM_maxInternalTemperature_addr);
    }
    mqttElPublish( setBufferFromFlash(getMaxInternalTemperature), intToString(maxInternalTemperature), false);
    return;
  }

  //Daily settings
  if (endpoint == setBufferFromFlash(setMorningTime)) {
      schedulerStartMorningHour = parseHourFromString(Value);
      schedulerStartMorningMinute = parseMinuteFromString(Value);
      configSaveSchedulerTimers();
      sprintf(bufferOut,"%02d:%02d", schedulerStartMorningHour, schedulerStartMorningMinute);
      bufferOut[5] = 0;
      mqttElPublish( setBufferFromFlash(getMorningTime), bufferOut, false );
      return;
  }

  if (endpoint == setBufferFromFlash(setAfternoonTime)) {
      schedulerStartAfternoonHour = parseHourFromString(Value);
      schedulerStartAfternoonMinute = parseMinuteFromString(Value);
      configSaveSchedulerTimers();
      sprintf(bufferOut,"%02d:%02d", schedulerStartAfternoonHour, schedulerStartAfternoonMinute);
      bufferOut[5] = 0;
      mqttElPublish( setBufferFromFlash(getAfternoonTime), bufferOut, false );
      return;
  }

  if (endpoint == setBufferFromFlash(setEveningTime)) {
      schedulerStartEveningHour = parseHourFromString(Value);
      schedulerStartEveningMinute = parseMinuteFromString(Value);
      configSaveSchedulerTimers();
      sprintf(bufferOut,"%02d:%02d", schedulerStartEveningHour, schedulerStartEveningMinute);
      bufferOut[5] = 0;
      mqttElPublish( setBufferFromFlash(getEveningTime), bufferOut, false );
      return;
  }

  if (endpoint == setBufferFromFlash(setNightTime)) {
      schedulerStartNightHour = parseHourFromString(Value);
      schedulerStartNightMinute = parseMinuteFromString(Value);
      configSaveSchedulerTimers();
      sprintf(bufferOut,"%02d:%02d", schedulerStartNightHour, schedulerStartNightMinute);
      bufferOut[5] = 0;
      mqttElPublish( setBufferFromFlash(getNightTime), bufferOut, false );
      return;
  }

  //Sensors
  String sensorsEndpoint;
  for (byte j = 0; j<SENSORS_COUNT; j++) {
    int i = j+1;
    sensorsEndpoint = setBufferFromFlash(charSetSensor)+intToString(i)+setBufferFromFlash(charName);
    if (endpoint == sensorsEndpoint) {
      sensorsSaveSensorName(i,RawValue);
      mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(i)+setBufferFromFlash(charName),RawValue.substring(0,NAME_LENGTH), false);
      return;
    }
    sensorsEndpoint = setBufferFromFlash(charSetSensor)+intToString(i)+setBufferFromFlash(charSensorType);
    if (endpoint == sensorsEndpoint) {
      byte bValue = sensorsGetByteSensorType(Value);
      if (bValue!=255) {
        sensorsSetSensor(i, SENSORS_VALUE_TYPE, bValue);
        mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(i)+setBufferFromFlash(charSensorType),Value, false);
      }
      return;
    }
    sensorsEndpoint = setBufferFromFlash(charSetSensor)+intToString(i)+setBufferFromFlash(charSensorCalibValue1);
    if (endpoint == sensorsEndpoint) {
      sensorsSetSensor(i, SENSORS_VALUE_CALIB_VALUE1, stringToFloat(Value));
      float myValue = sensorsGetSensor(i, SENSORS_VALUE_CALIB_VALUE1);
      mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(i)+setBufferFromFlash(charSensorCalibValue1),floatToString(myValue), false);
      return;
    }
    sensorsEndpoint = setBufferFromFlash(charSetSensor)+intToString(i)+setBufferFromFlash(charSensorCalibRawValue1);
    if (endpoint == sensorsEndpoint) {
      sensorsSetSensor(i, SENSORS_VALUE_CALIB_RAW_VALUE1, stringToFloat(Value));
      int myValue = sensorsGetSensor(i, SENSORS_VALUE_CALIB_RAW_VALUE1);
      mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(i)+setBufferFromFlash(charSensorCalibRawValue1),intToString(myValue), false);
      return;
    }
    sensorsEndpoint = setBufferFromFlash(charSetSensor)+intToString(i)+setBufferFromFlash(charSensorCalibValue2);
    if (endpoint == sensorsEndpoint) {
      sensorsSetSensor(i, SENSORS_VALUE_CALIB_VALUE2, stringToFloat(Value));
      float myValue = sensorsGetSensor(i, SENSORS_VALUE_CALIB_VALUE2);
      mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(i)+setBufferFromFlash(charSensorCalibValue2),floatToString(myValue), false);
      return;
    }
    sensorsEndpoint = setBufferFromFlash(charSetSensor)+intToString(i)+setBufferFromFlash(charSensorCalibRawValue2);
    if (endpoint == sensorsEndpoint) {
      sensorsSetSensor(i, SENSORS_VALUE_CALIB_RAW_VALUE2, stringToFloat(Value));
      int myValue = sensorsGetSensor(i, SENSORS_VALUE_CALIB_RAW_VALUE2);
      mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(i)+setBufferFromFlash(charSensorCalibRawValue2),intToString(myValue), false);
      return;
    }
  }
  
  //Relays
  String relaysEndpoint;
  for (byte j = 0; j<RELAYS_COUNT; j++) {
    int i = j+1;
    relaysEndpoint = setBufferFromFlash(charSetRelay)+intToString(i)+setBufferFromFlash(charName);
    if (endpoint == relaysEndpoint) {
      relaysSaveRelayName(i,RawValue);
      mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(i)+setBufferFromFlash(charName),RawValue.substring(0,NAME_LENGTH), false);
      return;
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
        mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(i)+setBufferFromFlash(charControlMode),Value, false);
      }
      return;
    }

    relaysEndpoint = setBufferFromFlash(charSetRelay)+intToString(i)+setBufferFromFlash(charMaxDeviation);
    if (endpoint == relaysEndpoint) {
      relaysSetRelayDouble(i, RELAY_MAX_DEVIATION, stringToDouble(Value));
      Value = floatToString(relaysGetRelayDouble(i,RELAY_MAX_DEVIATION));
      mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(i)+setBufferFromFlash(charMaxDeviation),Value, false);
      relaysCheckTresholdDirection(i, OUTPUT_TYPE_RELAY);
      return;
    }

    relaysEndpoint = setBufferFromFlash(charSetRelay)+intToString(i)+setBufferFromFlash(charSensorsSetpoint);
    if (endpoint == relaysEndpoint) {
      relaysSetRelayDouble(i, RELAY_SENSORS_SETPOINT, stringToDouble(Value));
      Value = floatToString(relaysGetRelayDouble(i,RELAY_SENSORS_SETPOINT));
      mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(i)+setBufferFromFlash(charSensorsSetpoint),Value, false);
      relaysCheckTresholdDirection(i, OUTPUT_TYPE_RELAY);
      return;
    }
    
    relaysEndpoint = setBufferFromFlash(charSetRelay)+intToString(i)+setBufferFromFlash(charControlDirection);
    if (endpoint == relaysEndpoint) {
      if (Value == setBufferFromFlash(charDirect)) {
        relaysSetRelay(i, RELAY_CONTROL_DIRECTION,CONTROL_DIRECT);
        mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(i)+setBufferFromFlash(charControlDirection),setBufferFromFlash(charDirect), false);
      }
      if (Value == setBufferFromFlash(charReverse)) {
        relaysSetRelay(i, RELAY_CONTROL_DIRECTION,PID_REVERSE);
        mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(i)+setBufferFromFlash(charControlDirection),setBufferFromFlash(charReverse), false);
      }
      return;
    }
    
    relaysEndpoint = setBufferFromFlash(charSetRelay)+intToString(i)+setBufferFromFlash(charState);
    if (endpoint == relaysEndpoint) {
      if (Value == setBufferFromFlash(charOn)) {
        relaysSetRelay(i, RELAY_MANUAL_ONOFF, RELAY_MANUAL_ONOFF_ON);
        mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(i)+setBufferFromFlash(charState),setBufferFromFlash(charOn), false);
      }
      if (Value == setBufferFromFlash(charOff)) {
        relaysSetRelay(i, RELAY_MANUAL_ONOFF, RELAY_MANUAL_ONOFF_OFF);
        mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(i)+setBufferFromFlash(charState),setBufferFromFlash(charOff), false);
      }
      return;
    }

    relaysEndpoint = setBufferFromFlash(charSetRelay)+intToString(i);
    if (endpoint == relaysEndpoint+setBufferFromFlash(charMorningMode) 
          || endpoint == relaysEndpoint+setBufferFromFlash(charAfternoonMode) 
          || endpoint == relaysEndpoint+setBufferFromFlash(charEveningMode)
          || endpoint == relaysEndpoint+setBufferFromFlash(charNightMode)) {
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
          mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(i)+setBufferFromFlash(charMorningMode),Value, false);
          return;
        }
      }

      if (endpoint == relaysEndpoint+setBufferFromFlash(charAfternoonMode)) {
        if (tempRelayMode!= RELAY_MODE_NONE) {
          relaysSetRelay(i, RELAY_MODE_AFTERNOON, tempRelayMode);
          mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(i)+setBufferFromFlash(charAfternoonMode),Value, false);
          return;
        }
      }

      if (endpoint == relaysEndpoint+setBufferFromFlash(charEveningMode)) {
        if (tempRelayMode!= RELAY_MODE_NONE) {
          relaysSetRelay(i, RELAY_MODE_EVENING, tempRelayMode);
          mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(i)+setBufferFromFlash(charEveningMode),Value, false);
          return;
        }
      }

      if (endpoint == relaysEndpoint+setBufferFromFlash(charNightMode)) {
        if (tempRelayMode!= RELAY_MODE_NONE) {
          relaysSetRelay(i, RELAY_MODE_NIGHT, tempRelayMode);
          mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(i)+setBufferFromFlash(charNightMode),Value, false);
          return;
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
      mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charName),RawValue.substring(0,NAME_LENGTH), false);
      return;
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
        mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charControlMode),Value, false);
        pwmOutputsCheckTresholdDirection(i,OUTPUT_TYPE_PWM);
      }
      return;
    }

    pwmOutputsEndpoint = setBufferFromFlash(charSetPwmOutput)+intToString(i)+setBufferFromFlash(charControlDirection);
    if (endpoint == pwmOutputsEndpoint) {
      if (Value == setBufferFromFlash(charDirect)) {
        pwmOutputsSetPwmOutput(i, PWMOUTPUT_CONTROL_DIRECTION,CONTROL_DIRECT);
        mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charControlDirection),setBufferFromFlash(charDirect), false);
      }
      if (Value == setBufferFromFlash(charReverse)) {
        pwmOutputsSetPwmOutput(i, PWMOUTPUT_CONTROL_DIRECTION,PID_REVERSE);
        mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charControlDirection),setBufferFromFlash(charReverse), false);
      }
      return;
    }

    pwmOutputsEndpoint = setBufferFromFlash(charSetPwmOutput)+intToString(i)+setBufferFromFlash(charMaxDeviation);
    if (endpoint == pwmOutputsEndpoint) {
      pwmOutputsSetPwmOutputDouble(i, PWMOUTPUT_MAX_DEVIATION, stringToDouble(Value));
      Value = floatToString(pwmOutputsGetPwmOutputDouble(i,PWMOUTPUT_MAX_DEVIATION));
      mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charMaxDeviation),Value, false);
      pwmOutputsCheckTresholdDirection(i, OUTPUT_TYPE_PWM);
      return;
    }
    
    pwmOutputsEndpoint = setBufferFromFlash(charSetPwmOutput)+intToString(i)+setBufferFromFlash(charSensorsSetpoint);
    if (endpoint == pwmOutputsEndpoint) {
      pwmOutputsSetPwmOutputDouble(i, PWMOUTPUT_SENSORS_SETPOINT, stringToDouble(Value));
      Value = floatToString(pwmOutputsGetPwmOutputDouble(i,PWMOUTPUT_SENSORS_SETPOINT));
      mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charSensorsSetpoint),Value, false);
      pwmOutputsCheckTresholdDirection(i, OUTPUT_TYPE_PWM);
      return;
    }

    pwmOutputsEndpoint = setBufferFromFlash(charSetPwmOutput)+intToString(i)+setBufferFromFlash(charPidKp);
    if (endpoint == pwmOutputsEndpoint) {
      pwmOutputsSetPwmOutputDouble(i, PWMOUTPUT_PID_KP, stringToDouble(Value));
      Value = doubleToString(pwmOutputsGetPwmOutputDouble(i,PWMOUTPUT_PID_KP));
      mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charPidKp),Value, false);
      return;
    }

    pwmOutputsEndpoint = setBufferFromFlash(charSetPwmOutput)+intToString(i)+setBufferFromFlash(charPidKi);
    if (endpoint == pwmOutputsEndpoint) {
      pwmOutputsSetPwmOutputDouble(i, PWMOUTPUT_PID_KI, stringToDouble(Value));
      Value = doubleToString(pwmOutputsGetPwmOutputDouble(i,PWMOUTPUT_PID_KI));
      mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charPidKi),Value, false);
      return;
    }

    pwmOutputsEndpoint = setBufferFromFlash(charSetPwmOutput)+intToString(i)+setBufferFromFlash(charPidKd);
    if (endpoint == pwmOutputsEndpoint) {
      pwmOutputsSetPwmOutputDouble(i, PWMOUTPUT_PID_KD, stringToDouble(Value));
      Value = doubleToString(pwmOutputsGetPwmOutputDouble(i,PWMOUTPUT_PID_KD));
      mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charPidKd),Value, false);
      return;
    }
    
    pwmOutputsEndpoint = setBufferFromFlash(charSetPwmOutput)+intToString(i)+setBufferFromFlash(charState);
    if (endpoint == pwmOutputsEndpoint) {
      if (pwmOutputsGetPwmOutput(i, PWMOUTPUT_CONTROL_MODE) == CONTROL_MODE_PID || pwmOutputsGetPwmOutput(i, PWMOUTPUT_CONTROL_MODE) == CONTROL_MODE_TRESHOLD) {
        if (pwmOutputsGetPwmOutput(i, PWMOUTPUT_OUTPUT_VALUE)==0) {
          pwmOutputsSetPwmOutput(i, PWMOUTPUT_MANUAL_ONOFF,PWMOUTPUT_MANUAL_ONOFF_OFF);
          mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charState),setBufferFromFlash(charOff), false);
        }
        else {
          pwmOutputsSetPwmOutput(i, PWMOUTPUT_MANUAL_ONOFF,PWMOUTPUT_MANUAL_ONOFF_ON);
          mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charState),setBufferFromFlash(charOn), false);
        }
      }
      else {
        if (Value == setBufferFromFlash(charOn)) {
          pwmOutputsSetPwmOutput(i, PWMOUTPUT_MANUAL_ONOFF,PWMOUTPUT_MANUAL_ONOFF_ON);
          mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charState),setBufferFromFlash(charOn), false);
        }
        if (Value == setBufferFromFlash(charOff)) {
          pwmOutputsSetPwmOutput(i, PWMOUTPUT_MANUAL_ONOFF,PWMOUTPUT_MANUAL_ONOFF_OFF);
          mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charState),setBufferFromFlash(charOff), false);
        }
      }
      return;
    }

    pwmOutputsEndpoint = setBufferFromFlash(charSetPwmOutput)+intToString(i);
    if (endpoint == pwmOutputsEndpoint+setBufferFromFlash(charMorningMode)
          || endpoint == pwmOutputsEndpoint+setBufferFromFlash(charAfternoonMode)
          || endpoint == pwmOutputsEndpoint+setBufferFromFlash(charEveningMode)
          || endpoint == pwmOutputsEndpoint+setBufferFromFlash(charNightMode)
          || endpoint == pwmOutputsEndpoint+setBufferFromFlash(charManualMode)) {
      
      if (endpoint == pwmOutputsEndpoint+setBufferFromFlash(charMorningMode)) {
        pwmOutputsSetPwmOutput(i, PWMOUTPUT_MODE_MORNING, stringToInt(Value));
        Value = intToString(pwmOutputsGetPwmOutput(i,PWMOUTPUT_MODE_MORNING));
        mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charMorningMode),Value, false);
        return;
      }

      if (endpoint == pwmOutputsEndpoint+setBufferFromFlash(charAfternoonMode)) {
        pwmOutputsSetPwmOutput(i, PWMOUTPUT_MODE_AFTERNOON, stringToInt(Value));
        Value = intToString(pwmOutputsGetPwmOutput(i,PWMOUTPUT_MODE_AFTERNOON));
        mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charAfternoonMode),Value, false);
        return;
      }

      if (endpoint == pwmOutputsEndpoint+setBufferFromFlash(charEveningMode)) {
        pwmOutputsSetPwmOutput(i, PWMOUTPUT_MODE_EVENING, stringToInt(Value));
        Value = intToString(pwmOutputsGetPwmOutput(i,PWMOUTPUT_MODE_EVENING));
        mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charEveningMode),Value, false);
        return;
      }

      if (endpoint == pwmOutputsEndpoint+setBufferFromFlash(charNightMode)) {
        pwmOutputsSetPwmOutput(i, PWMOUTPUT_MODE_NIGHT, stringToInt(Value));
        Value = intToString(pwmOutputsGetPwmOutput(i,PWMOUTPUT_MODE_NIGHT));
        mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charNightMode),Value, false);
        return;
      }

      if (endpoint == pwmOutputsEndpoint+setBufferFromFlash(charManualMode)) {
        pwmOutputsSetPwmOutput(i, PWMOUTPUT_MANUAL_MODE, stringToInt(Value));
        Value = intToString(pwmOutputsGetPwmOutput(i,PWMOUTPUT_MANUAL_MODE));
        mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(i)+setBufferFromFlash(charManualMode),Value, false);
        return;
      }
    }
  }
  
  //LED

  if (endpoint == setBufferFromFlash(setCustomColor1)) {
    ledCustomColor1 = stringToLong(Value);
    configSaveUint32Value(ledCustomColor1, EEPROM_ledCustomColor1_addr);
    mqttElPublish(setBufferFromFlash(getCustomColor1), Value, false);
    return;
  }

  if (endpoint == setBufferFromFlash(setCustomColor2)) {
    ledCustomColor2 = stringToLong(Value);
    configSaveUint32Value(ledCustomColor1, EEPROM_ledCustomColor1_addr);
    mqttElPublish(setBufferFromFlash(getCustomColor2), Value, false);
    return;
  }
  
  if (endpoint == setBufferFromFlash(setLedControlMode)) {
    if (Value == setBufferFromFlash(charManual)) {
      ledSetBrightness(LED_BRIGHTNESS_AUTO);
      ledControlMode = CONTROL_MODE_MANUAL;
      configSaveValue(ledControlMode,EEPROM_ledControlMode_addr);
      mqttElPublish(setBufferFromFlash(getLedControlMode), Value, false);
    }
    else if (Value == setBufferFromFlash(charPartofday)) {
      ledSetBrightness(LED_BRIGHTNESS_AUTO);
      ledControlMode = CONTROL_MODE_PART_OF_DAY;
      configSaveValue(ledControlMode,EEPROM_ledControlMode_addr);
      mqttElPublish(setBufferFromFlash(getLedControlMode), Value, false);
    }
    return;
  }
  
  if (endpoint == setBufferFromFlash(setLedManualMode)
        || endpoint == setBufferFromFlash(setLedColorMorning)
        || endpoint == setBufferFromFlash(setLedColorAfternoon)
        || endpoint == setBufferFromFlash(setLedColorEvening)
        || endpoint == setBufferFromFlash(setLedColorNight)) {

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
    if (Value == setBufferFromFlash(charCustom1)) {
      tempLedMode = LED_MODE_CUSTOM1;
    }
    if (Value == setBufferFromFlash(charCustom2)) {
      tempLedMode = LED_MODE_CUSTOM2;
    }
    if (endpoint == setBufferFromFlash(setLedManualMode)) {
      if (ledManualMode!=tempLedMode) {
        configSaveValue(tempLedMode,EEPROM_ledManualMode_addr);
        mqttElPublish(setBufferFromFlash(getLedManualMode), Value, false);
        ledManualMode = tempLedMode;
      }
    }
    else if (endpoint == setBufferFromFlash(setLedColorMorning)) {
      if (ledModeMorning!=tempLedMode) {
        configSaveValue(tempLedMode,EEPROM_ledModeMorning_addr);
        mqttElPublish(setBufferFromFlash(getLedColorMorning), Value, false);
        ledModeMorning = tempLedMode;
      }
    }
    else if (endpoint == setBufferFromFlash(setLedColorAfternoon)) {
      if (ledModeAfternoon!=tempLedMode) {
        configSaveValue(tempLedMode,EEPROM_ledModeAfternoon_addr);
        mqttElPublish(setBufferFromFlash(getLedColorAfternoon), Value, false);
        ledModeAfternoon = tempLedMode;
      }
    }
    else if (endpoint == setBufferFromFlash(setLedColorEvening)) {
      if (ledModeEvening!=tempLedMode) {
        configSaveValue(tempLedMode,EEPROM_ledModeEvening_addr);
        mqttElPublish(setBufferFromFlash(getLedColorEvening), Value, false);
        ledModeEvening = tempLedMode;
      }
    }
    else {
      if (ledModeNight!=tempLedMode) {
        configSaveValue(tempLedMode,EEPROM_ledModeNight_addr);
        mqttElPublish(setBufferFromFlash(getLedColorNight), Value, false);
        ledModeNight = tempLedMode;
      }
    }
    ledSetActualMode();
    return;
  }
}
