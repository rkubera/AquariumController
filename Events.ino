void eventTimerMillis() {
  clockMillisEvent();
  ledMillisEvent();
  relaysMillisEvent();
  watchdogMillisEvent();
  fanMillisEvent();
}

void eventTimerSecond() {
  thermometerSecondEvent(); 
  errorsSecondEvent();
  beepErrors();
  phSecondEvent();
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
  phMqttPublishAll();
  relaysMqttPublishAll();
  schedulerMqttPublishAll();
  thermometerMqttPublishAll();
  sensorsMqttPublishAll();
}

void eventMqttDisconnected() {

}


