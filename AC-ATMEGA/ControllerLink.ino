/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/



uint16_t linkTxFree(){return linkWireQueue.free();}

void linkSerialService(){
 static bool driving=false;if(driving)return;driving=true;
 linkCheckCriticalEvent();wireLink.service(Serial3,millis());
 // Drain replies/queued values before background configuration frames.
 linkWireQueue.drain(wireLink);
 configRestoreService();
 driving=false;
}

void linkDebugLog(String message) {
  if (message.length()+2 <= (unsigned int)Serial.availableForWrite()) Serial.println(message);
}









void linkRequestSnapshot() {
  publishValue=0;
  linkSnapshotField=0;
  linkSnapshotChannel=0;
}







bool linkFieldWanted(uint16_t field) {
 ledFadeService();
 if(!acFieldSupported(field,megaFeatures))return false;
 if(linkPanelReading) return field==linkPanelField;
 if(linkSnapshotCapture) return linkSnapshotFieldCount++==linkSnapshotField;
 return true;
}
void reportSelectedField(uint16_t field,String value){reportSelectedField(field,value,true);}
void reportSelectedField(uint16_t field,String value,bool callCriticalEvent){
 if(linkPanelReading){linkPanelReadValue=value;linkPanelFound=true;return;}
 if(linkSnapshotCapture){linkSnapshotSelectedSent=wireValue(field,value);return;}
 if(wireLink.connected())wireValue(field,value);
 if(callCriticalEvent)criticalEvent();
}




void linkSecondsEvent() {
  clockSyncEvent();
  static unsigned long lastBrokerStatusEvent = 0;
  static bool lastWifiQuery = false;
  
  if (millis()-lastBrokerStatusEvent >= 7000) {
    lastBrokerStatusEvent = millis();
    if (lastWifiQuery==false) {
      wireRequest(AcWire::Wifi);
      lastWifiQuery = true;
    }
    else {
      wireRequest(AcWire::Broker);
      lastWifiQuery = false;
    }
  }
}

void resetNetworkStatus() {
  wifiStatus = WIFI_STATUS_DISCONNECTED;
  brokerStatus = BROKER_STATUS_DISCONNECTED;
  eventWifiDisconnected();
}

void linkInit() {
  Serial3.begin(115200);
  uint32_t session=micros()^0xa35c7e19UL;
  for(byte i=0;i<8;i++)session=(session<<4)^analogRead(A15)^micros();
  wireLink.begin(session,AC_WIRE_SCHEMA,millis());

  resetNetworkStatus();
}

void linkCheckCriticalEvent(){
 static bool receiving=false;if(receiving)return;receiving=true;
 byte budget=64;while(budget--&&Serial3.available())wireLink.input((uint8_t)Serial3.read(),millis());
 serialCrcErrors=wireLink.stats.crcErrors;serialMalformedFrames=wireLink.stats.malformed;serialPartialTimeouts=wireLink.stats.timeouts;serialOversizedFrames=wireLink.stats.oversized;serialValidFrames=wireLink.stats.good;receiving=false;
}



