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
  int pwmLastOutputValue;
  
  bool pwmOutputLastpwmOutputState;
  byte pwmOutputLastpwmOutputValue;

  PID pwmOutputPID;
  double pwmOutputPIDInput;
  double pwmOutputPIDOutput;
  double pwmOutputSensorsSetpoint;
  double pwmOutputPIDKp;
  double pwmOutputPIDKi;
  double pwmOutputPIDKd;
  byte pwmOutputDirection;

  float maxDeviation;
  bool tresholdDirection = false;

  double lastPwmOutputPIDInput;
  double lastValueSendMillis;
  
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
    
    pwmOutputPID.Init(&pwmOutputPIDInput,&pwmOutputPIDOutput,&pwmOutputSensorsSetpoint,pwmOutputPIDKp,pwmOutputPIDKi,pwmOutputPIDKd,PID_P_ON_E,pwmOutputDirection);
    pwmOutputPID.SetSampleTime(1000);
    pwmOutputPID.SetMode(PID_AUTOMATIC);

    pwmOutputSetState();
    pwmOutputsCheckTresholdDirection(pwmOutputPin, OUTPUT_TYPE_PWM);
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
    if (pwmOutputDirection==CONTROL_DIRECT) {
      mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charControlDirection),setBufferFromFlash(charDirect));
    }
    else {
      mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charControlDirection),setBufferFromFlash(charReverse));
    }
    mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charSensorsSetpoint),floatToString(pwmOutputSensorsSetpoint));
    mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charPidKp),doubleToString(pwmOutputPIDKp));
    mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charPidKi),doubleToString(pwmOutputPIDKi));
    mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charPidKd),doubleToString(pwmOutputPIDKd));

    mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charMaxDeviation),floatToString(maxDeviation));
    
    pwmOutputSetState();
    pwmOutputsCheckTresholdDirection(pwmOutputPin, OUTPUT_TYPE_PWM);
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
      pwmOutputManualOnOff = PWMOUTPUT_MANUAL_ONOFF_AUTO;
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
    pwmOutputSetValue(pwmOutputValue);

    if (abs(lastValueSendMillis-millis())>MQTT_MIN_REFRESH_MILLIS) {
      lastValueSendMillis = millis();
      if (lastPwmOutputPIDInput!=pwmOutputPIDInput) {
        lastPwmOutputPIDInput = pwmOutputPIDInput;
        mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charSensorsValue),floatToString(pwmOutputPIDInput));
      }
      
      if (pwmOutputLastpwmOutputValue!=pwmOutputValue) {
        pwmOutputLastpwmOutputValue = pwmOutputValue;
        if (pwmOutputUpDown==true) {
          mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charState),setBufferFromFlash(charOn));
        }
        else {
          mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charState),setBufferFromFlash(charOff));
        }
        mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charValue),intToString(pwmOutputValue));
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

  void pwmOutputSetValue(byte pwmOutputVal) {
    analogWrite(digitalPin,pwmOutputVal);
  }
  
  void saveConfig(int EEPROM_addr) {
    double mydouble;
    EEPROM.write(EEPROM_addr+NAME_LENGTH+1,pwmOutputControlMode);
    EEPROM.write(EEPROM_addr+NAME_LENGTH+2,pwmOutputModeMorning);
    EEPROM.write(EEPROM_addr+NAME_LENGTH+3,pwmOutputModeAfternoon);
    EEPROM.write(EEPROM_addr+NAME_LENGTH+4,pwmOutputModeEvening);
    EEPROM.write(EEPROM_addr+NAME_LENGTH+5,pwmOutputModeNight);
    EEPROM.write(EEPROM_addr+NAME_LENGTH+6,pwmOutputManualMode);
    EEPROM.write(EEPROM_addr+NAME_LENGTH+7,pwmOutputDirection);
    configSaveDoubleValue(pwmOutputSensorsSetpoint,EEPROM_addr+NAME_LENGTH+8);
    configSaveDoubleValue(pwmOutputPIDKp,EEPROM_addr+NAME_LENGTH+12);
    configSaveDoubleValue(pwmOutputPIDKi,EEPROM_addr+NAME_LENGTH+16);
    configSaveDoubleValue(pwmOutputPIDKd,EEPROM_addr+NAME_LENGTH+20);
    configSaveFloatValue(maxDeviation,EEPROM_addr+NAME_LENGTH+24);
  }
  
  void loadConfig(int EEPROM_addr) {
    double mydouble;
    pwmOutputControlMode = configGetValue(EEPROM_addr+NAME_LENGTH+1);
    pwmOutputModeMorning = configGetValue(EEPROM_addr+NAME_LENGTH+2);
    pwmOutputModeAfternoon = configGetValue(EEPROM_addr+NAME_LENGTH+3);
    pwmOutputModeEvening = configGetValue(EEPROM_addr+NAME_LENGTH+4);
    pwmOutputModeNight = configGetValue(EEPROM_addr+NAME_LENGTH+5);
    pwmOutputManualMode = configGetValue(EEPROM_addr+NAME_LENGTH+6);
    pwmOutputDirection = configGetValue(EEPROM_addr+NAME_LENGTH+7);
    pwmOutputSensorsSetpoint = configGetDoubleValue(EEPROM_addr+NAME_LENGTH+8);
    pwmOutputPIDKp = configGetDoubleValue(EEPROM_addr+NAME_LENGTH+12);
    pwmOutputPIDKi = configGetDoubleValue(EEPROM_addr+NAME_LENGTH+16);
    pwmOutputPIDKd = configGetDoubleValue(EEPROM_addr+NAME_LENGTH+20);
    maxDeviation = configGetFloatValue(EEPROM_addr+NAME_LENGTH+24);
  }

  void ControlEvent() {
    if (pwmOutputControlMode == CONTROL_MODE_PID) {
      double pwmOutputInput = sensorsGetSensorsValue(pwmOutputPin, OUTPUT_TYPE_PWM);
      pwmOutputPID.Compute();
      pwmOutputValue = pwmOutputPIDOutput;
      pwmOutputSetValue(pwmOutputValue);
      pwmOutputManualOnOff=PWMOUTPUT_MANUAL_ONOFF_AUTO;
  
      if (pwmOutputLastpwmOutputState==false) {
        pwmOutputLastpwmOutputState = true;
        mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charState),setBufferFromFlash(charOn));
      }
     
      if (pwmLastOutputValue!=pwmOutputValue) {
        pwmLastOutputValue = pwmOutputValue;
        mqttElPublish(setBufferFromFlash(charGetPwmOutput)+intToString(pwmOutputPin)+setBufferFromFlash(charValue),intToString(pwmOutputValue));
      }
    }
    else if (pwmOutputControlMode == CONTROL_MODE_TRESHOLD) {
      
      pwmOutputPIDInput = sensorsGetSensorsValue(pwmOutputPin, OUTPUT_TYPE_PWM);
      pwmOutputManualOnOff=PWMOUTPUT_MANUAL_ONOFF_AUTO;
      float minValue = pwmOutputSensorsSetpoint-maxDeviation;
      float maxValue = pwmOutputSensorsSetpoint+maxDeviation;
      if (tresholdDirection==true) {
        if (pwmOutputPIDInput>maxValue) {
          tresholdDirection==false;
        }
      }
      else {
        if (pwmOutputPIDInput<minValue) {
          tresholdDirection==true;
        }
      }

      if (tresholdDirection==true) {
        if (pwmOutputDirection==CONTROL_DIRECT) {
          pwmOutputValue = 255;
          pwmOutputSetValue(255);
        }
        else {
          pwmOutputValue = 0;
          pwmOutputSetValue(0);
        }
      }
      else {
        if (pwmOutputDirection==CONTROL_DIRECT) {
          pwmOutputValue = 0;
          pwmOutputSetValue(0);
        }
        else {
          pwmOutputValue = 255;
          pwmOutputSetValue(255);
        }
      }
    }
    
    if (pwmOutputControlMode != CONTROL_MODE_PID) {
      pwmLastOutputValue =-1;
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
    myPwmOutputs[i].ControlEvent();
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

void pwmOutputsCheckTresholdDirection (byte pwmOutputNr , byte outputType) {
  if (sensorsGetSensorsValue(pwmOutputNr, outputType)>pwmOutputsGetPwmOutputDouble(pwmOutputNr, PWMOUTPUT_SENSORS_SETPOINT)) {
    pwmOutputsSetPwmOutput(pwmOutputNr,CONTROL_TRESHOLD_DIRECTION,0);
  }
  else {
    pwmOutputsSetPwmOutput(pwmOutputNr,CONTROL_TRESHOLD_DIRECTION,1);
  } 
}

void pwmOutputsSetPwmOutput(byte pwmOutputNr, byte valueType, byte Value) {
  byte pwmOutputNumber = pwmOutputNr-1;
  switch (valueType) {
      case CONTROL_TRESHOLD_DIRECTION:
        if (Value==0) {
          myPwmOutputs[pwmOutputNumber].tresholdDirection = true;
        }
        else {
          myPwmOutputs[pwmOutputNumber].tresholdDirection = false;
        }
        break;
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
      case PWMOUTPUT_CONTROL_DIRECTION:
        myPwmOutputs[pwmOutputNumber].pwmOutputDirection = Value;
        myPwmOutputs[pwmOutputNumber].pwmOutputPID.SetControllerDirection(Value);
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
      case PWMOUTPUT_CONTROL_DIRECTION:
        return (myPwmOutputs[pwmOutputNumber].pwmOutputDirection);
        break;
      case PWMOUTPUT_OUTPUT_VALUE:
        return (myPwmOutputs[pwmOutputNumber].pwmOutputValue);
        return;
  }
}

void pwmOutputsSetPwmOutputDouble(byte pwmOutputNr, byte valueType, double Value) {
  byte pwmOutputNumber = pwmOutputNr-1;
  switch (valueType) {
    case PWMOUTPUT_MAX_DEVIATION:
        myPwmOutputs[pwmOutputNumber].maxDeviation = Value;
        break;    
    case PWMOUTPUT_SENSORS_SETPOINT:
        myPwmOutputs[pwmOutputNumber].pwmOutputSensorsSetpoint = Value;
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

double pwmOutputsGetPwmOutputDouble(byte pwmOutputNr, byte valueType) {
  byte pwmOutputNumber= pwmOutputNr-1;
  switch (valueType) {
    case PWMOUTPUT_MAX_DEVIATION:
        return (myPwmOutputs[pwmOutputNumber].maxDeviation);
        break;   
    case PWMOUTPUT_SENSORS_SETPOINT:
        return (myPwmOutputs[pwmOutputNumber].pwmOutputSensorsSetpoint);
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

