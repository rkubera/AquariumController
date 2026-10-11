#pragma once
#include "ControllerFeatures.h"
// Page IDs stay stable internally. Keyboard numbers follow the visible order.
namespace AcLcdMenu {
enum Page:uint8_t {Home,Network,Clock,Inputs,Relays,Pwm,Led,Internal,Diagnostics,PageCount};
inline bool visible(uint8_t page,const ControllerFeatures& features){
 switch(page){
  case Home:case Network:case Clock:case Led:return true;
  case Inputs:return features.inputs>0;
  case Relays:return features.relays>0;
  case Pwm:return features.pwm>0;
  case Internal:return features.has(ControllerFeatures::Dht);
  case Diagnostics:return features.diagnostics();
  default:return false;
 }
}
inline uint8_t count(const ControllerFeatures& features){
 uint8_t result=0;for(uint8_t page=0;page<PageCount;++page)if(visible(page,features))++result;return result;
}
// Return an internal page ID for a zero-based visible position, or PageCount.
inline uint8_t at(uint8_t position,const ControllerFeatures& features){
 for(uint8_t page=0;page<PageCount;++page)if(visible(page,features)){if(!position)return page;--position;}return PageCount;
}
inline uint8_t number(uint8_t selected,const ControllerFeatures& features){
 uint8_t result=0;for(uint8_t page=0;page<PageCount;++page)if(visible(page,features)){++result;if(page==selected)return result;}return 0;
}
inline uint8_t channels(uint8_t page,const ControllerFeatures& features){
 switch(page){case Inputs:return features.inputs;case Relays:return features.relays;case Pwm:return features.pwm;default:return 1;}
}
inline void normalize(uint8_t& page,uint8_t& channel,const ControllerFeatures& features){
 if(!visible(page,features)){page=Home;channel=0;}
 const uint8_t available=channels(page,features);if(!available||channel>=available)channel=0;
}
}
