/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/

//based on:
 // Nicu Florica (aka niq_ro) from http://www.tehnic.go.ro made a small change for nice display for low frequence (bellow 100MHz)
// it use info from http://full-chip.net/arduino-proekty/97-cifrovoy-fm-priemnik-na-arduino-i-module-rda5807-s-graficheskim-displeem-i-funkciey-rds.html
// original look like is from http://seta43.hol.es/radiofm.html

//Servo redLed, greenLed, blueLed;

void ledInit() {

  //redLed.attach(LED_RED_PIN);
  //greenLed.attach(LED_GREEN_PIN);
  //blueLed.attach(LED_BLUE_PIN);
  
  ledWaveIdx = 0;

  //Power
  pinMode(LCD_GND,LOW);
  pinMode(LCD_POWER,HIGH);
  
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_BLUE_PIN, OUTPUT);
  lastLEDMicros = micros();

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
  mqttElPublish(setBufferFromFlash(getBrightness), String(ledBrightness));
  mqttElPublish(setBufferFromFlash(getLedManualMode), ledColorValueToString(ledManualMode));
  mqttElPublish(setBufferFromFlash(getLedColorMorning), ledColorValueToString(ledModeMorning));
  mqttElPublish(setBufferFromFlash(getLedColorAfternoon), ledColorValueToString(ledModeAfternoon));
  mqttElPublish(setBufferFromFlash(getLedColorEvening), ledColorValueToString(ledModeEvening));
  mqttElPublish(setBufferFromFlash(getLedColorNight), ledColorValueToString(ledModeNight));
  //mqttElPublish(setBufferFromFlash(getLedControlMode), getStringControlModeFromValue(ledControlMode));
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
  }
}

void ledSetActualMode() {
  
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
      //ledSetBrightness(ledMorningBrightness);
      ledMode = ledModeMorning;
      ledModeNext = ledModeAfternoon;
      ledModePrev = ledModeNight;
      nextHourMinute = startAfternoonHourMinute;
      prevHourMinute = startMorningHourMinute;
    }
    else if (actualPartOfDay==SCHEDULER_MODE_AFTERNOON) {
      //ledSetBrightness(ledAfternoonBrightness);
      ledMode = ledModeAfternoon;
      ledModeNext = ledModeEvening;
      ledModePrev = ledModeMorning;
      nextHourMinute = startEveningHourMinute;
      prevHourMinute = startAfternoonHourMinute;
    }
    else if (actualPartOfDay==SCHEDULER_MODE_EVENING) {
      //ledSetBrightness(ledEveningBrightness);
      ledMode = ledModeEvening;
      ledModeNext = ledModeNight;
      ledModePrev = ledModeAfternoon;
      nextHourMinute = startNightHourMinute;
      prevHourMinute = startEveningHourMinute;
    }
    else {
      //ledSetBrightness(ledNightBrightness);
      ledMode = ledModeNight;
      ledModeNext = ledModeMorning;
      ledModePrev = ledModeEvening;
      nextHourMinute = startMorningHourMinute;
      prevHourMinute = startNightHourMinute;
    }

    //LedStep
    ledStep = ledStepSwitchColorSeconds;
    
    //Auto Brightness
    ledAutoBrightness = _LED_RANGE_VALUES;
  
    //Fade in black
    if (ledModeNext==LED_MODE_BLACK) {
      int seconds = nextHourMinute-nowHourMinute;
      if (seconds>=0 && seconds<=ledFadeInFromBlackSeconds) {
        ledAutoBrightness = (_LED_RANGE_VALUES/ledFadeInFromBlackSeconds)*seconds;
      }
      if (seconds<=1) {
        ledActualRed=_LED_MIN_VALUE;
        ledActualGreen=_LED_MIN_VALUE;
        ledActualBlue=_LED_MIN_VALUE;
        ledRed=_LED_MIN_VALUE;
        ledGreen=_LED_MIN_VALUE;
        ledBlue=_LED_MIN_VALUE;
      }
    }
  
    //Fade from black
    if (ledModePrev == LED_MODE_BLACK) {
      int seconds = nowHourMinute-prevHourMinute;
      if (seconds>=0 && seconds<=ledFadeInFromBlackSeconds) {
        ledAutoBrightness = (_LED_RANGE_VALUES/ledFadeInFromBlackSeconds)*seconds;
      }
    }
  }
  else if (ledControlMode==CONTROL_MODE_MANUAL) {
    //ledSetBrightness(ledManualBrightness);
    ledMode = ledManualMode;
    ledStep = ledStepSwitchColorSeconds;
    ledAutoBrightness = _LED_RANGE_VALUES;
  }

  ledSetBrightness(LED_BRIGHTNESS_AUTO);
  ledSwitchToMode(ledMode);
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
}

