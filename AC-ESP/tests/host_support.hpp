#include <limits>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cfloat>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>
using byte = uint8_t;
using std::isfinite;
#ifndef F
#define F(x) (x)
#endif
#define PROGMEM
#define INPUT 0
#define OUTPUT 1
#define LOW 0
#define HIGH 1
#define constrain(x,a,b) ((x)<(a)?(a):((x)>(b)?(b):(x)))
#define analogInputToDigitalPin(p) ((p)+54)
#define pgm_read_byte(p) (*(p))
class String {
 public:
  std::string value;
  String() = default;
  String(char character): value(1,character) {}
  String(const char* text): value(text) {}
  String(std::string text): value(text) {}
  String(unsigned int number): value(std::to_string(number)) {}
  String(long number): value(std::to_string(number)) {}
  String(int number): value(std::to_string(number)) {}
  String(unsigned long number): value(std::to_string(number)) {}
  String(double number): String(number,2) {}
  String(double number, byte places) {
    char text[128]; std::snprintf(text,sizeof(text),"%.*f",places,number); value=text;
  }
  const char* c_str() const { return value.c_str(); }
  size_t length() const { return value.size(); }
  char operator[](size_t n) const { return n<value.size()?value[n]:0; }
  int indexOf(char ch) const { auto n=value.find(ch); return n==std::string::npos?-1:static_cast<int>(n); }
  String substring(int start) const { return value.substr(std::min(size_t(std::max(start,0)),value.size())); }
  String substring(int start,int end) const { return value.substr(start,std::max(0,end-start)); }
  long toInt() const { return std::strtol(c_str(),nullptr,10); }
  void trim() {
    auto first=value.find_first_not_of(" \r\n\t");
    if (first==std::string::npos) { value.clear(); return; }
    auto last=value.find_last_not_of(" \r\n\t"); value=value.substr(first,last-first+1);
  }
  bool startsWith(const String& prefix) const { return value.compare(0,prefix.length(),prefix.value)==0; }
  bool endsWith(const String& suffix) const { return value.size()>=suffix.length() && value.compare(value.size()-suffix.length(),suffix.length(),suffix.value)==0; }
  void remove(size_t start) { value.erase(start); }
  void toLowerCase() { std::transform(value.begin(),value.end(),value.begin(),[](unsigned char c){return std::tolower(c);}); }
  String& operator+=(const String& other) { value+=other.value; return *this; }
  bool operator!=(const String& other) const { return value!=other.value; }
  bool operator==(const String& other) const { return value==other.value; }
};
String operator+(const String& a,const String& b) { return a.value+b.value; }
String operator+(const String& a,char b) { return a.value+b; }
unsigned long ticks=0;
unsigned long millis() { return ticks; }
int adc[16]={};
int gpioMode[80]={}, gpioLevel[80]={}, pwmValue[80]={};
void pinMode(byte pin,byte mode) { gpioMode[pin]=mode; }
void digitalWrite(byte pin,byte value) { assert(gpioMode[pin]==OUTPUT); gpioLevel[pin]=value; }
void analogWrite(byte pin,int value) { gpioMode[pin]=OUTPUT; pwmValue[pin]=value; }
int analogRead(byte pin) { assert(pin<16); return adc[pin]; }
struct PowerLoss {};
struct FakeEEPROM {
  byte bytes[4096];
  int writes=0, stopAfter=-1;
  FakeEEPROM() { std::memset(bytes,255,sizeof(bytes)); }
  void begin() {}
  byte read(int addr) { assert(addr>=0 && addr<4096); return bytes[addr]; }
  void write(int addr,byte value) {
    assert(addr>=0 && addr<4096);
    if (stopAfter==writes) throw PowerLoss();
    writes++; bytes[addr]=value;
  }
  void update(int addr,byte value) { if(read(addr)!=value) write(addr,value); }
  template<typename T> void put(int addr,const T& value) {
    const byte* p=reinterpret_cast<const byte*>(&value);
    for(size_t i=0;i<sizeof(T);i++) update(addr+i,p[i]);
  }
  template<typename T> void get(int addr,T& value) {
    assert(addr>=0 && addr+sizeof(T)<=4096); std::memcpy(&value,bytes+addr,sizeof(T));
  }
} EEPROM;
#define configStorage EEPROM
byte configGetValue(int addr) { return EEPROM.read(addr); }
float configGetFloatValue(int addr) { float value; EEPROM.get(addr,value); return value; }
double configGetDoubleValue(int addr) { return configGetFloatValue(addr); }
void configSaveFloatValue(float value,int addr) { EEPROM.put(addr,value); }
void configSaveDoubleValue(double value,int addr) { configSaveFloatValue(value,addr); }
void configSaveString(String,int,int);
const int bufferOutSize=100;
char bufferOut[bufferOutSize];
int partOfDay=1;
int schedulerGetActualPartOfDay() { return partOfDay; }
struct Publication { String topic,payload; };
std::vector<Publication> publications;
void mqttElPublish(String topic,String payload,bool=false) { publications.push_back({topic,payload}); }
void mqttElPublishCommand(String topic,String payload) { publications.push_back({topic,payload}); }

#ifndef INPUT_PULLUP
#define INPUT_PULLUP 2
#endif
