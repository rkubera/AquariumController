/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/

void ledInit() {
  
  ledWaveIdx = 0;
  
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_BLUE_PIN, OUTPUT);

  lastEventMillis = micros()/1000;

  ledSetBrightness(0);
  ledSetActualMode();
}

String ledColorValueToString (byte Value) {
  switch (Value) {
    case LED_MODE_BLACK:
        return setBufferFromFlash(charBlack);
        break;
    case LED_MODE_RED:
        return setBufferFromFlash(charRed);
        break;
    case LED_MODE_GREEN:
        return setBufferFromFlash(charGreen);
        break;
    case LED_MODE_BLUE:
        return setBufferFromFlash(charBlue);
        break;
    case LED_MODE_CYAN:
        return setBufferFromFlash(charCyan);
        break;
    case LED_MODE_MAGENTA:
        return setBufferFromFlash(charMagenta);
        break;
    case LED_MODE_YELLOW:
        return setBufferFromFlash(charYellow);
        break;
    case LED_MODE_WHITE:
        return setBufferFromFlash(charWhite);
        break;
    case LED_MODE_WAVE:
        return setBufferFromFlash(charWave);
        break;
    case LED_MODE_CUSTOM1:
        return setBufferFromFlash(charCustom1);
        break;
    case LED_MODE_CUSTOM2:
        return setBufferFromFlash(charCustom2);
        break;
  }
  return "";
}

void ledMqttPublishAll() {
  String actualControlMode;
  if (ledControlMode==CONTROL_MODE_PART_OF_DAY) {
    actualControlMode = setBufferFromFlash(charPartofday);
  }
  else if (ledControlMode==CONTROL_MODE_MANUAL) {
    actualControlMode = setBufferFromFlash(charManual);
  }
  
  mqttElPublish(setBufferFromFlash(getLedControlMode), actualControlMode);
  mqttElPublish(setBufferFromFlash(getMorningBrightness), String(ledMorningBrightness));
  mqttElPublish(setBufferFromFlash(getAfternoonBrightness), String(ledAfternoonBrightness));
  mqttElPublish(setBufferFromFlash(getEveningBrightness), String(ledEveningBrightness));
  mqttElPublish(setBufferFromFlash(getNightBrightness), String(ledNightBrightness));
  mqttElPublish(setBufferFromFlash(getManualBrightness), String(ledManualBrightness));
  mqttElPublish(setBufferFromFlash(getLedManualMode), ledColorValueToString(ledManualMode));
  mqttElPublish(setBufferFromFlash(getLedColorMorning), ledColorValueToString(ledModeMorning));
  mqttElPublish(setBufferFromFlash(getLedColorAfternoon), ledColorValueToString(ledModeAfternoon));
  mqttElPublish(setBufferFromFlash(getLedColorEvening), ledColorValueToString(ledModeEvening));
  mqttElPublish(setBufferFromFlash(getLedColorNight), ledColorValueToString(ledModeNight));
  mqttElPublish(setBufferFromFlash(getCustomColor1), longToString(ledCustomColor1));
  mqttElPublish(setBufferFromFlash(getCustomColor2), longToString(ledCustomColor2));
}

void ledSwitchToMode(int myMode) {
  switch (myMode) {
    case LED_MODE_BLACK:
        ledSetBlack();
        break;
    case LED_MODE_WHITE:
        ledSetWhite();
        break;
     case LED_MODE_RED:
        ledSetRed();
        break;
     case LED_MODE_GREEN:
        ledSetGreen();
        break;
     case LED_MODE_BLUE:
        ledSetBlue();
        break;
     case LED_MODE_WAVE:
        ledSetWave();
        break;
     case LED_MODE_CYAN:
        ledSetCyan();
        break;
     case LED_MODE_MAGENTA:
        ledSetMagenta();
        break;
     case LED_MODE_YELLOW:
        ledSetYellow();
        break;
     case LED_MODE_CUSTOM1:
        ledSetCustom(1);
        break;
    case LED_MODE_CUSTOM2:
        ledSetCustom(2);
        break;
  }
}

