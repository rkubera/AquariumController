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

}

void eventMqttConnected() {
  buzzerMqttPublishAll();
  clockMqttPublishAll();
  dhtMqttPublishAll();
  errorsMqttPublishAll();
  fanMqttPublishAll();
  ledMqttPublishAll();
  relaysMqttPublishAll();
  pwmOutputsMqttPublishAll();
  schedulerMqttPublishAll();
  sensorsMqttPublishAll();
}

void eventMqttDisconnected() {

}


