from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1]
fixture=r"""
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <string>
#define PSTR(x) x
#define snprintf_P snprintf
struct String{std::string s;String(const char* p):s(p){}const char* c_str()const{return s.c_str();}};
using byte=uint8_t;
uint32_t clockLastNtpTime=0,clockLastSynchro=0,dhtAttempts=0,dhtSuccesses=0;
byte clockSyncStatus=0,clockRtcStatus=0,rtcFaultState=0,rtcFaultStatus=0;
bool dhtValid=false,ledFadeActive=false;
byte dhtFailures=0,dhtCaptureEndState=0,dhtCaptureBits=0,dhtStartPinLevel=1,dhtCaptureBytes[5]={};
uint16_t dhtCaptureMaxGap=0,dhtStartPulseMillis=0;
uint32_t ledFadeDuration=0,ledFadeMaxGap=0,ledFadeRenderCount=0;
double ledActualRed=0,ledActualGreen=0,ledActualBlue=0;
int digitalRead(int){return 1;}
#define DHT_PIN 32
uint32_t now(){return 0;}
uint32_t ticks=1;uint32_t millis(){return ticks;}
struct SerialMock{int room=0;std::string out;int availableForWrite(){return room;}void write(uint8_t x){assert(room>0);--room;out+=char(x);}}Serial;
struct Link{struct Stats{uint32_t good=0,crcErrors=0,retries=0,timeouts=0,malformed=0,schemaErrors=0;}stats;bool connected(){return true;}}wireLink;
struct Config{bool dirty(){return false;}}configStorage;
struct Queue{uint16_t free(){return 512;}}linkWireQueue;
bool megaConfigReady=true;uint32_t serialTxDroppedCommands=0;
"""
main=r"""
int main(){
 RuntimeBreadcrumb breadcrumb={};assert(!breadcrumb.valid());
 for(byte stage=RuntimeSetup;stage<=RuntimeDiagnostics;stage++){breadcrumb.mark(stage);assert(breadcrumb.valid()&&breadcrumb.stage==stage);}
 breadcrumb.inverse^=1;assert(!breadcrumb.valid());
 breadcrumb.mark(0);assert(!breadcrumb.valid());breadcrumb.mark(19);assert(!breadcrumb.valid());
 std::string line(100,'x');usbDiagnosticLine(line.c_str());usbDiagnosticLine(line.c_str());usbDiagnosticLine(line.c_str());assert(usbDiagnosticDropped==1);
 usbDiagnosticLoop(10);assert(Serial.out.empty());
 Serial.room=1000;usbDiagnosticLoop(20);assert(Serial.out.size()==16);
 while(usbDiagnosticTail!=usbDiagnosticHead)usbDiagnosticLoop(0);
 assert(Serial.out==line+"\n"+line+"\n");
 usbDiagnosticSetting(7,12,String("on"));ticks=21;usbDiagnosticAck(7,13,String("on"));
 while(usbDiagnosticTail!=usbDiagnosticHead)usbDiagnosticLoop(0);
 assert(Serial.out.find("SET op=7 field=12 value=on")!=std::string::npos);assert(Serial.out.find("after=20 ms")!=std::string::npos);
 ticks=5001;usbDiagnosticLoop(30);while(usbDiagnosticTail!=usbDiagnosticHead)usbDiagnosticLoop(0);
 assert(Serial.out.find("loopMax=30 us")!=std::string::npos);assert(Serial.out.find("UART frames=")!=std::string::npos);
 puts("PASS: actual USB diagnostic FIFO, drop on saturation, zero writes under backpressure, 16-byte budget, wraparound and SET/ACK timing.");
}
"""
with tempfile.TemporaryDirectory() as tmp:
 p=Path(tmp)/'test.cpp';p.write_text(fixture+(root/'UsbDiagnostics.ino').read_text()+main)
 subprocess.run(['c++','-std=c++11','-I',str(root),'-fsanitize=address,undefined',str(p),'-o',tmp+'/test'],check=True)
 subprocess.run([tmp+'/test'],check=True)
