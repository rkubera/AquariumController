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

  byte pwmOutputModeMorning = 0;
  byte pwmOutputModeAfternoon = 0;
  byte pwmOutputModeEvening = 0;
  byte pwmOutputModeNight = 0;
  byte pwmOutputManualMode = 0;

  byte pwmOutputManualOnOff = PWMOUTPUT_MANUAL_ONOFF_AUTO;
  
  byte pwmOutputLastMode;
  byte pwmOutputLastPartOfDay;
  byte pwmOutputValue;
  
  bool pwmOutputLastpwmOutputState;
  byte pwmOutputLastpwmOutputValue;

  PID pwmOutputPID;
  double pwmOutputPIDInput;
  double pwmOutputPIDOutput;
  double pwmOutputPIDSetpoint;
  double pwmOutputPIDKp;
  double pwmOutputPIDKi;
  double pwmOutputPIDKd;
  byte pwmOutputDirection;
  
  void Init(byte pin) {
    switch (pin) {
      case 0: digitalPin = DIGITAL_PWM_OUT_PIN_1;
            break;
      case 1: digitalPin = DIGITAL_PWM_OUT_PIN_2;
            break;
      case 2: digitalPin = DIGITAL_PWM_OUT_PIN_3;
            break;
      case 3: digitalPin = DIGITAL_PWM_12V_OUT_PIN_1;
            break;
      case 4: digitalPin = DIGITAL_PWM_12V_OUT_PIN_2;
            break;
    }
    pinMode(digitalPin, OUTPUT);
    analogWrite(digitalPin,0);
    pwmOutputPin = pin+1;
    
    pwmOutputPID.Init(&pwmOutputPIDInput,&pwmOutputPIDOutput,&pwmOutputPIDSetpoint,pwmOutputPIDKp,pwmOutputPIDKi,pwmOutputPIDKd,PID_P_ON_E,pwmOutputDirection);
    pwmOutputPID.SetSampleTime(1000);
    pwmOutputPID.SetMode(PID_AUTOMATIC);
  }

  void pwmOutputMqttPublishAll() {
    mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charMorningMode),intToString(pwmOutputModeMorning));
    mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charAfternoonMode),intToString(pwmOutputModeAfternoon));
    mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charEveningMode),intToString(pwmOutputModeEvening));
    mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charNightMode),intToString(pwmOutputModeNight));
    mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charManualMode),intToString(pwmOutputManualMode));
    mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charState),pwmOutputGetStringValueFromBool(pwmOutputLastpwmOutputState));
    mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charControlMode),getStringControlModeFromValue(pwmOutputControlMode));
    mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charValue),intToString(pwmOutputValue));
    if (pwmOutputDirection==PID_DIRECT) {
      mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charDirection),setBufferFromFlash(charDirect));
    }
    else {
      mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charDirection),setBufferFromFlash(charReverse));
    }
    mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charPidSetpoint),intToString((int)pwmOutputPIDSetpoint));
    mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charPidKp),doubleToString(pwmOutputPIDKp));
    mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charPidKi),doubleToString(pwmOutputPIDKi));
    mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charPidKd),doubleToString(pwmOutputPIDKd));
    
    pwmOutputSetState();
  }

  void pwmOutputSetState() {
    byte actualPartOfDay = schedulerGetActualPartOfDay();
    bool pwmOutputUpDown = pwmOutputLastpwmOutputState;
      
    if (pwmOutputControlMode == CONTROL_MODE_PART_OF_DAY) {
      if (actualPartOfDay==SCHEDULER_MODE_MORNING) {
        pwmOutputValue = pwmOutputModeMorning;
      }
      else if (actualPartOfDay==SCHEDULER_MODE_AFTERNOON) {
        pwmOutputValue = pwmOutputModeAfternoon;
      }
      else if (actualPartOfDay==SCHEDULER_MODE_EVENING) {
        pwmOutputValue = pwmOutputModeEvening;
      }
      else {
        pwmOutputValue = pwmOutputModeNight;
      }

      if (pwmOutputLastPartOfDay!=actualPartOfDay) {
        pwmOutputLastPartOfDay = actualPartOfDay;
        pwmOutputManualOnOff=PWMOUTPUT_MANUAL_ONOFF_AUTO;
      }
    }
    else if (pwmOutputControlMode == CONTROL_MODE_MANUAL) {
      pwmOutputValue = pwmOutputManualMode;
    }

    if (pwmOutputManualOnOff == PWMOUTPUT_MANUAL_ONOFF_ON) {
      pwmOutputUpDown = true;
    }
    
    if (pwmOutputManualOnOff == PWMOUTPUT_MANUAL_ONOFF_OFF) {
      pwmOutputValue = 0;
      pwmOutputUpDown = false;
    }

    if (pwmOutputValue>0) {
      pwmOutputUpDown = true;
    }
    else {
      pwmOutputUpDown = false;
    }
    
    if (pwmOutputLastpwmOutputValue!=pwmOutputValue) {
      pwmOutputLastpwmOutputValue = pwmOutputValue;
      pwmOutputSetValue(pwmOutputValue);
      if (pwmOutputUpDown==true) {
        mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charState),setBufferFromFlash(charOn));
      }
      else {
        mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charState),setBufferFromFlash(charOff));
      }
      mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charValue),intToString(pwmOutputValue));
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

  void pwmOutputSetValue(byte pwmOutputMode) {
    analogWrite(digitalPin,pwmOutputMode);
  }
  
  void saveConfig(int EEPROM_addr) {
    EEPROM.write(EEPROM_addr+NAME_LENGHTH+1,pwmOutputControlMode);
    EEPROM.write(EEPROM_addr+NAME_LENGHTH+2,pwmOutputModeMorning);
    EEPROM.write(EEPROM_addr+NAME_LENGHTH+3,pwmOutputModeAfternoon);
    EEPROM.write(EEPROM_addr+NAME_LENGHTH+4,pwmOutputModeEvening);
    EEPROM.write(EEPROM_addr+NAME_LENGHTH+5,pwmOutputModeNight);
    EEPROM.write(EEPROM_addr+NAME_LENGHTH+6,pwmOutputManualMode);
    EEPROM.write(EEPROM_addr+NAME_LENGHTH+7,pwmOutputDirection);
    configSaveDoubleValue(pwmOutputPIDSetpoint,EEPROM_addr+NAME_LENGHTH+7+sizeof(double));
    configSaveDoubleValue(pwmOutputPIDKp,EEPROM_addr+NAME_LENGHTH+7+(2*sizeof(double)));
    configSaveDoubleValue(pwmOutputPIDKi,EEPROM_addr+NAME_LENGHTH+7+(3*sizeof(double)));
    configSaveDoubleValue(pwmOutputPIDKd,EEPROM_addr+NAME_LENGHTH+7+(4*sizeof(double)));
  }
  
  void loadConfig(int EEPROM_addr) {
    pwmOutputControlMode = configGetValue(EEPROM_addr+NAME_LENGHTH+1);
    pwmOutputModeMorning = configGetValue(EEPROM_addr+NAME_LENGHTH+2);
    pwmOutputModeAfternoon = configGetValue(EEPROM_addr+NAME_LENGHTH+3);
    pwmOutputModeEvening = configGetValue(EEPROM_addr+NAME_LENGHTH+4);
    pwmOutputModeNight = configGetValue(EEPROM_addr+NAME_LENGHTH+5);
    pwmOutputManualMode = configGetValue(EEPROM_addr+NAME_LENGHTH+6);
    pwmOutputDirection = configGetValue(EEPROM_addr+NAME_LENGHTH+7);
    pwmOutputPIDSetpoint = configGetDoubleValue(EEPROM_addr+NAME_LENGHTH+7+sizeof(double));
    pwmOutputPIDKp = configGetDoubleValue(EEPROM_addr+NAME_LENGHTH+7+(2*sizeof(double)));
    pwmOutputPIDKi = configGetDoubleValue(EEPROM_addr+NAME_LENGHTH+7+(2*sizeof(double)));
    pwmOutputPIDKd = configGetDoubleValue(EEPROM_addr+NAME_LENGHTH+7+(2*sizeof(double)));
  }

  void PIDEvent() {
    if (pwmOutputControlMode == CONTROL_MODE_PID) {
      pwmOutputPID.Compute();
      pwmOutputPID.SetOutputLimits(0,255);
      pwmOutputSetValue(pwmOutputValue);
      pwmOutputManualOnOff=PWMOUTPUT_MANUAL_ONOFF_AUTO;
      if (pwmOutputLastpwmOutputState==false) {
        pwmOutputLastpwmOutputState = true;
        mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charState),setBufferFromFlash(charOn));
      }
      mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charValue),intToString(pwmOutputValue));
    }
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

