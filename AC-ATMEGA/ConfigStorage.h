#pragma once
#include <stdint.h>
#include <string.h>
// Runtime settings only. Native ArdPortal fields are the sole persistent source.
class ConfigStorage {
public:
 static const uint16_t PAYLOAD_SIZE=1050;
 void begin(){if(!initialized)defaults();}
 void defaults(){memset(data,255,sizeof(data));for(uint8_t i=0;i<8;i++)data[112+i*40]=0;for(uint8_t i=0;i<4;i++)data[432+i*30]=0;for(uint8_t i=0;i<6;i++)data[552+i*44]=0;for(uint8_t i=0;i<22;i++)data[956+i]=0;for(uint8_t i=0;i<8;i++){int a=978+i*7;data[a]=2;data[a+1]=50;data[a+2]=0;float pulses=450;memcpy(data+a+3,&pulses,4);}memset(data+1034,1,16);initialized=true;}
 uint8_t read(int address)const{return address>=0&&static_cast<unsigned int>(address)<PAYLOAD_SIZE?data[address]:255;}
 void update(int address,uint8_t value){if(address>=0&&static_cast<unsigned int>(address)<PAYLOAD_SIZE)data[address]=value;}
 template<typename T> void get(int address,T& value)const{uint8_t* p=reinterpret_cast<uint8_t*>(&value);for(uint16_t i=0;i<sizeof(T);i++)p[i]=read(address+i);}
 template<typename T> void put(int address,const T& value){const uint8_t* p=reinterpret_cast<const uint8_t*>(&value);for(uint16_t i=0;i<sizeof(T);i++)update(address+i,p[i]);}
private:
 uint8_t data[PAYLOAD_SIZE];bool initialized=false;
};