void ledModeWave() {
  if (ledActualRed==_LED_MAX_VALUE && ledActualGreen==_LED_MIN_VALUE && ledActualBlue==_LED_MIN_VALUE && ledWaveIdx==0) {
    if (ledActualTimer>=ledTimer) {
      //red to green
      ledRed=_LED_MIN_VALUE;
      ledGreen=_LED_MAX_VALUE;
      ledBlue=_LED_MIN_VALUE;
      ledActualTimer = 0;
    }
    else {
      ledActualTimer++;
    }
    return;
  }

  if (ledActualRed==_LED_MIN_VALUE && ledActualGreen==_LED_MAX_VALUE && ledActualBlue==_LED_MIN_VALUE && ledWaveIdx==0) {
    if (ledActualTimer>=ledTimer) {
      //green to blue
      ledRed=_LED_MIN_VALUE;
      ledGreen=_LED_MIN_VALUE;
      ledBlue=_LED_MAX_VALUE;
      ledActualTimer = 0;
    }
    else {
      ledActualTimer++;
    }
    return;
  }

  if (ledActualRed==_LED_MIN_VALUE && ledActualGreen==_LED_MIN_VALUE && ledActualBlue==_LED_MAX_VALUE && ledWaveIdx==0) {
    if (ledActualTimer>=ledTimer) {
      //blue to white
      ledRed=_LED_MAX_VALUE;
      ledGreen=_LED_MAX_VALUE;
      ledBlue=_LED_MAX_VALUE;
      ledActualTimer = 0;
    }
    else {
      ledActualTimer++;
    }
    return;
  }
  
  if (ledActualRed==_LED_MAX_VALUE && ledActualGreen==_LED_MAX_VALUE && ledActualBlue==_LED_MAX_VALUE && ledWaveIdx==0) {
    if (ledActualTimer>=ledTimer) {
      //white to red
      ledRed=_LED_MAX_VALUE;
      ledGreen=_LED_MIN_VALUE;
      ledBlue=_LED_MIN_VALUE;
      ledWaveIdx=1;
      ledActualTimer = 0;
    }
    else {
      ledActualTimer++;
    }
    return;
  }

  if (ledActualRed==_LED_MAX_VALUE && ledActualGreen==_LED_MIN_VALUE && ledActualBlue==_LED_MIN_VALUE && ledWaveIdx==1) {
    if (ledActualTimer>=ledTimer) {
      //red to blue
      ledRed=_LED_MIN_VALUE;
      ledGreen=_LED_MIN_VALUE;
      ledBlue=_LED_MAX_VALUE;
      ledActualTimer = 0;
    }
    else {
      ledActualTimer++;
    }
    return;
  }
  
  if (ledActualRed==_LED_MIN_VALUE && ledActualGreen==_LED_MIN_VALUE && ledActualBlue==_LED_MAX_VALUE && ledWaveIdx==1) {
    if (ledActualTimer>=ledTimer) {
      //blue to green
      ledRed=_LED_MIN_VALUE;
      ledGreen=_LED_MAX_VALUE;
      ledBlue=_LED_MIN_VALUE;
      ledActualTimer = 0;
    }
    else {
      ledActualTimer++;
    }
    return;
  }
  
  if (ledActualRed==_LED_MIN_VALUE && ledActualGreen==_LED_MAX_VALUE && ledActualBlue==_LED_MIN_VALUE && ledWaveIdx==1) {
    if (ledActualTimer>=ledTimer) {
      //green to white
      ledRed=_LED_MAX_VALUE;
      ledGreen=_LED_MAX_VALUE;
      ledBlue=_LED_MAX_VALUE;
      ledActualTimer = 0;
    }
    else {
      ledActualTimer++;
    }
    return;
  }
  
  if (ledActualRed==_LED_MAX_VALUE && ledActualGreen==_LED_MAX_VALUE && ledActualBlue==_LED_MAX_VALUE && ledWaveIdx==1) {
    if (ledActualTimer>=ledTimer) {
      //white to red
      ledRed=_LED_MAX_VALUE;
      ledGreen=_LED_MIN_VALUE;
      ledBlue=_LED_MIN_VALUE;  
      ledWaveIdx=0;
      ledActualTimer = 0;
    }
    else {
      ledActualTimer++;
    }
    return;
  }

  if (ledActualRed==_LED_MIN_VALUE && ledActualGreen==_LED_MAX_VALUE && ledActualBlue==_LED_MAX_VALUE) {
    if (ledActualTimer>=ledTimer) {
      //cyan to green
      ledRed=_LED_MIN_VALUE;
      ledGreen=_LED_MAX_VALUE;
      ledBlue=_LED_MIN_VALUE;
      ledActualTimer = 0;
    }
    else {
      ledActualTimer++;
    }
    return;
  }

  if (ledActualRed==_LED_MAX_VALUE && ledActualGreen==_LED_MAX_VALUE && ledActualBlue==_LED_MIN_VALUE) {
    if (ledActualTimer>=ledTimer) {
      //yellow to red
      ledRed=_LED_MAX_VALUE;
      ledGreen=_LED_MIN_VALUE;
      ledBlue=_LED_MIN_VALUE;
      ledActualTimer = 0;
    }
    else {
      ledActualTimer++;
    }
    return;
  }

  if (ledActualRed==_LED_MAX_VALUE && ledActualGreen==_LED_MIN_VALUE && ledActualBlue==_LED_MAX_VALUE) {
    if (ledActualTimer>=ledTimer) {
      //magenta to blue
      ledRed=_LED_MIN_VALUE;
      ledGreen=_LED_MIN_VALUE;
      ledBlue=_LED_MAX_VALUE;
      ledActualTimer = 0;
    }
    else {
      ledActualTimer++;
    }
    return;
  }

  if (ledActualRed==_LED_MIN_VALUE && ledActualGreen==_LED_MIN_VALUE && ledActualBlue==_LED_MIN_VALUE) {
    if (ledActualTimer>=ledTimer) {
      //dark to white
      ledRed=_LED_MAX_VALUE;
      ledGreen=_LED_MAX_VALUE;
      ledBlue=_LED_MAX_VALUE;
      ledActualTimer = 0;
    }
    else {
      ledActualTimer++;
      
    }
    return;
  }
}

