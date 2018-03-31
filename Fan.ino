/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/
 
void initFan() {
  pinMode(FAN_PIN, OUTPUT);
  analogWrite (FAN_PIN, 255);
  //fanMqttPublishAll();
}

void fanMqttPublishAll() {
  mqttElPublish(setBufferFromFlash(getFanStartTemperature), (String)fanStartTemperature);
  mqttElPublish(setBufferFromFlash(getFanMaxSpeedTemperature), (String)fanMaxSpeedTemperature);
  mqttElPublish(setBufferFromFlash(getFanPWMValue), intToString(fanPWM));
}

void fanMillisEvent() {
  fanEvent(false);
}

void fanMinuteEvent() {
  fanEvent(true);
}

void fanEvent(bool stopFan) {
  static uint32_t fanStartTime;
  static int fanLastPWM;

  byte mytemp = dhtTemperature;
  
  if (dhtTemperature>fanMaxSpeedTemperature) {
    mytemp = maxInternalTemperature;
  }
  if (dhtTemperature>fanStartTemperature) {
    fanPWM = map (mytemp, fanStartTemperature, fanMaxSpeedTemperature, 64, 255); 
  } 
  else {
    if (stopFan==true) {
      fanPWM = 0;
    }
  }
  analogWrite (FAN_PIN, fanPWM);
  if (abs(fanStartTime-millis())>1000) {
    fanStartTime = millis();
    if (fanLastPWM!=fanPWM) {
      mqttElPublish(setBufferFromFlash(getFanPWMValue), intToString(fanPWM));
    }
  }
}