void ledSetActualMode() {
  //Part of day mode
  if (ledControlMode==CONTROL_MODE_PART_OF_DAY) {

    int ledModeNext;
    int ledModePrev;

    int nextHourMinute = 0;
    int prevHourMinute = 0;

    time_t local = clockGetLocalTime();
    
    int mysec = second(local);
    int mymin = minute(local);
    int myhour = hour(local);
  
    double nowHourMinute = mysec+(mymin*60)+(myhour*3600);
  
    double startMorningHourMinute = 0+((double)schedulerStartMorningMinute*60)+((double)schedulerStartMorningHour*3600);
    double startAfternoonHourMinute = 0+((double)schedulerStartAfternoonMinute*60)+((double)schedulerStartAfternoonHour*3600);
    double startEveningHourMinute = 0+((double)schedulerStartEveningMinute*60)+((double)schedulerStartEveningHour*3600);
    double startNightHourMinute = 0+((double)schedulerStartNightMinute*60)+((double)schedulerStartNightHour*3600);
    
    int actualPartOfDay = schedulerGetActualPartOfDay();
    
    if (actualPartOfDay==SCHEDULER_MODE_MORNING) {
      ledMode = ledModeMorning;
      ledModeNext = ledModeAfternoon;
      ledModePrev = ledModeNight;
      nextHourMinute = startAfternoonHourMinute;
      prevHourMinute = startMorningHourMinute;
      ledSetBrightness(ledMorningBrightness);
    }
    else if (actualPartOfDay==SCHEDULER_MODE_AFTERNOON) {
      ledMode = ledModeAfternoon;
      ledModeNext = ledModeEvening;
      ledModePrev = ledModeMorning;
      nextHourMinute = startEveningHourMinute;
      prevHourMinute = startAfternoonHourMinute;
      ledSetBrightness(ledAfternoonBrightness);
    }
    else if (actualPartOfDay==SCHEDULER_MODE_EVENING) {
      ledMode = ledModeEvening;
      ledModeNext = ledModeNight;
      ledModePrev = ledModeAfternoon;
      nextHourMinute = startNightHourMinute;
      prevHourMinute = startEveningHourMinute;
      ledSetBrightness(ledEveningBrightness);
    }
    else {
      ledMode = ledModeNight;
      ledModeNext = ledModeMorning;
      ledModePrev = ledModeEvening;
      nextHourMinute = startMorningHourMinute;
      prevHourMinute = startNightHourMinute;
      ledSetBrightness(ledNightBrightness);
    }
    
    byte fadeMode = LED_FADEMODE_DEFAULT;
    if (ledModePrev!=ledMode) {
      fadeMode = LED_FADEMODE_FADE;
    }
    
    //LedStep
    ledTransitionTime = ledStepSwitchColorSeconds;

    static bool manualChangedOnOff = false;
     if (fadeMode == LED_FADEMODE_FADE) {
      int seconds = nowHourMinute - prevHourMinute;
      if (seconds>=0 && seconds<=ledFadeInFromBlackSeconds) {
        if (ledManualOnOff==LED_MANUAL_ONOFF_OFF || ledManualOnOff==LED_MANUAL_ONOFF_ON) {
           manualChangedOnOff = true;
        }
        if (manualChangedOnOff==false) ledTransitionTime = ledFadeInFromBlackSeconds;
      }
    }
    else if (fadeMode == LED_FADEMODE_DEFAULT) {
      manualChangedOnOff = false;
      if (ledMode == LED_MODE_WAVE) {
        ledTransitionTime = ledStepWaveSeconds;
      }
    }
  }

  //Manual mode
  else if (ledControlMode==CONTROL_MODE_MANUAL) {
    ledSetBrightness(ledManualBrightness);
    ledMode = ledManualMode;

    //LedStep
    if (ledMode == LED_MODE_WAVE) {
      ledTransitionTime = ledStepWaveSeconds;
    }
    else {
      ledTransitionTime = ledStepSwitchColorSeconds;
    }
  }

  ledSwitchToMode(ledMode);

  //Set LED state
  if (ledManualOnOff==LED_MANUAL_ONOFF_OFF) {
    ledSetManualOff();
  }
  if (ledManualOnOff==LED_MANUAL_ONOFF_ON) {
    ledSetManualOn();
  }
  
  if (ledLastMode!=ledMode) {
    ledLastMode = ledMode;
    if (ledMode==LED_MODE_BLACK) {
      mqttElPublish("cmnd/POWER1","OFF");
      mqttElPublish( setBufferFromFlash(getLedState), setBufferFromFlash(charOff));
    }
    else {
      mqttElPublish("cmnd/POWER1","ON");
      mqttElPublish( setBufferFromFlash(getLedState), setBufferFromFlash(charOn));
    }
    ledManualOnOff = LED_MANUAL_ONOFF_AUTO; 
  }
  if (ledManualOnOff==LED_MANUAL_ONOFF_OFF && ledMode==LED_MODE_BLACK) {
    ledManualOnOff = LED_MANUAL_ONOFF_AUTO;
  }
  if (ledManualOnOff==LED_MANUAL_ONOFF_ON && ledMode!=LED_MODE_BLACK) {
    ledManualOnOff = LED_MANUAL_ONOFF_AUTO;
  }

  //Change actual color and brightness to destination color
  double millisDelta = abs((micros()/1000) - (lastEventMillis));
  lastEventMillis = micros()/1000;
  double ledStepMillis = millisDelta*_LED_RANGE_VALUES;
  ledStepMillis = ledStepMillis/ledTransitionTime/1000;

  double ledrightnessLevel = (double)ledBrightness/_LED_RANGE_VALUES;
   
  double ledRedLevel = (ledrightnessLevel * (double)(ledRed-_LED_MIN_VALUE)) + (double)_LED_MIN_VALUE;
  double ledGreenLevel = (ledrightnessLevel * (double)(ledGreen-_LED_MIN_VALUE)) + (double)_LED_MIN_VALUE;
  double ledBlueLevel = (ledrightnessLevel * (double)(ledBlue-_LED_MIN_VALUE)) + (double)_LED_MIN_VALUE;
  
  if (ledActualRed>ledRedLevel) {
    if (ledActualRed-ledStepMillis<ledRedLevel) {
      ledActualRed = ledRedLevel;
    }
    else {
      ledActualRed = ledActualRed-ledStepMillis;
    }
    if (ledActualRed<_LED_MIN_VALUE) ledActualRed = _LED_MIN_VALUE;
  }
  else if (ledActualRed<ledRedLevel) {
    if (ledActualRed+ledStepMillis>ledRedLevel) {
      ledActualRed = ledRedLevel;
    }
    else {
      ledActualRed = ledActualRed+ledStepMillis;
    }
    if (ledActualRed>_LED_MAX_VALUE) ledActualRed = _LED_MAX_VALUE;
  }

  if (ledActualGreen>ledGreenLevel) {
    if (ledActualGreen-ledStepMillis<ledGreenLevel) {
      ledActualGreen = ledGreenLevel;
    }
    else {
      ledActualGreen = ledActualGreen-ledStepMillis;
    }
    if (ledActualGreen<_LED_MIN_VALUE) ledActualGreen = _LED_MIN_VALUE;
  }
  else if (ledActualGreen<ledGreenLevel) {
    if (ledActualGreen+ledStepMillis>ledGreenLevel) {
      ledActualGreen = ledGreenLevel;
    }
    else {
      ledActualGreen = ledActualGreen+ledStepMillis;
    }
    if (ledActualGreen>_LED_MAX_VALUE) ledActualGreen = _LED_MAX_VALUE;
  }

  if (ledActualBlue>ledBlueLevel) {
    if (ledActualBlue-ledStepMillis<ledBlueLevel) {
      ledActualBlue = ledBlueLevel;
    }
    else {
      ledActualBlue = ledActualBlue-ledStepMillis;
    }
    if (ledActualBlue<_LED_MIN_VALUE) ledActualBlue = _LED_MIN_VALUE;
  }
  else if (ledActualBlue<ledBlueLevel) {
    if (ledActualBlue+ledStepMillis>ledBlueLevel) {
      ledActualBlue = ledBlueLevel;
    }
    else {
      ledActualBlue = ledActualBlue+ledStepMillis;
    }
    if (ledActualBlue>_LED_MAX_VALUE) ledActualBlue = _LED_MAX_VALUE;
  }
  
  letSetColor (ledActualRed, ledActualGreen, ledActualBlue);
}

