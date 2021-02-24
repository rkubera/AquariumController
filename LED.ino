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
// version 1 - store frequency and volume step in EEPROM memory

void ledInit () {
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
    ledStep = ledStepSwitchColor;
    
    //Auto Brightness
    ledAutoBrightness = 255;
  
    //Fade in black
    if (ledModeNext==LED_MODE_BLACK) {
      int seconds = nextHourMinute-nowHourMinute;
      if (seconds>=0 && seconds<=ledFadeInFromBlackSeconds) {
        ledAutoBrightness = ((255/ledFadeInFromBlackSeconds)*seconds);
      }
      if (seconds<=1) {
        ledActualRed=0;
        ledActualGreen=0;
        ledActualBlue=0;
        ledRed=0;
        ledGreen=0;
        ledBlue=0;
      }
    }
  
    //Fade from black
    if (ledModePrev == LED_MODE_BLACK) {
      int seconds = nowHourMinute-prevHourMinute;
      if (seconds>=0 && seconds<=ledFadeInFromBlackSeconds) {
        ledAutoBrightness = ((255/ledFadeInFromBlackSeconds)*seconds);
      }
    }
  }
  else if (ledControlMode==CONTROL_MODE_MANUAL) {
    //ledSetBrightness(ledManualBrightness);
    ledMode = ledManualMode;
    ledStep = ledStepSwitchColor;
    ledAutoBrightness = 255;
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
  if (ledActualRed==255 && ledActualGreen==0 && ledActualBlue==0 && ledWaveIdx==0) {
    if (ledActualTimer>=ledTimer) {
      //red to green
      ledRed=0;
      ledGreen=255;
      ledBlue=0;
      ledActualTimer = 0;
    }
    else {
      ledActualTimer++;
    }
    return;
  }

  if (ledActualRed==0 && ledActualGreen==255 && ledActualBlue==0 && ledWaveIdx==0) {
    if (ledActualTimer>=ledTimer) {
      //green to blue
      ledRed=0;
      ledGreen=0;
      ledBlue=255;
      ledActualTimer = 0;
    }
    else {
      ledActualTimer++;
    }
    return;
  }

  if (ledActualRed==0 && ledActualGreen==0 && ledActualBlue==255 && ledWaveIdx==0) {
    if (ledActualTimer>=ledTimer) {
      //blue to white
      ledRed=255;
      ledGreen=255;
      ledBlue=255;
      ledActualTimer = 0;
    }
    else {
      ledActualTimer++;
    }
    return;
  }
  
  if (ledActualRed==255 && ledActualGreen==255 && ledActualBlue==255 && ledWaveIdx==0) {
    if (ledActualTimer>=ledTimer) {
      //white to red
      ledRed=255;
      ledGreen=0;
      ledBlue=0;
      ledWaveIdx=1;
      ledActualTimer = 0;
    }
    else {
      ledActualTimer++;
    }
    return;
  }

  if (ledActualRed==255 && ledActualGreen==0 && ledActualBlue==0 && ledWaveIdx==1) {
    if (ledActualTimer>=ledTimer) {
      //red to blue
      ledRed=0;
      ledGreen=0;
      ledBlue=255;
      ledActualTimer = 0;
    }
    else {
      ledActualTimer++;
    }
    return;
  }
  
  if (ledActualRed==0 && ledActualGreen==0 && ledActualBlue==255 && ledWaveIdx==1) {
    if (ledActualTimer>=ledTimer) {
      //blue to green
      ledRed=0;
      ledGreen=255;
      ledBlue=0;
      ledActualTimer = 0;
    }
    else {
      ledActualTimer++;
    }
    return;
  }
  
  if (ledActualRed==0 && ledActualGreen==255 && ledActualBlue==0 && ledWaveIdx==1) {
    if (ledActualTimer>=ledTimer) {
      //green to white
      ledRed=255;
      ledGreen=255;
      ledBlue=255;
      ledActualTimer = 0;
    }
    else {
      ledActualTimer++;
    }
    return;
  }
  
  if (ledActualRed==255 && ledActualGreen==255 && ledActualBlue==255 && ledWaveIdx==1) {
    if (ledActualTimer>=ledTimer) {
      //white to red
      ledRed=255;
      ledGreen=0;
      ledBlue=0;  
      ledWaveIdx=0;
      ledActualTimer = 0;
    }
    else {
      ledActualTimer++;
    }
    return;
  }

  if (ledActualRed==0 && ledActualGreen==255 && ledActualBlue==255) {
    if (ledActualTimer>=ledTimer) {
      //cyan to green
      ledRed=0;
      ledGreen=255;
      ledBlue=0;
      ledActualTimer = 0;
    }
    else {
      ledActualTimer++;
    }
    return;
  }

  if (ledActualRed==255 && ledActualGreen==255 && ledActualBlue==0) {
    if (ledActualTimer>=ledTimer) {
      //yellow to red
      ledRed=255;
      ledGreen=0;
      ledBlue=0;
      ledActualTimer = 0;
    }
    else {
      ledActualTimer++;
    }
    return;
  }

  if (ledActualRed==255 && ledActualGreen==0 && ledActualBlue==255) {
    if (ledActualTimer>=ledTimer) {
      //magenta to blue
      ledRed=0;
      ledGreen=0;
      ledBlue=255;
      ledActualTimer = 0;
    }
    else {
      ledActualTimer++;
    }
    return;
  }

  if (ledActualRed==0 && ledActualGreen==0 && ledActualBlue==0) {
    if (ledActualTimer>=ledTimer) {
      //dark to white
      ledRed=255;
      ledGreen=255;
      ledBlue=255;
      ledActualTimer = 0;
    }
    else {
      ledActualTimer++;
      
    }
    return;
  }
}

