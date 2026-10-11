static byte schedulerPlanMode=0;
static byte schedulerPlanTimes[2][8];
//Reserved sensor-record padding, distinct from PWM upper bits at offsets 33–34.
int schedulerConfigAddress(byte index){return CONFIG_sensors_addr+(index/5)*SENSORS_SENSOR_CONFIG_BYTES+35+index%5;}
void schedulerLoadConfiguration(){
 bool stored=configStorage.read(schedulerConfigAddress(17))==0x53;
 schedulerPlanMode=stored && configStorage.read(schedulerConfigAddress(0))==1?1:0;
 const byte daily[]={schedulerStartMorningHour,schedulerStartMorningMinute,schedulerStartAfternoonHour,schedulerStartAfternoonMinute,schedulerStartEveningHour,schedulerStartEveningMinute,schedulerStartNightHour,schedulerStartNightMinute};
 for(byte bank=0;bank<2;bank++)for(byte i=0;i<8;i++){
  byte value=stored?configStorage.read(schedulerConfigAddress(1+bank*8+i)):daily[i];
  schedulerPlanTimes[bank][i]=value<=(i%2?59:23)?value:daily[i];
 }
}
void schedulerSaveConfiguration(){
 configStorage.update(schedulerConfigAddress(0),schedulerPlanMode);
 for(byte bank=0;bank<2;bank++)for(byte i=0;i<8;i++)configStorage.update(schedulerConfigAddress(1+bank*8+i),schedulerPlanTimes[bank][i]);
 configStorage.update(schedulerConfigAddress(17),0x53);
}
static const uint16_t schedulerExtraFields[] PROGMEM={FieldId::WeekdayMorningTime,FieldId::WeekdayAfternoonTime,FieldId::WeekdayEveningTime,FieldId::WeekdayNightTime,FieldId::WeekendMorningTime,FieldId::WeekendAfternoonTime,FieldId::WeekendEveningTime,FieldId::WeekendNightTime};
void schedulerReportExtra(){
 reportField(FieldId::ScheduleMode,schedulerPlanMode?"weekdayweekend":"daily");
 for(byte i=0;i<8;i++){sprintf(bufferOut,"%02d:%02d:00",schedulerPlanTimes[i/4][(i%4)*2],schedulerPlanTimes[i/4][(i%4)*2+1]);reportBuffer(pgm_read_word(schedulerExtraFields+i));}
}
bool schedulerApplySetting(uint16_t endpoint,String value){
 if(endpoint==FieldId::ScheduleMode){if(value!="daily" && value!="weekdayweekend")return true;schedulerPlanMode=value=="weekdayweekend";schedulerSaveConfiguration();schedulerReportExtra();schedulerPublishActualPartOfDay();ledScheduleBrightnessReset=true;ledSetActualMode();return true;}
 for(byte i=0;i<8;i++)if(endpoint==pgm_read_word(schedulerExtraFields+i)){
  if(!linkIsTime(value) || (value.length()==8 && value.substring(6)!="00"))return true;
  schedulerPlanTimes[i/4][(i%4)*2]=parseHourFromString(value);schedulerPlanTimes[i/4][(i%4)*2+1]=parseMinuteFromString(value);
  schedulerSaveConfiguration();schedulerReportExtra();schedulerPublishActualPartOfDay();ledScheduleBrightnessReset=true;ledSetActualMode();return true;
 }
 return false;
}
ScheduleSelection schedulerSelect(byte weekdayValue,uint16_t minuteValue){
 const byte hours[]={schedulerStartMorningHour,schedulerStartAfternoonHour,schedulerStartEveningHour,schedulerStartNightHour};
 const byte minutes[]={schedulerStartMorningMinute,schedulerStartAfternoonMinute,schedulerStartEveningMinute,schedulerStartNightMinute};
 uint16_t times[3][4];for(byte phase=0;phase<4;phase++){times[0][phase]=uint16_t(hours[phase])*60+minutes[phase];for(byte bank=1;bank<3;bank++)times[bank][phase]=uint16_t(schedulerPlanTimes[bank-1][phase*2])*60+schedulerPlanTimes[bank-1][phase*2+1];}
 return SchedulePlan::select(schedulerPlanMode,weekdayValue,minuteValue,times);
}
unsigned long schedulerGetPhaseElapsedSeconds(){time_t local=clockGetLocalTime();return uint32_t(schedulerSelect(weekday(local),hour(local)*60+minute(local)).elapsed)*60+second(local);}

/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/
 

void schedulerReportAll() {
  schedulerReportExtra();
  sprintf(bufferOut,"%02d:%02d:00", schedulerStartMorningHour, schedulerStartMorningMinute);
  reportBuffer(FieldId::MorningTime);

  sprintf(bufferOut,"%02d:%02d:00", schedulerStartAfternoonHour, schedulerStartAfternoonMinute);
  reportBuffer(FieldId::AfternoonTime);
   
  sprintf(bufferOut,"%02d:%02d:00", schedulerStartEveningHour, schedulerStartEveningMinute);
  reportBuffer(FieldId::EveningTime);
      
  sprintf(bufferOut,"%02d:%02d:00", schedulerStartNightHour, schedulerStartNightMinute);
  reportBuffer(FieldId::NightTime);
}

int schedulerGetActualPartOfDay() {
  time_t local = clockGetLocalTime();
  return schedulerGetPartOfDay(hour(local), minute(local), false);
}

int schedulerPublishActualPartOfDay() {
  time_t local = clockGetLocalTime();
  return schedulerGetPartOfDay(hour(local), minute(local), true);
}

int schedulerGetPartOfDay(double h,double m,bool forcePublish) {
  static byte lastMode=SCHEDULER_MODE_INITIAL;
  const byte modes[]={SCHEDULER_MODE_MORNING,SCHEDULER_MODE_AFTERNOON,SCHEDULER_MODE_EVENING,SCHEDULER_MODE_NIGHT};
  const char* labels[]={schedulerMorning,schedulerAfternoon,schedulerEvening,schedulerNight};
  byte selected=schedulerSelect(weekday(clockGetLocalTime()),int(h)*60+int(m)).phase;
  byte mode=modes[selected];
  if (forcePublish || lastMode!=mode) {
    lastMode=mode;
    reportField(FieldId::ActualPartOfDay,setBufferFromFlash(labels[selected]));
  }
  return mode;
}
