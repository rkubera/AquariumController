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
  TemporaryState stateChoice;
  bool relayModeDemand=false;

  byte relayPin = 0;
  byte digitalPin = 0;
  
  bool relayState = false;
  bool regulatedState = false;
  bool thresholdInitialized = false;

  byte relayControlMode = CONTROL_MODE_MANUAL;

  byte relayModeMorning = RELAY_MODE_OFF;
  byte relayModeAfternoon = RELAY_MODE_OFF;
  byte relayModeEvening = RELAY_MODE_OFF;
  byte relayModeNight = RELAY_MODE_OFF;

  byte relayManualOnOff = RELAY_MANUAL_ONOFF_AUTO;
  
  
  bool relayLastRelayState = false;

  double relaySensorsSetpoint = 0;
  float maxDeviation = 0;
  byte relayDirection = CONTROL_DIRECT;
  bool tresholdDirection = false;
  double relayInput = NAN;

  unsigned long lastValueSendMillis = 0;
  double lastRelayInput = NAN;


  void Init(byte pin) {
    switch (pin) {
      case 0: digitalPin = RELAY_PIN_1;
            pinMode(digitalPin, OUTPUT);
            digitalWrite(digitalPin, LOW);
            break;
      case 1: digitalPin = RELAY_PIN_2;
            pinMode(digitalPin, OUTPUT);
            digitalWrite(digitalPin, LOW);
            break;
      case 2: digitalPin = RELAY_PIN_3;
            pinMode(digitalPin, OUTPUT);
            digitalWrite(digitalPin, LOW);
            break;
      case 3: digitalPin = RELAY_PIN_4;
            pinMode(digitalPin, OUTPUT);
            digitalWrite(digitalPin, LOW);
            break;
    }
    relayPin = pin+1;
    
    relaySetState();

  }

  void relayReportAll() {
    reportField(relayField(relayPin,RelayField::Input),sensorsOutputInputString(relayPin,OUTPUT_TYPE_RELAY));

    reportField(relayField(relayPin,RelayField::Name),readConfigName(CONFIG_relays_addr+(relayPin-1)*RELAYS_RELAY_CONFIG_BYTES,NAME_LENGTH));

    reportField(relayField(relayPin,RelayField::MorningMode),relayGetStringValue(relayModeMorning));
    reportField(relayField(relayPin,RelayField::AfternoonMode),relayGetStringValue(relayModeAfternoon));
    reportField(relayField(relayPin,RelayField::EveningMode),relayGetStringValue(relayModeEvening));
    reportField(relayField(relayPin,RelayField::NightMode),relayGetStringValue(relayModeNight));
    
    reportField(relayField(relayPin,RelayField::ControlMode),getStringControlModeFromValue(relayControlMode));

    reportField(relayField(relayPin,RelayField::SensorsValue),floatToString(relayInput));
    
    if (relayDirection==CONTROL_DIRECT) {
      reportField(relayField(relayPin,RelayField::ControlDirection),setBufferFromFlash(charDirect));
    }
    else {
      reportField(relayField(relayPin,RelayField::ControlDirection),setBufferFromFlash(charReverse));
    }
    reportField(relayField(relayPin,RelayField::SensorsSetpoint),floatToString(relaySensorsSetpoint));
    reportField(relayField(relayPin,RelayField::MaxDeviation),floatToString(maxDeviation));



    reportField(relayField(relayPin,RelayField::State),relayGetStringValueFromBool(relayLastRelayState));
  }

  void relaySetState() {
    relayInput = sensorsGetSensorsValue(relayPin,OUTPUT_TYPE_RELAY);
    bool inputValid = isfinite(relayInput);
    byte actualPartOfDay = schedulerGetActualPartOfDay();
    bool relayUpDown = relayModeDemand;
      
    if (relayControlMode == CONTROL_MODE_PART_OF_DAY) {
      byte relayMode;
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
  
      relayUpDown = relayMode == RELAY_MODE_ON;

    }
    if (relayControlMode == CONTROL_MODE_TRESHOLD) {
      relayUpDown = regulatedState;
    }
    relayModeDemand=relayUpDown;
    bool healthy=megaConfigReady && relayPin<=megaFeatures.relays && (relayControlMode!=CONTROL_MODE_TRESHOLD || inputValid);
    relayUpDown=stateChoice.resolve(relayUpDown,relayControlMode,relayControlMode==CONTROL_MODE_PART_OF_DAY?actualPartOfDay:0,relayManualOnOff,healthy);
    relayState = relayUpDown;


    if (relayLastRelayState!=relayUpDown) {
      relayLastRelayState = relayUpDown;
      if (relayUpDown == true) {
        relayUp();
        reportField(relayField(relayPin,RelayField::State),setBufferFromFlash(charOn));
      }
      else {
        relayDown();
        reportField(relayField(relayPin,RelayField::State),setBufferFromFlash(charOff));
      }
    }
      

    if (millis()-lastValueSendMillis >= TELEMETRY_MIN_INTERVAL_MS) {
      lastValueSendMillis = millis();

      if (lastRelayInput!=relayInput && (isfinite(lastRelayInput) || isfinite(relayInput))) {
        lastRelayInput = relayInput;
        reportField(relayField(relayPin,RelayField::SensorsValue),floatToString(relayInput));
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
    return setBufferFromFlash(charOff);
  }

  void relayUp() {
    digitalWrite(digitalPin,HIGH);
  }
  
  void relayDown() {
    digitalWrite(digitalPin,LOW);
  }
  
  void saveConfig(int CONFIG_addr) {

    configStorage.update(CONFIG_addr+NAME_LENGTH+1,relayControlMode);
    configStorage.update(CONFIG_addr+NAME_LENGTH+2,relayModeMorning);
    configStorage.update(CONFIG_addr+NAME_LENGTH+3,relayModeAfternoon);
    configStorage.update(CONFIG_addr+NAME_LENGTH+4,relayModeEvening);
    configStorage.update(CONFIG_addr+NAME_LENGTH+5,relayModeNight);
    configStorage.update(CONFIG_addr+NAME_LENGTH+6,relayDirection);
    configSaveDoubleValue(relaySensorsSetpoint,CONFIG_addr+NAME_LENGTH+7);
    configSaveFloatValue(maxDeviation,CONFIG_addr+NAME_LENGTH+11);
  }
  
  void loadConfig(int CONFIG_addr) {

    relayControlMode = configGetValue(CONFIG_addr+NAME_LENGTH+1);
    relayModeMorning = configGetValue(CONFIG_addr+NAME_LENGTH+2);
    relayModeAfternoon = configGetValue(CONFIG_addr+NAME_LENGTH+3);
    relayModeEvening = configGetValue(CONFIG_addr+NAME_LENGTH+4);
    relayModeNight = configGetValue(CONFIG_addr+NAME_LENGTH+5);
    relayDirection = configGetValue(CONFIG_addr+NAME_LENGTH+6);
    relaySensorsSetpoint = configGetDoubleValue(CONFIG_addr+NAME_LENGTH+7);
    maxDeviation = configGetFloatValue(CONFIG_addr+NAME_LENGTH+11);
    if (relayControlMode > CONTROL_MODE_TRESHOLD) relayControlMode = CONTROL_MODE_MANUAL;
    if (relayDirection > PID_REVERSE) relayDirection = CONTROL_DIRECT;
    if (!isfinite(relaySensorsSetpoint)) { relaySensorsSetpoint = 0; relayControlMode = CONTROL_MODE_MANUAL; }
    if (!isfinite(maxDeviation) || maxDeviation < 0) maxDeviation = 0;
    if (relayModeMorning > RELAY_MODE_ON) relayModeMorning = RELAY_MODE_OFF;
    if (relayModeAfternoon > RELAY_MODE_ON) relayModeAfternoon = RELAY_MODE_OFF;
    if (relayModeEvening > RELAY_MODE_ON) relayModeEvening = RELAY_MODE_OFF;
    if (relayModeNight > RELAY_MODE_ON) relayModeNight = RELAY_MODE_OFF;
    thresholdInitialized = false;
  }

  void ControlEvent() {
    if (relayControlMode == CONTROL_MODE_TRESHOLD) {
      relayInput = sensorsGetSensorsValue(relayPin, OUTPUT_TYPE_RELAY);
      if (!isfinite(relayInput)) {
        regulatedState = false;
        thresholdInitialized = false;
      }
      else {
        if (!thresholdInitialized) {
          tresholdDirection = relayInput < relaySensorsSetpoint;
          thresholdInitialized = true;
        }
        if (relayInput > relaySensorsSetpoint+maxDeviation) tresholdDirection = false;
        else if (relayInput < relaySensorsSetpoint-maxDeviation) tresholdDirection = true;
        regulatedState = tresholdDirection == (relayDirection == CONTROL_DIRECT);
      }
    }
    relaySetState();
  }

};

static relayClass myRelays[RELAYS_COUNT];

void relaysInit() {
 for (byte i=0; i<RELAYS_COUNT; i++) {
    myRelays[i].Init(i);
  }
}

void relaysReportAll() {
  for (byte i=0; i<RELAYS_COUNT; i++) {
    myRelays[i].relayReportAll();
  }
}

void relaysMillisEvent() {
  for (byte i=0; i<RELAYS_COUNT; i++) {
    myRelays[i].relaySetState();
  }
}

void relaysControlEvent() {
  for (byte i=0; i<megaFeatures.relays; i++) {
    myRelays[i].ControlEvent();
  }
}

void relaysLoadConfig() {
  for (byte relayNumber=0; relayNumber<RELAYS_COUNT; relayNumber++) {
    myRelays[relayNumber].loadConfig(CONFIG_relays_addr+(relayNumber*RELAYS_RELAY_CONFIG_BYTES));
  }
}

void relaysSaveConfig(byte relayNumber) {
  myRelays[relayNumber].saveConfig(CONFIG_relays_addr+(relayNumber*RELAYS_RELAY_CONFIG_BYTES));
}

void relaysSaveRelayName(byte relayNr, String Value) {
  byte relayNumber = relayNr-1;
  configSaveString(Value,CONFIG_relays_addr+(relayNumber*RELAYS_RELAY_CONFIG_BYTES),NAME_LENGTH);
}

void relaysSetRelay(byte relayNr, byte valueType, byte Value) {
  if (relayNr == 0 || relayNr > RELAYS_COUNT) return;
  byte relayNumber = relayNr-1;
  switch (valueType) {
    case CONTROL_TRESHOLD_DIRECTION:
        myRelays[relayNumber].thresholdInitialized = true;
        if (Value==0) {
          myRelays[relayNumber].tresholdDirection = true;
        }
        else {
          myRelays[relayNumber].tresholdDirection = false;
        }
        break;
    case RELAY_CONTROL_MODE:
        if (Value > CONTROL_MODE_TRESHOLD) return;
        myRelays[relayNumber].thresholdInitialized = false;
        myRelays[relayNumber].regulatedState = false;
        myRelays[relayNumber].relayManualOnOff = RELAY_MANUAL_ONOFF_AUTO;
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
        if (Value > RELAY_MANUAL_ONOFF_ON) return;
        myRelays[relayNumber].relayManualOnOff = Value;
        myRelays[relayNumber].ControlEvent();
        return;
      case RELAY_CONTROL_DIRECTION:
        if (Value > PID_REVERSE) return;
        myRelays[relayNumber].relayDirection = Value;
        break;
  }
  if (valueType == CONTROL_TRESHOLD_DIRECTION) return;
  relaysSaveConfig(relayNumber);
  myRelays[relayNumber].relaySetState();
}

byte relaysGetRelay(byte relayNr, byte valueType) {
  if (relayNr == 0 || relayNr > RELAYS_COUNT) return 0;
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
  if (relayNr == 0 || relayNr > RELAYS_COUNT || !isfinite(Value)) return;
  if (valueType == RELAY_MAX_DEVIATION && Value < 0) return;
  byte relayNumber = relayNr-1;
  switch (valueType) {
    case RELAY_MAX_DEVIATION:
        myRelays[relayNumber].maxDeviation = Value;
        break;
    case RELAY_SENSORS_SETPOINT:
        myRelays[relayNumber].relaySensorsSetpoint = Value;
        break; 
  }
  if (valueType == CONTROL_TRESHOLD_DIRECTION) return;
  relaysSaveConfig(relayNumber);
  myRelays[relayNumber].relaySetState();
}

double relaysGetRelayDouble(byte myRelayNr, byte valueType) {
  if (myRelayNr == 0 || myRelayNr > RELAYS_COUNT) return NAN;
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

void relaysCheckTresholdDirection(byte relayNr, byte outputType) {
  if (relayNr == 0 || relayNr > RELAYS_COUNT) return;
  float input = sensorsGetSensorsValue(relayNr, outputType);
  myRelays[relayNr-1].thresholdInitialized = isfinite(input);
  if (isfinite(input)) myRelays[relayNr-1].tresholdDirection = input < myRelays[relayNr-1].relaySensorsSetpoint;
}

//Spread complete snapshots across channels to keep acquisition and regulation running.
void relaysReportChannel(byte channel) {
  if (channel < RELAYS_COUNT) myRelays[channel].relayReportAll();
}

void relaysInputChanged(byte channel){if(!channel||channel>RELAYS_COUNT)return;auto& relay=myRelays[channel-1];relay.thresholdInitialized=false;relay.regulatedState=false;relay.ControlEvent();}
