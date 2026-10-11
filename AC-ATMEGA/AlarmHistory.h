#pragma once
#include <stdint.h>
// Active alarms, newest activation first. Keep all causes so a hidden alarm
// returns to the visible three slots when newer alarms clear.
struct AlarmHistory {
  static const uint8_t Capacity=27;
  uint32_t active=0,timestamps[Capacity]={};
  uint8_t codes[Capacity]={},count=0;
  bool update(uint32_t current,uint32_t localTime){
    current &= (uint32_t(1)<<Capacity)-1;
    if(current==active)return false;
    uint32_t newlyActive=current & ~active;
    uint8_t kept=0;
    for(uint8_t slot=0;slot<count;slot++){
      if(current & (uint32_t(1)<<(codes[slot]-1))){
        codes[kept]=codes[slot];timestamps[kept++]=timestamps[slot];
      }
    }
    count=kept;
    for(uint8_t bit=0;bit<Capacity;bit++)if(newlyActive & (uint32_t(1)<<bit)){
      for(uint8_t slot=count;slot>0;slot--){codes[slot]=codes[slot-1];timestamps[slot]=timestamps[slot-1];}
      codes[0]=bit+1;timestamps[0]=localTime;++count;
    }
    for(uint8_t slot=count;slot<Capacity;slot++){codes[slot]=0;timestamps[slot]=0;}
    active=current;return true;
  }
};
