#include "DhtMedian.h"
/**************************************************
 * AquaController by Radek Kubera (rkubera)        *
 * free of charge for non-commercial use only     *
 **************************************************/

//Short interrupt sampling only while a response is expected. No delay(),
//pulseIn(), or busy wait in the control loop.
volatile byte dhtCaptureState=0,dhtCaptureTicks=0,dhtCaptureBits=0;
volatile byte dhtCaptureBytes[5];
volatile bool dhtCaptureReady=false,dhtCaptureOk=false;
static byte dhtRequestState=0;
volatile bool dhtStartReleased=false;
static unsigned long dhtRequestMillis=0,dhtLastRequest=0,dhtLastReinit=0;
static byte dhtFailures=0;
volatile uint16_t dhtCaptureElapsed=0;
volatile uint16_t dhtCaptureMaxGap=0;
volatile byte dhtCaptureEndState=0;
static uint32_t dhtAttempts=0,dhtSuccesses=0;
static uint16_t dhtStartPulseMillis=0;
static byte dhtStartPinLevel=1;

void dhtCaptureBegin() {
  for (byte i=0;i<5;i++) dhtCaptureBytes[i]=0;
  dhtCaptureBits=0; dhtCaptureTicks=0; dhtCaptureElapsed=0; dhtCaptureReady=false; dhtCaptureOk=false;
  dhtCaptureMaxGap=0; dhtCaptureEndState=0;
  dhtCaptureState=1;
}

void dhtCaptureFinish(bool ok) {
  dhtCaptureEndState=dhtCaptureState;
  dhtCaptureState=0; dhtCaptureOk=ok;
  dhtTimerStop(); dhtCaptureReady=true;
}

void dhtCaptureTickElapsed(bool high,uint16_t elapsedTicks) {
  if (!dhtCaptureState) return;
  if (elapsedTicks>dhtCaptureMaxGap) dhtCaptureMaxGap=elapsedTicks;
  dhtCaptureElapsed+=elapsedTicks;
  if (++dhtCaptureTicks>20 || dhtCaptureElapsed>400) { dhtCaptureFinish(false); return; }
  switch (dhtCaptureState) {
    case 1: if (!high) { dhtCaptureState=2; dhtCaptureTicks=0; dhtCaptureElapsed=0; } break;
    case 2: if (high) { dhtCaptureState=3; dhtCaptureTicks=0; dhtCaptureElapsed=0; } break;
    case 3: if (!high) { dhtCaptureState=4; dhtCaptureTicks=0; dhtCaptureElapsed=0; } break;
    case 4: if (high) { dhtCaptureState=5; dhtCaptureTicks=0; dhtCaptureElapsed=0; } break;
    case 5:
      if (!high) {
        byte index=dhtCaptureBits/8;
        dhtCaptureBytes[index]=(dhtCaptureBytes[index]<<1) | (dhtCaptureElapsed>=100 ? 1 : 0);
        if (++dhtCaptureBits==40) dhtCaptureFinish(true);
        else { dhtCaptureState=4; dhtCaptureTicks=0; dhtCaptureElapsed=0; }
      }
      break;
  }
}

void dhtCaptureTick(bool high) { dhtCaptureTickElapsed(high,20); }

void dhtReleaseStartPulse() {
  dhtStartPulseMillis=18;
  dhtStartPinLevel=digitalRead(DHT_PIN);
  dhtCaptureBegin();
  pinMode(DHT_PIN,INPUT_PULLUP);
  dhtTimerStart();
  dhtStartReleased=true;
}

void dhtInit() {
#ifdef __AVR__
  dhtTimerStop();
#endif
  dhtTimerStop(); dhtCaptureState=0;
  dhtCaptureReady=false; dhtCaptureOk=false;
  dhtCaptureTicks=0; dhtCaptureElapsed=0; dhtCaptureBits=0;
  dhtTemperature=NAN; dhtHumidity=NAN; dhtFailures=0; dhtLastReinit=millis();
  dhtValid=false; dhtReadingCount=0;
  dhtRequestState=0; dhtLastRequest=millis();
  pinMode(DHT_PIN,INPUT_PULLUP);
}

