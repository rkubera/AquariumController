void errorsInit() {
  
}

void errorsMqttPublishAll() {
  errorReport();
}
void errorsSecondEvent() {
   errorsCount = 0;
   errorTemperatureEvent();
   errorPHEvent();

   errorReport();
}

void errorPHEvent() {
  if (phAqua<phMin || phAqua>phMax) {
    Serial.print(F("ERROR: PH: "));
    Serial.println(phAqua);
    errorPH = true;
    errorsCount = errorsCount+1;
  }
  else {
    errorPH = false;
  }
}

static byte errorTemperatureTimeout = 0;
void errorTemperatureEvent() {
  for (int i=errorsNumReadings-1; i>0; i--) {
    errorsTemparature[i] = errorsTemparature[i-1];
  }

  if (dhtTemperature>maxInternalTemperature) {
    errorsTemparature[0] = 1;
  }
  else {
    errorsTemparature[0] = 0;
  }

  if (errorTemperature==true) {
    errorTemperatureTimeout++;
    errorsCount = errorsCount+1;
    Serial.print(F("ERROR: Temperature too high: "));
    Serial.println(dhtTemperature);
  }

  if (errorTemperatureTimeout>60 || errorTemperature==false) {
    errorTemperatureTimeout = 0;
    if (stats.median(errorsTemparature,errorsNumReadings)==1) {
      errorTemperature = true;
    }
    else {
      errorTemperature = false;
    }
  }
}

int errorReportStatus;
int errorReportLastStatus = -1;

void errorReport() {
  String result;
  if (errorsCount==0) {
    errorReportStatus = 0;
    result = "Ok"; 
  }
  else {
    time_t local = clockGetLocalTime();
    int actualErrorShow = second(local)-(floor((float)second(local)/(float)errorsCount)*errorsCount)+1;
    int errorsLocalCount = 1;
    if (errorTemperature==true && errorsLocalCount<=actualErrorShow) {
      result = "Internal temp. error";
      errorReportStatus = 1;
      errorsLocalCount++;
    }
    if (errorPH==true && errorsLocalCount<=actualErrorShow) {
      result = "PH error";
      errorReportStatus = 2;
      errorsLocalCount++;
    }
  }
  if (errorReportLastStatus!=errorReportStatus) {
    errorReportLastStatus = errorReportStatus;
    mqttElPublish(setBufferFromFlash(getStatus),result);
  }
}