void applySetting(uint16_t endpoint,String RawValue) {
 if(!acFieldSupported(endpoint,megaFeatures))return;
 criticalEventNoLink();
 String Value=RawValue;Value.trim();Value.toLowerCase();
 if(!megaFeatures.inputs && (Value=="pid"||Value=="treshold"||Value=="threshold")) {
  byte channel=relayFieldChannel(endpoint);
  if(channel && endpoint==relayField(channel,RelayField::ControlMode))return;
  channel=pwmFieldChannel(endpoint);
  if(channel && endpoint==pwmField(channel,PWMOutputField::ControlMode))return;
 }
 if (RawValue.indexOf('\n')>=0||RawValue.indexOf('\r')>=0) return;
  if (endpoint==FieldId::LedState) {
    if (Value == "on") ledManualOnOff = LED_MANUAL_ONOFF_ON;
    else if (Value == "off") ledManualOnOff = LED_MANUAL_ONOFF_OFF;
    else return;
    ledSetActualMode();
    ledPublishState(true);
    return;
  }

  //Buzzer
  if (endpoint==FieldId::BuzzerOnStart) {
    if (Value == setBufferFromFlash(charOff)) {    
      buzzerOnStart = BUZZER_OFF;
      reportField( FieldId::BuzzerOnStart, setBufferFromFlash(charOff), false);
    }
    else if (Value == setBufferFromFlash(charOn)) {  
      buzzerOnStart = BUZZER_ON;
      reportField( FieldId::BuzzerOnStart, setBufferFromFlash(charOn), false);
    }
    if (Value != "on" && Value != "off") return;
    configSaveValue(buzzerOnStart, CONFIG_buzzerOnStart_addr);
    return;
  }

  if(endpoint==FieldId::BuzzerMorning || endpoint==FieldId::BuzzerAfternoon || endpoint==FieldId::BuzzerEvening || endpoint==FieldId::BuzzerNight){
    if(Value!="on" && Value!="off")return;
    byte bit=endpoint==FieldId::BuzzerMorning?1:endpoint==FieldId::BuzzerAfternoon?2:endpoint==FieldId::BuzzerEvening?4:8;
    if(Value=="on")buzzerPhaseMask |= bit;else buzzerPhaseMask &= ~bit;
    configSaveValue(byte(0xa0 | buzzerPhaseMask),CONFIG_buzzerPhases_addr);
    reportField(endpoint,Value,false);return;
  }
  if (endpoint==FieldId::BuzzerOnErrors) {
    if (Value == setBufferFromFlash(charOff)) {    
      buzzerOnErrors = BUZZER_OFF;
      reportField( FieldId::BuzzerOnErrors, setBufferFromFlash(charOff), false);
    }
    else if (Value == setBufferFromFlash(charOn)) {  
      buzzerOnErrors = BUZZER_ON;
      reportField( FieldId::BuzzerOnErrors, setBufferFromFlash(charOn), false);
    }
    if (Value != "on" && Value != "off") return;
    configSaveValue(buzzerOnErrors, CONFIG_buzzerOnErrors_addr);
    return;
  }

  if (endpoint==FieldId::MorningBrightness || endpoint==FieldId::AfternoonBrightness ||
      endpoint==FieldId::EveningBrightness || endpoint==FieldId::NightBrightness || endpoint==FieldId::CurrentBrightness) {
    uint32_t value;
    if (!linkParseUint32(Value,value) || value>LED_BRIGHTNESS_MAX) return;
    int address=CONFIG_ledManualBrightness_addr;
    if (endpoint==FieldId::MorningBrightness) {ledMorningBrightness=value;address=CONFIG_ledMorningBrightness_addr;}
    else if (endpoint==FieldId::AfternoonBrightness) {ledAfternoonBrightness=value;address=CONFIG_ledAfternoonBrightness_addr;}
    else if (endpoint==FieldId::EveningBrightness) {ledEveningBrightness=value;address=CONFIG_ledEveningBrightness_addr;}
    else if (endpoint==FieldId::NightBrightness) {ledNightBrightness=value;address=CONFIG_ledNightBrightness_addr;}
    else {
      ledCurrentBrightness=value;
      if(ledControlMode==CONTROL_MODE_MANUAL || !megaConfigReady)ledManualBrightness=value;
      if(megaConfigReady)ledSelectManualState(value>0);
    }
    if(endpoint!=FieldId::CurrentBrightness || ledControlMode==CONTROL_MODE_MANUAL || !megaConfigReady)configSaveUint32Value(value,address);
    // Phase settings take effect at the next phase trigger, not while editing.
    ledSetActualMode();
    reportField(endpoint,String(value),false);
    return;
  }

  if ((endpoint == FieldId::ActualTime ||
       endpoint == FieldId::MorningTime ||
       endpoint == FieldId::AfternoonTime ||
       endpoint == FieldId::EveningTime ||
       endpoint == FieldId::NightTime) && !linkIsTime(Value)) return;
  if(endpoint!=FieldId::ActualTime && (endpoint==FieldId::MorningTime || endpoint==FieldId::AfternoonTime || endpoint==FieldId::EveningTime || endpoint==FieldId::NightTime) && Value.length()==8 && Value.substring(6)!="00")return;
  if (endpoint == FieldId::ActualDate && !linkIsDate(Value)) return;
  //Date
  if (endpoint == FieldId::ActualDate) {
    globalYear = parseYearFromString(Value);
    globalMonth = parseMonthFromString(Value);
    globalMonthDay = parseDayFromString(Value);
    //Apply before publishing: publishing services LED callbacks that read the
    //clock and replace these global calendar fields with the previous time.
    clockSetLocalTime();
    clockGetLocalTime();
    sprintf(bufferOut,"%04d-%02d-%02d",globalYear,globalMonth,globalMonthDay);
    reportBuffer(FieldId::ActualDate,false);

    reportField(FieldId::ActualDayOfWeek, setBufferFromFlash(daysOfTheWeek[globalWeekDay-1]), false);
    return;
  }
  
  if (endpoint == FieldId::ActualTime) {
    globalHour = parseHourFromString(Value);
    globalMinute = parseMinuteFromString(Value);
    if(Value.length()==8)globalSecond=Value.substring(6,8).toInt();
    clockSetLocalTime();
  
    time_t local = clockGetLocalTime();
    int mymin = minute(local);
    int myhour = hour(local);
    sprintf(bufferOut,"%02d:%02d:%02d", myhour, mymin,second(local));
    bufferOut[8] = 0;
    reportBuffer(FieldId::ActualTime,false);

    return;
  }

  //Timezone
  if (endpoint==FieldId::TimezoneRule1Week || endpoint==FieldId::TimezoneRule2Week) {
    byte val;
    if (endpoint==FieldId::TimezoneRule1Week) {
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

    if (endpoint==FieldId::TimezoneRule1Week) {
      configSaveValue(val, CONFIG_timezoneRule1Week_addr);
      timezoneRule1Week = val;
      reportField(FieldId::TimezoneRule1Week, clockTimezoneCodeToWeek(val), false);
    }
    else {
      configSaveValue(val, CONFIG_timezoneRule2Week_addr);
      timezoneRule2Week = val;
      reportField(FieldId::TimezoneRule2Week, clockTimezoneCodeToWeek(val), false);
    }
    clockUpdateTimezoneRules();
    return;
  }

  if (endpoint==FieldId::TimezoneRule1DayOfWeek || endpoint==FieldId::TimezoneRule2DayOfWeek) {
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
      if (endpoint==FieldId::TimezoneRule1DayOfWeek) {
        configSaveValue(val, CONFIG_timezoneRule1DayOfWeek_addr);
        timezoneRule1DayOfWeek = val;
        reportField(FieldId::TimezoneRule1DayOfWeek, clockTimezoneCodeToDoW(val), false);
      }
      else {
        configSaveValue(val, CONFIG_timezoneRule2DayOfWeek_addr);
        timezoneRule2DayOfWeek = val;
        reportField(FieldId::TimezoneRule2DayOfWeek, clockTimezoneCodeToDoW(val), false);
      }
      clockUpdateTimezoneRules();
    }
    return;
  }

  if (endpoint==FieldId::TimezoneRule1Month || endpoint==FieldId::TimezoneRule2Month) {
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
      if (endpoint==FieldId::TimezoneRule1Month) {
        configSaveValue(val, CONFIG_timezoneRule1Month_addr);
        timezoneRule1Month = val;
        reportField(FieldId::TimezoneRule1Month, clockTimezoneCodeToMonth(val), false);
      }
      else {
        configSaveValue(val, CONFIG_timezoneRule2Month_addr);
        timezoneRule2Month = val;
        reportField(FieldId::TimezoneRule2Month, clockTimezoneCodeToMonth(val), false);
      }
      clockUpdateTimezoneRules();
    }
    return;
  }
  
  if (endpoint == FieldId::TimezoneRule1Hour) {
    if (!linkIsInteger(Value,0,23)) return;
    int temp = stringToInt(Value);
    if (temp>=0 && temp<=23) {
      timezoneRule1Hour = temp;
      configSaveValue(timezoneRule1Hour, CONFIG_timezoneRule1Hour_addr);
    }
    reportField(FieldId::TimezoneRule1Hour, intToString(timezoneRule1Hour), false);
    clockUpdateTimezoneRules();
    return;
  }

  if (endpoint == FieldId::TimezoneRule2Hour) {
    if (!linkIsInteger(Value,0,23)) return;
    int temp = stringToInt(Value);
    if (temp>=0 && temp<=23) {
      timezoneRule2Hour = temp;
      configSaveValue(timezoneRule2Hour, CONFIG_timezoneRule2Hour_addr);
    }
    reportField(FieldId::TimezoneRule2Hour, intToString(timezoneRule2Hour), false);
    clockUpdateTimezoneRules();
    return;
  }

  if (endpoint == FieldId::TimezoneRule1Offset) {
    if (!linkIsInteger(Value,-12,14)) return;
    char temp = stringToInt(Value);
    timezoneRule1Offset = temp;
    configSaveValue(timezoneRule1Offset, CONFIG_timezoneRule1Offset_addr);
    sprintf (bufferOut,"%+d", temp);
    reportBuffer(FieldId::TimezoneRule1Offset,false);
    clockUpdateTimezoneRules();
    return;
  }

  if (endpoint == FieldId::TimezoneRule2Offset) {
    if (!linkIsInteger(Value,-12,14)) return;
    char temp = stringToInt(Value);
    timezoneRule2Offset = temp;
    configSaveValue(timezoneRule2Offset, CONFIG_timezoneRule2Offset_addr);
    sprintf(bufferOut,"%+d", temp);
    reportBuffer(FieldId::TimezoneRule2Offset,false);
    clockUpdateTimezoneRules();
    return;
  }
  
  //Fan
  if (endpoint == FieldId::FanStartTemp) {
    if (!linkIsInteger(Value,0,100)) return;
    int temp = stringToInt(Value);
    if (temp >= 0 && temp < fanMaxSpeedTemperature) {
      fanStartTemperature = temp;
      configSaveValue(fanStartTemperature, CONFIG_fanStartTemperature_addr);
    }
    reportField( FieldId::FanStartTemp, intToString(fanStartTemperature), false);
    return;
  }

  if (endpoint == FieldId::FanMaxSpeedTemp) {
    if (!linkIsInteger(Value,0,100)) return;
    int temp = stringToInt(Value);
    if (temp > fanStartTemperature) {
      fanMaxSpeedTemperature = temp;
      configSaveValue(fanMaxSpeedTemperature, CONFIG_fanMaxSpeedTemperature_addr);
    }
    reportField( FieldId::FanMaxSpeedTemp, intToString(fanMaxSpeedTemperature), false);
    return;
  }
  
  if (endpoint == FieldId::MaxIntTemp) {
    if (!linkIsInteger(Value,0,100)) return;
    int temp = stringToInt(Value);
    if (temp >= 0 && temp <= 100) {
      maxInternalTemperature = temp;
      configSaveValue(maxInternalTemperature, CONFIG_maxInternalTemperature_addr);
    }
    errorReport(false);
    reportField( FieldId::MaxIntTemp, intToString(maxInternalTemperature), false);
    return;
  }

  if(schedulerApplySetting(endpoint,Value))return;

  //Daily settings
  if (endpoint == FieldId::MorningTime) {
    schedulerStartMorningHour = parseHourFromString(Value);
    schedulerStartMorningMinute = parseMinuteFromString(Value);
    configSaveSchedulerTimers();
    sprintf(bufferOut,"%02d:%02d:00", schedulerStartMorningHour, schedulerStartMorningMinute);
    bufferOut[8] = 0;
    reportBuffer(FieldId::MorningTime,false);
    schedulerPublishActualPartOfDay();
    return;
  }

  if (endpoint == FieldId::AfternoonTime) {
    schedulerStartAfternoonHour = parseHourFromString(Value);
    schedulerStartAfternoonMinute = parseMinuteFromString(Value);
    configSaveSchedulerTimers();
    sprintf(bufferOut,"%02d:%02d:00", schedulerStartAfternoonHour, schedulerStartAfternoonMinute);
    bufferOut[8] = 0;
    reportBuffer(FieldId::AfternoonTime,false);
    schedulerPublishActualPartOfDay();
    return;
  }

  if (endpoint == FieldId::EveningTime) {
    schedulerStartEveningHour = parseHourFromString(Value);
    schedulerStartEveningMinute = parseMinuteFromString(Value);
    configSaveSchedulerTimers();
    sprintf(bufferOut,"%02d:%02d:00", schedulerStartEveningHour, schedulerStartEveningMinute);
    bufferOut[8] = 0;
    reportBuffer(FieldId::EveningTime,false);
    schedulerPublishActualPartOfDay();
    return;
  }

  if (endpoint == FieldId::NightTime) {
    schedulerStartNightHour = parseHourFromString(Value);
    schedulerStartNightMinute = parseMinuteFromString(Value);
    configSaveSchedulerTimers();
    sprintf(bufferOut,"%02d:%02d:00", schedulerStartNightHour, schedulerStartNightMinute);
    bufferOut[8] = 0;
    reportBuffer(FieldId::NightTime,false);
    schedulerPublishActualPartOfDay();
    return;
  }

  //Sensors
  uint16_t sensorsEndpoint;
  for (byte j = 0; j<SENSORS_COUNT; j++) {
    int i = j+1;
    const SensorField::Part limits[] = {SensorField::MinValue,SensorField::MaxValue,SensorField::CriticalMinValue,SensorField::CriticalMaxValue};
    const byte limitTypes[] = {SENSORS_VALUE_MIN,SENSORS_VALUE_MAX,SENSORS_VALUE_CRITICAL_MIN,SENSORS_VALUE_CRITICAL_MAX};
    for (byte limit=0; limit<4; limit++) {
      if (endpoint == sensorField(i,limits[limit])) {
        sensorsSetSensor(i,limitTypes[limit],Value == "off" ? (limit%2 == 0 ? -FLT_MAX : FLT_MAX) : stringToFloat(Value));
        reportField(sensorField(i,limits[limit]),sensorsLimitToString(sensorsGetSensor(i,limitTypes[limit])),false);
        return;
      }
    }
    sensorsEndpoint = sensorField(i,SensorField::Name);
    if (endpoint == sensorsEndpoint) {
      reportField(sensorField(i,SensorField::Name),RawValue.substring(0,NAME_LENGTH), false);
      sensorsSaveSensorName(i,RawValue);
      return;
    }
    sensorsEndpoint = sensorField(i,SensorField::Type);
    if (endpoint == sensorsEndpoint) {
      byte bValue = sensorsGetByteSensorType(Value);
      if(bValue!=SENSOR_TYPE_NONE && (bValue<RELAYS_COUNT?bValue>=megaFeatures.relays:bValue-RELAYS_COUNT>=megaFeatures.pwm))return;
      if (bValue!=255) {
        sensorsSetSensor(i, SENSORS_VALUE_TYPE, bValue);
      }
      return;
    }
    sensorsEndpoint = sensorField(i,SensorField::CalibValue1);
    if (endpoint == sensorsEndpoint) {
      sensorsSetSensor(i,SENSORS_VALUE_CALIB_VALUE1,stringToFloat(Value));
      reportField(sensorField(i,SensorField::CalibValue1),floatToString(sensorsGetSensor(i,SENSORS_VALUE_CALIB_VALUE1)),false);
      return;
    }
    sensorsEndpoint = sensorField(i,SensorField::CalibRawValue1);
    if (endpoint == sensorsEndpoint) {
      sensorsSetSensor(i,SENSORS_VALUE_CALIB_RAW_VALUE1,stringToFloat(Value));
      reportField(sensorField(i,SensorField::CalibRawValue1),floatToString(sensorsGetSensor(i,SENSORS_VALUE_CALIB_RAW_VALUE1)),false);
      return;
    }
    sensorsEndpoint = sensorField(i,SensorField::CalibValue2);
    if (endpoint == sensorsEndpoint) {
      sensorsSetSensor(i,SENSORS_VALUE_CALIB_VALUE2,stringToFloat(Value));
      reportField(sensorField(i,SensorField::CalibValue2),floatToString(sensorsGetSensor(i,SENSORS_VALUE_CALIB_VALUE2)),false);
      return;
    }
    sensorsEndpoint = sensorField(i,SensorField::CalibRawValue2);
    if (endpoint == sensorsEndpoint) {
      sensorsSetSensor(i,SENSORS_VALUE_CALIB_RAW_VALUE2,stringToFloat(Value));
      reportField(sensorField(i,SensorField::CalibRawValue2),floatToString(sensorsGetSensor(i,SENSORS_VALUE_CALIB_RAW_VALUE2)),false);
      return;
    }
  }
  
  //Relays
  uint16_t relaysEndpoint;
  for (byte j = 0; j<RELAYS_COUNT; j++) {
    int i = j+1;
    if (endpoint == relayField(i,RelayField::Input)) {
      byte input=0;
      if(Value!="none"){
        if(!Value.startsWith("sensor") || !linkIsInteger(Value.substring(6),1,SENSORS_COUNT))return;
        input=Value.substring(6).toInt();
      }
      sensorsSetOutputInput(i,OUTPUT_TYPE_RELAY,input);
      reportField(relayField(i,RelayField::Input),sensorsOutputInputString(i,OUTPUT_TYPE_RELAY),false);
      return;
    }
    relaysEndpoint = relayField(i,RelayField::Name);
    if (endpoint == relaysEndpoint) {
      reportField(relayField(i,RelayField::Name),RawValue.substring(0,NAME_LENGTH), false);
      relaysSaveRelayName(i,RawValue);
      return;
    }
    
    relaysEndpoint = relayField(i,RelayField::ControlMode);
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
        reportField(relayField(i,RelayField::ControlMode),Value, false);
        relaysSetRelay(i, RELAY_CONTROL_MODE, bValue);
      }
      return;
    }

    relaysEndpoint = relayField(i,RelayField::MaxDeviation);
    if (endpoint == relaysEndpoint) {
      relaysSetRelayDouble(i, RELAY_MAX_DEVIATION, stringToDouble(Value));
      Value = floatToString(relaysGetRelayDouble(i,RELAY_MAX_DEVIATION));
      reportField(relayField(i,RelayField::MaxDeviation),Value, false);

      return;
    }

    relaysEndpoint = relayField(i,RelayField::SensorsSetpoint);
    if (endpoint == relaysEndpoint) {
      relaysSetRelayDouble(i, RELAY_SENSORS_SETPOINT, stringToDouble(Value));
      Value = floatToString(relaysGetRelayDouble(i,RELAY_SENSORS_SETPOINT));
      reportField(relayField(i,RelayField::SensorsSetpoint),Value, false);

      return;
    }
    
    relaysEndpoint = relayField(i,RelayField::ControlDirection);
    if (endpoint == relaysEndpoint) {
      if (Value == setBufferFromFlash(charDirect)) {
        reportField(relayField(i,RelayField::ControlDirection),setBufferFromFlash(charDirect), false);
        relaysSetRelay(i, RELAY_CONTROL_DIRECTION,CONTROL_DIRECT);
      }
      if (Value == setBufferFromFlash(charReverse)) {
        reportField(relayField(i,RelayField::ControlDirection),setBufferFromFlash(charReverse), false);
        relaysSetRelay(i, RELAY_CONTROL_DIRECTION,PID_REVERSE);

      }
      return;
    }
    
    relaysEndpoint = relayField(i,RelayField::State);
    if (endpoint == relaysEndpoint) {
      byte mode;
      if (Value == setBufferFromFlash(charOn)) mode = RELAY_MANUAL_ONOFF_ON;
      else if (Value == setBufferFromFlash(charOff)) mode = RELAY_MANUAL_ONOFF_OFF;
      else return;
      relaysSetRelay(i,RELAY_MANUAL_ONOFF,mode);
      reportField(relayField(i,RelayField::State),relaysGetRelay(i,RELAY_STATE) ? "on" : "off",false);
      return;
    }

    if (endpoint == relayField(i,RelayField::MorningMode) 
          || endpoint == relayField(i,RelayField::AfternoonMode) 
          || endpoint == relayField(i,RelayField::EveningMode)
          || endpoint == relayField(i,RelayField::NightMode)) {
      byte tempRelayMode = 255;
      if (Value == setBufferFromFlash(charOn)) {    
        tempRelayMode = RELAY_MODE_ON;
      }
      else if (Value == setBufferFromFlash(charOff)) {  
        tempRelayMode = RELAY_MODE_OFF;
      }

      if (endpoint == relayField(i,RelayField::MorningMode)) {
        if (tempRelayMode!=255) {
          reportField(relayField(i,RelayField::MorningMode),Value, false);
          relaysSetRelay(i, RELAY_MODE_MORNING, tempRelayMode);
          return;
        }
      }

      if (endpoint == relayField(i,RelayField::AfternoonMode)) {
        if (tempRelayMode!=255) {
          reportField(relayField(i,RelayField::AfternoonMode),Value, false);
          relaysSetRelay(i, RELAY_MODE_AFTERNOON, tempRelayMode);
          return;
        }
      }

      if (endpoint == relayField(i,RelayField::EveningMode)) {
        if (tempRelayMode!=255) {
          reportField(relayField(i,RelayField::EveningMode),Value, false);
          relaysSetRelay(i, RELAY_MODE_EVENING, tempRelayMode);
          return;
        }
      }

      if (endpoint == relayField(i,RelayField::NightMode)) {
        if (tempRelayMode!=255) {
          reportField(relayField(i,RelayField::NightMode),Value, false);
          relaysSetRelay(i, RELAY_MODE_NIGHT, tempRelayMode);
          return;
        }
      }
    }
  }
  
  //PWMOutputs
  uint16_t pwmOutputsEndpoint;
  for (byte j = 0; j<PWMOUTPUTS_COUNT; j++) {
    int i = j+1;
    if (endpoint == pwmField(i,PWMOutputField::Input)) {
      byte input=0;
      if(Value!="none"){
        if(!Value.startsWith("sensor") || !linkIsInteger(Value.substring(6),1,SENSORS_COUNT))return;
        input=Value.substring(6).toInt();
      }
      sensorsSetOutputInput(i,OUTPUT_TYPE_PWM,input);
      reportField(pwmField(i,PWMOutputField::Input),sensorsOutputInputString(i,OUTPUT_TYPE_PWM),false);
      return;
    }
    pwmOutputsEndpoint = pwmField(i,PWMOutputField::Name);
    if (endpoint == pwmOutputsEndpoint) {
      reportField(pwmField(i,PWMOutputField::Name),RawValue.substring(0,NAME_LENGTH), false);
      pwmOutputsSavePwmOutputName(i,RawValue);
      return;
    }
    
    pwmOutputsEndpoint = pwmField(i,PWMOutputField::ControlMode);
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
        reportField(pwmField(i,PWMOutputField::ControlMode),Value, false);
        pwmOutputsSetPwmOutput(i, PWMOUTPUT_CONTROL_MODE, bValue);

      }
      return;
    }

    pwmOutputsEndpoint = pwmField(i,PWMOutputField::ControlDirection);
    if (endpoint == pwmOutputsEndpoint) {
      if (Value == setBufferFromFlash(charDirect)) {
        reportField(pwmField(i,PWMOutputField::ControlDirection),setBufferFromFlash(charDirect), false);
        pwmOutputsSetPwmOutput(i, PWMOUTPUT_CONTROL_DIRECTION,CONTROL_DIRECT);
      }
      if (Value == setBufferFromFlash(charReverse)) {
        reportField(pwmField(i,PWMOutputField::ControlDirection),setBufferFromFlash(charReverse), false);
        pwmOutputsSetPwmOutput(i, PWMOUTPUT_CONTROL_DIRECTION,PID_REVERSE);
      }
      return;
    }

    pwmOutputsEndpoint = pwmField(i,PWMOutputField::MaxDeviation);
    if (endpoint == pwmOutputsEndpoint) {
      pwmOutputsSetPwmOutputDouble(i,PWMOUTPUT_MAX_DEVIATION,stringToDouble(Value));
      Value = floatToString(pwmOutputsGetPwmOutputDouble(i,PWMOUTPUT_MAX_DEVIATION));
      reportField(pwmField(i,PWMOutputField::MaxDeviation),Value,false);
      return;
    }
    
    pwmOutputsEndpoint = pwmField(i,PWMOutputField::SensorsSetpoint);
    if (endpoint == pwmOutputsEndpoint) {
      pwmOutputsSetPwmOutputDouble(i,PWMOUTPUT_SENSORS_SETPOINT,stringToDouble(Value));
      Value = floatToString(pwmOutputsGetPwmOutputDouble(i,PWMOUTPUT_SENSORS_SETPOINT));
      reportField(pwmField(i,PWMOutputField::SensorsSetpoint),Value,false);
      return;
    }

    pwmOutputsEndpoint = pwmField(i,PWMOutputField::PidKp);
    if (endpoint == pwmOutputsEndpoint) {
      pwmOutputsSetPwmOutputDouble(i,PWMOUTPUT_PID_KP,stringToDouble(Value));
      Value = doubleToString(pwmOutputsGetPwmOutputDouble(i,PWMOUTPUT_PID_KP));
      reportField(pwmField(i,PWMOutputField::PidKp),Value,false);
      return;
    }

    pwmOutputsEndpoint = pwmField(i,PWMOutputField::PidKi);
    if (endpoint == pwmOutputsEndpoint) {
      pwmOutputsSetPwmOutputDouble(i,PWMOUTPUT_PID_KI,stringToDouble(Value));
      Value = doubleToString(pwmOutputsGetPwmOutputDouble(i,PWMOUTPUT_PID_KI));
      reportField(pwmField(i,PWMOutputField::PidKi),Value,false);
      return;
    }

    pwmOutputsEndpoint = pwmField(i,PWMOutputField::PidKd);
    if (endpoint == pwmOutputsEndpoint) {
      pwmOutputsSetPwmOutputDouble(i,PWMOUTPUT_PID_KD,stringToDouble(Value));
      Value = doubleToString(pwmOutputsGetPwmOutputDouble(i,PWMOUTPUT_PID_KD));
      reportField(pwmField(i,PWMOutputField::PidKd),Value,false);
      return;
    }
    
    if (endpoint == pwmField(i,PWMOutputField::MorningMode)
          || endpoint == pwmField(i,PWMOutputField::AfternoonMode)
          || endpoint == pwmField(i,PWMOutputField::EveningMode)
          || endpoint == pwmField(i,PWMOutputField::NightMode)
          || endpoint == pwmField(i,PWMOutputField::ManualMode)) {
      if (!linkIsInteger(Value,0,PWM_MAX)) return;
      
      if (endpoint == pwmField(i,PWMOutputField::MorningMode)) {
        reportField(pwmField(i,PWMOutputField::MorningMode),Value, false);
        pwmOutputsSetPwmOutput(i, PWMOUTPUT_MODE_MORNING, uint16_t(Value.toInt()));
        Value = longToString(pwmOutputsGetPwmOutput(i,PWMOUTPUT_MODE_MORNING));
        return;
      }

      if (endpoint == pwmField(i,PWMOutputField::AfternoonMode)) {
        reportField(pwmField(i,PWMOutputField::AfternoonMode),Value, false);
        pwmOutputsSetPwmOutput(i, PWMOUTPUT_MODE_AFTERNOON, uint16_t(Value.toInt()));
        Value = longToString(pwmOutputsGetPwmOutput(i,PWMOUTPUT_MODE_AFTERNOON));
        return;
      }

      if (endpoint == pwmField(i,PWMOutputField::EveningMode)) {
        reportField(pwmField(i,PWMOutputField::EveningMode),Value, false);
        pwmOutputsSetPwmOutput(i, PWMOUTPUT_MODE_EVENING, uint16_t(Value.toInt()));
        Value = longToString(pwmOutputsGetPwmOutput(i,PWMOUTPUT_MODE_EVENING));
        return;
      }

      if (endpoint == pwmField(i,PWMOutputField::NightMode)) {
        reportField(pwmField(i,PWMOutputField::NightMode),Value, false);
        pwmOutputsSetPwmOutput(i, PWMOUTPUT_MODE_NIGHT, uint16_t(Value.toInt()));
        Value = longToString(pwmOutputsGetPwmOutput(i,PWMOUTPUT_MODE_NIGHT));
        return;
      }

      if (endpoint == pwmField(i,PWMOutputField::ManualMode)) {
        reportField(pwmField(i,PWMOutputField::ManualMode),Value, false);
        pwmOutputsSetPwmOutput(i, PWMOUTPUT_MANUAL_MODE, uint16_t(Value.toInt()));
        Value = longToString(pwmOutputsGetPwmOutput(i,PWMOUTPUT_MANUAL_MODE));
        return;
      }
    }
  }
  
  //LED
  if (endpoint == FieldId::CustomColor) {
    if (!linkIsInteger(Value,0,16777215)) return;
    ledCustomColor = stringToLong(Value);
    reportField(FieldId::CustomColor,longToString(ledCustomColor),false);
    configSaveUint32Value(ledCustomColor, CONFIG_ledCustomColor_addr);
    if(megaConfigReady){
      ledSetActualMode(); // Capture the current configured selection and phase.
      ledUseCurrentCustomColor();
    }
    ledSetActualMode();
    return;
  }

  if (endpoint == FieldId::LedControlMode) {
    if (Value != "manual" && Value != "partofday") return;
    if (Value == setBufferFromFlash(charManual)) {
      reportField(FieldId::LedControlMode, Value, false);
      ledControlMode = CONTROL_MODE_MANUAL;
      configSaveValue(ledControlMode,CONFIG_ledControlMode_addr);
    }
    else if (Value == setBufferFromFlash(charPartofday)) {
      reportField(FieldId::LedControlMode, Value, false);
      ledControlMode = CONTROL_MODE_PART_OF_DAY; ledScheduleBrightnessReset=true;
      configSaveValue(ledControlMode,CONFIG_ledControlMode_addr);
    }
    return;
  }
  
  if (endpoint == FieldId::LedManualMode
        || endpoint == FieldId::LedColorMorning
        || endpoint == FieldId::LedColorAfternoon
        || endpoint == FieldId::LedColorEvening
        || endpoint == FieldId::LedColorNight) {

    int tempLedMode = LED_MODE_NONE;

    if (Value == setBufferFromFlash(charBlack)) {
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
    if (Value == setBufferFromFlash(charCustom)) {
      tempLedMode = LED_MODE_CUSTOM;
    }
    if (tempLedMode == LED_MODE_NONE) return;
    uint16_t response;
    if (endpoint == FieldId::LedManualMode) response = FieldId::LedManualMode;
    else if (endpoint == FieldId::LedColorMorning) response = FieldId::LedColorMorning;
    else if (endpoint == FieldId::LedColorAfternoon) response = FieldId::LedColorAfternoon;
    else if (endpoint == FieldId::LedColorEvening) response = FieldId::LedColorEvening;
    else response = FieldId::LedColorNight;
    reportField(response,ledColorValueToString(tempLedMode),false);
    if (endpoint == FieldId::LedManualMode) {
      if (ledManualMode!=tempLedMode) {
        reportField(FieldId::LedManualMode, Value, false);
        configSaveValue(tempLedMode,CONFIG_ledManualMode_addr);
        ledManualMode = tempLedMode;
      }
    }
    else if (endpoint == FieldId::LedColorMorning) {
      if (ledModeMorning!=tempLedMode) {
        reportField(FieldId::LedColorMorning, Value, false);
        configSaveValue(tempLedMode,CONFIG_ledModeMorning_addr);
        ledModeMorning = tempLedMode;
      }
    }
    else if (endpoint == FieldId::LedColorAfternoon) {
      if (ledModeAfternoon!=tempLedMode) {
        reportField(FieldId::LedColorAfternoon, Value, false);
        configSaveValue(tempLedMode,CONFIG_ledModeAfternoon_addr);
        ledModeAfternoon = tempLedMode;
      }
    }
    else if (endpoint == FieldId::LedColorEvening) {
      if (ledModeEvening!=tempLedMode) {
        reportField(FieldId::LedColorEvening, Value, false);
        configSaveValue(tempLedMode,CONFIG_ledModeEvening_addr);
        ledModeEvening = tempLedMode;
      }
    }
    else {
      if (ledModeNight!=tempLedMode) {
        reportField(FieldId::LedColorNight, Value, false);
        configSaveValue(tempLedMode,CONFIG_ledModeNight_addr);
        ledModeNight = tempLedMode;
      }
    }
    ledSetActualMode();
    ledPublishState(true);
    return;
  }
}

