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

  byte pwmOutputPin = 0;
  byte digitalPin = 0;

  
  byte pwmOutputControlMode = CONTROL_MODE_MANUAL;

  uint16_t pwmOutputModeMorning = 0;
  uint16_t pwmOutputModeAfternoon = 0;
  uint16_t pwmOutputModeEvening = 0;
  uint16_t pwmOutputModeNight = 0;
  uint16_t pwmOutputManualMode = 0;

  
  byte pwmOutputLastPartOfDay = SCHEDULER_MODE_INITIAL;
  uint16_t pwmOutputValue = 0;
  uint16_t regulatedValue = 0;
  bool thresholdInitialized = false;

  
  uint16_t pwmOutputLastpwmOutputValue = 0;

  PID pwmOutputPID;
  double pwmOutputPIDInput = NAN;
  double pwmOutputPIDOutput = 0;
  double pwmOutputSensorsSetpoint = 0;
  double pwmOutputPIDKp = 0;
  double pwmOutputPIDKi = 0;
  double pwmOutputPIDKd = 0;
  byte pwmOutputDirection = CONTROL_DIRECT;

  float maxDeviation = 0;
  bool tresholdDirection = false;

  double lastPwmOutputPIDInput = NAN;
  unsigned long lastValueSendMillis = 0;
  
  void Init(byte pin) {
    switch (pin) {
      case 0: digitalPin = DIGITAL_PWM_OUT_PIN_1;
            break;
      case 1: digitalPin = DIGITAL_PWM_OUT_PIN_2;
            break;
      case 2: digitalPin = DIGITAL_PWM_OUT_PIN_3;
            break;
      case 3: digitalPin = DIGITAL_PWM_OUT_PIN_4;
            break;
      case 4: digitalPin = DIGITAL_PWM_12V_OUT_PIN_1;
            break;
      case 5: digitalPin = DIGITAL_PWM_12V_OUT_PIN_2;
            break;
    }
    pinMode(digitalPin, OUTPUT);
    pwmWrite16(digitalPin,0);
    
    pwmOutputPin = pin+1;
    
    pwmOutputPID.Init(&pwmOutputPIDInput,&pwmOutputPIDOutput,&pwmOutputSensorsSetpoint,pwmOutputPIDKp,pwmOutputPIDKi,pwmOutputPIDKd,PID_P_ON_E,pwmOutputDirection);
    pwmOutputPID.SetOutputLimits(0,PWM_MAX);
    pwmOutputPID.SetSampleTime(ANALOG_CONTROL_INTERVAL_MS);
    pwmOutputPID.SetMode(PID_MANUAL);

    pwmOutputApplyValue();

  }

  void pwmOutputReportAll() {
    reportField(pwmField(pwmOutputPin,PWMOutputField::Input),sensorsOutputInputString(pwmOutputPin,OUTPUT_TYPE_PWM));

    reportField(pwmField(pwmOutputPin,PWMOutputField::SensorsValue),floatToString(sensorsGetSensorsValue(pwmOutputPin,OUTPUT_TYPE_PWM)));
    reportField(pwmField(pwmOutputPin,PWMOutputField::Name),readConfigName(CONFIG_pwm_outputs_addr+(pwmOutputPin-1)*PWMOUTPUTS_PWMOUTPUT_CONFIG_BYTES,NAME_LENGTH));
    reportField(pwmField(pwmOutputPin,PWMOutputField::MorningMode),longToString(pwmOutputModeMorning));
    reportField(pwmField(pwmOutputPin,PWMOutputField::AfternoonMode),longToString(pwmOutputModeAfternoon));
    reportField(pwmField(pwmOutputPin,PWMOutputField::EveningMode),longToString(pwmOutputModeEvening));
    reportField(pwmField(pwmOutputPin,PWMOutputField::NightMode),longToString(pwmOutputModeNight));
    reportField(pwmField(pwmOutputPin,PWMOutputField::ManualMode),longToString(pwmOutputManualMode));
    reportField(pwmField(pwmOutputPin,PWMOutputField::ControlMode),getStringControlModeFromValue(pwmOutputControlMode));
    reportField(pwmField(pwmOutputPin,PWMOutputField::Value),longToString(pwmOutputValue));
    if (pwmOutputDirection==CONTROL_DIRECT) {
      reportField(pwmField(pwmOutputPin,PWMOutputField::ControlDirection),setBufferFromFlash(charDirect));
    }
    else {
      reportField(pwmField(pwmOutputPin,PWMOutputField::ControlDirection),setBufferFromFlash(charReverse));
    }
    reportField(pwmField(pwmOutputPin,PWMOutputField::SensorsSetpoint),floatToString(pwmOutputSensorsSetpoint));
    reportField(pwmField(pwmOutputPin,PWMOutputField::PidKp),doubleToString(pwmOutputPIDKp));
    reportField(pwmField(pwmOutputPin,PWMOutputField::PidKi),doubleToString(pwmOutputPIDKi));
    reportField(pwmField(pwmOutputPin,PWMOutputField::PidKd),doubleToString(pwmOutputPIDKd));

    reportField(pwmField(pwmOutputPin,PWMOutputField::MaxDeviation),floatToString(maxDeviation));
    


  }

  void pwmOutputApplyValue() {
    pwmOutputPIDInput = sensorsGetSensorsValue(pwmOutputPin,OUTPUT_TYPE_PWM);
    bool inputValid = isfinite(pwmOutputPIDInput);
    byte actualPartOfDay = schedulerGetActualPartOfDay();
    uint16_t value = regulatedValue;
    bool controlled = pwmOutputControlMode == CONTROL_MODE_PID || pwmOutputControlMode == CONTROL_MODE_TRESHOLD;
    if (pwmOutputControlMode == CONTROL_MODE_PART_OF_DAY) {
      if (actualPartOfDay == SCHEDULER_MODE_MORNING) value = pwmOutputModeMorning;
      else if (actualPartOfDay == SCHEDULER_MODE_AFTERNOON) value = pwmOutputModeAfternoon;
      else if (actualPartOfDay == SCHEDULER_MODE_EVENING) value = pwmOutputModeEvening;
      else value = pwmOutputModeNight;
      if (pwmOutputLastPartOfDay != actualPartOfDay) {
        pwmOutputLastPartOfDay = actualPartOfDay;

      }
    }
    else if (pwmOutputControlMode == CONTROL_MODE_MANUAL) value = pwmOutputManualMode;
    bool healthy=megaConfigReady && pwmOutputPin<=megaFeatures.pwm && (!controlled || inputValid);
    if(!healthy)value=0;
    pwmOutputValue=value;
    pwmOutputSetValue(value);
    if (pwmOutputControlMode == CONTROL_MODE_PID && pwmOutputPID.GetMode() == PID_MANUAL)
      pwmOutputPIDOutput = value; //Track the actual output for a bumpless return to automatic mode.

    if (millis()-lastValueSendMillis >= TELEMETRY_MIN_INTERVAL_MS) {
      lastValueSendMillis = millis();
      if (lastPwmOutputPIDInput != pwmOutputPIDInput && (isfinite(lastPwmOutputPIDInput) || isfinite(pwmOutputPIDInput))) {
        lastPwmOutputPIDInput = pwmOutputPIDInput;
        reportField(pwmField(pwmOutputPin,PWMOutputField::SensorsValue),floatToString(pwmOutputPIDInput));
      }
      if (pwmOutputLastpwmOutputValue != value) {
        pwmOutputLastpwmOutputValue = value;
        reportField(pwmField(pwmOutputPin,PWMOutputField::Value),longToString(value));
      }
    }
  }

  void pwmOutputSetValue(uint16_t pwmOutputVal) {
    pwmWrite16(digitalPin,pwmOutputVal);
  }
  
  void saveConfig(int CONFIG_addr) {

    configStorage.update(CONFIG_addr+NAME_LENGTH+1,pwmOutputControlMode);
    configStorage.update(CONFIG_addr+NAME_LENGTH+2,byte(pwmOutputModeMorning));
    configStorage.update(CONFIG_addr+NAME_LENGTH+3,byte(pwmOutputModeAfternoon));
    configStorage.update(CONFIG_addr+NAME_LENGTH+4,byte(pwmOutputModeEvening));
    configStorage.update(CONFIG_addr+NAME_LENGTH+5,byte(pwmOutputModeNight));
    configStorage.update(CONFIG_addr+NAME_LENGTH+6,byte(pwmOutputManualMode));
    uint16_t high=0;
    high |= ((pwmOutputModeMorning>>8)&3)<<0;
    high |= ((pwmOutputModeAfternoon>>8)&3)<<2;
    high |= ((pwmOutputModeEvening>>8)&3)<<4;
    high |= ((pwmOutputModeNight>>8)&3)<<6;
    high |= ((pwmOutputManualMode>>8)&3)<<8;
    //Upper six bits use reserved padding; payload size and existing offsets stay fixed.
    byte channel=(CONFIG_addr-CONFIG_pwm_outputs_addr)/PWMOUTPUTS_PWMOUTPUT_CONFIG_BYTES;
    uint32_t upper=uint32_t(pwmOutputModeMorning>>10) | (uint32_t(pwmOutputModeAfternoon>>10)<<6) | (uint32_t(pwmOutputModeEvening>>10)<<12) | (uint32_t(pwmOutputModeNight>>10)<<18) | (uint32_t(pwmOutputManualMode>>10)<<24);
    high|=uint16_t(upper&63)<<10;
    configStorage.update(CONFIG_addr+NAME_LENGTH,byte(upper>>6));
    configStorage.update(CONFIG_addr+43,byte(upper>>14));
    configStorage.update(CONFIG_sensors_addr+channel*SENSORS_SENSOR_CONFIG_BYTES+33,byte(upper>>22));
    configStorage.update(CONFIG_sensors_addr+channel*SENSORS_SENSOR_CONFIG_BYTES+34,0x16);
    configStorage.put(CONFIG_pwm_highbits_addr+((CONFIG_addr-CONFIG_pwm_outputs_addr)/PWMOUTPUTS_PWMOUTPUT_CONFIG_BYTES)*2,high);
    configStorage.update(CONFIG_addr+NAME_LENGTH+7,pwmOutputDirection);
    configSaveDoubleValue(pwmOutputSensorsSetpoint,CONFIG_addr+NAME_LENGTH+8);
    configSaveDoubleValue(pwmOutputPIDKp,CONFIG_addr+NAME_LENGTH+12);
    configSaveDoubleValue(pwmOutputPIDKi,CONFIG_addr+NAME_LENGTH+16);
    configSaveDoubleValue(pwmOutputPIDKd,CONFIG_addr+NAME_LENGTH+20);
    configSaveFloatValue(maxDeviation,CONFIG_addr+NAME_LENGTH+24);
  }
  
  void loadConfig(int CONFIG_addr) {

    pwmOutputControlMode = configGetValue(CONFIG_addr+NAME_LENGTH+1);
    uint16_t high;
    configStorage.get(CONFIG_pwm_highbits_addr+((CONFIG_addr-CONFIG_pwm_outputs_addr)/PWMOUTPUTS_PWMOUTPUT_CONFIG_BYTES)*2,high);
    if(high==UINT16_MAX && configStorage.read(CONFIG_sensors_addr+((CONFIG_addr-CONFIG_pwm_outputs_addr)/PWMOUTPUTS_PWMOUTPUT_CONFIG_BYTES)*SENSORS_SENSOR_CONFIG_BYTES+34)!=0x16)high=0;
    pwmOutputModeMorning=configGetValue(CONFIG_addr+NAME_LENGTH+2) | (((high>>0)&3)<<8);
    pwmOutputModeAfternoon=configGetValue(CONFIG_addr+NAME_LENGTH+3) | (((high>>2)&3)<<8);
    pwmOutputModeEvening=configGetValue(CONFIG_addr+NAME_LENGTH+4) | (((high>>4)&3)<<8);
    pwmOutputModeNight=configGetValue(CONFIG_addr+NAME_LENGTH+5) | (((high>>6)&3)<<8);
    pwmOutputManualMode=configGetValue(CONFIG_addr+NAME_LENGTH+6) | (((high>>8)&3)<<8);
    byte channel=(CONFIG_addr-CONFIG_pwm_outputs_addr)/PWMOUTPUTS_PWMOUTPUT_CONFIG_BYTES;
    if(configStorage.read(CONFIG_sensors_addr+channel*SENSORS_SENSOR_CONFIG_BYTES+34)==0x16){
      uint32_t upper=(high>>10) | (uint32_t(configStorage.read(CONFIG_addr+NAME_LENGTH))<<6) | (uint32_t(configStorage.read(CONFIG_addr+43))<<14) | (uint32_t(configStorage.read(CONFIG_sensors_addr+channel*SENSORS_SENSOR_CONFIG_BYTES+33))<<22);
      pwmOutputModeMorning|=uint16_t(upper&63)<<10;
      pwmOutputModeAfternoon|=uint16_t((upper>>6)&63)<<10;
      pwmOutputModeEvening|=uint16_t((upper>>12)&63)<<10;
      pwmOutputModeNight|=uint16_t((upper>>18)&63)<<10;
      pwmOutputManualMode|=uint16_t((upper>>24)&63)<<10;
    }
    pwmOutputDirection = configGetValue(CONFIG_addr+NAME_LENGTH+7);
    pwmOutputSensorsSetpoint = configGetDoubleValue(CONFIG_addr+NAME_LENGTH+8);
    pwmOutputPIDKp = configGetDoubleValue(CONFIG_addr+NAME_LENGTH+12);
    pwmOutputPIDKi = configGetDoubleValue(CONFIG_addr+NAME_LENGTH+16);
    pwmOutputPIDKd = configGetDoubleValue(CONFIG_addr+NAME_LENGTH+20);
    maxDeviation = configGetFloatValue(CONFIG_addr+NAME_LENGTH+24);
    if (pwmOutputControlMode > CONTROL_MODE_PID) {
      pwmOutputControlMode = CONTROL_MODE_MANUAL;
      pwmOutputManualMode = 0;
      pwmOutputModeMorning = pwmOutputModeAfternoon = pwmOutputModeEvening = pwmOutputModeNight = 0;
    }
    if (pwmOutputDirection > PID_REVERSE) pwmOutputDirection = CONTROL_DIRECT;
    if (!isfinite(pwmOutputSensorsSetpoint)) { pwmOutputSensorsSetpoint = 0; pwmOutputControlMode = CONTROL_MODE_MANUAL; }
    if (!isfinite(pwmOutputPIDKp) || pwmOutputPIDKp < 0) pwmOutputPIDKp = 0;
    if (!isfinite(pwmOutputPIDKi) || pwmOutputPIDKi < 0) pwmOutputPIDKi = 0;
    if (!isfinite(pwmOutputPIDKd) || pwmOutputPIDKd < 0) pwmOutputPIDKd = 0;
    if (!isfinite(maxDeviation) || maxDeviation < 0) maxDeviation = 0;
    thresholdInitialized = false;
  }

  void ControlEvent() {
    bool controlled = pwmOutputControlMode == CONTROL_MODE_PID || pwmOutputControlMode == CONTROL_MODE_TRESHOLD;
    if (controlled) pwmOutputPIDInput = sensorsGetSensorsValue(pwmOutputPin, OUTPUT_TYPE_PWM);
    bool healthy = controlled && isfinite(pwmOutputPIDInput);
    bool automatic = healthy && pwmOutputControlMode == CONTROL_MODE_PID;
    pwmOutputPID.SetMode(automatic ? PID_AUTOMATIC : PID_MANUAL);
    if (controlled && !healthy) {
      regulatedValue = 0;
      thresholdInitialized = false;
      pwmOutputPIDOutput = 0;
    }
    else if (pwmOutputControlMode == CONTROL_MODE_PID) {
      if (automatic) {
        pwmOutputPID.Compute();
        regulatedValue = isfinite(pwmOutputPIDOutput) ? (uint16_t)constrain(pwmOutputPIDOutput, 0.0, (double)PWM_MAX) : 0;
      }
    }
    else if (pwmOutputControlMode == CONTROL_MODE_TRESHOLD) {
      if (!thresholdInitialized) {
        tresholdDirection = pwmOutputPIDInput < pwmOutputSensorsSetpoint;
        thresholdInitialized = true;
      }
      if (pwmOutputPIDInput > pwmOutputSensorsSetpoint+maxDeviation) tresholdDirection = false;
      else if (pwmOutputPIDInput < pwmOutputSensorsSetpoint-maxDeviation) tresholdDirection = true;
      regulatedValue = (tresholdDirection == (pwmOutputDirection == CONTROL_DIRECT)) ? PWM_MAX : 0;
    }
    pwmOutputApplyValue();
  }

};

