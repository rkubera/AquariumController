#pragma once
#include <stdint.h>
// Seconds plus a millisecond remainder survive millis() wrap without 64-bit RAM.
// This is deliberately ordinary SRAM: an MCU reset must start a new boot record.
struct BootTimestamp {
 uint32_t sampledMillis=0,elapsedSeconds=0,utc=0;
 void tick(uint32_t ms){uint32_t delta=ms-sampledMillis;if(delta<1000UL)return;uint32_t whole=delta/1000UL;elapsedSeconds+=whole;sampledMillis+=whole*1000UL;}
 uint16_t remainder(uint32_t ms)const{return uint16_t(ms-sampledMillis);}
 bool synchronize(uint32_t currentUtc,uint32_t ms){
  tick(ms);
  if(currentUtc<1577836800UL||currentUtc==UINT32_MAX||currentUtc<elapsedSeconds)return false;
  uint32_t candidate=currentUtc-elapsedSeconds;
  if(candidate<1577836800UL)return false;
  uint32_t difference=candidate>utc?candidate-utc:utc-candidate;
  // Normal second-resolution NTP jitter must not look like another restart.
  if(!utc||difference>2){utc=candidate;return true;}
  return false;
 }
};
