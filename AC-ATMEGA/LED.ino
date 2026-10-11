// A live RGB edit overrides rendering, never the configured color selection.
static bool ledRgbOverride=false;
static byte ledRgbOverrideControl=0,ledRgbOverridePhase=0,ledRgbOverrideSelection=0;
void ledUseCurrentCustomColor(){
  ledRgbOverride=true;
  ledRgbOverrideControl=ledControlMode;
  ledRgbOverridePhase=ledControlMode==CONTROL_MODE_PART_OF_DAY?schedulerGetActualPartOfDay():0;
  ledRgbOverrideSelection=ledMode;
}

static TemporaryState ledStateChoice;
void ledSelectManualState(bool on){ledStateChoice.select(on,ledManualOnOff);}

/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/

// Fade rendering has no publication or mode callbacks: it can safely run
// between snapshot fields, including while reportField captures a readback.
static byte ledFadeBrightness=0;
static double ledFadeStart[3],ledFadeTarget[3]={-1,-1,-1};
static unsigned long ledFadeStarted=0,ledFadeDuration=0,ledFadeLastRender=0;
static bool ledFadeRendering=false,ledFadeActive=false,ledFadePending=false;
static unsigned long ledFadeMaxGap=0,ledFadeRenderCount=0;

void ledFadeService() {
  if (ledFadeRendering || !megaConfigReady || ledFadeTarget[0]<0 || (!ledFadeActive && !ledFadePending)) return;
  unsigned long nowMs=millis(),elapsed=nowMs-ledFadeStarted;
  if (nowMs-ledFadeLastRender<5 && elapsed<ledFadeDuration) return;
  ledFadeRendering=true;
  if (ledFadeActive) {
    unsigned long gap=nowMs-ledFadeLastRender;
    if (gap>ledFadeMaxGap) ledFadeMaxGap=gap;
    ++ledFadeRenderCount;
  }
  ledFadeLastRender=nowMs;
  double fraction=ledFadeDuration==0 || elapsed>=ledFadeDuration ? 1.0 : (double)elapsed/ledFadeDuration;
  ledActualRed=fraction>=1.0 ? ledFadeTarget[0] : ledFadeStart[0]+(ledFadeTarget[0]-ledFadeStart[0])*fraction;
  ledActualGreen=fraction>=1.0 ? ledFadeTarget[1] : ledFadeStart[1]+(ledFadeTarget[1]-ledFadeStart[1])*fraction;
  ledActualBlue=fraction>=1.0 ? ledFadeTarget[2] : ledFadeStart[2]+(ledFadeTarget[2]-ledFadeStart[2])*fraction;
  letSetColor(uint16_t(ledActualRed+0.5),uint16_t(ledActualGreen+0.5),uint16_t(ledActualBlue+0.5));
  ledFadeActive=fraction<1.0;
  ledFadePending=false;
  ledFadeRendering=false;
}

void ledInit() {
  ledRgbOverride=false;
  // Reconfiguration must not resume a fade from the previous settings.
  ledFadeActive=ledFadePending=false;
  ledFadeTarget[0]=ledFadeTarget[1]=ledFadeTarget[2]=-1;
  
  ledWaveIdx = 0;
  ledPwmInit();
  
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_BLUE_PIN, OUTPUT);


  ledCurrentBrightness=ledManualBrightness;
  ledSetBrightness(ledCurrentBrightness);
  ledScheduleBrightnessReset=true;
  ledSetActualMode();
}

String ledColorValueToString (byte Value) {
  switch (Value) {
    case LED_MODE_NONE:
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
    case LED_MODE_CUSTOM:
        return setBufferFromFlash(charCustom);
        break;
  }
  return setBufferFromFlash(charBlack);
}

