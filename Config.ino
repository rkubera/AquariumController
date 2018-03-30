void configLoad() {

  sensorsLoadConfig();
  
  ledModeMorning = configGetValue(EEPROM_ledModeMorning_addr);
  ledModeAfternoon = configGetValue(EEPROM_ledModeAfternoon_addr);
  ledModeEvening = configGetValue(EEPROM_ledModeEvening_addr);
  ledModeNight = configGetValue(EEPROM_ledModeNight_addr);

  schedulerStartMorningHour = configGetValue(EEPROM_schedulerStartMorningHour_addr);
  schedulerStartMorningMinute = configGetValue(EEPROM_schedulerStartMorningMinute_addr);
  schedulerStartAfternoonHour = configGetValue(EEPROM_schedulerStartAfternoonHour_addr);
  schedulerStartAfternoonMinute = configGetValue(EEPROM_schedulerStartAfternoonMinute_addr);
  schedulerStartEveningHour = configGetValue(EEPROM_schedulerStartEveningHour_addr);
  schedulerStartEveningMinute = configGetValue(EEPROM_schedulerStartEveningMinute_addr);
  schedulerStartNightHour = configGetValue(EEPROM_schedulerStartNightHour_addr);
  schedulerStartNightMinute = configGetValue(EEPROM_schedulerStartNightMinute_addr);
  
  ledMorningBrightness = configGetValue(EEPROM_ledMorningBrightness_addr);
  ledAfternoonBrightness = configGetValue(EEPROM_ledAfternoonBrightness_addr);
  ledEveningBrightness = configGetValue(EEPROM_ledEveningBrightness_addr);
  ledNightBrightness = configGetValue(EEPROM_ledNightBrightness_addr);
  ledManualBrightness = configGetValue(EEPROM_ledManualBrightness_addr);

  relay1ModeMorning = configGetValue(EEPROM_relay1ModeMorning_addr);
  relay1ModeAfternoon = configGetValue(EEPROM_relay1ModeAfternoon_addr);
  relay1ModeEvening = configGetValue(EEPROM_relay1ModeEvening_addr);
  relay1ModeNight = configGetValue(EEPROM_relay1ModeNight_addr);

  relay2ModeMorning = configGetValue(EEPROM_relay2ModeMorning_addr);
  relay2ModeAfternoon = configGetValue(EEPROM_relay2ModeAfternoon_addr);
  relay2ModeEvening = configGetValue(EEPROM_relay2ModeEvening_addr);
  relay2ModeNight = configGetValue(EEPROM_relay2ModeNight_addr);

  fanStartTemperature = configGetValue(EEPROM_fanStartTemperature_addr);
  fanMaxSpeedTemperature = configGetValue(EEPROM_fanMaxSpeedTemperature_addr);
  
  maxInternalTemperature = configGetValue(EEPROM_maxInternalTemperature_addr);

  phMin = configGetFloatValue(EEPROM_phMin_addr);
  phMax = configGetFloatValue(EEPROM_phMax_addr);
  phValue1 = configGetFloatValue(EEPROM_phValue1_addr);
  phRawRead1 = configGetFloatValue(EEPROM_phRawRead1_addr);
  phValue2 = configGetFloatValue(EEPROM_phValue2_addr);
  phRawRead2 = configGetFloatValue(EEPROM_phRawRead2_addr);

  ledControlMode = configGetValue(EEPROM_ledControlMode_addr);
  ledManualMode = configGetValue(EEPROM_ledManualMode_addr);

  buzzerOnStart = configGetValue(EEPROM_buzzerOnStart_addr);
  buzzerOnErrors = configGetValue(EEPROM_buzzerOnErrors_addr);

  tempMin = configGetFloatValue(EEPROM_tempMin_addr);
  tempMax = configGetFloatValue(EEPROM_tempMax_addr);
  tempValue1 = configGetFloatValue(EEPROM_tempValue1_addr);
  tempRawRead1 = configGetFloatValue(EEPROM_tempRawRead1_addr);
  tempValue2 = configGetFloatValue(EEPROM_tempValue2_addr);
  tempRawRead2 = configGetFloatValue(EEPROM_tempRawRead2_addr);

  timezoneRule1Week = configGetValue(EEPROM_timezoneRule1Week_addr);
  timezoneRule2Week = configGetValue(EEPROM_timezoneRule2Week_addr);
  timezoneRule1DayOfWeek = configGetValue(EEPROM_timezoneRule1DayOfWeek_addr);
  timezoneRule2DayOfWeek = configGetValue(EEPROM_timezoneRule2DayOfWeek_addr);
  timezoneRule1Hour = configGetValue(EEPROM_timezoneRule1Hour_addr);
  timezoneRule2Hour = configGetValue(EEPROM_timezoneRule2Hour_addr);
  timezoneRule1Offset = configGetValue(EEPROM_timezoneRule1Offset_addr);
  timezoneRule2Offset = configGetValue(EEPROM_timezoneRule2Offset_addr);
  timezoneRule1Month = configGetValue(EEPROM_timezoneRule1Month_addr);
  timezoneRule2Month = configGetValue(EEPROM_timezoneRule2Month_addr);

  relay1ControlMode = configGetValue(EEPROM_relay1ControlMode_addr);
  relay2ControlMode = configGetValue(EEPROM_relay2ControlMode_addr);
}

