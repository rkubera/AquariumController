#pragma once
#include <stdint.h>
#include <string.h>
// Timer1 runs at 16 MHz with a 65536-tick PWM period. Keep compare waits
// below one wrap, without changing TOP/prescaler or stealing another PWM channel.
class ServoPulses {
public:
 volatile uint16_t pending[6]={};uint16_t width[6]={};volatile uint8_t enabled=0,high=0;volatile uint32_t phase=320000;
 uint16_t advance(uint8_t& rise,uint8_t& fall){
  rise=fall=0;
  if(phase>=320000){phase=0;for(uint8_t i=0;i<6;i++)width[i]=pending[i];high=enabled;rise=high;}
  for(uint8_t i=0;i<6;i++)if((high&(1<<i))&&phase>=width[i]){high&=~(1<<i);fall|=1<<i;}
  uint32_t next=320000;
  for(uint8_t i=0;i<6;i++)if((high&(1<<i))&&width[i]<next)next=width[i];
  uint32_t wait=next-phase;if(wait>48000)wait=48000;if(wait<160)wait=160;
  phase+=wait;return uint16_t(wait);
 }
 static uint16_t pulseTicks(uint16_t value){return uint16_t(16000UL+(uint32_t(value)*16000UL+32767UL)/65535UL);}
};

// One reset, up to two command bytes, and up to nine response bytes. Each
// advance performs one bounded edge/sample and returns Timer2 /8 ticks (0.5 us).
class OneWireSlots {
public:
 typedef void(*Drive)();typedef bool(*Read)();
 Drive low,release;Read read;
 Drive shortWrite=nullptr;Read shortRead=nullptr;
 uint8_t data[9]={};volatile bool active=false,done=false,ok=false;
 OneWireSlots(Drive low,Drive release,Read read):low(low),release(release),read(read){}
 uint8_t begin(uint8_t a,uint8_t b,uint8_t commands,uint8_t reads){
  if(commands<1||commands>2||reads>sizeof(data)){abort();return 0;}
  command[0]=a;command[1]=b;writeCount=commands;readCount=reads;index=bit=0;
  memset(data,0,sizeof(data));done=ok=false;active=true;present=false;
  state=ResetLow;remaining=960;low();return delay=200;
 }
 void abort(){release();active=false;done=true;ok=false;}
 bool shortWindow()const{return state==ReadLow||state==ReadSample||(state==WriteLow&&one());}
 uint8_t advance(){
  switch(state){
   case ResetLow:
    remaining-=delay;if(remaining)return delay=remaining>200?200:uint8_t(remaining);
    release();state=Presence;return delay=140;
   case Presence:present=!read();state=Recover;remaining=820;return delay=200;
   case Recover:
    remaining-=delay;if(remaining)return delay=remaining>200?200:uint8_t(remaining);
    if(!present){abort();return 0;}return startBit();
   case WriteLow:release();state=WriteHigh;return delay=one()?128:20;
   case WriteHigh:
    if(++bit==8){bit=0;++index;}
    if(index==writeCount){index=0;if(!readCount)return finish();return startRead();}
    return startBit();
   case ReadLow:release();state=ReadSample;return delay=20;
   case ReadSample:if(read())data[index]|=uint8_t(1)<<bit;state=ReadHigh;return delay=114;
   case ReadHigh:
    if(++bit==8){bit=0;++index;}
    if(index==readCount)return finish();return startRead();
  }
  abort();return 0;
 }
private:
 enum State:uint8_t{ResetLow,Presence,Recover,WriteLow,WriteHigh,ReadLow,ReadSample,ReadHigh}state;
 uint8_t command[2],writeCount=0,readCount=0,index=0,bit=0,delay=0;uint16_t remaining=0;bool present=false;
 bool one()const{return (command[index]&(uint8_t(1)<<bit))!=0;}
 uint8_t startBit(){if(one()&&shortWrite){shortWrite();state=WriteHigh;return delay=128;}low();state=WriteLow;return delay=one()?12:120;}
 uint8_t startRead(){if(shortRead){if(shortRead())data[index]|=uint8_t(1)<<bit;state=ReadHigh;return delay=114;}low();state=ReadLow;return delay=6;}
 uint8_t finish(){release();active=false;done=ok=true;return 0;}
};