void ledModeWave() {
  if (ledActualRed==_LED_MAX_VALUE && ledActualGreen==_LED_MIN_VALUE && ledActualBlue==_LED_MIN_VALUE && ledWaveIdx==0) {
    //red to green
    ledRed=_LED_MIN_VALUE;
    ledGreen=_LED_MAX_VALUE;
    ledBlue=_LED_MIN_VALUE;
    return;
  }

  if (ledActualRed==_LED_MIN_VALUE && ledActualGreen==_LED_MAX_VALUE && ledActualBlue==_LED_MIN_VALUE && ledWaveIdx==0) {
    //green to blue
    ledRed=_LED_MIN_VALUE;
    ledGreen=_LED_MIN_VALUE;
    ledBlue=_LED_MAX_VALUE;
    return;
  }

  if (ledActualRed==_LED_MIN_VALUE && ledActualGreen==_LED_MIN_VALUE && ledActualBlue==_LED_MAX_VALUE && ledWaveIdx==0) {
    //blue to white
    ledRed=_LED_MAX_VALUE;
    ledGreen=_LED_MAX_VALUE;
    ledBlue=_LED_MAX_VALUE;
    return;
  }
  
  if (ledActualRed==_LED_MAX_VALUE && ledActualGreen==_LED_MAX_VALUE && ledActualBlue==_LED_MAX_VALUE && ledWaveIdx==0) {
    //white to red
    ledRed=_LED_MAX_VALUE;
    ledGreen=_LED_MIN_VALUE;
    ledBlue=_LED_MIN_VALUE;
    ledWaveIdx=1;
    return;
  }

  if (ledActualRed==_LED_MAX_VALUE && ledActualGreen==_LED_MIN_VALUE && ledActualBlue==_LED_MIN_VALUE && ledWaveIdx==1) {
    //red to blue
    ledRed=_LED_MIN_VALUE;
    ledGreen=_LED_MIN_VALUE;
    ledBlue=_LED_MAX_VALUE;
    return;
  }
  
  if (ledActualRed==_LED_MIN_VALUE && ledActualGreen==_LED_MIN_VALUE && ledActualBlue==_LED_MAX_VALUE && ledWaveIdx==1) {
    //blue to green
    ledRed=_LED_MIN_VALUE;
    ledGreen=_LED_MAX_VALUE;
    ledBlue=_LED_MIN_VALUE;
    return;
  }
  
  if (ledActualRed==_LED_MIN_VALUE && ledActualGreen==_LED_MAX_VALUE && ledActualBlue==_LED_MIN_VALUE && ledWaveIdx==1) {
    //green to white
    ledRed=_LED_MAX_VALUE;
    ledGreen=_LED_MAX_VALUE;
    ledBlue=_LED_MAX_VALUE;
    return;
  }
  
  if (ledActualRed==_LED_MAX_VALUE && ledActualGreen==_LED_MAX_VALUE && ledActualBlue==_LED_MAX_VALUE && ledWaveIdx==1) {
    //white to red
    ledRed=_LED_MAX_VALUE;
    ledGreen=_LED_MIN_VALUE;
    ledBlue=_LED_MIN_VALUE;  
    ledWaveIdx=0;
    return;
  }

  if (ledActualRed==_LED_MIN_VALUE && ledActualGreen==_LED_MAX_VALUE && ledActualBlue==_LED_MAX_VALUE) {
    //cyan to green
    ledRed=_LED_MIN_VALUE;
    ledGreen=_LED_MAX_VALUE;
    ledBlue=_LED_MIN_VALUE;
    return;
  }

  if (ledActualRed==_LED_MAX_VALUE && ledActualGreen==_LED_MAX_VALUE && ledActualBlue==_LED_MIN_VALUE) {
    //yellow to red
    ledRed=_LED_MAX_VALUE;
    ledGreen=_LED_MIN_VALUE;
    ledBlue=_LED_MIN_VALUE;
    return;
  }

  if (ledActualRed==_LED_MAX_VALUE && ledActualGreen==_LED_MIN_VALUE && ledActualBlue==_LED_MAX_VALUE) {
    //magenta to blue
    ledRed=_LED_MIN_VALUE;
    ledGreen=_LED_MIN_VALUE;
    ledBlue=_LED_MAX_VALUE;
    return;
  }

  if (ledActualRed==_LED_MIN_VALUE && ledActualGreen==_LED_MIN_VALUE && ledActualBlue==_LED_MIN_VALUE) {
    //dark to white
    ledRed=_LED_MAX_VALUE;
    ledGreen=_LED_MAX_VALUE;
    ledBlue=_LED_MAX_VALUE;
    return;
  }
}