void ledReportAll() {
  reportField(FieldId::LedState,ledActualState==LED_MANUAL_ONOFF_ON ? "on" : "off");
  String actualControlMode;
  if (ledControlMode==CONTROL_MODE_PART_OF_DAY) {
    actualControlMode = setBufferFromFlash(charPartofday);
  }
  else if (ledControlMode==CONTROL_MODE_MANUAL) {
    actualControlMode = setBufferFromFlash(charManual);
  }
  
  reportField(FieldId::LedControlMode, actualControlMode);
  reportField(FieldId::MorningBrightness, String(ledMorningBrightness));
  reportField(FieldId::AfternoonBrightness, String(ledAfternoonBrightness));
  reportField(FieldId::EveningBrightness, String(ledEveningBrightness));
  reportField(FieldId::NightBrightness, String(ledNightBrightness));
  reportField(FieldId::CurrentBrightness, String(ledCurrentBrightness));
  reportField(FieldId::LedManualMode, ledColorValueToString(ledManualMode));
  reportField(FieldId::LedColorMorning, ledColorValueToString(ledModeMorning));
  reportField(FieldId::LedColorAfternoon, ledColorValueToString(ledModeAfternoon));
  reportField(FieldId::LedColorEvening, ledColorValueToString(ledModeEvening));
  reportField(FieldId::LedColorNight, ledColorValueToString(ledModeNight));
  reportField(FieldId::CustomColor, longToString(ledCustomColor));
}

void ledSwitchToMode(int myMode) {
  switch (myMode) {
    case LED_MODE_NONE:
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
     case LED_MODE_CUSTOM:
        ledSetCustom();
        break;
  }
}

void ledSetActualMode() {
  if (!megaConfigReady) {ledBrightness=0;ledRed=ledGreen=ledBlue=0;ledActualRed=ledActualGreen=ledActualBlue=0;letSetColor(0,0,0);return;}
  //Part of day mode
  if (ledControlMode==CONTROL_MODE_PART_OF_DAY) {

    int actualPartOfDay = schedulerGetActualPartOfDay();
    static byte lastPartOfDay = SCHEDULER_MODE_INITIAL;
    bool brightnessTrigger=ledScheduleBrightnessReset || lastPartOfDay!=actualPartOfDay;
    ledScheduleBrightnessReset=false;
    if (lastPartOfDay != actualPartOfDay) {

      lastPartOfDay = actualPartOfDay;
    }
    
    if (actualPartOfDay==SCHEDULER_MODE_MORNING) {
      ledMode = ledModeMorning;
      if(brightnessTrigger)ledCurrentBrightness=ledMorningBrightness;
      ledSetBrightness(ledCurrentBrightness);
    }
    else if (actualPartOfDay==SCHEDULER_MODE_AFTERNOON) {
      ledMode = ledModeAfternoon;
      if(brightnessTrigger)ledCurrentBrightness=ledAfternoonBrightness;
      ledSetBrightness(ledCurrentBrightness);
    }
    else if (actualPartOfDay==SCHEDULER_MODE_EVENING) {
      ledMode = ledModeEvening;
      if(brightnessTrigger)ledCurrentBrightness=ledEveningBrightness;
      ledSetBrightness(ledCurrentBrightness);
    }
    else {
      ledMode = ledModeNight;
      if(brightnessTrigger)ledCurrentBrightness=ledNightBrightness;
      ledSetBrightness(ledCurrentBrightness);
    }
    
    ledTransitionTime=ledMode==LED_MODE_WAVE ? ledStepWaveSeconds : ledStepSwitchColorSeconds;
    unsigned long seconds=schedulerGetPhaseElapsedSeconds();
    //Brightness-only changes at a day boundary also use the long fade.
    if (ledManualOnOff==LED_MANUAL_ONOFF_AUTO && ledFadeInFromBlackSeconds>=0 && seconds<=static_cast<unsigned long>(ledFadeInFromBlackSeconds))
      ledTransitionTime=ledFadeInFromBlackSeconds;
  }

  //Manual mode
  else if (ledControlMode==CONTROL_MODE_MANUAL) {
    ledCurrentBrightness=ledManualBrightness;
    ledSetBrightness(ledCurrentBrightness);
    ledMode = ledManualMode;

    //LedStep
    if (ledMode == LED_MODE_WAVE) {
      ledTransitionTime = ledStepWaveSeconds;
    }
    else {
      ledTransitionTime = ledStepSwitchColorSeconds;
    }
  }

  if(ledRgbOverride && (ledRgbOverrideControl!=ledControlMode ||
      ledRgbOverridePhase!=(ledControlMode==CONTROL_MODE_PART_OF_DAY?schedulerGetActualPartOfDay():0) ||
      ledRgbOverrideSelection!=ledMode))ledRgbOverride=false;
  if(ledRgbOverride)ledSetCustom();
  else ledSwitchToMode(ledMode);

  bool modeDemand=ledBrightness>0 && (ledRed>0 || ledGreen>0 || ledBlue>0);
  bool on=ledStateChoice.resolve(modeDemand,ledControlMode,ledControlMode==CONTROL_MODE_PART_OF_DAY?schedulerGetActualPartOfDay():0,ledManualOnOff,megaConfigReady);
  if(!on)ledSetManualOff();
  else if(ledManualOnOff==LED_MANUAL_ONOFF_ON)ledSetManualOn();

  static byte lastBrightness=255;
  if(lastBrightness!=ledCurrentBrightness){lastBrightness=ledCurrentBrightness;reportField(FieldId::CurrentBrightness,String(ledCurrentBrightness));}
  ledLastMode = ledMode;
  ledPublishState(false);

  //Every component travels the same fraction of its own distance.
  //R=127, B=255 therefore retains its 1:2 ratio throughout a fade to black.
  double level = (double)ledBrightness * LED_PWM_MAX / (100.0 * 255.0);
  double destination[3] = {level*ledRed,level*ledGreen,level*ledBlue};
  if (destination[0]!=ledFadeTarget[0] || destination[1]!=ledFadeTarget[1] || destination[2]!=ledFadeTarget[2]) {
    // Short live color/brightness transitions; keep shutdown and wave timing.
    bool fastTransition=(ledFadeTarget[0]>0 || ledFadeTarget[1]>0 || ledFadeTarget[2]>0) &&
      (destination[0]>0 || destination[1]>0 || destination[2]>0) &&
      (ledMode!=LED_MODE_WAVE || ledRgbOverride || ledFadeBrightness!=ledBrightness);
    ledFadeBrightness=ledBrightness;
    ledFadeStart[0]=ledActualRed; ledFadeStart[1]=ledActualGreen; ledFadeStart[2]=ledActualBlue;
    for (byte i=0;i<3;i++) ledFadeTarget[i]=destination[i];
    ledFadeStarted=ledFadeLastRender=millis();
    ledFadeDuration=(unsigned long)ledTransitionTime*1000UL;
    if(fastTransition && ledFadeDuration>1000UL)
      ledFadeDuration=1000UL;
    ledFadeActive=ledFadeDuration!=0;
    ledFadePending=true;
  }
  ledFadeService();
}

