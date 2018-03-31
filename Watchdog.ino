/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/
 
void watchdogInit() {
  watchdogStartTime = clockGetGlobalDateTime();
  wdt_enable(WDTO_8S);
  wdt_reset();
}

void watchdogMillisEvent() {
  wdt_reset();
}

void watchdogMinuteEvent() {
  wdt_reset();
}