void ledMicrosEvent() {
  if (ledMode == LED_MODE_WAVE) {
    ledModeWave();
  }
  ledSetActualMode();
}

void ledSetBrightness(int value) {
  ledBrightness = value;
}

void letSetColor (int r, int g, int b) {
  analogWrite (LED_RED_PIN, r);
  analogWrite (LED_GREEN_PIN, g);
  analogWrite (LED_BLUE_PIN, b);
}

void ledSetCustom(byte custommode) {
  unsigned long CustomColor; 
  switch (custommode) {
    case 1:
      CustomColor = ledCustomColor1;
      break;
    case 2:
      CustomColor = ledCustomColor2;
      break;
  }
  unsigned long result = 0;
  unsigned long myAnd = 0x0000FF;
  unsigned long myColor = CustomColor;
   
  result = myColor & myAnd;
  ledBlue = result;

  myColor = CustomColor;
  myAnd = 0x00FF00;
  result = myColor & myAnd;
  result = result>>8;
  ledGreen = result;

  myColor = CustomColor;
  myAnd = 0xFF0000;
  result = myColor & myAnd;
  result = result>>16;
  ledRed = result;
}

void ledSetBlack() {
  ledRed = _LED_MIN_VALUE;
  ledGreen = _LED_MIN_VALUE;
  ledBlue = _LED_MIN_VALUE;
  ledMode = LED_MODE_BLACK;
}