void ledModeWave() {
  double level=(double)ledBrightness * LED_PWM_MAX / (100.0 * 255.0);
  if (ledBrightness==0 || ledActualRed!=level*ledRed ||
      ledActualGreen!=level*ledGreen || ledActualBlue!=level*ledBlue) return;
  if (ledRed==_LED_MAX_VALUE && ledGreen==_LED_MIN_VALUE && ledBlue==_LED_MIN_VALUE && ledWaveIdx==0) {
    //red to green
    ledRed=_LED_MIN_VALUE;
    ledGreen=_LED_MAX_VALUE;
    ledBlue=_LED_MIN_VALUE;
    return;
  }

  if (ledRed==_LED_MIN_VALUE && ledGreen==_LED_MAX_VALUE && ledBlue==_LED_MIN_VALUE && ledWaveIdx==0) {
    //green to blue
    ledRed=_LED_MIN_VALUE;
    ledGreen=_LED_MIN_VALUE;
    ledBlue=_LED_MAX_VALUE;
    return;
  }

  if (ledRed==_LED_MIN_VALUE && ledGreen==_LED_MIN_VALUE && ledBlue==_LED_MAX_VALUE && ledWaveIdx==0) {
    //blue to white
    ledRed=_LED_MAX_VALUE;
    ledGreen=_LED_MAX_VALUE;
    ledBlue=_LED_MAX_VALUE;
    return;
  }
  
  if (ledRed==_LED_MAX_VALUE && ledGreen==_LED_MAX_VALUE && ledBlue==_LED_MAX_VALUE && ledWaveIdx==0) {
    //white to red
    ledRed=_LED_MAX_VALUE;
    ledGreen=_LED_MIN_VALUE;
    ledBlue=_LED_MIN_VALUE;
    ledWaveIdx=1;
    return;
  }

  if (ledRed==_LED_MAX_VALUE && ledGreen==_LED_MIN_VALUE && ledBlue==_LED_MIN_VALUE && ledWaveIdx==1) {
    //red to blue
    ledRed=_LED_MIN_VALUE;
    ledGreen=_LED_MIN_VALUE;
    ledBlue=_LED_MAX_VALUE;
    return;
  }
  
  if (ledRed==_LED_MIN_VALUE && ledGreen==_LED_MIN_VALUE && ledBlue==_LED_MAX_VALUE && ledWaveIdx==1) {
    //blue to green
    ledRed=_LED_MIN_VALUE;
    ledGreen=_LED_MAX_VALUE;
    ledBlue=_LED_MIN_VALUE;
    return;
  }
  
  if (ledRed==_LED_MIN_VALUE && ledGreen==_LED_MAX_VALUE && ledBlue==_LED_MIN_VALUE && ledWaveIdx==1) {
    //green to white
    ledRed=_LED_MAX_VALUE;
    ledGreen=_LED_MAX_VALUE;
    ledBlue=_LED_MAX_VALUE;
    return;
  }
  
  if (ledRed==_LED_MAX_VALUE && ledGreen==_LED_MAX_VALUE && ledBlue==_LED_MAX_VALUE && ledWaveIdx==1) {
    //white to red
    ledRed=_LED_MAX_VALUE;
    ledGreen=_LED_MIN_VALUE;
    ledBlue=_LED_MIN_VALUE;  
    ledWaveIdx=0;
    return;
  }

  if (ledRed==_LED_MIN_VALUE && ledGreen==_LED_MAX_VALUE && ledBlue==_LED_MAX_VALUE) {
    //cyan to green
    ledRed=_LED_MIN_VALUE;
    ledGreen=_LED_MAX_VALUE;
    ledBlue=_LED_MIN_VALUE;
    return;
  }

  if (ledRed==_LED_MAX_VALUE && ledGreen==_LED_MAX_VALUE && ledBlue==_LED_MIN_VALUE) {
    //yellow to red
    ledRed=_LED_MAX_VALUE;
    ledGreen=_LED_MIN_VALUE;
    ledBlue=_LED_MIN_VALUE;
    return;
  }

  if (ledRed==_LED_MAX_VALUE && ledGreen==_LED_MIN_VALUE && ledBlue==_LED_MAX_VALUE) {
    //magenta to blue
    ledRed=_LED_MIN_VALUE;
    ledGreen=_LED_MIN_VALUE;
    ledBlue=_LED_MAX_VALUE;
    return;
  }

  if (ledRed==_LED_MIN_VALUE && ledGreen==_LED_MIN_VALUE && ledBlue==_LED_MIN_VALUE) {
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

void ledSetBrightness(uint32_t value) {
  ledBrightness = value>LED_BRIGHTNESS_MAX ? LED_BRIGHTNESS_MAX : value;
}

void ledPwmInit(){} //Shared timers initialized once before peripherals.
void letSetColor(uint16_t r,uint16_t g,uint16_t b){pwmWrite16(LED_RED_PIN,r);pwmWrite16(LED_GREEN_PIN,g);pwmWrite16(LED_BLUE_PIN,b);}

void ledSetCustom() {
  unsigned long CustomColor=ledCustomColor;
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
  if (ledLastMode!=LED_MODE_WAVE) {
    ledRed=255; ledGreen=0; ledBlue=0; ledWaveIdx=0;
  }
  ledMode = LED_MODE_WAVE;
}

void ledSetManualOn() {
  if (ledBrightness == 0) {ledCurrentBrightness=LED_BRIGHTNESS_MAX;if(ledControlMode==CONTROL_MODE_MANUAL)ledManualBrightness=LED_BRIGHTNESS_MAX;ledSetBrightness(LED_BRIGHTNESS_MAX);}
  ledActualState = LED_MANUAL_ONOFF_ON;
  if (ledControlMode==CONTROL_MODE_MANUAL) {
    if(ledRgbOverride)ledSetCustom();
    else ledSwitchToMode(ledManualMode);
  }
  if (ledRed == 0 && ledGreen == 0 && ledBlue == 0) {
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

void ledPublishState(bool forcePublish) {
  bool state = ledBrightness > 0 && (ledRed > 0 || ledGreen > 0 || ledBlue > 0);
  ledActualState=state ? LED_MANUAL_ONOFF_ON : LED_MANUAL_ONOFF_OFF;
  static bool lastTarget=false;
  if (forcePublish || state!=lastTarget) {
    lastTarget=state;
    reportField(FieldId::LedState,state ? "on" : "off");
  }
}
