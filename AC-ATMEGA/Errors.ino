/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/
 
#include "AlarmHistory.h"
static AlarmHistory alarmHistory;

void errorsReportHistory(){
  for(byte slot=0;slot<3;slot++){
    uint16_t field=slot==0?FieldId::AlarmLast1:slot==1?FieldId::AlarmLast2:FieldId::AlarmLast3;
    if(!linkFieldWanted(field))continue;
    byte code=alarmHistory.codes[slot];
    if(!code){reportSelectedField(field,String(F("Brak aktywnego alarmu")));continue;}
    tmElements_t parts;breakTime(alarmHistory.timestamps[slot],parts);
    String text;
    if(alarmHistory.timestamps[slot]){
      snprintf_P(bufferOut,bufferOutSize,PSTR("%04d-%02d-%02d %02d:%02d:%02d — "),tmYearToCalendar(parts.Year),parts.Month,parts.Day,parts.Hour,parts.Minute,parts.Second);
      text=bufferOut;
    }else text=F("Czas nieustalony — ");
    if(code<=8)text+="S"+String(code)+F(": brak poprawnego odczytu");
    else if(code<=12)text+="R"+String(code-8)+F(": brak poprawnego wejścia");
    else if(code<=18)text+="PWM"+String(code-12)+F(": błąd obliczeń PID");
    else if(code<=24)text+="PWM"+String(code-18)+F(": brak poprawnego wejścia");
    else if(code==25)text+=F("DHT: brak poprawnego odczytu");
    else if(code==26)text+=F("Przekroczony alarm temperatury");
    else text+=F("RTC: brak modułu lub nieprawidłowy odczyt czasu");
    reportSelectedField(field,text);
  }
}

void errorsInit() {
  errorsLastReportStatus = -1;
}

void errorsReportAll() {
  errorReport(true);
  errorsReportHistory();
}

void errorsSecondEvent() {
  errorReport(false);
}

void errorReport() {
  errorReport(false);
}

void errorReport(bool forcePublish) {
  static String lastReport;
  String result;
  uint32_t activeAlarms=0;
  errorsCount = 0;
  errorsReportStatus = 0;
  for (byte i=1; i<=megaFeatures.inputs; i++) {
    if (sensorsGetSensor(i,SENSORS_VALUE_TYPE) != SENSOR_TYPE_NONE &&
        !sensorsGetSensor(i,SENSORS_VALUE_VALID)) {
      result += "S"+intToString(i)+" invalid; ";
      errorsCount++;
      errorsReportStatus |= 1;
      activeAlarms |= uint32_t(1)<<(i-1);
    }
  }
  for (byte i=1; i<=megaFeatures.relays; i++) {
    if (relaysGetRelay(i,RELAY_CONTROL_MODE) == CONTROL_MODE_TRESHOLD &&
        sensorsHasOutputInput(i,OUTPUT_TYPE_RELAY) &&
        !isfinite(sensorsGetSensorsValue(i,OUTPUT_TYPE_RELAY))) {
      result += "R"+intToString(i)+" invalid; ";
      errorsCount++;
      errorsReportStatus |= 2;
      activeAlarms |= uint32_t(1)<<(8+i-1);
    }
  }
  for (byte i=1; i<=megaFeatures.pwm; i++) {
    byte mode = pwmOutputsGetPwmOutput(i,PWMOUTPUT_CONTROL_MODE);
    if (mode==CONTROL_MODE_PID && isfinite(sensorsGetSensorsValue(i,OUTPUT_TYPE_PWM)) &&
        !pwmOutputsGetPwmOutput(i,PWMOUTPUT_PID_VALID)) {
      result+="P"+intToString(i)+" PID arithmetic; "; errorsCount++; errorsReportStatus|=16; activeAlarms |= uint32_t(1)<<(12+i-1);
    }
    if ((mode == CONTROL_MODE_TRESHOLD || mode == CONTROL_MODE_PID) &&
        sensorsHasOutputInput(i,OUTPUT_TYPE_PWM) &&
        !isfinite(sensorsGetSensorsValue(i,OUTPUT_TYPE_PWM))) {
      result += "P"+intToString(i)+" invalid; ";
      errorsCount++;
      errorsReportStatus |= 2;
      activeAlarms |= uint32_t(1)<<(18+i-1);
    }
  }
  if (megaFeatures.has(ControllerFeatures::Dht) && !dhtHasValidReading()) {
    result += "DHT invalid; ";
    errorsCount++;
    errorsReportStatus |= 8;
    activeAlarms |= uint32_t(1)<<24;
  }
  if (megaFeatures.has(ControllerFeatures::Dht) && isfinite(dhtTemperature) && dhtTemperature >= maxInternalTemperature) {
    result += "DHT high; ";
    errorsCount++;
    errorsReportStatus |= 4;
    activeAlarms |= uint32_t(1)<<25;
  }
  if (megaFeatures.has(ControllerFeatures::Rtc) && (clockRtcStatus==2 || clockRtcStatus==3)) {
    result += clockRtcStatus==2 ? F("RTC missing; ") : F("RTC invalid; ");
    errorsCount++;
    errorsReportStatus |= 32;
    activeAlarms |= uint32_t(1)<<26;
  }
  // Ignore pre-configuration startup checks; timestamps come from Mega's local clock.
  if(megaConfigReady && !linkSnapshotCapture){
    uint32_t localTime=clockGetLocalTime();
    if(globalYear<2020)localTime=0;
    if(alarmHistory.update(activeAlarms,localTime))errorsReportHistory();
  }
  if (errorsCount == 0) result = "Ok";
  if (forcePublish || result != lastReport || errorsLastReportStatus != errorsReportStatus) {
    lastReport = result;
    errorsLastReportStatus = errorsReportStatus;
    reportField(FieldId::Status,result);
    reportField(FieldId::ErrorCount,intToString(errorsCount));
  }
}