void ledMicrosEvent() {
  if (abs(micros()-lastLEDMicros)>1000) {
    lastLEDMicros = micros();
  
    ledSetActualMode();
    
    if (ledActualRed>ledRed) {
      ledActualRed = ledActualRed-ledStep;
      if (ledActualRed<0) ledActualRed = 0;
    }
  
    if (ledActualRed<ledRed) {
      ledActualRed = ledActualRed+ledStep;
      if (ledActualRed>255) ledActualRed = 255;
    }
  
    if (ledActualGreen>ledGreen) {
      ledActualGreen = ledActualGreen-ledStep;
      if (ledActualGreen<0) ledActualGreen = 0;
    }
  
    if (ledActualGreen<ledGreen) {
      ledActualGreen = ledActualGreen+ledStep;
      if (ledActualGreen>255) ledActualGreen = 255;
    }
  
    if (ledActualBlue>ledBlue) {
      ledActualBlue = ledActualBlue-ledStep;
      if (ledActualBlue<0) ledActualBlue = 0;
    }
  
    if (ledActualBlue<ledBlue) {
      ledActualBlue = ledActualBlue+ledStep;
      if (ledActualBlue>255) ledActualBlue = 255;
    }
  
    if (ledActualBrightness<ledBrightness) {
      ledActualBrightness = ledActualBrightness+ledStep;
      if (ledActualBrightness>255) ledActualBrightness = 255;
    }
  
    if (ledActualBrightness>ledBrightness) {
      ledActualBrightness = ledActualBrightness-ledStep;
      if (ledActualBrightness<0) ledActualBrightness = 0;
    }
    
    letSetColor (ledActualRed, ledActualGreen, ledActualBlue);
  }
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

  double ledRedLevelDouble = (double)r*(double)ledAutoBrightness*(double)ledActualBrightness/255/255;
  double ledGreenLevelDouble = (double)g*(double)ledAutoBrightness*(double)ledActualBrightness/255/255;
  double ledBlueLevelDouble = (double)b*(double)ledAutoBrightness*(double)ledActualBrightness/255/255;

  ledRedLevel = ledRedLevelDouble;
  ledGreenLevel = ledGreenLevelDouble;
  ledBlueLevel = ledBlueLevelDouble;
  
  analogWrite (LED_RED_PIN, ledRedLevel);
  analogWrite (LED_GREEN_PIN, ledGreenLevel);
  analogWrite (LED_BLUE_PIN, ledBlueLevel);
}

void ledSetBlack() {
  ledRed = 0;
  ledGreen = 0;
  ledBlue = 0;
  ledMode = LED_MODE_BLACK;
}

void ledSetRed() {
  ledRed = 255;
  ledGreen = 0;
  ledBlue = 0;
  ledMode = LED_MODE_RED;
}

void ledSetGreen() {
  ledRed = 0;
  ledGreen = 255;
  ledBlue = 0;
  ledMode = LED_MODE_GREEN;
}

void ledSetBlue() {
  ledRed = 0;
  ledGreen = 0;
  ledBlue = 255;
  ledMode = LED_MODE_BLUE;
}

void ledSetCyan() {
  ledRed = 0;
  ledGreen = 255;
  ledBlue = 255;
  ledMode = LED_MODE_CYAN;
}

void ledSetMagenta() {
  ledRed = 255;
  ledGreen = 0;
  ledBlue = 255;
  ledMode = LED_MODE_MAGENTA;
}

void ledSetYellow() {
  ledRed = 255;
  ledGreen = 255;
  ledBlue = 0;
  ledMode = LED_MODE_YELLOW;
}

void ledSetWhite() {
  ledGreen = 255;
  ledBlue = 255;
  ledRed = 255;
  ledMode = LED_MODE_WHITE;
}

void ledSetWave() {
  ledMode = LED_MODE_WAVE;
  ledStep = ledStepWave;
}

void ledSetManualOn() {
  ledStep = ledStepSwitchColor;
  if (ledControlMode==CONTROL_MODE_MANUAL) {
    ledSwitchToMode(ledManualMode);
  }
  else if (ledMode==LED_MODE_BLACK) {
    ledRed = 255;
    ledGreen = 255;
    ledBlue = 255;
  }
}

void ledSetManualOff() {
  ledRed = 0;
  ledGreen = 0;
  ledBlue = 0;
  ledStep = ledStepSwitchColor;
}
