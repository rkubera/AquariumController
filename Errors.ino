/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/
 
void errorsInit() {
  
}

void errorsMqttPublishAll() {
  errorReport();
}
void errorsSecondEvent() {
   errorReport();
}

void errorReport() {
  String result;
  if (errorsCount==0) {
    errorsReportStatus = 0;
    result = "Ok"; 
  }
  if (errorsLastReportStatus!=errorsReportStatus) {
    errorsLastReportStatus = errorsReportStatus;
    mqttElPublish(setBufferFromFlash(getStatus),result);
  }
}


