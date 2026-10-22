/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/


void criticalEvent() {
  linkCheckCriticalEvent();
  criticalEventNoLink();
}

void criticalEventNoLink() {
  ledFadeService();
  if (millis()-lastCriticalEvent >= CRITICAL_EVENT_MIN_MILLIS) {
    lastCriticalEvent = millis(); //Set before callbacks that can re-enter this function.
    ledMicrosEvent();

     //Serial.println(millis());
     wdt_reset();

    //IMPORTANT!!!
    lastCriticalEvent = millis();

  }
}

void eventTimerMillis() {
  uint32_t workAt;
  static uint32_t lastAnalogControl=0;
  if(uint32_t(millis()-lastAnalogControl)>=ANALOG_CONTROL_INTERVAL_MS){
    lastAnalogControl=millis();
    pwmOutputsControlEvent();relaysControlEvent();
  }
  buzzerMillisEvent();

  runtimeMark(RuntimeRelay);workAt=micros();relaysMillisEvent();usbDiagnosticStage(0,micros()-workAt);
  
  runtimeMark(RuntimePwm);workAt=micros();pwmOutputsMillisEvent();usbDiagnosticStage(1,micros()-workAt);
  
  runtimeMark(RuntimeClock);workAt=micros();clockMillisEvent();usbDiagnosticStage(2,micros()-workAt);

  runtimeMark(RuntimeLcd);workAt=micros();menuShowCMillis();usbDiagnosticStage(3,micros()-workAt);

  runtimeMark(RuntimeFan);workAt=micros();fanMillisEvent();usbDiagnosticStage(4,micros()-workAt);

  runtimeMark(RuntimeDht);workAt=micros();dhtGetDataMillisEvent();usbDiagnosticStage(5,micros()-workAt);

  runtimeMark(RuntimeKeyboard);workAt=micros();keyboardCheckMillisEvent();usbDiagnosticStage(6,micros()-workAt);
  lcdBacklightService();
  
  //IMPORTANT!!!
  runtimeMark(RuntimeCritical);workAt=micros();criticalEvent();usbDiagnosticStage(7,micros()-workAt);
}

void eventTimerSecond() {
  linkSecondsEvent();
  
  errorsSecondEvent();
  beepErrors();

  //IMPORTANT!!!
  criticalEvent();
}
void eventTimerTenSeconds() {

  wireRequest(AcWire::Ssid);
  
  //IMPORTANT!!!
  criticalEvent();
}

void eventTimerMinute() {
  clockMinuteEvent();
  
  fanMinuteEvent();
  
  //IMPORTANT!!!
  criticalEvent();
}

void eventTimerHour() {

  //IMPORTANT!!!
  criticalEvent();
}

void eventWifiConnected() {
  if (hostnameReceived==false) {
    wireRequest(AcWire::Hostname);
  }

  if (brokerServerReceived==false && hostnameReceived==true) {
    wireRequest(AcWire::BrokerServer);
  }

  if (wifiStatus == WIFI_STATUS_DISCONNECTED) {
    wireRequest(AcWire::Broker);
  }
  wifiStatus = WIFI_STATUS_CONNECTED;
  
  //IMPORTANT!!!
  criticalEvent();
}

void eventBrokerServerReceived() {
  brokerServerReceived = true;
  reportNetwork();
}

void eventHostnameReceived() {
  if (deviceName!="") {
    hostnameReceived = true;
    linkRequestSnapshot();
    
  }

  //IMPORTANT!!!
  criticalEvent();
}

void eventWifiDisconnected() {
  brokerStatus = BROKER_STATUS_DISCONNECTED;
  if (!linkLocalPanel) publishValue = -1;
  wifiStatus = WIFI_STATUS_DISCONNECTED;
  // Periodic status polls handle reconnection; do not recursively query each
  // disconnected reply. The local UART panel still has a valid device name.
  if (!linkLocalPanel) hostnameReceived = false;


  //IMPORTANT!!!
  criticalEvent();
}

void eventBrokerConnected() {
  if (brokerStatus == BROKER_STATUS_DISCONNECTED) {
    linkRequestSnapshot();
    brokerStatus = BROKER_STATUS_CONNECTED;
  }
  brokerStatus = BROKER_STATUS_CONNECTED;

  //IMPORTANT!!!
  criticalEvent();
}

void eventBrokerDisconnected() {
  if (!linkLocalPanel) publishValue = -1;
  brokerStatus = BROKER_STATUS_DISCONNECTED;

  //IMPORTANT!!!
  criticalEvent();
}
