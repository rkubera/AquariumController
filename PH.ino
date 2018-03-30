  void phInit() {
  pinMode(ANALOG_IN_PIN_0, INPUT);
  for (int i=0; i<phNumReadings; i++) {
    phReadings[i] = 7.0;
    phRawReadings[i] = phRawRead1;
  }
}

void phMqttPublishAll() {
  mqttElPublish( setBufferFromFlash(getPHMin), floatToString(phMin));
  mqttElPublish( setBufferFromFlash(getPHMax), floatToString(phMax));
  mqttElPublish( setBufferFromFlash(getPH1Calib), floatToString(phValue1));
  mqttElPublish( setBufferFromFlash(getPH2Calib), floatToString(phValue2));
  mqttElPublish( setBufferFromFlash(getPH1RawCalib), intToString((int)phRawRead1));
  mqttElPublish( setBufferFromFlash(getPH2RawCalib), intToString((int)phRawRead2));
  mqttElPublish( setBufferFromFlash(getRawPH), intToString(phLastPhRawReading));
  mqttElPublish( setBufferFromFlash(getPH), floatToString(phLastPhAqua));
}

void phSecondEvent() {
  float a,b;
  if (phRawRead2!=phRawRead1) {
    a = (phValue2-phValue1)/(phRawRead2-phRawRead1);
  }
  else {
    a = 0;
  }
  b = phValue2-(a*phRawRead2);
  
  float analog;
  float myph;
  float phsum;
  long phrawsum;

  int count = 0;
  long rawReading = 0;
  for (int i=0; i<300; i++) {
    analog = analogRead(ANALOG_IN_PIN_0);
    myph = (a*analog)+b;
    if (myph>=0 && myph<=14) {
      phsum = phsum+myph;
      rawReading = rawReading+analog;
      count++;
    }
  }

  myph = phsum/count;
  rawReading = (rawReading/count);

  float mymin = phReadings[0];
  float mymax = phReadings[0];
  
  phsum = myph;
  phrawsum = rawReading;
  for (int i=phNumReadings-1; i>=1; i--) {
    phReadings[i] = phReadings[i-1];
    phRawReadings[i] = phRawReadings[i-1];
    phsum = phsum+phReadings[i];
    phrawsum = phrawsum+phRawReadings[i];
    if (mymin>phReadings[i]) mymin = phReadings[i];
    if (mymax<phReadings[i]) mymax = phReadings[i];
  }

  phReadings[0] = myph;
  phRawReadings[0] = rawReading;

  rawReading = phrawsum/phNumReadings;
  phAqua = (phsum-mymin-mymax)/(phNumReadings-2);
  phAqua = (float)round(phAqua*10)/10;
  
  if (phLastPhRawReading!=rawReading) {
    phLastPhRawReading = rawReading;
    mqttElPublish(setBufferFromFlash(getRawPH), intToString(rawReading));
  }

  if (abs(phLastPhAqua-phAqua)>0) {
    phLastPhAqua = phAqua;
    mqttElPublish(setBufferFromFlash(getPH), floatToString(phAqua));
  }
}


