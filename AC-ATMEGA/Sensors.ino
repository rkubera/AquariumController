/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/
 
#include "CooperativeAdc.h"

class sensorClass {
  public:
  sensorClass():valid(false),validityPublished(false),lastPublishedValid(false){}

  byte sensorType= SENSOR_TYPE_NONE;
  
  float calibValue1 = 0;
  float calibRawRead1 = 0;

  float calibValue2 = 1;
  float calibRawRead2 = 1023;

  float minValue = -FLT_MAX;
  float maxValue = FLT_MAX;
  
  float criticalMinValue = -FLT_MAX;
  float criticalMaxValue = FLT_MAX;
  
  float Value = NAN;
  uint8_t valid:1,validityPublished:1,lastPublishedValid:1;
  byte readingCount = 0;
  unsigned long lastReadingMillis = 0;
  float rawValue = NAN;
  
  byte sensorPin = 0;
  byte analogPin = 0;
  
  byte numReadings = SENSOR_FILTER_WINDOW;
  uint16_t rawReadings[SENSOR_FILTER_WINDOW];
  
  float lastRawReading = NAN;
  float lastValue = NAN;


  unsigned long lastValueSendMillis = 0;

  void Init(byte pin) {
    switch (pin) {
      case 0: analogPin = ANALOG_IN_PIN_0;
            break;
      case 1: analogPin = ANALOG_IN_PIN_1;
            break;
      case 2: analogPin = ANALOG_IN_PIN_2;
            break;
      case 3: analogPin = ANALOG_IN_PIN_3;
            break;
      case 4: analogPin = ANALOG_IN_PIN_4;
            break;
      case 5: analogPin = ANALOG_IN_PIN_5;
            break;
      case 6: analogPin = ANALOG_IN_PIN_6;
            break;
      case 7: analogPin = ANALOG_IN_PIN_7;
            break;
    }
    pinMode(analogInputToDigitalPin(analogPin), INPUT);
    sensorPin = pin+1;
  
    resetReadings();
    for (int i=0; i<SENSOR_FILTER_WINDOW; i++) {
      rawReadings[i] = -1;
    }
  }

  void resetReadings() {
    readingCount = 0;
    valid = false;
    Value = NAN;
    rawValue = NAN;
    publishValidity();
  }

  void publishValidity() {
    if(sensorPin==0||uint32_t(millis()-lastValueSendMillis)<SENSOR_PUBLISH_INTERVAL_MS)return;
    bool current=isValid();
    if(!current){
      if(!validityPublished||lastPublishedValid){
        reportField(sensorField(sensorPin,SensorField::Value),"invalid");
        reportField(sensorField(sensorPin,SensorField::RawValue),"invalid");
        lastValue=lastRawReading=NAN;lastValueSendMillis=millis();
      }
    }else{
      bool changed=false;
      if(lastRawReading!=rawValue){lastRawReading=rawValue;reportField(sensorField(sensorPin,SensorField::RawValue),floatToString(rawValue));changed=true;}
      if(lastValue!=Value){lastValue=Value;reportField(sensorField(sensorPin,SensorField::Value),floatToString(Value));changed=true;}
      if(changed)lastValueSendMillis=millis();
    }
    lastPublishedValid=current;validityPublished=true;
  }

  bool isValid() {
    return valid && millis() - lastReadingMillis <= (AcPeripheral::isExternal(peripheralInputDevice(sensorPin)) ? 10000UL : SENSOR_MAX_AGE_MILLIS);
  }

  void finishReading(uint16_t rawSum, float scale = SENSOR_ADC_SAMPLES, bool signedSample = false) {
    if (!isfinite(calibRawRead1) || !isfinite(calibRawRead2) ||
        !isfinite(calibValue1) || !isfinite(calibValue2) ||
        calibRawRead1 == calibRawRead2) {
      resetReadings();
      return;
    }
    float a = (calibValue2-calibValue1)/(calibRawRead2-calibRawRead1);
    float b = calibValue1-a*calibRawRead1;
    float rawReading = (signedSample ? int16_t(rawSum) : float(rawSum))/scale;
    float myValue = a*rawReading+b;
    //Check the current sample, before smoothing can conceal a critical value.
    if (!isfinite(myValue) || myValue < minValue || myValue > maxValue ||
        myValue < criticalMinValue || myValue > criticalMaxValue) {
      resetReadings();
      return;
    }
    byte window = AcPeripheral::isBinary(peripheralInputDevice(sensorPin)) ? 1 : constrain(numReadings, 1, SENSOR_FILTER_WINDOW);
    if (readingCount > window) readingCount = window;
    for (int i=window-1; i>0; i--) {
      rawReadings[i] = rawReadings[i-1];
    }
    rawReadings[0] = rawSum;
    if (readingCount < window) readingCount++;
    float rawValueSum = 0;
    for (byte i=0; i<readingCount; i++) {
      rawValueSum += signedSample ? int16_t(rawReadings[i]) : float(rawReadings[i]);
    }
    rawValue = rawValueSum/(scale*(float)readingCount);
    //Calibration is affine: calibrating the raw average equals averaging
    //calibrated samples, without a second 20-float buffer per input.
    Value = a*rawValue+b;
    if (!isfinite(Value)) { resetReadings(); return; }
    valid = true;
    lastReadingMillis = millis();
    publishValidity();
  }