bool linkIsInteger(String value,long low,long high) {
  value.trim();
  if (value.length() == 0) return false;
  unsigned int start = (value[0] == '+' || value[0] == '-') ? 1 : 0;
  if (start == value.length()) return false;
  for (unsigned int i=start; i<value.length(); i++)
    if (value[i] < '0' || value[i] > '9') return false;
  double number = stringToDouble(value);
  return isfinite(number) && number >= low && number <= high;
}

bool linkIsTime(String value) {
  return (value.length()==5 || (value.length()==8 && value[5]==':' && linkIsInteger(value.substring(6,8),0,59))) && value[2]==':' &&
    linkIsInteger(value.substring(0,2),0,23) && linkIsInteger(value.substring(3,5),0,59);
}

bool linkIsDate(String value) {
  if (value.length() != 10 || (value[4]!='/' && value[4]!='-') || value[7]!=value[4]) return false;
  if (!linkIsInteger(value.substring(0,4),1970,2105) ||
      !linkIsInteger(value.substring(5,7),1,12) || !linkIsInteger(value.substring(8),1,31)) return false;
  int year = value.substring(0,4).toInt();
  byte month = value.substring(5,7).toInt();
  byte days[] = {31,28,31,30,31,30,31,31,30,31,30,31};
  if (year%4 == 0 && (year%100 != 0 || year%400 == 0)) days[1] = 29;
  return value.substring(8).toInt() <= days[month-1];
}

