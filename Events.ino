/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/

void eventTimerMillis() {
  clockMillisEvent();
  ledMillisEvent();
  relaysMillisEvent();
  pwmOutputsMillisEvent();
  watchdogMillisEvent();
  fanMillisEvent();
}

void eventTimerSecond() {
  errorsSecondEvent();
  beepErrors();
  sensorsSecondEvent();
  pwmOutputsSecondEvent();

  //Publish topics
  if (publishValue==0) {
    buzzerMqttPublishAll();
    publishValue++;
  }
  else if (publishValue==1) {
    clockMqttPublishAll();
    publishValue++;
  }
  else if (publishValue==2) {
    dhtMqttPublishAll();
    publishValue++;
  }
  else if (publishValue==3) {
    errorsMqttPublishAll();
    publishValue++;
  }
  else if (publishValue==4) {
    fanMqttPublishAll();
    publishValue++;
  }
  else if (publishValue==5) {
    ledMqttPublishAll();
    publishValue++;
  }
  else if (publishValue==6) {
    relaysMqttPublishAll();
    publishValue++;
  }
  else if (publishValue==7) {
    pwmOutputsMqttPublishAll();
    publishValue++;
  }
  else if (publishValue==8) {
    schedulerMqttPublishAll();
    publishValue++;
  }
  else if (publishValue==9) {
    sensorsMqttPublishAll();
    publishValue++;
  }
}

void eventTimerMinute() {
  clockMinuteEvent();
  mqttMinuteEvent();
  fanMinuteEvent();
}

void eventTimerHour() {

}

void eventWifiConnected() {

}

void eventWifiDisconnected() {
  publishValue = -1;
}

void eventMqttConnected() {
  publishValue = 0;
}

void eventMqttDisconnected() {
  publishValue = -1;
}