  void loadConfig(int CONFIG_addr) {
    sensorType = configGetValue(CONFIG_addr+NAME_LENGTH+1);
    calibValue1 = configGetFloatValue(CONFIG_addr+NAME_LENGTH+2);
    calibRawRead1 = configGetFloatValue(CONFIG_addr+NAME_LENGTH+6);
    calibValue2 = configGetFloatValue(CONFIG_addr+NAME_LENGTH+10);
    calibRawRead2 = configGetFloatValue(CONFIG_addr+NAME_LENGTH+14);
    if (sensorType >= RELAYS_COUNT+PWMOUTPUTS_COUNT) sensorType = SENSOR_TYPE_NONE;
    if (!isfinite(calibValue1) || !isfinite(calibValue2) ||
        !isfinite(calibRawRead1) || !isfinite(calibRawRead2) ||
        calibRawRead1 < -55 || calibRawRead1 > 1023 ||
        calibRawRead2 < -55 || calibRawRead2 > 1023 || calibRawRead1 == calibRawRead2) {
      sensorType = SENSOR_TYPE_NONE;
      calibValue1 = 0; calibRawRead1 = 0;
      calibValue2 = 1; calibRawRead2 = 1023;
    }
    int limitsAddr = CONFIG_sensor_limits_addr +
      ((CONFIG_addr-CONFIG_sensors_addr)/SENSORS_SENSOR_CONFIG_BYTES)*SENSOR_LIMITS_CONFIG_BYTES;
    minValue = configGetFloatValue(limitsAddr);
    maxValue = configGetFloatValue(limitsAddr+4);
    criticalMinValue = configGetFloatValue(limitsAddr+8);
    criticalMaxValue = configGetFloatValue(limitsAddr+12);
    if (!isfinite(minValue) || !isfinite(maxValue) || minValue > maxValue) {
      minValue = -FLT_MAX; maxValue = FLT_MAX;
    }
    if (!isfinite(criticalMinValue) || !isfinite(criticalMaxValue) || criticalMinValue > criticalMaxValue) {
      criticalMinValue = -FLT_MAX; criticalMaxValue = FLT_MAX;
    }
    resetReadings();
  }

  void saveConfig(int CONFIG_addr) {
    configStorage.update(CONFIG_addr+NAME_LENGTH+1,sensorType);
    configStorage.put(CONFIG_addr+NAME_LENGTH+2,calibValue1);
    configStorage.put(CONFIG_addr+NAME_LENGTH+6,calibRawRead1);
    configStorage.put(CONFIG_addr+NAME_LENGTH+10,calibValue2);
    configStorage.put(CONFIG_addr+NAME_LENGTH+14,calibRawRead2);
    int limitsAddr = CONFIG_sensor_limits_addr +
      ((CONFIG_addr-CONFIG_sensors_addr)/SENSORS_SENSOR_CONFIG_BYTES)*SENSOR_LIMITS_CONFIG_BYTES;
    configStorage.put(limitsAddr,minValue);
    configStorage.put(limitsAddr+4,maxValue);
    configStorage.put(limitsAddr+8,criticalMinValue);
    configStorage.put(limitsAddr+12,criticalMaxValue);
  }

