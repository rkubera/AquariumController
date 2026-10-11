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
uint32_t ticks=1;uint32_t millis(){return ticks;}
struct SerialMock{int room=0;std::string out;int availableForWrite(){return room;}void write(uint8_t x){assert(room>0);--room;out+=char(x);}}Serial;
struct Link{struct Stats{uint32_t good=0,crcErrors=0,retries=0,timeouts=0,malformed=0,schemaErrors=0;}stats;bool connected(){return true;}}wireLink;
struct Config{bool dirty(){return false;}}configStorage;
struct Queue{uint16_t free(){return 512;}}linkWireQueue;
bool megaConfigReady=true;uint32_t serialTxDroppedCommands=0;
"""
main=r"""
int main(){
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
 subprocess.run(['c++','-std=c++11','-fsanitize=address,undefined',str(p),'-o',tmp+'/test'],check=True)
 subprocess.run([tmp+'/test'],check=True)
