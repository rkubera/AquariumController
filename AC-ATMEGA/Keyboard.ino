/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/
 

void keyboardCheckMillisEvent() {
  char key = keypad.getKey();
  if (key) {
    lcdBacklightWake();
    lastKey=key;
    lastKeyMillis = millis();
  }
  if (key=='*') {
    ledManualOnOff = ledActualState==LED_MANUAL_ONOFF_OFF ? LED_MANUAL_ONOFF_ON : LED_MANUAL_ONOFF_OFF;
    ledSetActualMode();
    ledPublishState(true);
  }
  else if (key=='#') {
    ledManualOnOff=LED_MANUAL_ONOFF_AUTO;
    ledSetActualMode();
    ledPublishState(true);
  }
}
