/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/
 
class sensorClass {
  public:

  byte sensorType= SENSOR_TYPE_NONE;
  
  float calibValue1 = 0;
  float calibRawRead1 = 0;

  float calibValue2 = 1;
  float calibRawRead2 = 1023;

  float minValue = 0;
  float maxValue = 1023;
  
  float criticalMinValue = 0;
  float criticalMaxValue = 1023;
  
  float Value;
  float rawValue;
  
  byte sensorPin;
  byte analogPin;
  
  byte numReadings = 20;
  float readings[20];
  float rawReadings[20];
  
  int lastRawReading;
  float lastValue;

  byte precision = 10;

  double lastValueSendMillis;

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
    pinMode(analogPin, INPUT);
    sensorPin = pin+1;
  
    for (int i=0; i<numReadings; i++) {
      readings[i] = -1;
      rawReadings[i] = -1;
    }
  }

  void secondEvent() {
    float a,b;
    if (calibRawRead2!=calibRawRead1) {
      a = (calibValue2-calibValue1)/(calibRawRead2-calibRawRead1);
    }
    else {
      a = 0;
    }
    b = calibValue2-(a*calibRawRead2);
    
    float analog;
    float myValue;
    float ValuesSum;
    long ValuesRawSum;
  
    int count = 0;
    long rawReading = 0;
    for (int i=0; i<300; i++) {
      analog = analogRead(analogPin);
      myValue = (a*analog)+b;
      ValuesSum = ValuesSum+myValue;
      rawReading = rawReading+analog;
      count++;
    }
    myValue = ValuesSum/count;
    rawReading = (rawReading/count);
    
    ValuesSum = myValue;
    ValuesRawSum = rawReading;

    int realNumReadings = 0;
    for (int i=numReadings-1; i>=1; i--) {
      if (readings[i-1]>0) {
        readings[i] = readings[i-1];
        rawReadings[i] = rawReadings[i-1];
        ValuesSum = ValuesSum+readings[i];
        ValuesRawSum = ValuesRawSum+rawReadings[i];
        realNumReadings++;
      }
    }
  
    readings[0] = myValue;
    rawReadings[0] = rawReading;

    if (realNumReadings>0) {
      rawReading = ValuesRawSum/realNumReadings;
      Value = ValuesSum/realNumReadings;
      Value = (float)round(Value*precision)/precision;
    }
    else {
      rawReading = 0;
      Value = 0;
    }
    
    rawValue = rawReading;
    
    if (abs(lastValueSendMillis-millis())>MQTT_MIN_REFRESH_MILLIS) {
      lastValueSendMillis = millis();
      if (lastRawReading!=rawReading) {
        lastRawReading = rawReading;
        mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(sensorPin)+setBufferFromFlash(charRawValue),intToString(rawReading));
      }
    
      if (lastValue!=Value) {
        lastValue = Value;
        mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(sensorPin)+setBufferFromFlash(charValue),floatToString(Value));
      }
    }
  }

  void loadConfig(int EEPROM_addr) {
    sensorType = configGetValue(EEPROM_addr+NAME_LENGTH+1);
    calibValue1 = configGetFloatValue(EEPROM_addr+NAME_LENGTH+2);
    calibRawRead1 = configGetFloatValue(EEPROM_addr+NAME_LENGTH+6);
    calibValue2 = configGetFloatValue(EEPROM_addr+NAME_LENGTH+10);
    calibRawRead2 = configGetFloatValue(EEPROM_addr+NAME_LENGTH+14);
  }

  void saveConfig(int EEPROM_addr) {
    EEPROM.write(EEPROM_addr+NAME_LENGTH+1,sensorType);
    EEPROM.put(EEPROM_addr+NAME_LENGTH+2,calibValue1);
    EEPROM.put(EEPROM_addr+NAME_LENGTH+6,calibRawRead1);
    EEPROM.put(EEPROM_addr+NAME_LENGTH+10,calibValue2);
    EEPROM.put(EEPROM_addr+NAME_LENGTH+14,calibRawRead2);
  }

  void publishAll() {
    String sType = sensorsGetStringSensorType(sensorType);

    mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(sensorPin)+setBufferFromFlash(charName),setBufferFromEeprom(EEPROM_sensors_addr+((sensorPin-1)*SENSORS_SENSOR_EEPROM_BYTES),10));
    mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(sensorPin)+setBufferFromFlash(charValue),floatToString(Value));
    mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(sensorPin)+setBufferFromFlash(charRawValue),intToString(rawValue));
    mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(sensorPin)+setBufferFromFlash(charSensorType),sType);
    mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(sensorPin)+setBufferFromFlash(charSensorCalibValue1),floatToString(calibValue1));
    mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(sensorPin)+setBufferFromFlash(charSensorCalibRawValue1),intToString(calibRawRead1));
    mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(sensorPin)+setBufferFromFlash(charSensorCalibValue2),floatToString(calibValue2));
    mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(sensorPin)+setBufferFromFlash(charSensorCalibRawValue2),intToString(calibRawRead2)); 
  }
};

