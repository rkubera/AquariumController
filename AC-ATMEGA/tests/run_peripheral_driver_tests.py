#!/usr/bin/env python3
"""Exercise actual peripheral ownership, conversion scheduling and servo GPIO driver."""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1]
support=r'''
#include <cstdint>
#include <cmath>
#include <string>
#include <cassert>
#include <cstdio>
#include <map>
using byte=uint8_t;using String=std::string;using std::isfinite;
#define SENSORS_COUNT 8
#define DHT_PIN 32
#define INPUT 0
#define INPUT_PULLUP 2
#define OUTPUT 1
#define LOW 0
#define _BV(n) (1U<<(n))
#define OCIE1C 3
#define OCF1C 3
#define OCIE2B 2
#define OCF2B 2
#define PCIE2 2
#define PCIF2 2
byte PCMSK2=0,PCICR=0,PCIFR=0,PINK=0;
#define ISR(x) void x()
#include "PeripheralDevices.h"
#include "ControllerFeatures.h"
#include "WireFields.h"
uint8_t SREG=128,TIMSK1=0,TIFR1=0,TIMSK2=0,TIFR2=0,OCR2B=0,TCNT2=0;
uint16_t OCR1C=0,TCNT1=0;void cli(){}void _delay_us(double){}
byte ports[80]={},ddrs[80]={},inputs[80]={};byte digitalPinToPort(byte pin){return pin;}byte digitalPinToBitMask(byte){return 1;}
volatile byte* portOutputRegister(byte p){return ports+p;}volatile byte* portModeRegister(byte p){return ddrs+p;}volatile byte* portInputRegister(byte p){return inputs+p;}
void pinMode(byte pin,byte mode){ddrs[pin]=mode==OUTPUT;if(mode==INPUT)ports[pin]=0;}
void digitalWrite(byte pin,byte value){ports[pin]=value;}
ControllerFeatures megaFeatures={15,4,6,8};bool megaConfigReady=true,dhtPeripheralActive=false,peripheralOneWireActive=false;
byte dhtBusPin=32,dhtCaptureState=0,dhtCaptureBytes[5]={};bool dhtCaptureReady=false,dhtCaptureOk=false,dhtStartReleased=false,legacyIdle=true;
void dhtTimerStop(){TIMSK2&=~_BV(OCIE2B);}void dhtSelectPin(byte pin){dhtBusPin=pin;}void dhtStartPulse(){}bool dhtBusAvailable(){return legacyIdle;}
uint32_t clockMs=0;uint32_t millis(){return clockMs;}byte device[8]={},measurement[8]={},outputDevice[6]={};
byte peripheralInputDevice(byte ch){return device[ch-1];}byte peripheralInputMeasurement(byte ch){return measurement[ch-1];}byte peripheralOutputDevice(byte ch){return outputDevice[ch-1];}
byte peripheralOptions(byte){return 2;}uint16_t peripheralDebounce(byte){return 50;}float peripheralPulsesPerLitre(byte){return 450;}byte digitalRead(byte p){return inputs[p];}
byte sensorsPeripheralPin(byte ch){return 57+ch;}float values[8]={};byte scales[8]={};void sensorsDigitalReading(byte ch,float value,byte scale){values[ch-1]=value;scales[ch-1]=scale;}
std::map<uint16_t,String> reports;void reportField(uint16_t id,String value){reports[id]=value;}String floatToString(float f){return std::to_string(f);}
uint16_t duty[80]={};void pwmWrite16(byte pin,uint16_t value){duty[pin]=value;ports[pin]=0;}
'''
main=r'''
void finishWire(bool ok){TIMSK2&=~_BV(OCIE2B);peripheralSlots.active=false;peripheralSlots.done=true;peripheralSlots.ok=ok;}
int main(){
 outputDevice[0]=1;peripheralWritePwm(1,5,0,true);assert(peripheralServo.enabled==1&&TIMSK1&_BV(OCIE1C));
 TIMER1_COMPC_vect();assert(ports[5]==1&&OCR1C==16000);TCNT1=16000;TIMER1_COMPC_vect();assert(ports[5]==0);
 peripheralWritePwm(1,5,65535,true);assert(peripheralServo.pending[0]==32000);
 outputDevice[0]=0;peripheralWritePwm(1,5,32768,true);assert(!peripheralServo.enabled&&duty[5]==32768&&!(TIMSK1&_BV(OCIE1C)));ports[5]=1;peripheralServoDetach(0);assert(ports[5]==1);outputDevice[0]=1;peripheralWritePwm(1,5,65535,true);
 peripheralWritePwm(2,44,12345,true);assert(duty[44]==12345&&peripheralServo.enabled==1);
 peripheralWritePwm(1,5,65535,false);assert(!peripheralServo.enabled&&!(TIMSK1&_BV(OCIE1C))&&!ports[5]);
 // Legacy fan DHT owns the bus until it finishes.
 device[0]=AcPeripheral::Dht22;clockMs=2000;legacyIdle=false;peripheralsService();assert(!peripheralChannel);
 legacyIdle=true;peripheralsService();assert(peripheralChannel==1&&dhtPeripheralActive&&dhtBusPin==58);
 uint8_t dht[]={2,43,0,255,44};for(byte i=0;i<5;i++)dhtCaptureBytes[i]=dht[i];dhtCaptureOk=dhtCaptureReady=true;
 peripheralsService();assert(!peripheralChannel&&!peripheralsBusBusy()&&fabs(values[0]-25.5)<.01&&dhtBusPin==32);
 measurement[0]=1;clockMs+=2000;peripheralsService();assert(peripheralChannel==1);dhtCaptureReady=dhtCaptureOk=true;peripheralsService();assert(fabs(values[0]-55.5)<.01);
 clockMs+=2000;peripheralsService();clockMs+=101;peripheralsService();assert(!peripheralChannel&&std::isnan(values[0])&&peripheralHumidityString(1)=="invalid");
 // DS18B20: ROM validation, conversion command, release during conversion, CRC read.
 device[0]=AcPeripheral::Ds18b20;peripheralsInputChanged(1);clockMs+=2000;peripheralsService();assert(peripheralStage==1&&peripheralOneWireActive);
 uint8_t rom[]={0x28,1,2,3,4,5,6,0};rom[7]=AcPeripheral::crc8(rom,7);for(byte i=0;i<8;i++)peripheralSlots.data[i]=rom[i];finishWire(true);peripheralsService();assert(peripheralStage==2);
 finishWire(true);peripheralsService();assert(!peripheralChannel&&(peripheralDsPending&1));
 clockMs+=799;peripheralsService();assert(!peripheralChannel);clockMs++;peripheralsService();assert(peripheralStage==3);
 uint8_t scratch[]={0x91,1,75,70,0x7f,0xff,0x0c,0x10,0};scratch[8]=AcPeripheral::crc8(scratch,8);for(byte i=0;i<9;i++)peripheralSlots.data[i]=scratch[i];finishWire(true);peripheralsService();assert(fabs(values[0]-25.0625)<.001&&scales[0]==16&&!(peripheralDsPending&1));
 clockMs+=2000;peripheralsService();finishWire(false);peripheralsService();assert(std::isnan(values[0])&&!peripheralsBusBusy());
 clockMs+=2000;peripheralsService();peripheralsInputChanged(1);assert(!peripheralChannel&&!peripheralsBusBusy());
 peripheralWritePwm(1,5,65535,true);peripheralsDisable();assert(!peripheralServo.enabled&&!peripheralChannel&&!peripheralsBusBusy());
 // Debounced binary inputs remain exact 0/1, not averaged fractions.
 device[0]=AcPeripheral::Level;peripheralsInputChanged(1);inputs[58]=1;peripheralsService();clockMs+=49;peripheralsService();assert(!peripheralSwitches[0].valid);clockMs++;peripheralsService();assert(values[0]==1);
 inputs[58]=0;peripheralsService();clockMs+=20;inputs[58]=1;peripheralsService();clockMs+=51;peripheralsService();assert(values[0]==1);
 inputs[58]=0;peripheralsService();clockMs+=50;peripheralsService();assert(values[0]==0);
 device[4]=AcPeripheral::Flow;peripheralsInputChanged(5);clockMs+=1000;peripheralsService();assert(PCMSK2==1&&(PCICR&_BV(PCIE2)));
 for(unsigned i=0;i<450;i++){PINK=1;PCINT2_vect();PINK=0;PCINT2_vect();}
 clockMs+=1000;peripheralsService();assert(fabs(values[4]-60)<.001);
 clockMs+=1000;peripheralsService();assert(values[4]==0);
 peripheralFlowPulses[0]=PERIPHERAL_FLOW_PULSE_LIMIT;PINK=1;PCINT2_vect();assert(!(PCMSK2&1));clockMs+=1000;peripheralsService();assert(std::isnan(values[4])&&(PCMSK2&1));
 peripheralFlowInterrupts=PERIPHERAL_FLOW_IRQ_LIMIT;PCINT2_vect();assert(!PCMSK2&&peripheralFlowOverflow);
 clockMs+=1000;peripheralsService();assert(std::isnan(values[4])&&(PCMSK2&1));
 // Editing S6 must not reset S5's count, deadline or overflow lockout.
 device[5]=AcPeripheral::Flow;peripheralsInputChanged(6);peripheralsService();
 clockMs+=1000;peripheralsService(); // discard S6's partial first window
 PINK=0;PCINT2_vect();
 for(unsigned i=0;i<225;i++){PINK=1;PCINT2_vect();PINK=0;PCINT2_vect();}
 uint32_t deadline=peripheralFlowAt;uint16_t irqCount=peripheralFlowInterrupts;
 clockMs+=500;peripheralsInputChanged(6);
 assert(peripheralFlowPulses[0]==225&&peripheralFlowAt==deadline&&peripheralFlowInterrupts==irqCount);
 peripheralsService();
 for(unsigned i=0;i<225;i++){PINK=1;PCINT2_vect();PINK=0;PCINT2_vect();}
 clockMs+=500;peripheralsService();assert(fabs(values[4]-60)<.001&&std::isnan(values[5]));
 PINK=1; // S5 has a rising edge pending while S6 is edited.
 peripheralsInputChanged(6);peripheralsService();PCINT2_vect();
 assert(peripheralFlowPulses[0]==1);PINK=0;PCINT2_vect();
 peripheralFlowOverflow=1;PCMSK2&=~1;peripheralsInputChanged(6);peripheralsService();
 assert(peripheralFlowOverflow&1);assert(!(PCMSK2&1));
 peripheralsDisable();assert(!PCMSK2&&!(PCICR&_BV(PCIE2)));
 // No OneWire state may mask the servo compare interrupt across slots.
 peripheralWritePwm(1,5,65535,true);
 peripheralPort=ports+58;peripheralDdr=ddrs+58;peripheralInput=inputs+58;peripheralMask=1;inputs[58]=0;
 peripheralOneWireStart(0x33,0,1,8);
 // Also cover the generic edge/sample fallback, rather than only short callbacks.
 peripheralSlots.shortRead=nullptr;peripheralSlots.shortWrite=nullptr;
 for(unsigned i=0;i<1000&&peripheralSlots.active;i++){
  peripheralOneWireTick();assert(TIMSK1&_BV(OCIE1C));
 }
 assert(peripheralSlots.done);peripheralReleaseBus();peripheralsDisable();

 puts("PASS: actual peripheral driver: servo attach/update/detach and PWM isolation; shared DHT arbitration, humidity selection and timeout; DS ROM/conversion/CRC sequencing and cancellation; debounced binary inputs, interrupt flow counting, zero flow and overflow; independent channel edits and servo interrupt availability across OneWire slots.");
}
'''
source=support+(root/'Peripherals.ino').read_text().replace('#include <util/delay.h>','')+main
# Arduino normally supplies these prototypes across .ino tabs.
source=source.replace('#include "PeripheralTiming.h"','void peripheralOneWireTick();void peripheralReleaseBus();\n#include "PeripheralTiming.h"',1)
with tempfile.TemporaryDirectory(prefix='ac-peripheral-driver-') as tmp:
 p=Path(tmp)/'test.cpp';p.write_text(source)
 subprocess.run(['c++','-std=c++11','-fsanitize=address,undefined','-I',str(root),str(p),'-o',tmp+'/test'],check=True)
 subprocess.run([tmp+'/test'],check=True)
