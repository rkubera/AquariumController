#!/usr/bin/env python3
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1]
source=r'''
#include <stdint.h>
#include <cassert>
#define __AVR__ 1
#define _BV(bit) (1U<<(bit))
enum{ADPS0=0,ADPS1=1,ADPS2=2,ADSC=6,ADEN=7,REFS0=6,MUX5=3};
uint8_t ADCSRA=0,ADCSRB=0,ADMUX=0;uint16_t ADC=0;
#include "CooperativeAdc.h"
int main(){CooperativeAdc adc;adc.reset();assert(ADCSRA==(_BV(ADEN)|7));
 adc.start(0);assert(ADMUX==_BV(REFS0));assert(!adc.ready());
 ADC=777;ADCSRA&=~_BV(ADSC);assert(adc.ready()&&adc.read()==777);
 adc.start(8);assert(ADCSRB&_BV(MUX5));assert(ADMUX==_BV(REFS0));assert(!adc.ready());
 adc.reset();assert(adc.ready());adc.start(55);assert(!(ADCSRB&_BV(MUX5)));assert(ADMUX==(_BV(REFS0)|1));
}
'''
with tempfile.TemporaryDirectory(prefix='ac-adc-tests-') as t:
 t=Path(t);(t/'test.cpp').write_text(source)
 subprocess.run(['c++','-std=c++11','-Wall','-Wextra','-Werror','-fsanitize=address,undefined','-I',str(root),str(t/'test.cpp'),'-o',str(t/'test')],check=True)
 subprocess.run([str(t/'test')],check=True)
print('PASS: AVR ADC start/poll, completed read, MUX5 selection and configuration abort')
