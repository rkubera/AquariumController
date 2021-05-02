/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/
 
void keyboardInit() {
  
}

void keyboardCheckMillisEvent() {
  char key = keypad.getKey();
  if (key) {
    lastKey=key;
    lastKeyMillis = millis();
  }
  if (key=='*') {
    if (ledActualState == LED_MANUAL_ONOFF_OFF) {
      ledManualOnOff = LED_MANUAL_ONOFF_ON;
      mqttElPublish("cmnd/POWER1","ON", false);
      mqttElPublish( setBufferFromFlash(getLedState), setBufferFromFlash(charOn), false);
    }
    else {
      ledManualOnOff = LED_MANUAL_ONOFF_OFF;
      mqttElPublish("cmnd/POWER1","OFF", false);
      mqttElPublish( setBufferFromFlash(getLedState), setBufferFromFlash(charOff), false);
    }
  }
}
