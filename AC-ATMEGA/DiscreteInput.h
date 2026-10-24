#pragma once
#include <stdint.h>
struct DiscreteInput {
 uint32_t changedAt=0;uint8_t candidate:1,value:1,valid:1,started:1;
 DiscreteInput():candidate(false),value(false),valid(false),started(false){}
 void reset(){valid=false;started=false;changedAt=0;}
 bool sample(bool raw,uint32_t now,uint16_t debounce){
  if(!started){candidate=raw;changedAt=now;started=true;}
  if(raw!=candidate){candidate=raw;changedAt=now;}
  if(uint32_t(now-changedAt)>=debounce){value=candidate;valid=true;}
  return valid;
 }
};
