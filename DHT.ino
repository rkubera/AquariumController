/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/
 
void dhtInit() {
  //dhtMqttPublishAll();
}

void dhtMqttPublishAll() {
  mqttElPublish(setBufferFromFlash(getMaxInternalTemperature), (String)maxInternalTemperature);
  mqttElPublish(setBufferFromFlash(getInternalTemperature), String(dhtTemperature));
  mqttElPublish(setBufferFromFlash(getInternalHumidity),String(dhtHumidity));
}

void dhtGetDataMillisEvent() {
  static double lastPubTime = millis(); 
  static float oldDhtHumidity = 0;
  static float oldDhtTemperature = 0;
  static double lastDHTReadMillis = millis();

  if (abs(lastDHTReadMillis-millis())>100) {
    lastDHTReadMillis = millis();
    int chk = DHT.read11(DHT_PIN);
    switch (chk) {
      case DHTLIB_OK:  
        if (DHT.humidity<100) {
          int myHumidity = DHT.humidity;
          int myTemperature = DHT.temperature;
          
          for (int i=dhtNumReadings-1; i>=1; i--) {
            dhtTemparatureReadings[i] = dhtTemparatureReadings[i-1];
            dhtHumidityReadings[i] = dhtHumidityReadings[i-1];
          }
  
          dhtTemparatureReadings[0] = myTemperature;
          dhtHumidityReadings[0] = myHumidity;
          
          dhtTemperature = stats.median(dhtTemparatureReadings,dhtNumReadings);
          dhtHumidity = stats.median(dhtHumidityReadings,dhtNumReadings);
          
          if (abs(millis()-lastPubTime)>MQTT_MIN_REFRESH_MILLIS) {
            if (oldDhtHumidity!=dhtHumidity) {
              mqttElPublish(setBufferFromFlash(getInternalHumidity),String(dhtHumidity));
            }
            if (oldDhtTemperature != dhtTemperature) {
              mqttElPublish(setBufferFromFlash(getInternalTemperature),String(dhtTemperature));
            }
            lastPubTime = millis();
            oldDhtHumidity = dhtHumidity;
            oldDhtTemperature = dhtTemperature;
          }
        }
      break;
    }
  }
}
