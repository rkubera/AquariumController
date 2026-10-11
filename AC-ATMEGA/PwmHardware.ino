//RGB: Fast PWM 16-bit, /1 -> 244.140625 Hz. All channels: 16-bit, /1 -> 244.140625 Hz.
void pwmHardwareInit(){
#ifdef __AVR__
 byte saved=SREG;cli();TCCR0A &= ~(_BV(COM0A1)|_BV(COM0A0));
 TCCR1A=0;TCCR1B=0;TCNT1=0;TIMSK1=0;OCR1A=OCR1B=OCR1C=0;
 ICR1=65535;TCCR1A=_BV(WGM11);TCCR1B=_BV(WGM13)|_BV(WGM12)|_BV(CS10);
 TCCR3A=0;TCCR3B=0;TCNT3=0;TIMSK3=0;OCR3A=OCR3B=OCR3C=0;
 ICR3=65535;TCCR3A=_BV(WGM31);TCCR3B=_BV(WGM33)|_BV(WGM32)|_BV(CS30);
 TCCR4A=0;TCCR4B=0;TCNT4=0;TIMSK4=0;OCR4A=OCR4B=OCR4C=0;
 ICR4=65535;TCCR4A=_BV(WGM41);TCCR4B=_BV(WGM43)|_BV(WGM42)|_BV(CS40);
 TCCR5A=0;TCCR5B=0;TCNT5=0;TIMSK5=0;OCR5A=OCR5B=OCR5C=0;
 ICR5=65535;TCCR5A=_BV(WGM51);TCCR5B=_BV(WGM53)|_BV(WGM52)|_BV(CS50);
 SREG=saved;
#endif
}
// Keep COM connected during duty changes. For DC endpoints preset the PORT
// latch before disconnecting the timer; digitalWrite() disconnects too early.
void pwmSetChannel(volatile uint16_t& compare,volatile uint8_t& control,byte mask,byte enable,byte pin,uint16_t value,uint16_t maximum){
#ifdef __AVR__
 compare=value;
 if(value==0 || value==maximum){
   volatile uint8_t* output=portOutputRegister(digitalPinToPort(pin));
   byte bit=digitalPinToBitMask(pin);
   if(value) *output|=bit; else *output&=~bit;
   control &= ~mask;
 }else control=(control & ~mask)|enable;
#endif
}
void pwmWrite16(byte pin,uint16_t value){
#ifdef __AVR__
 byte saved=SREG;cli();
 switch(pin){
 case 11:pwmSetChannel(OCR1A,TCCR1A,_BV(COM1A1)|_BV(COM1A0),_BV(COM1A1),pin,value,65535);break;
 case 12:pwmSetChannel(OCR1B,TCCR1A,_BV(COM1B1)|_BV(COM1B0),_BV(COM1B1),pin,value,65535);break;
 case 13:pwmSetChannel(OCR1C,TCCR1A,_BV(COM1C1)|_BV(COM1C0),_BV(COM1C1),pin,value,65535);break;

 case 5:pwmSetChannel(OCR3A,TCCR3A,_BV(COM3A1)|_BV(COM3A0),_BV(COM3A1),pin,value,65535);break;
 case 2:pwmSetChannel(OCR3B,TCCR3A,_BV(COM3B1)|_BV(COM3B0),_BV(COM3B1),pin,value,65535);break;
 case 3:pwmSetChannel(OCR3C,TCCR3A,_BV(COM3C1)|_BV(COM3C0),_BV(COM3C1),pin,value,65535);break;
 case 6:pwmSetChannel(OCR4A,TCCR4A,_BV(COM4A1)|_BV(COM4A0),_BV(COM4A1),pin,value,65535);break;
 case 7:pwmSetChannel(OCR4B,TCCR4A,_BV(COM4B1)|_BV(COM4B0),_BV(COM4B1),pin,value,65535);break;
 case 8:pwmSetChannel(OCR4C,TCCR4A,_BV(COM4C1)|_BV(COM4C0),_BV(COM4C1),pin,value,65535);break;
 case 46:pwmSetChannel(OCR5A,TCCR5A,_BV(COM5A1)|_BV(COM5A0),_BV(COM5A1),pin,value,65535);break;
 case 45:pwmSetChannel(OCR5B,TCCR5A,_BV(COM5B1)|_BV(COM5B0),_BV(COM5B1),pin,value,65535);break;
 case 44:pwmSetChannel(OCR5C,TCCR5A,_BV(COM5C1)|_BV(COM5C0),_BV(COM5C1),pin,value,65535);break;
 }
 SREG=saved;
#else
 analogWrite(pin,value);
#endif
}