  void publishAll() {
    String sType = sensorsGetStringSensorType(sensorType);
    peripheralsReportSettings(sensorPin);peripheralsReportFunction(sensorPin);
    reportField(sensorField(sensorPin,SensorField::Device),peripheralDeviceName(peripheralInputDevice(sensorPin)));
    reportField(sensorField(sensorPin,SensorField::Measurement),peripheralInputMeasurement(sensorPin)?"humidity":"temperature");
    reportField(sensorField(sensorPin,SensorField::Humidity),peripheralHumidityString(sensorPin));
    reportField(sensorField(sensorPin,SensorField::MinValue),sensorsLimitToString(minValue));
    reportField(sensorField(sensorPin,SensorField::MaxValue),sensorsLimitToString(maxValue));
    reportField(sensorField(sensorPin,SensorField::CriticalMinValue),sensorsLimitToString(criticalMinValue));
    reportField(sensorField(sensorPin,SensorField::CriticalMaxValue),sensorsLimitToString(criticalMaxValue));

    reportField(sensorField(sensorPin,SensorField::Name),readConfigName(CONFIG_sensors_addr+((sensorPin-1)*SENSORS_SENSOR_CONFIG_BYTES),NAME_LENGTH));
    reportField(sensorField(sensorPin,SensorField::Value),isValid() ? floatToString(Value) : "invalid");
    reportField(sensorField(sensorPin,SensorField::RawValue),isValid() ? floatToString(rawValue) : "invalid");
    reportField(sensorField(sensorPin,SensorField::Type),sType);
    reportField(sensorField(sensorPin,SensorField::CalibValue1),floatToString(calibValue1));
    reportField(sensorField(sensorPin,SensorField::CalibRawValue1),floatToString(calibRawRead1));
    reportField(sensorField(sensorPin,SensorField::CalibValue2),floatToString(calibValue2));
    reportField(sensorField(sensorPin,SensorField::CalibRawValue2),floatToString(calibRawRead2)); 
  }
};

static sensorClass mySensors[SENSORS_COUNT];

void sensorsInit() {
  for (byte i=0; i<SENSORS_COUNT; i++) {
    mySensors[i].Init(i);
  }
}

void sensorsReportAll() {
  for (byte i=0; i<SENSORS_COUNT; i++) {
    mySensors[i].publishAll();
  }
}

byte sensorsOutputTarget(byte number,byte type){
 if(number==0 || (type!=OUTPUT_TYPE_RELAY&&type!=OUTPUT_TYPE_PWM) || number>(type==OUTPUT_TYPE_RELAY?RELAYS_COUNT:PWMOUTPUTS_COUNT))return SENSOR_TYPE_NONE;
 return number-1+(type==OUTPUT_TYPE_PWM?RELAYS_COUNT:0);
}
bool sensorsHasOutputInput(byte number,byte type){
 byte target=sensorsOutputTarget(number,type);
 if(target==SENSOR_TYPE_NONE)return false;
 for(byte i=0;i<SENSORS_COUNT;i++)if(mySensors[i].sensorType==target)return true;
 return false;
}
byte sensorsGetOutputInput(byte number,byte type){
 byte target=sensorsOutputTarget(number,type),found=0;
 if(target==SENSOR_TYPE_NONE)return 0;
 for(byte i=0;i<SENSORS_COUNT;i++)if(mySensors[i].sensorType==target){if(found)return 0;found=i+1;}
 return found;
}
String sensorsOutputInputString(byte number,byte type){
 byte input=sensorsGetOutputInput(number,type);return input ? String("sensor")+String(input) : String("none");
}
float sensorsGetSensorsValue(byte number,byte type){
 byte input=sensorsGetOutputInput(number,type);
 if(!input || !mySensors[input-1].isValid())return NAN;
 return mySensors[input-1].Value;
}
void sensorsReportOutputInput(byte target){
 if(target<RELAYS_COUNT)reportField(relayField(target+1,RelayField::Input),sensorsOutputInputString(target+1,OUTPUT_TYPE_RELAY));
 else if(target<RELAYS_COUNT+PWMOUTPUTS_COUNT)reportField(pwmField(target-RELAYS_COUNT+1,PWMOutputField::Input),sensorsOutputInputString(target-RELAYS_COUNT+1,OUTPUT_TYPE_PWM));
}
void sensorsOutputInputChanged(byte target){
 if(target<RELAYS_COUNT)relaysInputChanged(target+1);
 else if(target<RELAYS_COUNT+PWMOUTPUTS_COUNT)pwmOutputsInputChanged(target-RELAYS_COUNT+1);
 sensorsReportOutputInput(target);
}
void sensorsAssignInput(byte input,byte target){
 if(!input || input>megaFeatures.inputs || (target!=SENSOR_TYPE_NONE && (target<RELAYS_COUNT?target>=megaFeatures.relays:target-RELAYS_COUNT>=megaFeatures.pwm)))return;
 if(target!=SENSOR_TYPE_NONE && peripheralInputDevice(input)==AcPeripheral::Disabled)return;
 bool affected[RELAYS_COUNT+PWMOUTPUTS_COUNT]={false};
 for(byte i=0;i<SENSORS_COUNT;i++){
  byte old=mySensors[i].sensorType;
  byte next=i==input-1?target:(target!=SENSOR_TYPE_NONE && old==target?SENSOR_TYPE_NONE:old);
  if(old==next)continue;
  if(old<RELAYS_COUNT+PWMOUTPUTS_COUNT)affected[old]=true;
  if(next<RELAYS_COUNT+PWMOUTPUTS_COUNT)affected[next]=true;
  mySensors[i].sensorType=next;mySensors[i].resetReadings();sensorsSaveConfig(i);
  reportField(sensorField(i+1,SensorField::Type),sensorsGetStringSensorType(next));
 }
 for(byte i=0;i<RELAYS_COUNT+PWMOUTPUTS_COUNT;i++)if(affected[i])sensorsOutputInputChanged(i);
}
void sensorsSetOutputInput(byte number,byte type,byte input){
 byte target=sensorsOutputTarget(number,type);if(target==SENSOR_TYPE_NONE||input>megaFeatures.inputs||(type==OUTPUT_TYPE_RELAY&&number>megaFeatures.relays)||(type==OUTPUT_TYPE_PWM&&number>megaFeatures.pwm))return;
 if(input){sensorsAssignInput(input,target);return;}
 bool changed=false;
 for(byte i=0;i<SENSORS_COUNT;i++)if(mySensors[i].sensorType==target){
  mySensors[i].sensorType=SENSOR_TYPE_NONE;mySensors[i].resetReadings();sensorsSaveConfig(i);changed=true;
  reportField(sensorField(i+1,SensorField::Type),sensorsGetStringSensorType(SENSOR_TYPE_NONE));
 }
 if(changed)sensorsOutputInputChanged(target);
}

