/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * free of charge for non-commercial use only     *
 * all rights reserved                            *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/
 
class pwmOutputClass {
  public:

  byte pwmOutputPin;
  byte digitalPin;
  
  bool pwmOutputState = false;
  byte pwmOutputControlMode = CONTROL_MODE_MANUAL;

  byte pwmOutputModeMorning = PWMOUTPUT_MODE_NONE;
  byte pwmOutputModeAfternoon = PWMOUTPUT_MODE_NONE;
  byte pwmOutputModeEvening = PWMOUTPUT_MODE_NONE;
  byte pwmOutputModeNight = PWMOUTPUT_MODE_NONE;

  byte pwmOutputManualOnOff = PWMOUTPUT_MANUAL_ONOFF_AUTO;
  
  byte pwmOutputLastMode;
  byte pwmOutputLastPartOfDay;
  byte pwmOutputMode;
  
  bool pwmOutputLastpwmOutputState;

  void Init(byte pin) {
    switch (pin) {
      case 0: digitalPin = DIGITAL_PWM_12V_OUT_PIN_1;
            pinMode(digitalPin, LOW);
            break;
      case 1: digitalPin = DIGITAL_PWM_12V_OUT_PIN_2;
            pinMode(digitalPin, LOW);
            break;
      case 2: digitalPin = DIGITAL_PWM_OUT_PIN_1;
            pinMode(digitalPin, LOW);
            break;
      case 3: digitalPin = DIGITAL_PWM_OUT_PIN_2;
            pinMode(digitalPin, LOW);
            break;
      case 4: digitalPin = DIGITAL_PWM_OUT_PIN_3;
            pinMode(digitalPin, LOW);
            break;
    }
    pwmOutputPin = pin+1;
  }

