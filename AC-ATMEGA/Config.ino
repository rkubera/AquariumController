/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/
 
//Configuration lives in RAM; the ESP restores native ArdPortal settings over UART.
void configInitializeLayout() { configStorage.begin(); }

static_assert(NAME_LENGTH+24+sizeof(float) <= PWMOUTPUTS_PWMOUTPUT_CONFIG_BYTES,
              "PWM configuration record is too small");
static_assert(NAME_LENGTH+11+sizeof(float) <= RELAYS_RELAY_CONFIG_BYTES,
              "Relay configuration record is too small");
static_assert(sizeof(double) == 4, "Runtime configuration requires AVR 32-bit double");

void configLoad() {
  configInitializeLayout();

  sensorsLoadConfig();
  relaysLoadConfig();
  pwmOutputsLoadConfig();
  
  ledModeMorning = configGetByteOrDefault(CONFIG_ledModeMorning_addr,0,LED_MODE_CUSTOM,LED_MODE_BLACK);
  ledModeAfternoon = configGetByteOrDefault(CONFIG_ledModeAfternoon_addr,0,LED_MODE_CUSTOM,LED_MODE_BLACK);
  ledModeEvening = configGetByteOrDefault(CONFIG_ledModeEvening_addr,0,LED_MODE_CUSTOM,LED_MODE_BLACK);
  ledModeNight = configGetByteOrDefault(CONFIG_ledModeNight_addr,0,LED_MODE_CUSTOM,LED_MODE_BLACK);

  schedulerStartMorningHour = configGetByteOrDefault(CONFIG_schedulerStartMorningHour_addr,0,23,9);
  schedulerStartMorningMinute = configGetByteOrDefault(CONFIG_schedulerStartMorningMinute_addr,0,59,0);
  schedulerStartAfternoonHour = configGetByteOrDefault(CONFIG_schedulerStartAfternoonHour_addr,0,23,10);
  schedulerStartAfternoonMinute = configGetByteOrDefault(CONFIG_schedulerStartAfternoonMinute_addr,0,59,0);
  schedulerStartEveningHour = configGetByteOrDefault(CONFIG_schedulerStartEveningHour_addr,0,23,19);
  schedulerStartEveningMinute = configGetByteOrDefault(CONFIG_schedulerStartEveningMinute_addr,0,59,0);
  schedulerStartNightHour = configGetByteOrDefault(CONFIG_schedulerStartNightHour_addr,0,23,22);
  schedulerStartNightMinute = configGetByteOrDefault(CONFIG_schedulerStartNightMinute_addr,0,59,0);
  schedulerLoadConfiguration();
  
  ledMorningBrightness = configGetBrightness(CONFIG_ledMorningBrightness_addr);
  ledAfternoonBrightness = configGetBrightness(CONFIG_ledAfternoonBrightness_addr);
  ledEveningBrightness = configGetBrightness(CONFIG_ledEveningBrightness_addr);
  ledNightBrightness = configGetBrightness(CONFIG_ledNightBrightness_addr);
  ledManualBrightness = configGetBrightness(CONFIG_ledManualBrightness_addr);

  fanStartTemperature = configGetValue(CONFIG_fanStartTemperature_addr);
  fanMaxSpeedTemperature = configGetValue(CONFIG_fanMaxSpeedTemperature_addr);
  
  maxInternalTemperature = configGetValue(CONFIG_maxInternalTemperature_addr);
  if (fanStartTemperature > 100) fanStartTemperature = 30;
  if (fanMaxSpeedTemperature <= fanStartTemperature || fanMaxSpeedTemperature > 100) fanMaxSpeedTemperature = fanStartTemperature+5;
  if (maxInternalTemperature > 100) maxInternalTemperature = 100;

  ledControlMode = configGetByteOrDefault(CONFIG_ledControlMode_addr,0,1,0);
  ledManualMode = configGetByteOrDefault(CONFIG_ledManualMode_addr,0,LED_MODE_CUSTOM,LED_MODE_BLACK);

  buzzerOnStart = configGetByteOrDefault(CONFIG_buzzerOnStart_addr,BUZZER_OFF,BUZZER_ON,BUZZER_OFF);
  buzzerOnErrors = configGetByteOrDefault(CONFIG_buzzerOnErrors_addr,BUZZER_OFF,BUZZER_ON,BUZZER_OFF);
  byte storedBuzzerPhases=configGetValue(CONFIG_buzzerPhases_addr);
  buzzerPhaseMask=(storedBuzzerPhases & 0xf0)==0xa0 ? storedBuzzerPhases & 15 : 7;

  timezoneRule1Week = configGetByteOrDefault(CONFIG_timezoneRule1Week_addr,0,4,0);
  timezoneRule2Week = configGetByteOrDefault(CONFIG_timezoneRule2Week_addr,0,4,0);
  timezoneRule1DayOfWeek = configGetByteOrDefault(CONFIG_timezoneRule1DayOfWeek_addr,1,7,1);
  timezoneRule2DayOfWeek = configGetByteOrDefault(CONFIG_timezoneRule2DayOfWeek_addr,1,7,1);
  timezoneRule1Hour = configGetByteOrDefault(CONFIG_timezoneRule1Hour_addr,0,23,0);
  timezoneRule2Hour = configGetByteOrDefault(CONFIG_timezoneRule2Hour_addr,0,23,0);
  timezoneRule1Offset = configGetValue(CONFIG_timezoneRule1Offset_addr);
  timezoneRule2Offset = configGetValue(CONFIG_timezoneRule2Offset_addr);
  timezoneRule1Month = configGetByteOrDefault(CONFIG_timezoneRule1Month_addr,1,12,1);
  timezoneRule2Month = configGetByteOrDefault(CONFIG_timezoneRule2Month_addr,1,12,1);

  if (timezoneRule1Offset < -12 || timezoneRule1Offset > 14) timezoneRule1Offset = 0;
  if (timezoneRule2Offset < -12 || timezoneRule2Offset > 14) timezoneRule2Offset = 0;
  ledCustomColor = configGetUint32Value(CONFIG_ledCustomColor_addr);
  ledCustomColor &= 0xFFFFFFUL;
}