byte sensorsGetByteSensorType (String sensorType) {
  byte i;
  if (sensorType==setBufferFromFlash(charSensornone)) {
    return SENSOR_TYPE_NONE;
  }
  String strTemp;
  for (i=0; i<RELAYS_COUNT; i++) {
    strTemp = setBufferFromFlash(charRelay)+intToString(i+1);
    if (sensorType==strTemp) {
      return(i);
    }
  }
  for (i=RELAYS_COUNT; i<PWMOUTPUTS_COUNT+RELAYS_COUNT; i++) {
    strTemp = setBufferFromFlash(charPWMOutput)+intToString(i+1-RELAYS_COUNT);
    if (sensorType==strTemp) {
      return(i);
    }
  }
  return 255;
}

String sensorsGetStringSensorType (byte sensorType) {
  byte i;
  if (sensorType==SENSOR_TYPE_NONE) {
    return(setBufferFromFlash(charSensornone));
  }
  for (i=0; i<RELAYS_COUNT; i++) {
    if (sensorType==i) {
      return(setBufferFromFlash(charRelay)+intToString(i+1));
    }
  }
  for (i=RELAYS_COUNT; i<PWMOUTPUTS_COUNT+RELAYS_COUNT; i++) {
    if (sensorType==i) {
      return(setBufferFromFlash(charPWMOutput)+intToString(i+1-RELAYS_COUNT));
    }
  }
  return(setBufferFromFlash(charSensornone));
}

void sensorsLoadConfig() {
  for (byte sensorNumber=0; sensorNumber<SENSORS_COUNT; sensorNumber++) {
    mySensors[sensorNumber].loadConfig(CONFIG_sensors_addr+(sensorNumber*SENSORS_SENSOR_CONFIG_BYTES));
  }
}

void sensorsSaveConfig(byte sensorNumber) {
  mySensors[sensorNumber].saveConfig(CONFIG_sensors_addr+(sensorNumber*SENSORS_SENSOR_CONFIG_BYTES));
}

void sensorsSaveSensorName(byte sensorNr, String Value) {
  byte sensorNumber= sensorNr-1;
  configSaveString(Value,CONFIG_sensors_addr+(sensorNumber*SENSORS_SENSOR_CONFIG_BYTES),NAME_LENGTH);
}