void ledMicrosEvent() {
  //if (abs(micros()-lastLEDMicros)>1000) {
    double microsDelta = abs(micros()- (lastLEDMicros));
    lastLEDMicros = micros();

    if (ledMode == LED_MODE_WAVE) {
      ledModeWave();
    }
   
    ledSetActualMode();


    double milisSum = ledStep*1000;
    
    //double ledStepMillis = (microsDelta/1000)*(double)_LED_RANGE_VALUES/milisSum;
    double ledStepMillis = (microsDelta/1000)*_LED_RANGE_VALUES;
    ledStepMillis = ledStepMillis/milisSum;
    
    if (ledActualRed>ledRed) {
      ledActualRed = ledActualRed-ledStepMillis;
      if (ledActualRed<_LED_MIN_VALUE) ledActualRed = _LED_MIN_VALUE;
    }
  
    if (ledActualRed<ledRed) {
      ledActualRed = ledActualRed+ledStepMillis;
      if (ledActualRed>_LED_MAX_VALUE) ledActualRed = _LED_MAX_VALUE;
    }
  
    if (ledActualGreen>ledGreen) {
      ledActualGreen = ledActualGreen-ledStepMillis;
      if (ledActualGreen<_LED_MIN_VALUE) ledActualGreen = _LED_MIN_VALUE;
    }
  
    if (ledActualGreen<ledGreen) {
      ledActualGreen = ledActualGreen+ledStepMillis;
      if (ledActualGreen>_LED_MAX_VALUE) ledActualGreen = _LED_MAX_VALUE;
    }
  
    if (ledActualBlue>ledBlue) {
      ledActualBlue = ledActualBlue-ledStepMillis;
      if (ledActualBlue<_LED_MIN_VALUE) ledActualBlue = _LED_MIN_VALUE;
    }
  
    if (ledActualBlue<ledBlue) {
      ledActualBlue = ledActualBlue+ledStepMillis;
      if (ledActualBlue>_LED_MAX_VALUE) ledActualBlue = _LED_MAX_VALUE;
    }

    if (ledActualBrightness<ledBrightness) {
      ledActualBrightness = ledActualBrightness+ledStepMillis;
      if (ledActualBrightness>_LED_RANGE_VALUES) ledActualBrightness = _LED_RANGE_VALUES;
    }

    if (ledActualBrightness>ledBrightness) {
      ledActualBrightness = ledActualBrightness-ledStepMillis;
      if (ledActualBrightness<0) ledActualBrightness = 0;
    }
    
    letSetColor (ledActualRed, ledActualGreen, ledActualBlue);
  //}
}

