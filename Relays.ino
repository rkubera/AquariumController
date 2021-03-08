/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/
 
class relayClass {
  public:

  byte relayPin;
  byte digitalPin;
  
  bool relayState = false;
  byte relayControlMode = CONTROL_MODE_MANUAL;

  byte relayModeMorning = RELAY_MODE_NONE;
  byte relayModeAfternoon = RELAY_MODE_NONE;
  byte relayModeEvening = RELAY_MODE_NONE;
  byte relayModeNight = RELAY_MODE_NONE;

  byte relayManualOnOff = RELAY_MANUAL_ONOFF_AUTO;
  
  byte relayLastMode;
  byte relayLastPartOfDay = SCHEDULER_MODE_INITIAL;
  byte relayMode;
  
  bool relayLastRelayState;

  double relaySensorsSetpoint;
  float maxDeviation;
  byte relayDirection;
  bool tresholdDirection = false;
  double relayInput;

  double lastValueSendMillis;
  double lastRelayInput;

  bool lastRelayState;

  void Init(byte pin) {
    switch (pin) {
      case 0: digitalPin = RELAY_PIN_1;
            pinMode(digitalPin, LOW);
            break;
      case 1: digitalPin = RELAY_PIN_2;
            pinMode(digitalPin, LOW);
            break;
      case 2: digitalPin = RELAY_PIN_3;
            pinMode(digitalPin, LOW);
            break;
      case 3: digitalPin = RELAY_PIN_4;
            pinMode(digitalPin, LOW);
            break;
    }
    relayPin = pin+1;
    
    relaySetState();
    relaysCheckTresholdDirection(relayPin, OUTPUT_TYPE_RELAY);
  }

