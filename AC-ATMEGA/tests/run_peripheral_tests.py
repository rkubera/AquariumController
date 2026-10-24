#!/usr/bin/env python3
"""Validate wire codecs, servo edge timing, and asynchronous OneWire transactions."""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1]
source=r'''
#include <cassert>
#include <cmath>
#include <cstdio>
#include <vector>
#include "PeripheralDevices.h"
#include "PeripheralTiming.h"
#include "DiscreteInput.h"
using namespace AcPeripheral;
bool low=false,present=true;unsigned reads=0;std::vector<bool> bits;
void drive(){low=true;}void release(){low=false;}
bool read(){if(reads++==0)return !present;unsigned index=reads-2;return index<bits.size()?bits[index]:true;}
void packet(uint8_t type,uint8_t a,uint8_t b,uint8_t c,uint8_t d,float t,float h){uint8_t p[]={a,b,c,d,uint8_t(a+b+c+d)};float temp,humidity;assert(decodeDht(type,p,temp,humidity));assert(fabs(temp-t)<.01&&fabs(humidity-h)<.01);p[4]++;assert(!decodeDht(type,p,temp,humidity));}
int main(){
 DiscreteInput input;assert(!input.sample(false,0,50));assert(!input.sample(false,49,50));assert(input.sample(false,50,50)&&!input.value);input.reset();assert(!input.sample(true,0xffffffebUL,50));assert(input.sample(true,29,50)&&input.value);
 packet(Dht11,55,3,24,5,24.5,55.3);packet(Dht11,55,0,4,0x85,-4.5,55);
 packet(Dht12,55,3,0x84,5,-4.5,55.3);packet(Dht21,2,43,0x80,123,-12.3,55.5);packet(Dht22,2,43,0,255,25.5,55.5);
 float t,h;uint8_t bad[]={101,0,20,0,121};assert(!decodeDht(Dht11,bad,t,h));
 uint8_t scratch[]={0x5e,0xff,75,70,0x7f,0xff,0x0c,0x10,0};scratch[8]=crc8(scratch,8);
 assert(decodeDs18b20(scratch,t)&&t==-10.125);scratch[8]^=1;assert(!decodeDs18b20(scratch,t));
 for(unsigned resolution=0;resolution<4;resolution++){scratch[0]=0x5f;scratch[1]=1;scratch[4]=0x1f|(resolution<<5);scratch[8]=crc8(scratch,8);assert(decodeDs18b20(scratch,t));assert(t==float(0x15f&~((1<<(3-resolution))-1))/16);}
 ServoPulses s;assert(s.pulseTicks(0)==16000&&s.pulseTicks(65535)==32000);
 s.enabled=63;for(unsigned i=0;i<6;i++)s.pending[i]=16000+i*3000;
 uint8_t rise,fall;uint32_t time=0;unsigned frames=0;unsigned falls[6]={};
 while(frames<3){auto wait=s.advance(rise,fall);assert(wait>=160&&wait<=48000);if(rise){assert(time==frames*320000UL);frames++;}for(unsigned i=0;i<6;i++)if(fall&(1<<i)){assert(time%320000UL==16000+i*3000);falls[i]++;}time+=wait;}
 for(unsigned i=0;i<6;i++)assert(falls[i]==2);
 s=ServoPulses();s.enabled=1;s.pending[0]=16000;assert(s.advance(rise,fall)==16000&&rise==1);s.pending[0]=32000;s.advance(rise,fall);assert(fall==1); // width latched until next frame.
 OneWireSlots bus(drive,release,read);bits.clear();for(unsigned byte=0;byte<9;byte++)for(unsigned bit=0;bit<8;bit++)bits.push_back((byte*23+17)&(1<<bit));
 reads=0;auto wait=bus.begin(0xcc,0xbe,2,9);unsigned steps=0;while(bus.active){assert(wait>0&&wait<=200);wait=bus.advance();assert(++steps<1000);}assert(bus.done&&bus.ok&&!low);for(unsigned i=0;i<9;i++)assert(bus.data[i]==uint8_t(i*23+17));
 present=false;reads=0;wait=bus.begin(0x33,0,1,8);while(bus.active)wait=bus.advance();assert(bus.done&&!bus.ok&&!low);
 present=true;reads=0;bus.begin(0xcc,0x44,2,0);while(bus.active)bus.advance();assert(bus.ok&&!low);
 bus.begin(0xcc,0xbe,2,9);bus.abort();assert(bus.done&&!bus.ok&&!low);
 assert(bus.begin(0xcc,0xbe,3,9)==0&&!bus.active&&!bus.ok);assert(bus.begin(0xcc,0xbe,2,10)==0&&!bus.active&&!bus.ok);
 puts("PASS: DHT11/12/21/22 checksums and signed decoding, DS18B20 CRC/resolution, six servo pulse widths/20 ms frames, OneWire read/write and missing device.");
}
'''
with tempfile.TemporaryDirectory(prefix='ac-peripheral-tests-') as tmp:
 p=Path(tmp)/'test.cpp';p.write_text(source)
 subprocess.run(['c++','-std=c++11','-fsanitize=address,undefined','-I',str(root),str(p),'-o',tmp+'/test'],check=True)
 subprocess.run([tmp+'/test'],check=True)