void sensorsSetSensor(byte sensorNr, byte valueType, float Value) {
  if (sensorNr == 0 || sensorNr > SENSORS_COUNT) return;
  if (!isfinite(Value)) return;
  byte sensorNumber= sensorNr-1;
  switch (valueType) {
      case SENSORS_VALUE_TYPE:
        if (Value != SENSOR_TYPE_NONE && (Value < 0 || Value >= RELAYS_COUNT+PWMOUTPUTS_COUNT)) return;
        if(Value!=(byte)Value)return;
        sensorsAssignInput(sensorNr,(byte)Value);
        return;
      case SENSORS_VALUE_CALIB_VALUE1:
        mySensors[sensorNumber].calibValue1 = Value;
        break;
      case SENSORS_VALUE_CALIB_RAW_VALUE1:
        if (Value < (AcPeripheral::isDigital(peripheralInputDevice(sensorNr)) ? -55 : 0) || Value > 1023) return;
        mySensors[sensorNumber].calibRawRead1 = Value;
        break;
      case SENSORS_VALUE_CALIB_VALUE2:
        mySensors[sensorNumber].calibValue2 = Value;
        break;
      case SENSORS_VALUE_CALIB_RAW_VALUE2:
        if (Value < (AcPeripheral::isDigital(peripheralInputDevice(sensorNr)) ? -55 : 0) || Value > 1023) return;
        mySensors[sensorNumber].calibRawRead2 = Value;
        break;
      case SENSORS_VALUE_MIN:
        if (Value > mySensors[sensorNumber].maxValue) return;
        mySensors[sensorNumber].minValue = Value; break;
      case SENSORS_VALUE_MAX:
        if (Value < mySensors[sensorNumber].minValue) return;
        mySensors[sensorNumber].maxValue = Value; break;
      case SENSORS_VALUE_CRITICAL_MIN:
        if (Value > mySensors[sensorNumber].criticalMaxValue) return;
        mySensors[sensorNumber].criticalMinValue = Value; break;
      case SENSORS_VALUE_CRITICAL_MAX:
        if (Value < mySensors[sensorNumber].criticalMinValue) return;
        mySensors[sensorNumber].criticalMaxValue = Value; break;
      default: return;
  }
  mySensors[sensorNumber].resetReadings();
  sensorsSaveConfig(sensorNumber);
}

float sensorsGetSensor(byte sensorNr, byte valueType) {
  if (sensorNr == 0 || sensorNr > SENSORS_COUNT) return NAN;
  byte sensorNumber= sensorNr-1;
  switch (valueType) {
      case SENSORS_VALUE_MIN: return mySensors[sensorNumber].minValue;
      case SENSORS_VALUE_MAX: return mySensors[sensorNumber].maxValue;
      case SENSORS_VALUE_CRITICAL_MIN: return mySensors[sensorNumber].criticalMinValue;
      case SENSORS_VALUE_CRITICAL_MAX: return mySensors[sensorNumber].criticalMaxValue;
      case SENSORS_VALUE_VALID: return mySensors[sensorNumber].isValid();
      case SENSORS_VALUE:
        return (mySensors[sensorNumber].Value);
        break;
      case SENSORS_VALUE_RAW:
        return (mySensors[sensorNumber].rawValue);
        break;
      case SENSORS_VALUE_TYPE:
        return (mySensors[sensorNumber].sensorType);
        break;
      case SENSORS_VALUE_CALIB_VALUE1:
        return (mySensors[sensorNumber].calibValue1);
        break;
      case SENSORS_VALUE_CALIB_RAW_VALUE1:
        return (mySensors[sensorNumber].calibRawRead1);
        break;
      case SENSORS_VALUE_CALIB_VALUE2:
        return (mySensors[sensorNumber].calibValue2);
        break;
      case SENSORS_VALUE_CALIB_RAW_VALUE2:
        return (mySensors[sensorNumber].calibRawRead2);
        break;
  }
  return 0;
}

static CooperativeAdc sensorAdc;
static uint32_t sensorCycleAt=0;
static uint16_t sensorBatchSum=0;
static byte sensorAcquisitionChannel=0,sensorBatchSamples=0;
static bool sensorSweepActive=false,sensorConversionPending=false,sensorDiscardSample=true;

void sensorsResetAcquisition() {
  sensorAdc.reset();sensorSweepActive=sensorConversionPending=false;
  sensorAcquisitionChannel=sensorBatchSamples=0;sensorBatchSum=0;sensorDiscardSample=true;
  sensorCycleAt=millis()-ANALOG_CONTROL_INTERVAL_MS;
  for(byte i=0;i<SENSORS_COUNT;++i)mySensors[i].resetReadings();
}

