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