static pwmOutputClass myPwmOutputs[PWMOUTPUTS_COUNT];

void pwmOutputsInit() {
 for (byte i=0; i<PWMOUTPUTS_COUNT; i++) {
    myPwmOutputs[i].Init(i);
  }
}

void pwmOutputsReportAll() {
  for (byte i=0; i<PWMOUTPUTS_COUNT; i++) {
    myPwmOutputs[i].pwmOutputReportAll();
  }
}

void pwmOutputsMillisEvent() {
  for (byte i=0; i<PWMOUTPUTS_COUNT; i++) {
    myPwmOutputs[i].pwmOutputApplyValue();
  }
}

void pwmOutputsControlEvent() {
  for (byte i=0; i<megaFeatures.pwm; i++) {
    myPwmOutputs[i].ControlEvent();
  }
}


void pwmOutputsLoadConfig() {
  for (byte pwmOutputNumber=0; pwmOutputNumber<PWMOUTPUTS_COUNT; pwmOutputNumber++) {
    myPwmOutputs[pwmOutputNumber].loadConfig(CONFIG_pwm_outputs_addr+(pwmOutputNumber*PWMOUTPUTS_PWMOUTPUT_CONFIG_BYTES));
  }
}

void pwmOutputsSaveConfig(byte pwmOutputNumber) {
  myPwmOutputs[pwmOutputNumber].saveConfig(CONFIG_pwm_outputs_addr+(pwmOutputNumber*PWMOUTPUTS_PWMOUTPUT_CONFIG_BYTES));
}

