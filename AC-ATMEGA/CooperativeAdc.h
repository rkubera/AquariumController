#pragma once
#include <stdint.h>
// Own the Mega ADC; start conversions and poll completion without waiting.
class CooperativeAdc {
#ifndef __AVR__
 uint16_t result=0;
#endif
public:
 void reset(){
#ifdef __AVR__
  ADCSRA &= ~_BV(ADEN); // Abort a conversion when the configuration is replaced.
  ADCSRA=_BV(ADEN)|_BV(ADPS2)|_BV(ADPS1)|_BV(ADPS0);
  ADMUX=_BV(REFS0); // AVcc, matching the sketch's default analog reference.
#endif
 }
 void start(uint8_t pin){
#ifdef __AVR__
  if(pin>=54)pin-=54;
  ADCSRB=(ADCSRB&~_BV(MUX5))|(((pin>>3)&1)<<MUX5);
  ADMUX=_BV(REFS0)|(pin&7);
  ADCSRA|=_BV(ADSC);
#else
  result=analogRead(pin);
#endif
 }
 bool ready()const{
#ifdef __AVR__
  return !(ADCSRA&_BV(ADSC));
#else
  return true;
#endif
 }
 uint16_t read()const{
#ifdef __AVR__
  return ADC;
#else
  return result;
#endif
 }
};
