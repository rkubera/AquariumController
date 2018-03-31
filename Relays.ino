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
  byte relayLastPartOfDay;
  byte relayMode;
  
  bool relayLastRelayState;

  void Init(byte pin) {
    switch (pin) {
      case 0: digitalPin = RELAY_PIN_1;
            pinMode(digitalPin, LOW);
            break;
      case 1: digitalPin = RELAY_PIN_2;
            pinMode(digitalPin, LOW);
            break;
    }
    relayPin = pin+1;
  }

  void relayMqttPublishAll() {

    mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(relayPin)+setBufferFromFlash(charMorningMode),relayGetStringValue(relayModeMorning));
    mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(relayPin)+setBufferFromFlash(charAfternoonMode),relayGetStringValue(relayModeAfternoon));
    mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(relayPin)+setBufferFromFlash(charEveningMode),relayGetStringValue(relayModeEvening));
    mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(relayPin)+setBufferFromFlash(charNightMode),relayGetStringValue(relayModeNight));
    mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(relayPin)+setBufferFromFlash(charState),relayGetStringValueFromBool(relayLastRelayState));
    mqttElPublish(setBufferFromFlash(charGetRelay)+intToString(relayPin)+setBufferFromFlash(charControlMode),getStringControlModeFromValue(relayControlMode));
    
    relaySetState();
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
  
    if (relayManualOnOff == RELAY_MANUAL_ONOFF_ON) {
      relayUpDown = true;
    }
    else if (relayManualOnOff == RELAY_MANUAL_ONOFF_OFF){
      relayUpDown = false;
    }

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
  }
  
  void relayDown() {
    pinMode(digitalPin,LOW);
  }
  
  void saveConfig(int EEPROM_addr) {
    EEPROM.write(EEPROM_addr+NAME_LENGHTH+1,relayControlMode);
    EEPROM.write(EEPROM_addr+NAME_LENGHTH+2,relayModeMorning);
    EEPROM.write(EEPROM_addr+NAME_LENGHTH+3,relayModeAfternoon);
    EEPROM.write(EEPROM_addr+NAME_LENGHTH+4,relayModeEvening);
    EEPROM.write(EEPROM_addr+NAME_LENGHTH+5,relayModeNight);
  }
  
  void loadConfig(int EEPROM_addr) {
    relayControlMode = configGetValue(EEPROM_addr+NAME_LENGHTH+1);
    relayModeMorning = configGetValue(EEPROM_addr+NAME_LENGHTH+2);
    relayModeAfternoon = configGetValue(EEPROM_addr+NAME_LENGHTH+3);
    relayModeEvening = configGetValue(EEPROM_addr+NAME_LENGHTH+4);
    relayModeNight = configGetValue(EEPROM_addr+NAME_LENGHTH+5);
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
  }
}