void ledSetRed() {
  ledRed = _LED_MAX_VALUE;
  ledGreen = _LED_MIN_VALUE;
  ledBlue = _LED_MIN_VALUE;
  ledMode = LED_MODE_RED;
}

void ledSetGreen() {
  ledRed = _LED_MIN_VALUE;
  ledGreen = _LED_MAX_VALUE;
  ledBlue = _LED_MIN_VALUE;
  ledMode = LED_MODE_GREEN;
}

void ledSetBlue() {
  ledRed = _LED_MIN_VALUE;
  ledGreen = _LED_MIN_VALUE;
  ledBlue = _LED_MAX_VALUE;
  ledMode = LED_MODE_BLUE;
}

void ledSetCyan() {
  ledRed = _LED_MIN_VALUE;
  ledGreen = _LED_MAX_VALUE;
  ledBlue = _LED_MAX_VALUE;
  ledMode = LED_MODE_CYAN;
}

void ledSetMagenta() {
  ledRed = _LED_MAX_VALUE;
  ledGreen = _LED_MIN_VALUE;
  ledBlue = _LED_MAX_VALUE;
  ledMode = LED_MODE_MAGENTA;
}

void ledSetYellow() {
  ledRed = _LED_MAX_VALUE;
  ledGreen = _LED_MAX_VALUE;
  ledBlue = _LED_MIN_VALUE;
  ledMode = LED_MODE_YELLOW;
}

void ledSetWhite() {
  ledGreen = _LED_MAX_VALUE;
  ledBlue = _LED_MAX_VALUE;
  ledRed = _LED_MAX_VALUE;
  ledMode = LED_MODE_WHITE;
}

void ledSetWave() {
  ledMode = LED_MODE_WAVE;
}

void ledSetManualOn() {
  ledActualState = LED_MANUAL_ONOFF_ON;
  if (ledControlMode==CONTROL_MODE_MANUAL) {
    ledSwitchToMode(ledManualMode);
  }
  else if (ledMode==LED_MODE_BLACK) {
    ledRed = _LED_MAX_VALUE;
    ledGreen = _LED_MAX_VALUE;
    ledBlue = _LED_MAX_VALUE;
  }
}

void ledSetManualOff() {
  ledActualState = LED_MANUAL_ONOFF_OFF;
  ledRed = _LED_MIN_VALUE;
  ledGreen = _LED_MIN_VALUE;
  ledBlue = _LED_MIN_VALUE;
}