void pwmOutputsSavePwmOutputName(byte pwmOutputNr, String Value) {
  byte pwmOutputNumber = pwmOutputNr-1;
  configSaveString(Value,CONFIG_pwm_outputs_addr+(pwmOutputNumber*PWMOUTPUTS_PWMOUTPUT_CONFIG_BYTES),NAME_LENGTH);
}

void pwmOutputsCheckTresholdDirection(byte pwmOutputNr, byte outputType) {
  if (pwmOutputNr == 0 || pwmOutputNr > PWMOUTPUTS_COUNT) return;
  float input = sensorsGetSensorsValue(pwmOutputNr, outputType);
  myPwmOutputs[pwmOutputNr-1].thresholdInitialized = isfinite(input);
  if (isfinite(input)) myPwmOutputs[pwmOutputNr-1].tresholdDirection =
    input < myPwmOutputs[pwmOutputNr-1].pwmOutputSensorsSetpoint;
}

void pwmOutputsSetPwmOutput(byte pwmOutputNr, byte valueType, uint16_t Value) {
  if (pwmOutputNr == 0 || pwmOutputNr > PWMOUTPUTS_COUNT) return;
  byte pwmOutputNumber = pwmOutputNr-1;
  switch (valueType) {
      case CONTROL_TRESHOLD_DIRECTION:
        myPwmOutputs[pwmOutputNumber].thresholdInitialized = true;
        if (Value==0) {
          myPwmOutputs[pwmOutputNumber].tresholdDirection = true;
        }
        else {
          myPwmOutputs[pwmOutputNumber].tresholdDirection = false;
        }
        break;
      case PWMOUTPUT_CONTROL_MODE:
        if (Value > CONTROL_MODE_PID) return;
        myPwmOutputs[pwmOutputNumber].pwmOutputPID.SetMode(PID_MANUAL);
        myPwmOutputs[pwmOutputNumber].pwmOutputPIDOutput = myPwmOutputs[pwmOutputNumber].pwmOutputValue;
        myPwmOutputs[pwmOutputNumber].regulatedValue = 0;
        myPwmOutputs[pwmOutputNumber].thresholdInitialized = false;
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
      case PWMOUTPUT_CONTROL_DIRECTION:
        if (Value > PID_REVERSE) return;
        myPwmOutputs[pwmOutputNumber].pwmOutputDirection = Value;
        myPwmOutputs[pwmOutputNumber].pwmOutputPID.SetControllerDirection(Value);
        break;
  }
  if (valueType == CONTROL_TRESHOLD_DIRECTION) return;
  pwmOutputsSaveConfig(pwmOutputNumber);
  myPwmOutputs[pwmOutputNumber].pwmOutputApplyValue();
}