byte configGetValue(int addr) {
  return EEPROM.read(addr);
}

float configGetFloatValue(int addr) {
  float ret;
  EEPROM.get(addr,ret);
  return ret;
}

void configSaveFloatValue (float value, int addr) {
  EEPROM.put(addr,value); 
}

void configSaveValue (int value, int addr) {
  EEPROM.write(addr,(byte)value); 
}

void configSaveValue (byte value, int addr) {
  EEPROM.write(addr,value); 
}

String configReadString (int addr, int maxSize) {
  return setBufferFromEeprom(addr, maxSize);
}

void configSaveString  (String str, int addr, int maxSize) {
  str = str+char(0);
  int index;
  for (index = 0; index < maxSize; index++) {
    EEPROM.write(addr+index, str[index]);
    if (str[index]==0 ) {
      return;
    }
  }  
}

void configSaveLedBrightness(int type, int value) {
  if (ledControlMode==CONTROL_MODE_PART_OF_DAY) {
    if (type==SCHEDULER_MODE_MORNING) {
      EEPROM.write(EEPROM_ledMorningBrightness_addr,(byte)value); 
    }
    else if (type==SCHEDULER_MODE_AFTERNOON) {
      EEPROM.write(EEPROM_ledAfternoonBrightness_addr,(byte)value); 
    }
    else if (type==SCHEDULER_MODE_EVENING) {
      EEPROM.write(EEPROM_ledEveningBrightness_addr,(byte)value); 
    }
    else if (type==SCHEDULER_MODE_NIGHT) {
      EEPROM.write(EEPROM_ledNightBrightness_addr,(byte)value); 
    }
  }
  else {
    EEPROM.write(EEPROM_ledManualBrightness_addr,(byte)value); 
  }
}

void configSaveSchedulerTimers() {
  EEPROM.write(EEPROM_schedulerStartMorningHour_addr,schedulerStartMorningHour);
  EEPROM.write(EEPROM_schedulerStartMorningMinute_addr,schedulerStartMorningMinute);
  EEPROM.write(EEPROM_schedulerStartAfternoonHour_addr,schedulerStartAfternoonHour);
  EEPROM.write(EEPROM_schedulerStartAfternoonMinute_addr,schedulerStartAfternoonMinute);
  EEPROM.write(EEPROM_schedulerStartEveningHour_addr,schedulerStartEveningHour);
  EEPROM.write(EEPROM_schedulerStartEveningMinute_addr,schedulerStartEveningMinute);
  EEPROM.write(EEPROM_schedulerStartNightHour_addr,schedulerStartNightHour);
  EEPROM.write(EEPROM_schedulerStartNightMinute_addr,schedulerStartNightMinute);
}


