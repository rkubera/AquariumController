#include "PeripheralTiming.h"
#include "DiscreteInput.h"
static DiscreteInput peripheralSwitches[8];
static volatile uint16_t peripheralFlowPulses[4]={};
// Bound interrupt work if a noisy/disconnected pulse input oscillates.
static const uint16_t PERIPHERAL_FLOW_PULSE_LIMIT=4096,PERIPHERAL_FLOW_IRQ_LIMIT=32768;
static volatile uint16_t peripheralFlowInterrupts=0;
static volatile byte peripheralFlowOverflow=0,peripheralFlowMask=0,peripheralFlowPrevious=0;
static uint32_t peripheralFlowAt=0,peripheralBinaryAt=0;
static bool peripheralPinsDirty=true;
#include <util/delay.h>
// One physical sensor per input. Timer2B is shared with the independent fan DHT.
static uint32_t peripheralAttempt[SENSORS_COUNT]={};
// DHT humidity is transmitted in tenths of a percent; 65535 marks invalid.
static uint16_t peripheralHumidity[SENSORS_COUNT]={65535,65535,65535,65535,65535,65535,65535,65535};
static byte peripheralChannel=0,peripheralStage=0,peripheralNext=0;
static byte peripheralDsPending=0;
static uint32_t peripheralStarted=0;
static volatile byte* peripheralPort;
static volatile byte* peripheralDdr;
static volatile byte* peripheralInput;
static byte peripheralMask=0;
static ServoPulses peripheralServo;
static volatile byte* peripheralServoPorts[6];
static byte peripheralServoMasks[6];
void peripheralBusLow(){*peripheralPort&=~peripheralMask;*peripheralDdr|=peripheralMask;}
void peripheralBusRelease(){*peripheralDdr&=~peripheralMask;*peripheralPort|=peripheralMask;}
bool peripheralBusRead(){return (*peripheralInput&peripheralMask)!=0;}
// Only the short 6/13 us wire slots are atomic. Conversion and reset waits
// are asynchronous; no millisecond waits run in the main loop.
void peripheralShortWrite(){peripheralBusLow();_delay_us(6);peripheralBusRelease();}
bool peripheralShortRead(){peripheralBusLow();_delay_us(3);peripheralBusRelease();_delay_us(10);return peripheralBusRead();}
static OneWireSlots peripheralSlots(peripheralBusLow,peripheralBusRelease,peripheralBusRead);