uint16_t pwmOutputsGetPwmOutput(byte pwmOutputNr, byte valueType) {
  if (pwmOutputNr == 0 || pwmOutputNr > PWMOUTPUTS_COUNT) return 0;
  byte pwmOutputNumber= pwmOutputNr-1;
  switch (valueType) {
      case PWMOUTPUT_PID_VALID: return myPwmOutputs[pwmOutputNumber].pwmOutputPID.GetArithmeticValid();

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
      case PWMOUTPUT_CONTROL_DIRECTION:
        return (myPwmOutputs[pwmOutputNumber].pwmOutputDirection);
        break;
      case PWMOUTPUT_OUTPUT_VALUE:
        return (myPwmOutputs[pwmOutputNumber].pwmOutputValue);
        break;
  }
  return 0;
}

void pwmOutputsSetPwmOutputDouble(byte pwmOutputNr, byte valueType, double Value) {
  if (pwmOutputNr == 0 || pwmOutputNr > PWMOUTPUTS_COUNT || !isfinite(Value)) return;
  if ((valueType == PWMOUTPUT_MAX_DEVIATION || valueType == PWMOUTPUT_PID_KP ||
       valueType == PWMOUTPUT_PID_KI || valueType == PWMOUTPUT_PID_KD) && Value < 0) return;
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
  if (valueType == CONTROL_TRESHOLD_DIRECTION) return;
  pwmOutputsSaveConfig(pwmOutputNumber);
  myPwmOutputs[pwmOutputNumber].pwmOutputApplyValue();
}

double pwmOutputsGetPwmOutputDouble(byte pwmOutputNr, byte valueType) {
  if (pwmOutputNr == 0 || pwmOutputNr > PWMOUTPUTS_COUNT) return NAN;
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
  return 0;
}

//Spread complete snapshots across channels to keep acquisition and regulation running.
void pwmOutputsReportChannel(byte channel) {
  if (channel < PWMOUTPUTS_COUNT) myPwmOutputs[channel].pwmOutputReportAll();
}

void pwmOutputsInputChanged(byte channel){if(!channel||channel>PWMOUTPUTS_COUNT)return;auto& pwm=myPwmOutputs[channel-1];pwm.thresholdInitialized=false;pwm.regulatedValue=0;pwm.pwmOutputPIDOutput=0;pwm.pwmOutputPID.SetMode(PID_MANUAL);pwm.ControlEvent();}