void pwmOutputsSecondEvent() {
  for (byte i=0; i<PWMOUTPUTS_COUNT; i++) {
    myPwmOutputs[i].PIDEvent();
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
      case PWMOUTPUT_MANUAL_MODE:
        myPwmOutputs[pwmOutputNumber].pwmOutputManualMode = Value;
        break;      
      case PWMOUTPUT_MANUAL_ONOFF:
        myPwmOutputs[pwmOutputNumber].pwmOutputManualOnOff = Value;
        break;
      case PWMOUTPUT_PID_DIRECTION:
        myPwmOutputs[pwmOutputNumber].pwmOutputDirection = Value;
        myPwmOutputs[pwmOutputNumber].pwmOutputPID.SetControllerDirection(Value);
        break;
      case PWMOUTPUT_PID_SETPOINT:
        myPwmOutputs[pwmOutputNumber].pwmOutputPIDSetpoint = Value;
        break;
      case PWMOUTPUT_PID_KP:
        myPwmOutputs[pwmOutputNumber].pwmOutputPIDKp = Value;
        myPwmOutputs[pwmOutputNumber].pwmOutputPID.SetTunings(myPwmOutputs[pwmOutputNumber].pwmOutputPIDKp, myPwmOutputs[pwmOutputNumber].pwmOutputPIDKi, myPwmOutputs[pwmOutputNumber].pwmOutputPIDKd);
        break;
      case PWMOUTPUT_PID_KI:
        myPwmOutputs[pwmOutputNumber].pwmOutputPIDKi = Value;
        myPwmOutputs[pwmOutputNumber].pwmOutputPID.SetTunings(myPwmOutputs[pwmOutputNumber].pwmOutputPIDKp, myPwmOutputs[pwmOutputNumber].pwmOutputPIDKi, myPwmOutputs[pwmOutputNumber].pwmOutputPIDKd);
        break;
      case PWMOUTPUT_PID_KD:
        myPwmOutputs[pwmOutputNumber].pwmOutputPIDKd = Value;
        myPwmOutputs[pwmOutputNumber].pwmOutputPID.SetTunings(myPwmOutputs[pwmOutputNumber].pwmOutputPIDKp, myPwmOutputs[pwmOutputNumber].pwmOutputPIDKi, myPwmOutputs[pwmOutputNumber].pwmOutputPIDKd);
        break;
  }
  pwmOutputsSaveConfig(pwmOutputNumber);
}