void sensorsMillisEvent() {
  if(!megaConfigReady){if(sensorSweepActive||sensorConversionPending)sensorsResetAcquisition();return;}
  uint32_t now=millis();
  for(byte i=0;i<megaFeatures.inputs;++i)mySensors[i].publishValidity();
  if(!sensorSweepActive){
    if(uint32_t(now-sensorCycleAt)<ANALOG_CONTROL_INTERVAL_MS)return;
    sensorCycleAt=now;sensorSweepActive=true;sensorAcquisitionChannel=0;
    sensorBatchSum=0;sensorBatchSamples=0;sensorDiscardSample=true;
  }
  if(sensorAcquisitionChannel>=megaFeatures.inputs){sensorSweepActive=false;return;}
  sensorClass& sensor=mySensors[sensorAcquisitionChannel];
  if(!AcPeripheral::isAnalog(peripheralInputDevice(sensorAcquisitionChannel+1)) || ((sensor.sensorType==SENSOR_TYPE_NONE||sensor.sensorType==255)&&peripheralInputDevice(sensorAcquisitionChannel+1)!=AcPeripheral::Ph)){
    if(sensorConversionPending&&!sensorAdc.ready())return;
    if(!AcPeripheral::isExternal(peripheralInputDevice(sensorAcquisitionChannel+1)))sensor.resetReadings();sensorConversionPending=false;
    ++sensorAcquisitionChannel;sensorBatchSamples=0;sensorBatchSum=0;sensorDiscardSample=true;return;
  }
  if(!sensorConversionPending){sensorAdc.start(sensor.analogPin);sensorConversionPending=true;return;}
  if(!sensorAdc.ready())return;
  uint16_t sample=sensorAdc.read();sensorConversionPending=false;
  // Discard the first conversion after switching channels to reduce crosstalk.
  if(sensorDiscardSample)sensorDiscardSample=false;
  else {sensorBatchSum+=sample;++sensorBatchSamples;}
  if(sensorBatchSamples>=SENSOR_ADC_SAMPLES){
    sensor.finishReading(sensorBatchSum);++sensorAcquisitionChannel;
    sensorBatchSamples=0;sensorBatchSum=0;sensorDiscardSample=true;
  }else{sensorAdc.start(sensor.analogPin);sensorConversionPending=true;}
}

String sensorsLimitToString(float value) {
  return value == FLT_MAX || value == -FLT_MAX ? "off" : floatToString(value);
}

//Spread complete snapshots across channels to keep acquisition and regulation running.
void sensorsReportChannel(byte channel) {
  if (channel < SENSORS_COUNT) mySensors[channel].publishAll();
}

// Digital devices enter the same calibrated, filtered and range-checked path.
byte sensorsPeripheralPin(byte channel){return channel&&channel<=SENSORS_COUNT?analogInputToDigitalPin(mySensors[channel-1].analogPin):255;}
void sensorsDigitalReading(byte channel,float value,byte scale){
 if(!channel||channel>megaFeatures.inputs)return;
 if(!scale||!isfinite(value)||value*scale < -32768.0f||value*scale > 32767.0f){mySensors[channel-1].resetReadings();return;}
 mySensors[channel-1].finishReading(uint16_t(int16_t(round(value*scale))),scale,true);
}
void sensorsDeviceChanged(byte channel,bool resetCalibration){
 if(!channel||channel>SENSORS_COUNT)return;
 sensorClass& s=mySensors[channel-1];s.resetReadings();
 if(resetCalibration){
  s.calibValue1=s.calibRawRead1=0;s.calibValue2=1;
  s.calibRawRead2=AcPeripheral::isExternal(peripheralInputDevice(channel))?1:1023;
  if(peripheralInputDevice(channel)==AcPeripheral::Ph)s.calibValue2=14;
  sensorsSaveConfig(channel-1);
  reportField(sensorField(channel,SensorField::CalibValue1),floatToString(s.calibValue1));
  reportField(sensorField(channel,SensorField::CalibValue2),floatToString(s.calibValue2));
  reportField(sensorField(channel,SensorField::CalibRawValue1),floatToString(s.calibRawRead1));
  reportField(sensorField(channel,SensorField::CalibRawValue2),floatToString(s.calibRawRead2));
 }
}

bool sensorsPeripheralAlarm(byte ch){
 if(!ch||ch>megaFeatures.inputs)return false;
 byte type=peripheralInputDevice(ch);sensorClass& s=mySensors[ch-1];
 if(!s.isValid())return false;
 return (type==AcPeripheral::Level&&s.rawValue==0)||(type==AcPeripheral::Leak&&s.rawValue==1);
}
