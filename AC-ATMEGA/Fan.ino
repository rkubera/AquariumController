/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/
 
void initFan() {
  fanPWM=0;
  pinMode(FAN_PIN, OUTPUT);
  analogWrite(FAN_PIN,0);
  //fanReportAll();
}

void fanReportAll() {
  reportField(FieldId::FanStartTemp, (String)fanStartTemperature);
  reportField(FieldId::FanMaxSpeedTemp, (String)fanMaxSpeedTemperature);
  reportField(FieldId::FanPWMValue, longToString(fanPWM));
}

void fanMillisEvent() {
  fanEvent();
}

void fanMinuteEvent() {
  fanEvent();
}

void fanEvent() {
  if(!megaFeatures.has(ControllerFeatures::Fan)){if(fanPWM){fanPWM=0;analogWrite(FAN_PIN,0);}return;}
  static unsigned long lastPublishedAt;
  static long lastPublishedPWM=-1;
  byte previousPWM=fanPWM;
  bool failsafe=!dhtHasValidReading() || fanMaxSpeedTemperature<=fanStartTemperature;
  if (!megaConfigReady) fanPWM=0;
  else if (failsafe) fanPWM=FAN_PWM_MAX;
  else if (dhtTemperature<=fanStartTemperature) fanPWM=0;
  else {
    float fraction=(dhtTemperature-fanStartTemperature)/(fanMaxSpeedTemperature-fanStartTemperature);
    fraction=constrain(fraction,0.0f,1.0f);
    fanPWM=byte(FAN_PWM_MAX*(0.25f+0.75f*fraction)+0.5f);
  }
  if (fanPWM!=previousPWM) analogWrite(FAN_PIN,fanPWM);
  bool immediate=failsafe || previousPWM==0 || fanPWM==0;
  if (lastPublishedPWM!=fanPWM && (immediate || millis()-lastPublishedAt>=TELEMETRY_MIN_INTERVAL_MS)) {
    lastPublishedAt=millis();
    lastPublishedPWM=fanPWM;
    reportField(FieldId::FanPWMValue,longToString(fanPWM));
  }
}
