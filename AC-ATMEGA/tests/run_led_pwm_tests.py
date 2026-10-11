#!/usr/bin/env python3
"""Actual PWM registers and Timer2 scheduling, mocked AVR hardware."""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1]
support=r'''
#include <cstdint>
#include <cassert>
#define __AVR__ 1
#define F_CPU 16000000UL
#define _BV(x) (1<<(x))
#define ISR(name) void name()
using byte=uint8_t;
enum {COM0A1=7,COM0A0=6,CS21=1,OCIE2A=1,OCIE2B=2,OCF2A=1,OCF2B=2,OUTPUT=1,HIGH=1,LOW=0,BUZZER_PIN=22,DHT_PIN=32};
'''
for n in [1,3,4,5]:support+=f'enum {{COM{n}A1=7,COM{n}A0=6,COM{n}B1=5,COM{n}B0=4,COM{n}C1=3,COM{n}C0=2,WGM{n}1=1,WGM{n}0=0,WGM{n}2=3,WGM{n}3=4,CS{n}1=1,CS{n}0=0}};\nbyte TCCR{n}A=0,TCCR{n}B=0,TIMSK{n}=0;uint16_t TCNT{n}=0,ICR{n}=0,OCR{n}A=0,OCR{n}B=0,OCR{n}C=0;\n'
support+=r'''
byte SREG=0x80,TCCR0A=0xff,TCCR2A=0,TCCR2B=0,TIMSK2=0,TCNT2=0,OCR2A=0,OCR2B=0,TIFR2=0,PINC=0xff,port=0;
bool expectedDhtHigh=true;
int pins[70]={},releaseAt=-1,clockTicks=0,samples=0,toggles=0;
void cli(){SREG&=0x7f;}
void digitalWrite(int pin,int state){pins[pin]=state;}
void pinMode(int,int){}
int digitalPinToPort(int){return 0;}
byte* portOutputRegister(int){return &port;}
byte* portInputRegister(int){return &PINC;}
byte digitalPinToBitMask(int){return 1;}
void dhtTimerStart();
void dhtReleaseStartPulse(){releaseAt=clockTicks;dhtTimerStart();}
void dhtCaptureTickElapsed(bool high,uint16_t elapsed){assert(high==expectedDhtHigh);assert(elapsed==20);++samples;}
'''
tests=r'''
void advance(int count){for(int j=0;j<count;j++){++clockTicks;++TCNT2;
 if((TIMSK2&_BV(OCIE2A)) && TCNT2==OCR2A){byte before=port;TIMER2_COMPA_vect();toggles+=port!=before;}
 if((TIMSK2&_BV(OCIE2B)) && TCNT2==OCR2B)TIMER2_COMPB_vect();
}}
int main(){
 controlTimersInit();assert(TCCR0A==0x3f && SREG==0x80);
 assert(TCCR1A==2 && TCCR1B==25 && ICR1==65535 && ICR3==65535 && ICR4==65535 && ICR5==65535);
 for(byte pin:{byte(2),byte(3),byte(5),byte(7),byte(8),byte(11),byte(12),byte(13),byte(44),byte(45),byte(46)})pwmWrite16(pin,32768);
 assert(OCR1A==32768 && OCR5A==32768 && TCCR1A==0xaa && TCCR5A==0xaa);
 byte pwm5=TCCR5A;
 buzzerToneStart(1000);dhtStartPulse();advance(36000);
 assert(releaseAt==36000 && toggles==36 && TIMSK2==6 && TCCR5A==pwm5);
 expectedDhtHigh=false;PINC=0;advance(400);assert(samples==20);
 expectedDhtHigh=true;PINC=1;
 buzzerToneStop();assert(TIMSK2==4);advance(200);assert(samples==30);
 buzzerToneStart(1000);dhtTimerStop();assert(TIMSK2==2);
 advance(2000);assert(TCCR1A==0xaa && TCCR5A==pwm5);
}
'''
with tempfile.TemporaryDirectory() as d:
 p=Path(d);(p/'test.cpp').write_text('#include <initializer_list>\n'+support+(root/'PwmHardware.ino').read_text()+(root/'ControlTimers.ino').read_text()+tests)
 subprocess.run(['c++','-std=c++11','-Wall','-Wextra','-Werror','-fsanitize=address,undefined',str(p/'test.cpp'),'-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True)
print('PASS: eleven 16-bit PWM channels, Timer0 preserved; concurrent Timer2 buzzer/DHT, 18ms release, sampling and independent stop.')
