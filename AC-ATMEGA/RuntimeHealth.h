#pragma once
#include <stdint.h>
// Retained SRAM only: no settings or alarms are written to EEPROM.
struct RuntimeBreadcrumb {
 uint16_t magic;uint8_t stage,inverse;
 bool valid()const volatile{return magic==0xa63d && stage>0 && stage<=18 && uint8_t(stage^inverse)==255;}
 void mark(uint8_t value)volatile{magic=0xa63d;stage=value;inverse=uint8_t(~value);}
};
enum RuntimeStage:uint8_t {RuntimeSetup=1,RuntimeIdle,RuntimeRtc,RuntimeConfig,RuntimeLink,RuntimeAdc,RuntimeSnapshot,RuntimeEvents,RuntimeRelay,RuntimePwm,RuntimeClock,RuntimeLcd,RuntimeFan,RuntimeDht,RuntimeKeyboard,RuntimeCritical,RuntimeReport,RuntimeDiagnostics};
#ifdef __AVR__
#include <avr/io.h>
extern volatile RuntimeBreadcrumb runtimeBreadcrumb;
extern uint8_t watchdogResetFlags,watchdogPreviousStage;
extern int16_t runtimeMinimumGap;
extern char __heap_start;
extern char* __brkval;
inline int16_t runtimeMemorySample(){
 int16_t gap=int16_t(uint16_t(SP)-uint16_t(__brkval?__brkval:&__heap_start));
 if(gap<runtimeMinimumGap)runtimeMinimumGap=gap;
 return gap;
}
inline void runtimeMark(uint8_t stage){runtimeBreadcrumb.mark(stage);runtimeMemorySample();}
#else
inline int16_t runtimeMemorySample(){return -1;}
inline void runtimeMark(uint8_t){}
#endif
