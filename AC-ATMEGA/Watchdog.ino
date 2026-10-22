/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/
 
#include "RuntimeHealth.h"
#ifdef __AVR__
volatile RuntimeBreadcrumb runtimeBreadcrumb __attribute__((section(".noinit")));
uint8_t watchdogResetFlags __attribute__((section(".noinit")));
uint8_t watchdogPreviousStage=0;
int16_t runtimeMinimumGap=INT16_MAX;
// Disable a watchdog inherited from a reset before constructors/setup run.
void watchdogEarlyDisable() __attribute__((naked,used,section(".init3")));
void watchdogEarlyDisable(){watchdogResetFlags=MCUSR;MCUSR=0;wdt_disable();}
#endif
void watchdogInit() {
#ifdef __AVR__
  // Some bootloaders clear MCUSR: flags=0 means unknown, not "no watchdog".
  if(!(watchdogResetFlags&_BV(PORF)) && runtimeBreadcrumb.valid())watchdogPreviousStage=runtimeBreadcrumb.stage;
  runtimeMark(RuntimeSetup);
#endif
  wdt_enable(WDTO_8S);
  wdt_reset();
}
