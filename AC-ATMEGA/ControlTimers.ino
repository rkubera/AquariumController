//Timer2 is reserved for buzzer and DHT; Timer1/3/4/5 drive PWM.
volatile uint16_t buzzerHalfPeriodTicks=0,buzzerRemainingTicks=0;
volatile byte buzzerChunk=0;
volatile byte* buzzerPort;
byte buzzerPortMask;
static volatile byte* dhtInputPort;
static byte dhtInputMask;
static volatile byte dhtLastSampleTicks=0;
static volatile uint16_t dhtPulseRemainingTicks=0;
void controlTimersInit(){
 pinMode(BUZZER_PIN,OUTPUT);digitalWrite(BUZZER_PIN,LOW);
 buzzerPort=portOutputRegister(digitalPinToPort(BUZZER_PIN));buzzerPortMask=digitalPinToBitMask(BUZZER_PIN);
 dhtInputPort=portInputRegister(digitalPinToPort(DHT_PIN));
 dhtInputMask=digitalPinToBitMask(DHT_PIN);
 byte saved=SREG;cli();TCCR2A=0;TCCR2B=_BV(CS21);TIMSK2=0;SREG=saved;
 pwmHardwareInit();
}
void buzzerToneStop(){byte saved=SREG;cli();TIMSK2 &= ~_BV(OCIE2A);*buzzerPort &= ~buzzerPortMask;SREG=saved;}
void buzzerToneStart(int frequency){
 if(frequency<=0){buzzerToneStop();return;}
 byte saved=SREG;cli();buzzerHalfPeriodTicks=(F_CPU/8UL)/(2UL*frequency);
 if(buzzerHalfPeriodTicks<20)buzzerHalfPeriodTicks=20;
 buzzerRemainingTicks=buzzerHalfPeriodTicks;buzzerChunk=buzzerRemainingTicks>200?200:buzzerRemainingTicks;
 OCR2A=TCNT2+buzzerChunk;TIFR2=_BV(OCF2A);TIMSK2|=_BV(OCIE2A);SREG=saved;
}
ISR(TIMER2_COMPA_vect){
 buzzerRemainingTicks-=buzzerChunk;
 if(!buzzerRemainingTicks){*buzzerPort^=buzzerPortMask;buzzerRemainingTicks=buzzerHalfPeriodTicks;}
 buzzerChunk=buzzerRemainingTicks>200?200:buzzerRemainingTicks;
 if(buzzerChunk<20)buzzerChunk=20;
 if(buzzerRemainingTicks<buzzerChunk)buzzerRemainingTicks=buzzerChunk;
 OCR2A+=buzzerChunk;
}
void dhtTimerStart(){
 byte saved=SREG;cli();dhtPulseRemainingTicks=0;dhtLastSampleTicks=TCNT2;
 OCR2B=dhtLastSampleTicks+20;TIFR2=_BV(OCF2B);TIMSK2|=_BV(OCIE2B);SREG=saved;
}
void dhtTimerStop(){TIMSK2 &= ~_BV(OCIE2B);dhtPulseRemainingTicks=0;}
void dhtStartPulse(){byte saved=SREG;cli();dhtPulseRemainingTicks=36000;OCR2B=TCNT2+200;TIFR2=_BV(OCF2B);TIMSK2|=_BV(OCIE2B);SREG=saved;}
ISR(TIMER2_COMPB_vect){
 if(dhtPulseRemainingTicks){
  dhtPulseRemainingTicks-=200;
  if(!dhtPulseRemainingTicks){TIMSK2 &= ~_BV(OCIE2B);dhtReleaseStartPulse();}
  else OCR2B+=200;
  return;
 }
 byte sampledAt=TCNT2;byte elapsed=sampledAt-dhtLastSampleTicks;
 dhtLastSampleTicks=sampledAt;OCR2B=sampledAt+20;
 dhtCaptureTickElapsed((*dhtInputPort&dhtInputMask)!=0,elapsed);
}
