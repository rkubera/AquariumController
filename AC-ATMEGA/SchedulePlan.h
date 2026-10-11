#pragma once
#include <stdint.h>
struct ScheduleSelection{uint8_t phase,bank;uint16_t elapsed;};
struct SchedulePlan{
 static uint8_t bank(uint8_t mode,uint8_t weekday){return mode?(weekday==1||weekday==7?2:1):0;}
 static ScheduleSelection select(uint8_t mode,uint8_t weekday,uint16_t minute,const uint16_t times[3][4]){
  ScheduleSelection result={3,0,2881};
  for(uint8_t day=0;day<2;day++){
   uint8_t b=bank(mode,day?(weekday==1?7:weekday-1):weekday);
   for(uint8_t phase=0;phase<4;phase++){
    int16_t age=int16_t(minute)+int16_t(day)*1440-int16_t(times[b][phase]);
    if(age>=0 && uint16_t(age)<=result.elapsed)result={phase,b,uint16_t(age)};
   }
  }
  return result;
 }
};
