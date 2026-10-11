#pragma once
#include <stdint.h>
#include <string.h>
#include "ReliableUart.h"
// Bounded queue: whole messages enter atomically, no partially queued command.
template<uint16_t Capacity> class WireQueue {
 uint8_t data[Capacity];uint16_t head=0,tail=0,count=0;
 uint8_t peek(uint16_t n)const{return data[(tail+n)%Capacity];}
public:
 uint16_t free()const{return Capacity-count;}
 void clear(){head=tail=count=0;}
 bool push(uint8_t kind,const uint8_t* p,uint8_t n){if(n>AcWire::Link::MaxMessage||free()<uint16_t(n)+2U)return false;data[head]=kind;head=(head+1)%Capacity;data[head]=n;head=(head+1)%Capacity;for(uint8_t i=0;i<n;i++){data[head]=p[i];head=(head+1)%Capacity;}count+=n+2;return true;}
 bool drain(AcWire::Link& link){if(!count||!link.idle()||!link.connected())return false;uint8_t n=peek(1),p[AcWire::Link::MaxMessage];for(uint8_t i=0;i<n;i++)p[i]=peek(i+2);if(!link.send(peek(0),p,n))return false;tail=(tail+n+2)%Capacity;count-=n+2;return true;}
};