bool dhtHasValidReading() {
  return dhtValid && millis()-dhtLastReadingMillis <= 10000UL;
}

void dhtReportAll() {
  bool valid = dhtHasValidReading();
  reportField(FieldId::MaxIntTemp,intToString(maxInternalTemperature));
  reportField(FieldId::InternalTemperature,valid ? floatToString(dhtTemperature) : "invalid");
  reportField(FieldId::InternalHumidity,valid ? floatToString(dhtHumidity) : "invalid");
}

void dhtGetDataMillisEvent() {
  if(!megaFeatures.has(ControllerFeatures::Dht))return;
  static unsigned long lastPublish=0;
  static bool measurementsPublished=false;
  if (dhtRequestState==0) {
    //Release the data line and restart the receiver after persistent failures.
    //A soft reinit cannot power-cycle a sensor wired directly to 5V.
    if (dhtFailures>=5 && millis()-dhtLastReinit>=30000UL) { dhtInit(); return; }
    if (millis()-dhtLastRequest<2000UL) return;
    ++dhtAttempts;
    dhtLastRequest=millis(); dhtRequestMillis=millis();
    pinMode(DHT_PIN,OUTPUT); digitalWrite(DHT_PIN,LOW);
    dhtStartReleased=false;
#ifdef __AVR__
    dhtStartPulse();
#endif
    dhtRequestState=1; return;
  }
  if (dhtRequestState==1) {
#ifdef __AVR__
    if (!dhtStartReleased) return;
#else
    if (millis()-dhtRequestMillis<18UL) return;
    dhtReleaseStartPulse();
#endif
    dhtRequestMillis=millis(); dhtRequestState=2; return;
  }
  if (!dhtCaptureReady && millis()-dhtRequestMillis<10UL) return;
  dhtTimerStop(); dhtCaptureState=0;
  dhtRequestState=0;
  byte data[5];
  for (byte i=0;i<5;i++) data[i]=dhtCaptureBytes[i];
  float temperature=data[2]+data[3]*0.1f;
  float humidity=data[0]+data[1]*0.1f;
  dhtValid=dhtCaptureReady && dhtCaptureOk &&
    (byte)(data[0]+data[1]+data[2]+data[3])==data[4] &&
    (data[0] || data[1] || data[2] || data[3]) && data[1]<=9 && data[3]<=9 &&
    temperature<=50 && humidity<=100;
  if (dhtValid) {
    ++dhtSuccesses;
    dhtFailures=0;
    byte window = constrain(dhtNumReadings,1,20);
    for (int i=window-1; i>0; i--) {
      dhtTemparatureReadings[i] = dhtTemparatureReadings[i-1];
      dhtHumidityReadings[i] = dhtHumidityReadings[i-1];
    }
    dhtTemparatureReadings[0] = uint16_t(data[2])*10U+data[3];
    dhtHumidityReadings[0] = uint16_t(data[0])*10U+data[1];
    if (dhtReadingCount < window) dhtReadingCount++;
    if (dhtReadingCount > window) dhtReadingCount = window;
    dhtTemperature = dhtMedianDeci(dhtTemparatureReadings,dhtReadingCount);
    dhtHumidity = dhtMedianDeci(dhtHumidityReadings,dhtReadingCount);
    dhtLastReadingMillis = millis();
  }
  else {
    if (dhtFailures<255) dhtFailures++;
    dhtReadingCount = 0;
    dhtTemperature = NAN;
    dhtHumidity = NAN;
  }
  bool valid = dhtHasValidReading();
  if (!measurementsPublished || millis()-lastPublish>=60000UL) {
    measurementsPublished=true; lastPublish=millis();
    reportField(FieldId::InternalTemperature,valid ? floatToString(dhtTemperature) : "invalid");
    reportField(FieldId::InternalHumidity,valid ? floatToString(dhtHumidity) : "invalid");
  }
}

void dhtDisable(){dhtTimerStop();dhtCaptureState=0;dhtRequestState=0;dhtCaptureReady=false;dhtCaptureOk=false;dhtValid=false;dhtTemperature=NAN;dhtHumidity=NAN;pinMode(DHT_PIN,INPUT_PULLUP);}