byte configGetValue(int addr) {   //1 byte
  return configStorage.read(addr);
}

float configGetFloatValue(int addr) { //4 bytes
  float ret;
  configStorage.get(addr,ret);
  return ret;
}

double configGetDoubleValue(int addr) {  //4bytes
  double ret;
  configStorage.get(addr,ret);
  return ret;
}

uint32_t configGetUint32Value(int addr) { //4 bytes
  uint32_t ret;
  configStorage.get(addr,ret);
  return ret;
}

void configSaveFloatValue (float value, int addr) {
  configStorage.put(addr,value); 
}

void configSaveDoubleValue (double value, int addr) {
  configStorage.put(addr,value);
}

void configSaveUint32Value (uint32_t value, int addr) {
  configStorage.put(addr,value);
}
void configSaveValue (int value, int addr) {
  configStorage.update(addr,(byte)value); 
}

void configSaveValue (byte value, int addr) {
  configStorage.update(addr,value); 
}

String configReadString (int addr, int maxSize) {
  return readConfigName(addr, maxSize);
}

void configSaveString  (const String& str, int addr, int maxSize) {
  int index;
  for (index = 0; index < maxSize; index++) {
    configStorage.update(addr+index, index < (int)str.length() ? str[index] : 0);
  }
  //Readers are bounded by maxSize; the next byte belongs to configuration.  
}

void configSaveLedBrightness(int type, uint16_t value) {
  int addr;
  if (type == SCHEDULER_MODE_MORNING) addr = CONFIG_ledMorningBrightness_addr;
  else if (type == SCHEDULER_MODE_AFTERNOON) addr = CONFIG_ledAfternoonBrightness_addr;
  else if (type == SCHEDULER_MODE_EVENING) addr = CONFIG_ledEveningBrightness_addr;
  else if (type == SCHEDULER_MODE_NIGHT) addr = CONFIG_ledNightBrightness_addr;
  else return;
  configSaveUint32Value(value,addr);
}

void configSaveSchedulerTimers() {
  schedulerSaveConfiguration();
  configStorage.update(CONFIG_schedulerStartMorningHour_addr,schedulerStartMorningHour);
  configStorage.update(CONFIG_schedulerStartMorningMinute_addr,schedulerStartMorningMinute);
  configStorage.update(CONFIG_schedulerStartAfternoonHour_addr,schedulerStartAfternoonHour);
  configStorage.update(CONFIG_schedulerStartAfternoonMinute_addr,schedulerStartAfternoonMinute);
  configStorage.update(CONFIG_schedulerStartEveningHour_addr,schedulerStartEveningHour);
  configStorage.update(CONFIG_schedulerStartEveningMinute_addr,schedulerStartEveningMinute);
  configStorage.update(CONFIG_schedulerStartNightHour_addr,schedulerStartNightHour);
  configStorage.update(CONFIG_schedulerStartNightMinute_addr,schedulerStartNightMinute);
}

byte configGetByteOrDefault(int addr,byte low,byte high,byte fallback) {
  byte value = configStorage.read(addr);
  return value >= low && value <= high ? value : fallback;
}

uint16_t configGetBrightness(int addr) {
  uint32_t value = configGetUint32Value(addr);
  return value <= LED_BRIGHTNESS_MAX ? (uint16_t)value : 0;
}
