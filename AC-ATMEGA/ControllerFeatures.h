#ifndef AC_CONTROLLER_FEATURES_H
#define AC_CONTROLLER_FEATURES_H
#include <stdint.h>
// One runtime manifest shared by both firmwares. Mega has no feature defines.
struct ControllerFeatures {
 enum Flag:uint8_t {Dht=1,Rtc=2,Buzzer=4,Fan=8,NoPeripherals=16};
 uint8_t flags,relays,pwm,inputs;
 bool has(uint8_t flag)const{return (flags&flag)!=0;}
 bool diagnostics()const{return inputs||has(Dht)||has(Rtc);}
 bool valid()const{return !(flags&~uint8_t(31))&&relays<=4&&pwm<=6&&inputs<=8&&(!has(Fan)||has(Dht));}
 bool same(const ControllerFeatures& other)const{return flags==other.flags&&relays==other.relays&&pwm==other.pwm&&inputs==other.inputs;}
 void encode(uint8_t* p)const{p[0]=1;p[1]=flags;p[2]=relays;p[3]=pwm;p[4]=inputs;}
 static bool decode(const uint8_t* p,uint8_t n,ControllerFeatures& result){if(n!=5||p[0]!=1)return false;ControllerFeatures next={p[1],p[2],p[3],p[4]};if(!next.valid())return false;result=next;return true;}
};

#endif