ISR(TIMER1_COMPC_vect){
 byte rise,fall;uint16_t wait=peripheralServo.advance(rise,fall);
 for(byte i=0;i<6;i++){
  if(rise&(1<<i))*peripheralServoPorts[i]|=peripheralServoMasks[i];
  if(fall&(1<<i))*peripheralServoPorts[i]&=~peripheralServoMasks[i];
 }
 OCR1C=TCNT1+wait;
}
void peripheralServoDetach(byte index){
 if(!(peripheralServo.enabled&(1<<index))&&!(peripheralServo.high&(1<<index)))return;
 byte saved=SREG;cli();peripheralServo.enabled&=~(1<<index);peripheralServo.high&=~(1<<index);
 if(peripheralServoPorts[index])*peripheralServoPorts[index]&=~peripheralServoMasks[index];
 if(!peripheralServo.enabled)TIMSK1&=~_BV(OCIE1C);SREG=saved;
}
void peripheralWritePwm(byte channel,byte pin,uint16_t value,bool healthy){
 if(!channel||channel>6)return;
 byte index=channel-1;
 if(!healthy||peripheralOutputDevice(channel)!=AcPeripheral::Servo){peripheralServoDetach(index);pwmWrite16(pin,healthy?value:0);return;}
 byte saved=SREG;cli();
 bool attached=peripheralServo.enabled&(1<<index);
 if(!attached){
  pwmWrite16(pin,0);peripheralServoPorts[index]=portOutputRegister(digitalPinToPort(pin));peripheralServoMasks[index]=digitalPinToBitMask(pin);
 }
 peripheralServo.pending[index]=ServoPulses::pulseTicks(value);
 if(!peripheralServo.enabled){peripheralServo.phase=320000;OCR1C=TCNT1+160;TIFR1=_BV(OCF1C);TIMSK1|=_BV(OCIE1C);}
 peripheralServo.enabled|=1<<index;SREG=saved;
}
bool peripheralsBusBusy(){return dhtPeripheralActive||peripheralOneWireActive;}
void peripheralOneWireTick(){
 uint8_t next=peripheralSlots.advance();
 // Prevent a servo edge ISR from stretching a short OneWire slot. Other PWM
 // channels remain hardware driven. UART and Timer0 interrupts stay enabled.
 if(peripheralSlots.shortWindow())TIMSK1&=~_BV(OCIE1C);
 else if(peripheralServo.enabled)TIMSK1|=_BV(OCIE1C);
 if(next)OCR2B=TCNT2+next;else {TIMSK2&=~_BV(OCIE2B);}
}
void peripheralOneWireStart(byte first,byte second,byte commands,byte reads){
 byte saved=SREG;cli();peripheralOneWireActive=true;
 peripheralSlots.shortRead=peripheralShortRead;peripheralSlots.shortWrite=peripheralShortWrite;
 uint8_t next=peripheralSlots.begin(first,second,commands,reads);
 OCR2B=TCNT2+next;TIFR2=_BV(OCF2B);TIMSK2|=_BV(OCIE2B);SREG=saved;
 peripheralStarted=millis();
}
void peripheralReleaseBus(){
 byte saved=SREG;cli();dhtTimerStop();dhtCaptureState=0;
 if(peripheralOneWireActive)peripheralSlots.abort();
 peripheralOneWireActive=false;dhtPeripheralActive=false;
 if(peripheralServo.enabled)TIMSK1|=_BV(OCIE1C);
 SREG=saved;
 if(peripheralChannel)pinMode(sensorsPeripheralPin(peripheralChannel),INPUT);
 dhtSelectPin(DHT_PIN);peripheralChannel=0;peripheralStage=0;
}
void peripheralsInputChanged(byte channel){
 peripheralSwitches[channel-1].reset();peripheralPinsDirty=true;
 if(channel>=5){byte saved=SREG;cli();peripheralFlowMask&=~(1<<(channel-5));PCMSK2=peripheralFlowMask;for(byte i=0;i<4;i++)peripheralFlowPulses[i]=0;peripheralFlowOverflow=0;peripheralFlowInterrupts=0;peripheralFlowAt=millis();SREG=saved;}
 if(peripheralChannel==channel)peripheralReleaseBus();
 peripheralDsPending&=~(1<<(channel-1));peripheralAttempt[channel-1]=millis();peripheralHumidity[channel-1]=65535;
 pinMode(sensorsPeripheralPin(channel),INPUT);
}
void peripheralsDisable(){
 byte saved=SREG;cli();PCMSK2=0;PCICR&=~_BV(PCIE2);peripheralFlowMask=0;SREG=saved;peripheralPinsDirty=true;
 for(byte i=0;i<8;i++)peripheralSwitches[i].reset();
 peripheralReleaseBus();peripheralDsPending=0;peripheralNext=0;
 for(byte i=0;i<6;i++)peripheralServoDetach(i);
 for(byte i=0;i<SENSORS_COUNT;i++)peripheralHumidity[i]=65535;
}
String peripheralHumidityString(byte channel){return channel&&channel<=SENSORS_COUNT&&peripheralHumidity[channel-1]!=65535&&millis()-peripheralAttempt[channel-1]<10000UL?floatToString(peripheralHumidity[channel-1]*0.1f):String("invalid");}
void peripheralFailed(){
 byte channel=peripheralChannel;
 peripheralHumidity[channel-1]=65535;peripheralDsPending&=~(1<<(channel-1));
 sensorsDigitalReading(channel,NAN,16);
 if(AcPeripheral::isDht(peripheralInputDevice(channel)))reportField(sensorField(channel,SensorField::Humidity),"invalid");
 peripheralReleaseBus();
}
// A8-A11 (S5-S8) are PORTK / PCINT16-19. Count edges even while the
// main loop handles UART, LCD or other sensors. No timer is reconfigured.
ISR(PCINT2_vect){
 byte mask=PCMSK2;if(!mask)return;
 // Count every IRQ too: fast noise can alias to an unchanged sampled level.
 if(peripheralFlowInterrupts>=PERIPHERAL_FLOW_IRQ_LIMIT){peripheralFlowOverflow|=mask;PCMSK2=0;return;}
 ++peripheralFlowInterrupts;
 byte state=PINK,edges=(state&~peripheralFlowPrevious)&PCMSK2;
 peripheralFlowPrevious=state;
 if(!edges)return;
 for(byte i=0;i<4;i++)if(edges&(1<<i)){
  if(peripheralFlowPulses[i]>=PERIPHERAL_FLOW_PULSE_LIMIT){peripheralFlowOverflow|=1<<i;PCMSK2&=~(1<<i);}
  else ++peripheralFlowPulses[i];
 }
}
void peripheralsDiscreteService(){
 uint32_t now=millis();
 if(peripheralPinsDirty){
  byte mask=0;
  for(byte ch=1;ch<=megaFeatures.inputs;ch++){
   byte type=peripheralInputDevice(ch);
   if(AcPeripheral::isBinary(type)||type==AcPeripheral::Flow){
    pinMode(sensorsPeripheralPin(ch),peripheralOptions(ch)&2?INPUT_PULLUP:INPUT);
    if(type==AcPeripheral::Flow&&ch>=5)mask|=1<<(ch-5);
   }
  }
  byte saved=SREG;cli();peripheralFlowPrevious=PINK;peripheralFlowMask=mask;PCMSK2=mask;
  if(mask){PCIFR=_BV(PCIF2);PCICR|=_BV(PCIE2);}else PCICR&=~_BV(PCIE2);
  SREG=saved;peripheralPinsDirty=false;
 }
 bool refresh=uint32_t(now-peripheralBinaryAt)>=100;if(refresh)peripheralBinaryAt=now;
 for(byte ch=1;ch<=megaFeatures.inputs;ch++)if(AcPeripheral::isBinary(peripheralInputDevice(ch))){
  bool state=(digitalRead(sensorsPeripheralPin(ch))!=0)^bool(peripheralOptions(ch)&1);
  DiscreteInput& input=peripheralSwitches[ch-1];
  bool wasValid=input.valid,old=input.value;
  if(input.sample(state,now,peripheralDebounce(ch))&&(refresh||!wasValid||old!=input.value))sensorsDigitalReading(ch,input.value?1:0,1);
 }
 uint32_t elapsed=now-peripheralFlowAt;if(elapsed<1000)return;
 peripheralFlowAt=now;
 uint16_t counts[4];byte saved=SREG;cli();byte overflow=peripheralFlowOverflow;peripheralFlowOverflow=0;peripheralFlowInterrupts=0;
 for(byte i=0;i<4;i++){counts[i]=peripheralFlowPulses[i];peripheralFlowPulses[i]=0;}
 peripheralFlowPrevious=PINK;PCIFR=_BV(PCIF2);PCMSK2=peripheralFlowMask;SREG=saved;
 for(byte i=0;i<4;i++)if(i+5<=megaFeatures.inputs&&peripheralInputDevice(i+5)==AcPeripheral::Flow){
  float flow=(overflow&(1<<i))?NAN:float(counts[i])*60000.0f/(peripheralPulsesPerLitre(i+5)*elapsed);
  sensorsDigitalReading(i+5,flow,10);
 }
}
void peripheralsService(){
 if(!megaConfigReady)return;
 peripheralsDiscreteService();
 if(peripheralChannel){
  if(millis()-peripheralStarted>100UL){peripheralFailed();return;}
  byte channel=peripheralChannel,type=peripheralInputDevice(channel);
  if(dhtPeripheralActive){
   if(!dhtCaptureReady)return;
   byte bytes[5];byte saved=SREG;cli();for(byte i=0;i<5;i++)bytes[i]=dhtCaptureBytes[i];SREG=saved;
   float temperature,humidity;
   if(!dhtCaptureOk||!AcPeripheral::decodeDht(type,bytes,temperature,humidity)){peripheralFailed();return;}
   peripheralHumidity[channel-1]=uint16_t(humidity*10.0f+0.5f);
   sensorsDigitalReading(channel,peripheralInputMeasurement(channel)?humidity:temperature,10);
   reportField(sensorField(channel,SensorField::Humidity),floatToString(humidity));
   peripheralReleaseBus();return;
  }
  if(!peripheralSlots.done)return;
  if(!peripheralSlots.ok){peripheralFailed();return;}
  if(peripheralStage==1){
   if(peripheralSlots.data[0]!=0x28||AcPeripheral::crc8(peripheralSlots.data,8)!=0){peripheralFailed();return;}
   peripheralStage=2;peripheralOneWireStart(0xcc,0x44,2,0);return;
  }
  if(peripheralStage==2){
   peripheralDsPending|=1<<(channel-1);peripheralAttempt[channel-1]=millis();peripheralReleaseBus();return;
  }
  float temperature;
  if(!AcPeripheral::decodeDs18b20(peripheralSlots.data,temperature)){peripheralFailed();return;}
  peripheralDsPending&=~(1<<(channel-1));sensorsDigitalReading(channel,temperature,16);peripheralReleaseBus();return;
 }
 if(!dhtBusAvailable()||peripheralsBusBusy())return;
 for(byte checked=0;checked<megaFeatures.inputs;checked++){
  byte index=peripheralNext++;if(peripheralNext>=megaFeatures.inputs)peripheralNext=0;
  byte channel=index+1,type=peripheralInputDevice(channel);
  if(!AcPeripheral::isDigital(type))continue;
  bool pending=peripheralDsPending&(1<<index);
  if(millis()-peripheralAttempt[index]<(pending?800UL:2000UL))continue;
  peripheralChannel=channel;peripheralStarted=millis();
  byte pin=sensorsPeripheralPin(channel),port=digitalPinToPort(pin);
  peripheralPort=portOutputRegister(port);peripheralDdr=portModeRegister(port);peripheralInput=portInputRegister(port);peripheralMask=digitalPinToBitMask(pin);
  if(AcPeripheral::isDht(type)){
   peripheralAttempt[index]=millis();dhtPeripheralActive=true;dhtSelectPin(pin);
   dhtCaptureReady=false;dhtStartReleased=false;digitalWrite(pin,LOW);pinMode(pin,OUTPUT);dhtStartPulse();
  }else{
   peripheralStage=pending?3:1;if(!pending)peripheralAttempt[index]=millis();
   peripheralOneWireStart(pending?0xcc:0x33,pending?0xbe:0,pending?2:1,pending?9:8);
  }
  return;
 }
}
