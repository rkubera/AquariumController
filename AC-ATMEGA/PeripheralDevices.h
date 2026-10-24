#pragma once
#include <stdint.h>
#include <math.h>
namespace AcPeripheral {
enum Input:uint8_t {Analog=0,Dht11,Dht12,Dht21,Dht22,Ds18b20,Disabled,Digital,Level,Leak,Flow,Ph};
enum Output:uint8_t {Pwm=0,Servo};
static const uint16_t InputConfig=956,OutputConfig=972,OptionsConfig=978,ModelConfig=1034,ConfigEnd=1050;
inline bool isBinary(uint8_t type){return type==Digital||type==Level||type==Leak;}
inline bool isAnalog(uint8_t type){return type==Analog||type==Ph;}
inline bool isExternal(uint8_t type){return isBinary(type)||type==Flow||(type>=Dht11&&type<=Ds18b20);}
inline bool isDht(uint8_t type){return type>=Dht11&&type<=Dht22;}
inline bool isDigital(uint8_t type){return isDht(type)||type==Ds18b20;}
inline uint8_t crc8(const uint8_t* p,uint8_t count){uint8_t crc=0;while(count--){crc^=*p++;for(uint8_t bit=0;bit<8;bit++)crc=(crc>>1)^((crc&1)?0x8c:0);}return crc;}
inline bool decodeDht(uint8_t type,const uint8_t* p,float& temperature,float& humidity){
 temperature=humidity=NAN;
 if(!isDht(type)||uint8_t(p[0]+p[1]+p[2]+p[3])!=p[4])return false;
 if(type==Dht11||type==Dht12){
  if(p[1]>9||(p[3]&0x7f)>9)return false;
  humidity=p[0]+p[1]*0.1f;
  if(type==Dht12)temperature=(p[2]&0x7f)+(p[3]&15)*0.1f;
  else temperature=p[2]+(p[3]&15)*0.1f;
  if(type==Dht12&&(p[2]&0x80))temperature=-temperature;
  if(type==Dht11&&(p[3]&0x80))temperature=-1.0f-p[2]+(p[3]&15)*0.1f;
 }else{
  humidity=((uint16_t(p[0])<<8)|p[1])*0.1f;
  temperature=((uint16_t(p[2]&0x7f)<<8)|p[3])*0.1f;
  if(p[2]&0x80)temperature=-temperature;
 }
 return humidity>=0&&humidity<=100&&temperature>=-40&&temperature<=80;
}
inline bool decodeDs18b20(const uint8_t* p,float& temperature){
 temperature=NAN;
 if(crc8(p,8)!=p[8]||(p[4]&0x9f)!=0x1f||p[5]!=0xff||p[7]!=0x10)return false;
 int16_t raw=int16_t(uint16_t(p[0])|(uint16_t(p[1])<<8));
 uint8_t undefinedBits=3-((p[4]>>5)&3);raw&=int16_t(~((1<<undefinedBits)-1));
 temperature=raw/16.0f;return temperature>=-55&&temperature<=125;
}
}