  void pwmOutputMqttPublishAll() {

    mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charMorningMode),pwmOutputGetStringValue(pwmOutputModeMorning));
    mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charAfternoonMode),pwmOutputGetStringValue(pwmOutputModeAfternoon));
    mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charEveningMode),pwmOutputGetStringValue(pwmOutputModeEvening));
    mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charNightMode),pwmOutputGetStringValue(pwmOutputModeNight));
    mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charState),pwmOutputGetStringValueFromBool(pwmOutputLastpwmOutputState));
    mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charControlMode),getStringControlModeFromValue(pwmOutputControlMode));
    
    pwmOutputSetState();
  }

  void pwmOutputSetState() {
    byte actualPartOfDay = schedulerGetActualPartOfDay();
    bool pwmOutputUpDown = pwmOutputLastpwmOutputState;
      
    if (pwmOutputControlMode == CONTROL_MODE_PART_OF_DAY) {
      if (actualPartOfDay==SCHEDULER_MODE_MORNING) {
        if (pwmOutputModeMorning==PWMOUTPUT_MODE_ON) {
          pwmOutputMode = pwmOutputModeMorning;
        }
        else if (pwmOutputModeMorning==PWMOUTPUT_MODE_OFF) {
          pwmOutputMode = pwmOutputModeMorning;
        }
      }
      else if (actualPartOfDay==SCHEDULER_MODE_AFTERNOON) {
        if (pwmOutputModeAfternoon==PWMOUTPUT_MODE_ON) {
          pwmOutputMode = pwmOutputModeAfternoon;
        }
        else if (pwmOutputModeAfternoon==PWMOUTPUT_MODE_OFF) {
          pwmOutputMode = pwmOutputModeAfternoon;
        }
      }
      else if (actualPartOfDay==SCHEDULER_MODE_EVENING) {
        if (pwmOutputModeEvening==PWMOUTPUT_MODE_ON) {
          pwmOutputMode = pwmOutputModeEvening;
        }
        else if (pwmOutputModeEvening==PWMOUTPUT_MODE_OFF) {
          pwmOutputMode = pwmOutputModeEvening;
        }
      }
      else {
        if (pwmOutputModeNight==PWMOUTPUT_MODE_ON) {
          pwmOutputMode = pwmOutputModeNight;
        }
        else if (pwmOutputModeNight==PWMOUTPUT_MODE_OFF) {
          pwmOutputMode = pwmOutputModeNight;
        }
      }
  
      if (pwmOutputMode == PWMOUTPUT_MODE_ON) {
        pwmOutputUpDown = true;
      }
      else if (pwmOutputMode == PWMOUTPUT_MODE_OFF) {
        pwmOutputUpDown = false;
      }
    }
  
    if (pwmOutputControlMode != CONTROL_MODE_MANUAL) {
      if (pwmOutputLastPartOfDay!=actualPartOfDay) {
        pwmOutputLastPartOfDay = actualPartOfDay;
        pwmOutputManualOnOff=PWMOUTPUT_MANUAL_ONOFF_AUTO;
      }
    }
    
    if (pwmOutputManualOnOff == PWMOUTPUT_MANUAL_ONOFF_ON) {
      pwmOutputUpDown = true;
    }
    else if (pwmOutputManualOnOff == PWMOUTPUT_MANUAL_ONOFF_OFF){
      pwmOutputUpDown = false;
    }

    pwmOutputState = pwmOutputUpDown;
    if (pwmOutputLastpwmOutputState!=pwmOutputUpDown) {
      pwmOutputLastpwmOutputState = pwmOutputUpDown;
      if (pwmOutputUpDown == true) {
        pwmOutputUp();
        mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charState),setBufferFromFlash(charOn));
      }
      else {
        pwmOutputDown();
        mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charState),setBufferFromFlash(charOff));
      }
    }
  }

  String pwmOutputGetStringValueFromBool (bool Value) {
    if (Value) {    
      return setBufferFromFlash(charOn);
    }
    else {  
      return setBufferFromFlash(charOff);
    }
  }

  String pwmOutputGetStringValue (byte Value) {
    if (Value == PWMOUTPUT_MODE_ON) {    
      return setBufferFromFlash(charOn);
    }
    else if (Value == PWMOUTPUT_MODE_OFF) {  
      return setBufferFromFlash(charOff);
    }
    return "";
  }

  void pwmOutputUp() {
    switch (pwmOutputPin) {
      case 0: pinMode(digitalPin,HIGH);
            break;
      case 1: pinMode(digitalPin,HIGH);
            break;
    }
  }
  
  void pwmOutputDown() {
    switch (pwmOutputPin) {
      case 0: pinMode(digitalPin,LOW);
            break;
      case 1: pinMode(digitalPin,LOW);
            break;
    }
  }
  
  void saveConfig(int EEPROM_addr) {
    EEPROM.write(EEPROM_addr+11,pwmOutputControlMode);
    EEPROM.write(EEPROM_addr+12,pwmOutputModeMorning);
    EEPROM.write(EEPROM_addr+13,pwmOutputModeAfternoon);
    EEPROM.write(EEPROM_addr+14,pwmOutputModeEvening);
    EEPROM.write(EEPROM_addr+15,pwmOutputModeNight);
  }
  
  void loadConfig(int EEPROM_addr) {
    pwmOutputControlMode = configGetValue(EEPROM_addr+11);
    pwmOutputModeMorning = configGetValue(EEPROM_addr+12);
    pwmOutputModeAfternoon = configGetValue(EEPROM_addr+13);
    pwmOutputModeEvening = configGetValue(EEPROM_addr+14);
    pwmOutputModeNight = configGetValue(EEPROM_addr+15);
  }
};

static pwmOutputClass myPwmOutputs[PWMOUTPUTS_COUNT];

void pwmOutputsInit() {
 for (byte i=0; i<PWMOUTPUTS_COUNT; i++) {
    myPwmOutputs[i].Init(i);
  }
}

