#pragma once
#include <stdint.h>
// A manual click expires only on a new mode demand, mode or schedule phase.
class TemporaryState {
 bool initialized=false,lastDemand=false,manual=false,manualState=false;
 uint8_t lastMode=0,lastPhase=0,lastSelection=0;
public:
 // An explicit command must count as a click even if the selection is unchanged.
 void select(bool on,uint8_t& selection){selection=on?2:1;lastSelection=0;}
 bool resolve(bool demand,uint8_t mode,uint8_t phase,uint8_t& selection,bool healthy=true){
  bool clicked=selection!=lastSelection;
  if(clicked){manual=selection!=0;manualState=selection==2;}
  bool event=initialized&&(demand!=lastDemand||mode!=lastMode||phase!=lastPhase);
  if(event&&!clicked)manual=false;
  if(!healthy)manual=false;
  initialized=true;lastDemand=demand;lastMode=mode;lastPhase=phase;
  selection=manual?(manualState?2:1):0;lastSelection=selection;
  return healthy&&(manual?manualState:demand);
 }
};
