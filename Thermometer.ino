void thermometerInit() {
  pinMode(ANALOG_IN_PIN_1, INPUT);  
  for (int i=0; i<tempNumReadings; i++) {
    tempReadings[i] = tempValue1;
    tempRawReadings[i] = tempRawRead1;
  }
}

void thermometerMqttPublishAll() {
  mqttElPublish(setBufferFromFlash(getTempMin), floatToString(tempMin));
  mqttElPublish(setBufferFromFlash(getTempMax), floatToString(tempMax));
  mqttElPublish(setBufferFromFlash(getTemp1Calib), floatToString(tempValue1));
  mqttElPublish(setBufferFromFlash(getTemp2Calib), floatToString(tempValue2));
  mqttElPublish(setBufferFromFlash(getTemp1RawCalib), intToString((int)tempRawRead1));
  mqttElPublish(setBufferFromFlash(getTemp2RawCalib), intToString((int)tempRawRead2));  
}

void thermometerSecondEvent() {
  static int lastTempRawReading;
  static float oldTempAqua;
  
  int val = analogRead(ANALOG_IN_PIN_1);

  float a,b;
  if (tempRawRead2!=tempRawRead1) {
    a = (tempValue2-tempValue1)/(tempRawRead2-tempRawRead1);
  }
  else {
    a = 0;
  }
  b = tempValue2-(a*tempRawRead2);
  
  float analog;
  float mytemp;
  float tempsum;
  long temprawsum;

  int count = 0;
  long rawReading = 0;
  for (int i=0; i<300; i++) {
    analog = analogRead(ANALOG_IN_PIN_1);
    mytemp = (a*analog)+b;
    tempsum = tempsum+mytemp;
    rawReading = rawReading+analog;
    count++;
  }

  mytemp = tempsum/count;
  rawReading = (rawReading/count);

  float mymin = tempReadings[0];
  float mymax = tempReadings[0];
  
  tempsum = mytemp;
  temprawsum = rawReading;
  for (int i=tempNumReadings-1; i>=1; i--) {
    tempReadings[i] = tempReadings[i-1];
    tempRawReadings[i] = tempRawReadings[i-1];
    tempsum = tempsum+tempReadings[i];
    temprawsum = temprawsum+tempRawReadings[i];
    if (mymin>tempReadings[i]) mymin = tempReadings[i];
    if (mymax<tempReadings[i]) mymax = tempReadings[i];
  }

  tempReadings[0] = mytemp;
  tempRawReadings[0] = rawReading;

  rawReading = temprawsum/tempNumReadings;
  tempAqua = (tempsum-mymin-mymax)/(tempNumReadings-2);
  tempAqua = (float)round(tempAqua*10)/10;
  
  if (lastTempRawReading!=rawReading) {
    lastTempRawReading = rawReading;
    mqttElPublish(setBufferFromFlash(getRawTemperature), intToString(rawReading));
  }

  if (abs(oldTempAqua-tempAqua)>0) {
    oldTempAqua = tempAqua;
    mqttElPublish(setBufferFromFlash(getTemperature), floatToString(tempAqua));
  }
}