byte pwmOutputsGetPwmOutput(byte pwmOutputNr, byte valueType) {
  byte pwmOutputNumber= pwmOutputNr-1;
  switch (valueType) {
      case PWMOUTPUT_STATE:
        return (myPwmOutputs[pwmOutputNumber].pwmOutputValue);
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
      case PWMOUTPUT_MANUAL_MODE:
        return (myPwmOutputs[pwmOutputNumber].pwmOutputManualMode);
        break;
      case PWMOUTPUT_MANUAL_ONOFF:
        return (myPwmOutputs[pwmOutputNumber].pwmOutputManualOnOff);
        break;
      case PWMOUTPUT_PID_DIRECTION:
        return (myPwmOutputs[pwmOutputNumber].pwmOutputDirection);
        break;
      case PWMOUTPUT_PID_SETPOINT:
        return (myPwmOutputs[pwmOutputNumber].pwmOutputPIDSetpoint);
        break;
      case PWMOUTPUT_PID_KP:
        return (myPwmOutputs[pwmOutputNumber].pwmOutputPIDKp);
        break;
      case PWMOUTPUT_PID_KI:
        return (myPwmOutputs[pwmOutputNumber].pwmOutputPIDKi);
        break;
      case PWMOUTPUT_PID_KD:
        return (myPwmOutputs[pwmOutputNumber].pwmOutputPIDKd);
        break;
  }
}