void reportNetwork() {
  reportLinkDiagnostics();
  reportField(FieldId::Hostname,deviceName);
  reportField(FieldId::Ssid,wifi_ssid);
  reportField(FieldId::BrokerServer,brokerServer);
  reportField(FieldId::WifiStatus,wifiStatus == WIFI_STATUS_CONNECTED ? "connected" : "disconnected");
  reportField(FieldId::BrokerStatus,brokerStatus == BROKER_STATUS_CONNECTED ? "connected" : "disconnected");
}

//Copy bufferOut before cooperative callbacks can reuse it.
void reportBuffer(uint16_t field) {
  reportBuffer(field,true);
}

void reportBuffer(uint16_t field,bool callCriticalEvent) {
  if (!linkFieldWanted(field)) return;
  String payload(bufferOut);
  reportSelectedField(field,payload,callCriticalEvent);
}

void reportLinkDiagnostics() {
  reportField(FieldId::SerialPartialTimeouts,longToString(serialPartialTimeouts));
  reportField(FieldId::SerialTxDroppedCommands,String(serialTxDroppedCommands));
  reportField(FieldId::SerialCrcErrors,longToString(serialCrcErrors));
  reportField(FieldId::SerialOversizedFrames,longToString(serialOversizedFrames));
  reportField(FieldId::SerialMalformedFrames,longToString(serialMalformedFrames));
  reportField(FieldId::SerialValidFrames,longToString(serialValidFrames));
}