  void relayMqttPublishAll() {

    mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(relayPin)+setBufferFromFlash(charMorningMode),relayGetStringValue(relayModeMorning));
    mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(relayPin)+setBufferFromFlash(charAfternoonMode),relayGetStringValue(relayModeAfternoon));
    mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(relayPin)+setBufferFromFlash(charEveningMode),relayGetStringValue(relayModeEvening));
    mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(relayPin)+setBufferFromFlash(charNightMode),relayGetStringValue(relayModeNight));
    mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(relayPin)+setBufferFromFlash(charState),relayGetStringValueFromBool(relayLastRelayState));
    mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(relayPin)+setBufferFromFlash(charControlMode),getStringControlModeFromValue(relayControlMode));

    mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(relayPin)+setBufferFromFlash(charSensorsValue),intToString(relayInput));
    
    if (relayDirection==CONTROL_DIRECT) {
      mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(relayPin)+setBufferFromFlash(charControlDirection),setBufferFromFlash(charDirect));
    }
    else {
      mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(relayPin)+setBufferFromFlash(charControlDirection),setBufferFromFlash(charReverse));
    }
    mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(relayPin)+setBufferFromFlash(charSensorsSetpoint),floatToString(relaySensorsSetpoint));
    mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(relayPin)+setBufferFromFlash(charMaxDeviation),floatToString(maxDeviation));

    relaySetState();
    relaysCheckTresholdDirection(relayPin, OUTPUT_TYPE_RELAY);
  }

  void relaySetState() {
    byte actualPartOfDay = schedulerGetActualPartOfDay();
    bool relayUpDown = relayLastRelayState;
      
    if (relayControlMode == CONTROL_MODE_PART_OF_DAY) {
      if (actualPartOfDay==SCHEDULER_MODE_MORNING) {
        relayMode = relayModeMorning;
      }
      else if (actualPartOfDay==SCHEDULER_MODE_AFTERNOON) {
        relayMode = relayModeAfternoon;
      }
      else if (actualPartOfDay==SCHEDULER_MODE_EVENING) {
        relayMode = relayModeEvening;
      }
      else {
        relayMode = relayModeNight;
      }
  
      if (relayMode == RELAY_MODE_ON) {
        relayUpDown = true;
      }
      else if (relayMode == RELAY_MODE_OFF) {
        relayUpDown = false;
      }

      if (relayLastPartOfDay!=actualPartOfDay) {
        relayLastPartOfDay = actualPartOfDay;
        relayManualOnOff=RELAY_MANUAL_ONOFF_AUTO;
      }
    }
    /*
    if (relayPin==3) {
    Serial.println("----------------");
    Serial.print("RelayNumber=");
    Serial.println(relayPin);
    Serial.print("actualPartOfDay=");
    Serial.println(actualPartOfDay);
    Serial.print("relayControlMode=");
    Serial.println(relayControlMode);
    Serial.print("relayMode=");
    Serial.println(relayMode);
    Serial.print("relayManualOnOff=");
    Serial.println(relayManualOnOff);
    */
    
    if (relayControlMode == CONTROL_MODE_TRESHOLD) {
      relayUpDown = relayState;
    }
    if (relayManualOnOff == RELAY_MANUAL_ONOFF_ON) {
      relayUpDown = true;
    }
    else if (relayManualOnOff == RELAY_MANUAL_ONOFF_OFF){
      relayUpDown = false;
    }

    /*
    if (relayUpDown==true) {
      Serial.println("relayUpDown=true");
    }
    else{
      Serial.println("relayUpDown=false");
    }
    */
    
    relayState = relayUpDown;


    if (relayLastRelayState!=relayUpDown) {
      relayLastRelayState = relayUpDown;
      if (relayUpDown == true) {
        relayUp();
        mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(relayPin)+setBufferFromFlash(charState),setBufferFromFlash(charOn));
      }
      else {
        relayDown();
        mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(relayPin)+setBufferFromFlash(charState),setBufferFromFlash(charOff));
      }
    }
      
    if (abs(lastValueSendMillis-millis())>MQTT_MIN_REFRESH_MILLIS) {
      lastValueSendMillis = millis();

      if (lastRelayInput!=relayInput) {
        lastRelayInput = relayInput;
        mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(relayPin)+setBufferFromFlash(charSensorsValue),floatToString(relayInput));
      }
    }
    //}
  }

  String relayGetStringValueFromBool (bool Value) {
    if (Value) {    
      return setBufferFromFlash(charOn);
    }
    else {  
      return setBufferFromFlash(charOff);
    }
  }

  String relayGetStringValue (byte Value) {
    if (Value == RELAY_MODE_ON) {    
      return setBufferFromFlash(charOn);
    }
    else if (Value == RELAY_MODE_OFF) {  
      return setBufferFromFlash(charOff);
    }
    return "";
  }

  void relayUp() {
    pinMode(digitalPin,HIGH);
    if (lastRelayState!=true && relayPin==3) {
      mqttElPublish("cmnd/POWER2","ON");
      lastRelayState = true;
    }
  }
  
  void relayDown() {
    pinMode(digitalPin,LOW);
    if (lastRelayState!=false && relayPin==3) {
      mqttElPublish("cmnd/POWER2","OFF");
      lastRelayState = false;
    }
  }
  
  void saveConfig(int EEPROM_addr) {
    EEPROM.write(EEPROM_addr+NAME_LENGTH+1,relayControlMode);
    EEPROM.write(EEPROM_addr+NAME_LENGTH+2,relayModeMorning);
    EEPROM.write(EEPROM_addr+NAME_LENGTH+3,relayModeAfternoon);
    EEPROM.write(EEPROM_addr+NAME_LENGTH+4,relayModeEvening);
    EEPROM.write(EEPROM_addr+NAME_LENGTH+5,relayModeNight);
    EEPROM.write(EEPROM_addr+NAME_LENGTH+6,relayDirection);
    configSaveDoubleValue(relaySensorsSetpoint,EEPROM_addr+NAME_LENGTH+7);
    configSaveFloatValue(maxDeviation,EEPROM_addr+NAME_LENGTH+11);
  }
  
  void loadConfig(int EEPROM_addr) {
    relayControlMode = configGetValue(EEPROM_addr+NAME_LENGTH+1);
    relayModeMorning = configGetValue(EEPROM_addr+NAME_LENGTH+2);
    relayModeAfternoon = configGetValue(EEPROM_addr+NAME_LENGTH+3);
    relayModeEvening = configGetValue(EEPROM_addr+NAME_LENGTH+4);
    relayModeNight = configGetValue(EEPROM_addr+NAME_LENGTH+5);
    relayDirection = configGetValue(EEPROM_addr+NAME_LENGTH+6);
    relaySensorsSetpoint = configGetDoubleValue(EEPROM_addr+NAME_LENGTH+7);
    maxDeviation = configGetFloatValue(EEPROM_addr+NAME_LENGTH+11);
  }

  void ControlEvent() {
    if (relayControlMode == CONTROL_MODE_TRESHOLD) {
      relayInput = sensorsGetSensorsValue(relayPin, OUTPUT_TYPE_RELAY);
      relayManualOnOff=RELAY_MANUAL_ONOFF_AUTO;
      float minValue = relaySensorsSetpoint-maxDeviation;
      float maxValue = relaySensorsSetpoint+maxDeviation;
      
      if (tresholdDirection==true) {
        if (relayInput>maxValue) {
          tresholdDirection=false;
        }
      }
      else {
        if (relayInput<minValue) {
          tresholdDirection=true;
        }
      }

      if (tresholdDirection==true) {
        if (relayDirection==CONTROL_DIRECT) {
          relayState = true;
          relayUp();
        }
        else {
          relayState = false;
          relayDown();
        }
      }
      else {
        if (relayDirection==CONTROL_DIRECT) {
          relayState = false;
          relayDown();
        }
        else {
          relayState = true;
          relayUp();
        }
      }
    }
  }
};

static relayClass myRelays[RELAYS_COUNT];

void relaysInit() {
 for (byte i=0; i<RELAYS_COUNT; i++) {
    myRelays[i].Init(i);
  }
}

