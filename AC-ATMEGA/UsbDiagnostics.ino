#include "RuntimeHealth.h"
static uint32_t usbWorkMax[9]={0};
void usbDiagnosticStage(byte stage,uint32_t elapsed){if(stage<9 && elapsed>usbWorkMax[stage])usbWorkMax[stage]=elapsed;}
// USB diagnostics never share the ESP UART or wait for the USB TX ring.
static char usbDiagnosticQueue[256];
static uint8_t usbDiagnosticHead=0,usbDiagnosticTail=0;
static uint32_t usbDiagnosticDropped=0,usbDiagnosticSettingAt=0;
static uint32_t usbDiagnosticMaxLoopUs=0;
void usbDiagnosticLine(const char* line){
 size_t length=strlen(line);
 uint8_t freeBytes=uint8_t(usbDiagnosticTail-usbDiagnosticHead-1);
 if(length+1>freeBytes){++usbDiagnosticDropped;return;}
 for(size_t i=0;i<length;i++)usbDiagnosticQueue[usbDiagnosticHead++]=line[i];
 usbDiagnosticQueue[usbDiagnosticHead++]='\n';
}
void usbDiagnosticSetting(uint32_t operation,uint16_t field,const String& value){
 usbDiagnosticSettingAt=millis();char line[96];
 snprintf_P(line,sizeof(line),PSTR("SET op=%lu field=%u value=%.32s"),(unsigned long)operation,field,value.c_str());usbDiagnosticLine(line);
}
void usbDiagnosticAck(uint32_t operation,uint16_t field,const String& value){
 char line[112];snprintf_P(line,sizeof(line),PSTR("ACK queued op=%lu field=%u value=%.32s after=%lu ms"),(unsigned long)operation,field,value.c_str(),(unsigned long)(millis()-usbDiagnosticSettingAt));usbDiagnosticLine(line);
}
void usbDiagnosticLoop(uint32_t elapsedUs){
#ifdef __AVR__
 static bool bootReported=false;
 if(!bootReported){
  bootReported=true;char bootLine[96];
  snprintf_P(bootLine,sizeof(bootLine),PSTR("BOOT MCUSR=0x%02x previousStage=%u (0=unknown)"),watchdogResetFlags,watchdogPreviousStage);
  usbDiagnosticLine(bootLine);
 }
#endif
 if(elapsedUs>usbDiagnosticMaxLoopUs)usbDiagnosticMaxLoopUs=elapsedUs;
 static uint32_t lastReport=0;uint32_t now=millis();
 if(now-lastReport>=5000UL){
  lastReport=now;char line[150];
  snprintf_P(line,sizeof(line),PSTR("MEGA up=%lu s link=%u config=%u loopMax=%lu us txFree=%u usbDropped=%lu"),(unsigned long)(now/1000),wireLink.connected(),megaConfigReady,(unsigned long)usbDiagnosticMaxLoopUs,linkWireQueue.free(),(unsigned long)usbDiagnosticDropped);
  usbDiagnosticLine(line);usbDiagnosticMaxLoopUs=0;
  if ((now/5000UL)%5==1) {
  snprintf_P(line,sizeof(line),PSTR("UART frames=%lu crc=%lu retry=%lu timeout=%lu malformed=%lu schema=%lu dropped=%lu"),(unsigned long)wireLink.stats.good,(unsigned long)wireLink.stats.crcErrors,(unsigned long)wireLink.stats.retries,(unsigned long)wireLink.stats.timeouts,(unsigned long)wireLink.stats.malformed,(unsigned long)wireLink.stats.schemaErrors,(unsigned long)serialTxDroppedCommands);
  usbDiagnosticLine(line);
  } else if ((now/5000UL)%5==2) {
  snprintf_P(line,sizeof(line),PSTR("CLOCK utc=%lu sync=%u ntp=%lu age=%lu s rtc=%u phase=%u status=0x%02x SDA=%u SCL=%u"),
    (unsigned long)::now(),clockSyncStatus,(unsigned long)clockLastNtpTime,
    (unsigned long)(clockLastNtpTime ? (millis()-clockLastSynchro)/1000UL : 0),
    clockRtcStatus,rtcFaultState,rtcFaultStatus,digitalRead(20),digitalRead(21));
  usbDiagnosticLine(line);
  } else if ((now/5000UL)%5==3) {
  snprintf_P(line,sizeof(line),PSTR("DHT tries=%lu ok=%lu valid=%u fail=%u phase=%u bits=%u gap=%u lowMs=%u lowPin=%u raw=%02x,%02x,%02x,%02x,%02x pin=%u"),
    (unsigned long)dhtAttempts,(unsigned long)dhtSuccesses,dhtValid,dhtFailures,dhtCaptureEndState,dhtCaptureBits,dhtCaptureMaxGap,dhtStartPulseMillis,dhtStartPinLevel,
    dhtCaptureBytes[0],dhtCaptureBytes[1],dhtCaptureBytes[2],dhtCaptureBytes[3],dhtCaptureBytes[4],digitalRead(DHT_PIN));
  usbDiagnosticLine(line);
  } else if ((now/5000UL)%5==0) {
#ifdef __AVR__
  snprintf_P(line,sizeof(line),PSTR("RAM gap=%d minGap=%d"),runtimeMemorySample(),runtimeMinimumGap);
  usbDiagnosticLine(line);
#endif

  snprintf_P(line,sizeof(line),PSTR("LED fade=%u duration=%lu ms gapMax=%lu ms steps=%lu RGB=%u,%u,%u"),ledFadeActive,(unsigned long)ledFadeDuration,(unsigned long)ledFadeMaxGap,(unsigned long)ledFadeRenderCount,(unsigned int)ledActualRed,(unsigned int)ledActualGreen,(unsigned int)ledActualBlue);
  usbDiagnosticLine(line);ledFadeMaxGap=ledFadeRenderCount=0;
  } else {
  snprintf_P(line,sizeof(line),PSTR("WORK us relay=%lu pwm=%lu clock=%lu lcd=%lu fan=%lu dht=%lu key=%lu critical=%lu snap=%lu"),(unsigned long)usbWorkMax[0],(unsigned long)usbWorkMax[1],(unsigned long)usbWorkMax[2],(unsigned long)usbWorkMax[3],(unsigned long)usbWorkMax[4],(unsigned long)usbWorkMax[5],(unsigned long)usbWorkMax[6],(unsigned long)usbWorkMax[7],(unsigned long)usbWorkMax[8]);
  usbDiagnosticLine(line);memset(usbWorkMax,0,sizeof(usbWorkMax));
  }
 }
 uint8_t budget=16;
 while(budget--&&usbDiagnosticTail!=usbDiagnosticHead&&Serial.availableForWrite()>0)Serial.write(uint8_t(usbDiagnosticQueue[usbDiagnosticTail++]));
}