void ledSetBrightness(int value) {
  int brightness;
  if (value<0) {
    int actualPartOfDay = schedulerGetActualPartOfDay();
    if (ledControlMode==CONTROL_MODE_PART_OF_DAY) {
      if (actualPartOfDay==SCHEDULER_MODE_MORNING) {
        brightness = ledMorningBrightness;
      }
      else if (actualPartOfDay==SCHEDULER_MODE_AFTERNOON) {
        brightness = ledAfternoonBrightness;
      }
      else if (actualPartOfDay==SCHEDULER_MODE_EVENING) {
        brightness = ledEveningBrightness;
      }
      else {
        brightness = ledNightBrightness;
      }
    }
    else {
      brightness = ledManualBrightness;
    }
  }
  else {
    brightness = value;
  }
  if (ledBrightness!=brightness) {
    ledBrightness = brightness;
    mqttElPublish( setBufferFromFlash(getBrightness), String(ledBrightness));
  }
}

void letSetColor (int r, int g, int b) {

  double fullledBrightness;
  
  fullledBrightness = (double)ledActualBrightness/_LED_RANGE_VALUES;
  fullledBrightness = (double)fullledBrightness*(double)ledAutoBrightness/_LED_RANGE_VALUES;

  double ledRedLevelDouble = (fullledBrightness * (double)(r-_LED_MIN_VALUE)) + (double)_LED_MIN_VALUE;
  double ledGreenLevelDouble = (fullledBrightness * (double)(g-_LED_MIN_VALUE)) + (double)_LED_MIN_VALUE;
  double ledBlueLevelDouble = (fullledBrightness * (double)(b-_LED_MIN_VALUE)) + (double)_LED_MIN_VALUE;
  
  ledRedLevel = ledRedLevelDouble;
  ledGreenLevel = ledGreenLevelDouble;
  ledBlueLevel = ledBlueLevelDouble;

  //redLed.writeMicroseconds(ledRedLevel);
  //greenLed.writeMicroseconds(ledGreenLevel);
  //blueLed.writeMicroseconds(ledBlueLevel);

  ledRedLevel = map (ledRedLevel, _LED_MIN_VALUE, _LED_MAX_VALUE, 0, 255);
  ledGreenLevel = map (ledGreenLevel, _LED_MIN_VALUE, _LED_MAX_VALUE, 0, 255);
  ledBlueLevel = map (ledBlueLevel, _LED_MIN_VALUE, _LED_MAX_VALUE, 0, 255);
  
  analogWrite (LED_RED_PIN, ledRedLevel);
  analogWrite (LED_GREEN_PIN, ledGreenLevel);
  analogWrite (LED_BLUE_PIN, ledBlueLevel);

/*
  Serial.print("fullledBrightness=");
  Serial.print(fullledBrightness);
  Serial.print(" ledActualBrightness=");
  Serial.print(ledActualBrightness);
  Serial.print(" ledAutoBrightness=");
  Serial.print(ledAutoBrightness);
  Serial.print("red=");
  Serial.print(ledRedLevel);
  Serial.print(" green=");
  Serial.print(ledGreenLevel);
  Serial.print(" blue=");
  Serial.println(ledBlueLevel);
  */
  
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
  ledStep = ledStepWaveSeconds;
}

void ledSetManualOn() {
  ledStep = ledStepSwitchColorSeconds;
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
  ledRed = _LED_MIN_VALUE;
  ledGreen = _LED_MIN_VALUE;
  ledBlue = _LED_MIN_VALUE;
  ledStep = ledStepSwitchColorSeconds;
}