void relaysMqttPublishAll() {
  for (byte i=0; i<RELAYS_COUNT; i++) {
    myRelays[i].relayMqttPublishAll();
  }
}

void relaysMillisEvent() {
  for (byte i=0; i<RELAYS_COUNT; i++) {
    myRelays[i].relaySetState();
  }
}

void relaysSecondEvent() {
  for (byte i=0; i<RELAYS_COUNT; i++) {
    myRelays[i].ControlEvent();
  }
}

void relaysLoadConfig() {
  for (byte relayNumber=0; relayNumber<RELAYS_COUNT; relayNumber++) {
    myRelays[relayNumber].loadConfig(EEPROM_relays_addr+(relayNumber*RELAYS_RELAY_EEPROM_BYTES));
  }
}

void relaysSaveConfig(byte relayNumber) {
  myRelays[relayNumber].saveConfig(EEPROM_relays_addr+(relayNumber*RELAYS_RELAY_EEPROM_BYTES));
}

void relaysSaveRelayName(byte relayNr, String Value) {
  byte relayNumber = relayNr-1;
  configSaveString(Value,EEPROM_relays_addr+(relayNumber*RELAYS_RELAY_EEPROM_BYTES),10);
}

void relaysSetRelay(byte relayNr, byte valueType, byte Value) {
  byte relayNumber = relayNr-1;
  switch (valueType) {
    case CONTROL_TRESHOLD_DIRECTION:
        if (Value==0) {
          myRelays[relayNumber].tresholdDirection = true;
        }
        else {
          myRelays[relayNumber].tresholdDirection = false;
        }
        break;
    case RELAY_CONTROL_MODE:
        myRelays[relayNumber].relayControlMode = Value;
        break;
      case RELAY_MODE_MORNING:
        myRelays[relayNumber].relayModeMorning = Value;
        break;
      case RELAY_MODE_AFTERNOON:
        myRelays[relayNumber].relayModeAfternoon = Value;
        break;
      case RELAY_MODE_EVENING:
        myRelays[relayNumber].relayModeEvening = Value;
        break;
      case RELAY_MODE_NIGHT:
        myRelays[relayNumber].relayModeNight = Value;
        break;
      case RELAY_MANUAL_ONOFF:
        myRelays[relayNumber].relayManualOnOff = Value;
        break;
      case RELAY_CONTROL_DIRECTION:
        myRelays[relayNumber].relayDirection = Value;
        break;
  }
  relaysSaveConfig(relayNumber);
}

byte relaysGetRelay(byte relayNr, byte valueType) {
  byte relayNumber= relayNr-1;
  switch (valueType) {
      case RELAY_STATE:
        if (myRelays[relayNumber].relayState) {
          return (1);
        }
        else {
          return (0);
        }
        break;
      case RELAY_CONTROL_MODE:
        return (myRelays[relayNumber].relayControlMode);
        break;
      case RELAY_MODE_MORNING:
        return (myRelays[relayNumber].relayModeMorning);
        break;
      case RELAY_MODE_AFTERNOON:
        return (myRelays[relayNumber].relayModeAfternoon);
        break;
      case RELAY_MODE_EVENING:
        return (myRelays[relayNumber].relayModeEvening);
        break;
      case RELAY_MODE_NIGHT:
        return (myRelays[relayNumber].relayModeNight);
        break;
      case RELAY_MANUAL_ONOFF:
        return (myRelays[relayNumber].relayManualOnOff);
        break;
      case RELAY_CONTROL_DIRECTION:
        return (myRelays[relayNumber].relayDirection);
        break;
  }
  return 0;
}

void relaysSetRelayDouble(byte relayNr, byte valueType, double Value) {
  byte relayNumber = relayNr-1;
  switch (valueType) {
    case RELAY_MAX_DEVIATION:
        myRelays[relayNumber].maxDeviation = Value;
        break;
    case RELAY_SENSORS_SETPOINT:
        myRelays[relayNumber].relaySensorsSetpoint = Value;
        break; 
  }
  relaysSaveConfig(relayNumber);
}

double relaysGetRelayDouble(byte myRelayNr, byte valueType) {
  byte relayNumber= myRelayNr-1;
  switch (valueType) {
    case RELAY_MAX_DEVIATION:
      return (myRelays[relayNumber].maxDeviation);
      break;
    case RELAY_SENSORS_SETPOINT:
      return (myRelays[relayNumber].relaySensorsSetpoint);
      break;    
  }
  return 0;
}

void relaysCheckTresholdDirection (byte relayNr , byte outputType) {
  if (sensorsGetSensorsValue(relayNr, outputType)>relaysGetRelayDouble(relayNr, RELAY_SENSORS_SETPOINT)) {
    relaysSetRelay(relayNr,CONTROL_TRESHOLD_DIRECTION,0);
  }
  else {
    relaysSetRelay(relayNr,CONTROL_TRESHOLD_DIRECTION,1);
  } 
}
