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
  mqttElPublish( setBufferFromFlash(getMaxInternalTemperature), (String)maxInternalTemperature);
  mqttElPublish(setBufferFromFlash(getInternalTemperature), String(dhtTemperature));
  mqttElPublish(setBufferFromFlash(getInternalHumidity),String(dhtHumidity));
}

void dhtGetData() {
  static double lastPubTime; 
  static bool changedTemp, changedHum;
  int chk = DHT.read11(DHT_PIN);
  switch (chk) {
    case DHTLIB_OK:  
      if (DHT.humidity<100) {
        float oldDhtHumidity = dhtHumidity;
        float oldDhtTemperature = dhtTemperature;
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
        
        if (oldDhtHumidity!=dhtHumidity) {
          changedHum = true;
        }
        if (oldDhtTemperature != dhtTemperature) {
          changedTemp = true;
        }

        if ((changedTemp==true || changedHum==true) && (abs(millis()-lastPubTime)>MQTT_MIN_REFRESH_MILLIS)) {
          if (changedTemp==true) {
            changedTemp = false;
            mqttElPublish(setBufferFromFlash(getInternalTemperature),String(dhtTemperature));
          }
          if (changedHum==true) {
            changedHum = false;
            mqttElPublish(setBufferFromFlash(getInternalHumidity),String(dhtHumidity));
          }
          lastPubTime = millis();
        }
      }
    break;
  }
}

