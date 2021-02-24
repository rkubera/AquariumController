/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/


void criticalEvent() {
  mqttCheck();
  criticalEventNoMqtt();
  menuShow();
}

void criticalEventNoMqtt() {
//  ledMillisEvent();
  ledMicrosEvent();
  wdt_reset();
}

void eventTimerMillis() {
  clockMillisEvent();
  
  relaysMillisEvent();
  
  pwmOutputsMillisEvent();
  
  watchdogMillisEvent();
  
  fanMillisEvent();

  //IMPORTANT!!!
  criticalEvent();
}

void eventTimerSecond() {
  errorsSecondEvent();
  
  beepErrors();
  
  sensorsSecondEvent();
  
  pwmOutputsSecondEvent();
  
  relaysSecondEvent();

  //Publish topics
  if (mqttStatus == MQTT_STATUS_CONNECTED && hostnameReceived == true && mqttElDeviceName!="") {
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

  //IMPORTANT!!!
  criticalEvent();
}
void eventTimerTenSeconds() {
  mqttTenSecondsEvent();

  String command = setBufferFromFlash(getSsid);
  mqttSendCommand(command);
  
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
    String command = setBufferFromFlash(getHostname);
    mqttSendCommand(command);
  }

  if (wifiStatus == WIFI_STATUS_DISCONNECTED) {
    String command = setBufferFromFlash(getMqttStatus);
    mqttSendCommand(command);
    
  }
  wifiStatus = WIFI_STATUS_CONNECTED;
}

void eventHostnameReceived() {
  if (mqttElDeviceName!="") {
    hostnameReceived = true;
  }
}

void eventWifiDisconnected() {
  publishValue = -1;
  wifiStatus = WIFI_STATUS_DISCONNECTED;
  String command = setBufferFromFlash(getWifiStatus);
  mqttSendCommand(command);
}

void eventMqttConnected() {
  if (mqttStatus == MQTT_STATUS_DISCONNECTED) {
    publishValue = 0;
    mqttStatus = MQTT_STATUS_CONNECTED;
  }
  mqttStatus = MQTT_STATUS_CONNECTED;

  if (subscriptionSent==false) {
    mqttSubscribe();
  }
}

void eventMqttDisconnected() {
  publishValue = -1;
  mqttStatus = MQTT_STATUS_DISCONNECTED;
}
