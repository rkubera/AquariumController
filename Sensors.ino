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
    sensorPin = pin;
  
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
      bool okReading = true;
      if (sensorType==SENSOR_TYPE_PH) {
        if (myValue<0 || myValue>14) {
          okReading = false;
        }
      }
      if (okReading==true) {
        ValuesSum = ValuesSum+myValue;
        rawReading = rawReading+analog;
        count++;
      }
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
  
    rawReading = ValuesRawSum/realNumReadings;
    rawValue = rawReading;
    Value = ValuesSum/realNumReadings;
    Value = (float)round(Value*precision)/precision;
    
    if (lastRawReading!=rawReading) {
      lastRawReading = rawReading;
      mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(sensorPin)+setBufferFromFlash(charSensorRawValue),intToString(rawReading));
      //mqttElPublish(setBufferFromFlash(getRawPH), intToString(rawReading));
    }
  
    if (lastValue!=Value) {
      lastValue = Value;
      //mqttElPublish(setBufferFromFlash(getPH), floatToString(Value));
      mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(sensorPin)+setBufferFromFlash(charSensorValue),floatToString(Value));
    }
  }

  void loadConfig(int EEPROM_addr) {
    sensorType = configGetValue(EEPROM_addr);
    minValue = configGetFloatValue(EEPROM_addr+1);
    maxValue = configGetFloatValue(EEPROM_addr+5);
    criticalMinValue = configGetFloatValue(EEPROM_addr+9);
    criticalMaxValue = configGetFloatValue(EEPROM_addr+13);
    calibValue1 = configGetFloatValue(EEPROM_addr+17);
    calibRawRead1 = configGetFloatValue(EEPROM_addr+21);
    calibValue2 = configGetFloatValue(EEPROM_addr+25);
    calibRawRead2 = configGetFloatValue(EEPROM_addr+29);
  }

  void saveConfig(int EEPROM_addr) {
    EEPROM.write(EEPROM_addr,sensorType);
    EEPROM.put(EEPROM_addr+1,minValue);
    EEPROM.put(EEPROM_addr+5,maxValue);
    EEPROM.put(EEPROM_addr+9,criticalMinValue);
    EEPROM.put(EEPROM_addr+13,criticalMaxValue);
    EEPROM.put(EEPROM_addr+17,calibValue1);
    EEPROM.put(EEPROM_addr+21,calibRawRead1);
    EEPROM.put(EEPROM_addr+15,calibValue2);
    EEPROM.put(EEPROM_addr+29,calibRawRead2);
  }

  void publishAll() {
    String sType;
    switch (sensorType) {
      case SENSOR_TYPE_NONE:
        sType = setBufferFromFlash(charSensornone);
        break;
      case SENSOR_TYPE_PH:
         sType = setBufferFromFlash(charPhsensor);
        break;
      case SENSOR_TYPE_THERMOMETER:
        sType = setBufferFromFlash(charThermometer);
        break;
      case SENSOR_TYPE_AQUA_WATER_LEVEL:
        sType = setBufferFromFlash(charAquawaterlevel);
        break;
      case SENSOR_TYPE_TANK_WATER_LEVEL:
        sType = setBufferFromFlash(charTankwaterlevel);
        break;
    }
    
    mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(sensorPin)+setBufferFromFlash(charSensorValue),floatToString(Value));
    mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(sensorPin)+setBufferFromFlash(charSensorRawValue),intToString(rawValue));
    mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(sensorPin)+setBufferFromFlash(charSensorType),sType);
    mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(sensorPin)+setBufferFromFlash(charSensorCalibValue1),floatToString(calibValue1));
    mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(sensorPin)+setBufferFromFlash(charSensorCalibRawValue1),intToString(calibRawRead1));
    mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(sensorPin)+setBufferFromFlash(charSensorCalibValue2),floatToString(calibValue2));
    mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(sensorPin)+setBufferFromFlash(charSensorCalibRawValue2),intToString(calibRawRead2));
    mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(sensorPin)+setBufferFromFlash(charSensorMinValue),floatToString(minValue));
    mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(sensorPin)+setBufferFromFlash(charSensorMaxValue),floatToString(maxValue));
    mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(sensorPin)+setBufferFromFlash(charSensorCriticalMinValue),floatToString(criticalMinValue));
    mqttElPublish(setBufferFromFlash(charGetSensor)+intToString(sensorPin)+setBufferFromFlash(charSensorCriticalMaxValue),floatToString(criticalMaxValue));
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

void sensorsLoadConfig() {
  for (byte i=0; i<SENSORS_COUNT; i++) {
    mySensors[i].loadConfig(EEPROM_sensors_addr+(i*33));
  }
}

void sensorsSaveConfig(byte sensorNumber) {
  mySensors[sensorNumber].saveConfig(EEPROM_sensors_addr+(sensorNumber*33));
}

void sensorsSetSensor(byte sensorNumber, byte valueType, float Value) {
  switch (valueType) {
      case SENSORS_VALUE_TYPE:
        mySensors[sensorNumber].sensorType = (byte)Value;
        break;
      case SENSORS_VALUE_MIN:
        mySensors[sensorNumber].minValue = Value;
        break;
      case SENSORS_VALUE_MAX:
        mySensors[sensorNumber].maxValue = Value;
        break;
      case SENSORS_VALUE_CRITICAL_MIN:
        mySensors[sensorNumber].criticalMinValue = Value;
        break;
      case SENSORS_VALUE_CRITICAL_MAX:
        mySensors[sensorNumber].criticalMaxValue = Value;
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

float sensorsGetSensor(byte sensorNumber, byte valueType) {
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
      case SENSORS_VALUE_MIN:
        return (mySensors[sensorNumber].minValue);
        break;
      case SENSORS_VALUE_MAX:
        return (mySensors[sensorNumber].maxValue);
        break;
      case SENSORS_VALUE_CRITICAL_MIN:
        return (mySensors[sensorNumber].criticalMinValue);
        break;
      case SENSORS_VALUE_CRITICAL_MAX:
        return (mySensors[sensorNumber].criticalMaxValue);
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
    if (mySensors[i].sensorType!=SENSOR_TYPE_NONE) {
      mySensors[i].secondEvent();
    }
  }
}