static sensorClass mySensors[SENSORS_COUNT];

void sensorsInit() {
  for (byte i=0; i<SENSORS_COUNT; i++) {
    mySensors[i].Init(i);
  }
}

void sensorsMqttPublishAll() {
  for (byte i=0; i<SENSORS_COUNT; i++) {
    mySensors[i].publishAll();
  }
}

float sensorsGetSensorsValue (byte sensorNumber, byte outputType) {
  sensorNumber = sensorNumber-1;
  
  float valSum = 0;
  int valCount = 0;
  float valMin;
  float valMax;

  if (outputType==OUTPUT_TYPE_PWM) {
    sensorNumber = sensorNumber+RELAYS_COUNT;
  }
  
  for (int i =0; i<SENSORS_COUNT; i++) {
    if (mySensors[i].sensorType==sensorNumber) {
      if (valCount==0) {
        valMin = valSum+mySensors[i].Value;
        valMax = valSum+mySensors[i].Value;
      }
      
      valSum = valSum+mySensors[i].Value;
      valCount++;
      
      if (valMin>valSum+mySensors[i].Value) {
        valMin = valSum+mySensors[i].Value;
      }
      if (valMax>valSum+mySensors[i].Value) {
        valMax = valSum+mySensors[i].Value;
      }
    }
  }
  if (valCount>0) {
    return valSum/valCount;
  }
  return 0;
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
    mySensors[sensorNumber].loadConfig(EEPROM_sensors_addr+(sensorNumber*SENSORS_SENSOR_EEPROM_BYTES));
  }
}

void sensorsSaveConfig(byte sensorNumber) {
  mySensors[sensorNumber].saveConfig(EEPROM_sensors_addr+(sensorNumber*SENSORS_SENSOR_EEPROM_BYTES));
}

void sensorsSaveSensorName(byte sensorNr, String Value) {
  byte sensorNumber= sensorNr-1;
  configSaveString(Value,EEPROM_sensors_addr+(sensorNumber*SENSORS_SENSOR_EEPROM_BYTES),10);
}

void sensorsSetSensor(byte sensorNr, byte valueType, float Value) {
  byte sensorNumber= sensorNr-1;
  switch (valueType) {
      case SENSORS_VALUE_TYPE:
        mySensors[sensorNumber].sensorType = (byte)Value;
        break;
      case SENSORS_VALUE_CALIB_VALUE1:
        mySensors[sensorNumber].calibValue1 = Value;
        break;
      case SENSORS_VALUE_CALIB_RAW_VALUE1:
        mySensors[sensorNumber].calibRawRead1 = Value;
        break;
      case SENSORS_VALUE_CALIB_VALUE2:
        mySensors[sensorNumber].calibValue2 = Value;
        break;
      case SENSORS_VALUE_CALIB_RAW_VALUE2:
        mySensors[sensorNumber].calibRawRead2 = Value;
        break;
  }
  sensorsSaveConfig(sensorNumber);
}

float sensorsGetSensor(byte sensorNr, byte valueType) {
  byte sensorNumber= sensorNr-1;
  switch (valueType) {
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
}

void sensorsSecondEvent() {
  for (byte i=0; i<SENSORS_COUNT; i++) {
    if (mySensors[i].sensorType!=SENSOR_TYPE_NONE && mySensors[i].sensorType!=255) {
      mySensors[i].secondEvent();
    }
  }
}

