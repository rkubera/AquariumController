#!/usr/bin/env python3
"""Run the AVR register branch with simulated timers and GPIO latches."""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1]
support='''#include <cstdint>
#include <cassert>
#include <cstdio>
using byte=uint8_t;
#define __AVR__ 1
#define _BV(n) (1U<<(n))
volatile uint8_t SREG=0x80,port=0;unsigned latchCalls=0;
void cli(){SREG=0;}
byte digitalPinToPort(byte){return 0;}byte digitalPinToBitMask(byte){return 1;}
volatile uint8_t* portOutputRegister(byte){++latchCalls;return &port;}
'''
for n in (0,1,3,4,5):
 support+=f'volatile uint8_t TCCR{n}A=0,TCCR{n}B=0,TIMSK{n}=0;volatile uint16_t TCNT{n}=0,OCR{n}A=0,OCR{n}B=0,OCR{n}C=0;\n'
 for channel,bit in [('A',6),('B',4),('C',2)]:
  support+=f'#define COM{n}{channel}1 {bit+1}\n#define COM{n}{channel}0 {bit}\n'
 for suffix,bit in [('0',0),('1',1),('2',3),('3',4)]:support+=f'#define WGM{n}{suffix} {bit}\n'
 for suffix,bit in [('0',0),('1',1)]:support+=f'#define CS{n}{suffix} {bit}\n'
support+='volatile uint16_t ICR1=0,ICR3=0,ICR4=0,ICR5=0;\n'
main='''
int main(){
 pwmHardwareInit();assert(ICR1==65535 && TCCR1A==_BV(WGM11));
 assert(TCCR1B==(_BV(WGM13)|_BV(WGM12)|_BV(CS10)) && SREG==0x80);
 pwmWrite16(11,65535);assert(OCR1A==65535 && port==1 && !(TCCR1A&_BV(COM1A1)));
 unsigned calls=latchCalls;
 pwmWrite16(11,328);assert(OCR1A==328 && (TCCR1A&_BV(COM1A1)) && latchCalls==calls);
 for(uint16_t v=327;v>0;--v){pwmWrite16(11,v);assert(OCR1A==v && (TCCR1A&_BV(COM1A1)) && latchCalls==calls);}
 pwmWrite16(11,0);assert(OCR1A==0 && port==0 && !(TCCR1A&_BV(COM1A1)));
 pwmWrite16(12,7);pwmWrite16(13,13);assert(OCR1B==7 && OCR1C==13);
 pwmWrite16(7,65535);assert(OCR4B==65535 && port==1 && !(TCCR4A&_BV(COM4B1)));
 pwmWrite16(7,1);assert(OCR4B==1 && (TCCR4A&_BV(COM4B1)));
 pwmWrite16(11,65535);assert(OCR1A==65535 && !(TCCR1A&_BV(COM1A1)));
 pwmWrite16(11,0);assert(OCR1A==0 && port==0);
 assert(ICR3==65535 && ICR4==65535 && ICR5==65535); assert(SREG==0x80);
 puts("PASS: all four 16-bit timers and all outputs, continuous PWM connection, GPIO endpoints and interrupt restoration");
}
'''
with tempfile.TemporaryDirectory() as tmp:
 p=Path(tmp)/'test.cpp';p.write_text(support+(root/'PwmHardware.ino').read_text()+main)
 subprocess.run(['c++','-std=c++11','-fsanitize=address,undefined',str(p),'-o',tmp+'/test'],check=True)
 subprocess.run([tmp+'/test'],check=True)
