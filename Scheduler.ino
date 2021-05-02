/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/
 
void schedulerInit() {

}

void schedulerMqttPublishAll() {
  sprintf(bufferOut,"%02d:%02d", schedulerStartMorningHour, schedulerStartMorningMinute);
  mqttElPublish( setBufferFromFlash(getMorningTime), bufferOut );

  sprintf(bufferOut,"%02d:%02d", schedulerStartAfternoonHour, schedulerStartAfternoonMinute);
  mqttElPublish(setBufferFromFlash(getAfternoonTime), bufferOut );
   
  sprintf(bufferOut,"%02d:%02d", schedulerStartEveningHour, schedulerStartEveningMinute);
  mqttElPublish(setBufferFromFlash(getEveningTime), bufferOut );
      
  sprintf(bufferOut,"%02d:%02d", schedulerStartNightHour, schedulerStartNightMinute);
  mqttElPublish(setBufferFromFlash(getNightTime), bufferOut );
}

int schedulerGetActualPartOfDay() {
  time_t local = clockGetLocalTime();
  return schedulerGetPartOfDay(hour(local), minute(local), false);
}

int schedulerPublishActualPartOfDay() {
  time_t local = clockGetLocalTime();
  return schedulerGetPartOfDay(hour(local), minute(local), true);
}

int schedulerGetPartOfDay(double h, double m,  bool forcePublish) {
  static int schedulerLastActualMode;
  double nowHourMinute = 0+(m*60)+(h*3600);
  double startMorningHourMinute = 0+((double)schedulerStartMorningMinute*60)+((double)schedulerStartMorningHour*3600);
  double startAfternoonHourMinute = 0+((double)schedulerStartAfternoonMinute*60)+((double)schedulerStartAfternoonHour*3600);
  double startEveningHourMinute = 0+((double)schedulerStartEveningMinute*60)+((double)schedulerStartEveningHour*3600);
  double startNightHourMinute = 0+((double)schedulerStartNightMinute*60)+((double)schedulerStartNightHour*3600);  
  
  //Morning
  if (startMorningHourMinute<=startAfternoonHourMinute) {
    if (nowHourMinute>=startMorningHourMinute && nowHourMinute<startAfternoonHourMinute) {
      if (schedulerLastActualMode!=SCHEDULER_MODE_MORNING || forcePublish==true) {
        schedulerLastActualMode = SCHEDULER_MODE_MORNING;
        mqttElPublish(setBufferFromFlash(getActualPartOfDay), setBufferFromFlash(schedulerMorning));
      }
      return SCHEDULER_MODE_MORNING;
    }
  }
  if (startMorningHourMinute>startAfternoonHourMinute) {
    if (nowHourMinute>=startMorningHourMinute || nowHourMinute<startAfternoonHourMinute) {
      if (schedulerLastActualMode!=SCHEDULER_MODE_MORNING || forcePublish==true) {
        schedulerLastActualMode = SCHEDULER_MODE_MORNING;
        mqttElPublish(setBufferFromFlash(getActualPartOfDay), setBufferFromFlash(schedulerMorning));
      }
      return SCHEDULER_MODE_MORNING;
    }
  }

  //Afternoon
  if (startAfternoonHourMinute<=startEveningHourMinute) {
    if (nowHourMinute>=startAfternoonHourMinute && nowHourMinute<startEveningHourMinute) {
      if (schedulerLastActualMode!=SCHEDULER_MODE_AFTERNOON || forcePublish==true) {
        schedulerLastActualMode = SCHEDULER_MODE_AFTERNOON;
        mqttElPublish(setBufferFromFlash(getActualPartOfDay), setBufferFromFlash(schedulerAfternoon));
      }
      return SCHEDULER_MODE_AFTERNOON;
    }
  }
  if (startAfternoonHourMinute>startEveningHourMinute) {
    if (nowHourMinute>=startAfternoonHourMinute || nowHourMinute<startEveningHourMinute) {
      if (schedulerLastActualMode!=SCHEDULER_MODE_AFTERNOON|| forcePublish==true) {
        schedulerLastActualMode = SCHEDULER_MODE_AFTERNOON;
        mqttElPublish(setBufferFromFlash(getActualPartOfDay), setBufferFromFlash(schedulerAfternoon));
      }
      return SCHEDULER_MODE_AFTERNOON;
    }
  }

   //Evening
  if (startEveningHourMinute<=startNightHourMinute) {
    if (nowHourMinute>=startEveningHourMinute && nowHourMinute<startNightHourMinute) {
      if (schedulerLastActualMode!=SCHEDULER_MODE_EVENING || forcePublish==true) {
        schedulerLastActualMode = SCHEDULER_MODE_EVENING;
        mqttElPublish(setBufferFromFlash(getActualPartOfDay), setBufferFromFlash(schedulerEvening));
      }
      return SCHEDULER_MODE_EVENING;
    }
  }
  if (startEveningHourMinute>startNightHourMinute) {
    if (nowHourMinute>=startEveningHourMinute || nowHourMinute<startNightHourMinute) {
      if (schedulerLastActualMode!=SCHEDULER_MODE_EVENING || forcePublish==true) {
        schedulerLastActualMode = SCHEDULER_MODE_EVENING;
        mqttElPublish(setBufferFromFlash(getActualPartOfDay), setBufferFromFlash(schedulerEvening));
      }
      return SCHEDULER_MODE_EVENING;
    }
  }
  
  //Night
  if (schedulerLastActualMode!=SCHEDULER_MODE_NIGHT || forcePublish==true) {
    schedulerLastActualMode = SCHEDULER_MODE_NIGHT;
    mqttElPublish(setBufferFromFlash(getActualPartOfDay), setBufferFromFlash(schedulerNight));
  }
  return SCHEDULER_MODE_NIGHT;
}