bool linkParseUint32(String text,uint32_t& value) {
  if (text.length() == 0 || text.length() > 10) return false;
  uint32_t result = 0;
  for (unsigned int i=0; i<text.length(); i++) {
    if (text[i] < '0' || text[i] > '9') return false;
    byte digit = text[i]-'0';
    if (result > (UINT32_MAX-digit)/10UL) return false;
    result = result*10UL+digit;
  }
  value = result;
  return true;
}

//Select one field from existing snapshot publishers, rather than bursting a
//whole channel into the UART. Retry unchanged field if queue is full.
void linkSnapshotStep() {
  linkPanelService();
  if (!megaConfigReady || !wireLink.connected()) return;
  if (publishValue>=11 && linkSnapshotRetry) { linkRequestSnapshot(); linkSnapshotRetry=false; }
  if (publishValue<0 || publishValue>=11 || linkTxFree()<384) return;
  linkSnapshotCapture=true; linkSnapshotSelectedSent=false; linkSnapshotFieldCount=0;
  switch (publishValue) {
    case 0: buzzerReportAll(); break;
    case 1: clockReportAll(); break;
    case 2: dhtReportAll(); break;
    case 3: errorsReportAll(); break;
    case 4: fanReportAll(); break;
    case 5: ledReportAll(); break;
    case 6: relaysReportChannel(linkSnapshotChannel); break;
    case 7: pwmOutputsReportChannel(linkSnapshotChannel); break;
    case 8: schedulerReportAll(); break;
    case 9: sensorsReportChannel(linkSnapshotChannel); break;
    case 10: reportNetwork(); break;
  }
  linkSnapshotCapture=false;
  if (!linkSnapshotSelectedSent && linkSnapshotField<linkSnapshotFieldCount) return;
  linkSnapshotField++;
  if (linkSnapshotField>=linkSnapshotFieldCount) {
    linkSnapshotField=0;
    byte channels=publishValue==6 ? RELAYS_COUNT : publishValue==7 ? PWMOUTPUTS_COUNT : publishValue==9 ? SENSORS_COUNT : 1;
    if (++linkSnapshotChannel>=channels) { linkSnapshotChannel=0; publishValue++; }
  }
}

// Read the current value after dispatch, including rejected/clamped settings.
// Existing publishers supply values; capture prevents a burst into the UART.
void linkPanelService() {
  // Result confirms application/readback. Persistence belongs to ArdPortal.
  if (!linkPanelField || linkTxFree()<384) return;
  linkPanelReading=true; linkPanelFound=false;
  byte channel;
  if((channel=sensorFieldChannel(linkPanelField)))sensorsReportChannel(channel-1);
  else if((channel=relayFieldChannel(linkPanelField)))relaysReportChannel(channel-1);
  else if((channel=pwmFieldChannel(linkPanelField)))pwmOutputsReportChannel(channel-1);
  else {buzzerReportAll();clockReportAll();dhtReportAll();fanReportAll();ledReportAll();schedulerReportAll();}
  linkPanelReading=false;
  if (wireResult(linkPanelId,linkPanelField,linkPanelFound ? linkPanelReadValue : String("unsupported"))) {
#ifdef ARDUINO
    usbDiagnosticAck(linkPanelId,linkPanelField,linkPanelFound ? linkPanelReadValue : String("unsupported"));
#endif
    linkPanelField=0; linkPanelReadValue=""; linkPanelId=0;
  }
}