void pwmOutputsMqttPublishAll() {
  for (byte i=0; i<PWMOUTPUTS_COUNT; i++) {
    myPwmOutputs[i].pwmOutputMqttPublishAll();
  }
}

void pwmOutputsMillisEvent() {
  for (byte i=0; i<PWMOUTPUTS_COUNT; i++) {
    myPwmOutputs[i].pwmOutputSetState();
  }
}

void pwmOutputsLoadConfig() {
  for (byte pwmOutputNumber=0; pwmOutputNumber<PWMOUTPUTS_COUNT; pwmOutputNumber++) {
    myPwmOutputs[pwmOutputNumber].loadConfig(EEPROM_pwm_outputs_addr+(pwmOutputNumber*PWMOUTPUTS_PWMOUTPUT_EEPROM_BYTES));
  }
}

void pwmOutputsSaveConfig(byte pwmOutputNumber) {
  myPwmOutputs[pwmOutputNumber].saveConfig(EEPROM_pwm_outputs_addr+(pwmOutputNumber*PWMOUTPUTS_PWMOUTPUT_EEPROM_BYTES));
}

void pwmOutputsSavePwmOutputName(byte pwmOutputNr, String Value) {
  byte pwmOutputNumber = pwmOutputNr-1;
  configSaveString(Value,EEPROM_pwm_outputs_addr+(pwmOutputNumber*PWMOUTPUTS_PWMOUTPUT_EEPROM_BYTES),10);
}

void pwmOutputsSetPwmOutput(byte pwmOutputNr, byte valueType, byte Value) {
  byte pwmOutputNumber = pwmOutputNr-1;
  switch (valueType) {
    case PWMOUTPUT_CONTROL_MODE:
        myPwmOutputs[pwmOutputNumber].pwmOutputControlMode = Value;
        break;
      case PWMOUTPUT_MODE_MORNING:
        myPwmOutputs[pwmOutputNumber].pwmOutputModeMorning = Value;
        break;
      case PWMOUTPUT_MODE_AFTERNOON:
        myPwmOutputs[pwmOutputNumber].pwmOutputModeAfternoon = Value;
        break;
      case PWMOUTPUT_MODE_EVENING:
        myPwmOutputs[pwmOutputNumber].pwmOutputModeEvening = Value;
        break;
      case PWMOUTPUT_MODE_NIGHT:
        myPwmOutputs[pwmOutputNumber].pwmOutputModeNight = Value;
        break;
      case PWMOUTPUT_MANUAL_ONOFF:
        myPwmOutputs[pwmOutputNumber].pwmOutputManualOnOff = Value;
        break;
  }
  pwmOutputsSaveConfig(pwmOutputNumber);
}

byte pwmOutputsGetPwmOutput(byte pwmOutputNr, byte valueType) {
  byte pwmOutputNumber= pwmOutputNr-1;
  switch (valueType) {
      case PWMOUTPUT_STATE:
        if (myPwmOutputs[pwmOutputNumber].pwmOutputState) {
          return (1);
        }
        else {
          return (0);
        }
        break;
      case PWMOUTPUT_CONTROL_MODE:
        return (myPwmOutputs[pwmOutputNumber].pwmOutputControlMode);
        break;
      case PWMOUTPUT_MODE_MORNING:
        return (myPwmOutputs[pwmOutputNumber].pwmOutputModeMorning);
        break;
      case PWMOUTPUT_MODE_AFTERNOON:
        return (myPwmOutputs[pwmOutputNumber].pwmOutputModeAfternoon);
        break;
      case PWMOUTPUT_MODE_EVENING:
        return (myPwmOutputs[pwmOutputNumber].pwmOutputModeEvening);
        break;
      case PWMOUTPUT_MODE_NIGHT:
        return (myPwmOutputs[pwmOutputNumber].pwmOutputModeNight);
        break;
      case PWMOUTPUT_MANUAL_ONOFF:
        return (myPwmOutputs[pwmOutputNumber].pwmOutputManualOnOff);
        break;
  }
}

