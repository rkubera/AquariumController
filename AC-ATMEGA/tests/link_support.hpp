enum {Last=0, First, Second, Third, Fourth};
enum {Sun=1,Mon,Tue,Wed,Thu,Fri,Sat};
enum {Jan=1,Feb,Mar,Apr,May,Jun,Jul,Aug,Sep,Oct,Nov,Dec};
const byte KBD_ROWS=4, KBD_COLS=4;
using boolean = bool;
#define F(x) (x)
#define pgm_read_dword_near(p) (*(p))
struct FakeSerial {
  std::string output,input;
  int writable=63;
  int availableForWrite() { return writable; }
  void write(byte) {} // Binary framing is exercised by run_wire_tests.py.
  void begin(int) {}
  void flush() {}
  int available() { return input.size(); }
  char read() { char c=input.front(); input.erase(0,1); return c; }
  void print(char c) { output+=c; }
  void print(const char* s) { output+=s; }
  void print(String s) { output+=s.value; }
  void print(unsigned long v) { output+=std::to_string(v); }
  void println() { output+='\n'; }
  template<typename T> void println(T v) { print(v); println(); }
} Serial,Serial3;
void configSaveUint32Value(uint32_t v,int addr) { CONFIG.put(addr,v); }
uint32_t configGetUint32Value(int addr) { uint32_t v; CONFIG.get(addr,v); return v; }
void configSaveValue(byte v,int addr) { CONFIG.update(addr,v); }
void configSaveValue(int v,int addr) { CONFIG.update(addr,v); }
void schedulerSaveConfiguration(); void configSaveSchedulerTimers() {schedulerSaveConfiguration();}
void clockSetLocalTime() {}
time_t clockGetLocalTime() { return 0; }
int hour(time_t) { return 12; }
int minute(time_t) { return 34; }
int second(time_t) { return 56; }
int testWeekday=2;int weekday(time_t){return testWeekday;}
void clockUpdateTimezoneRules() {}

void eventWifiConnected();
void eventWifiDisconnected();
void eventBrokerConnected();
void eventBrokerDisconnected();
void eventHostnameReceived();
void eventBrokerServerReceived();
int schedulerGetPartOfDay(double,double,bool);
int schedulerPublishActualPartOfDay() { return schedulerGetPartOfDay(12,34,true); }
String clockTimezoneCodeToWeek(int value);
String clockTimezoneCodeToDoW(int value);
String clockTimezoneCodeToMonth(int value);

void wdt_reset() {}

void clockMillisEvent() {}





void dhtTimerStart() {}
void dhtTimerStop() {}
void clockMinuteEvent() {}




#define INPUT_PULLUP 2

static bool clockInited=true;
time_t mockUtc=0;
time_t now() { return mockUtc; }
void setTime(time_t value) { mockUtc=value; }
int year(time_t) { return 2026; }
int month(time_t) { return 10; }
int day(time_t) { return 6; }
struct MockKeypad { char next=0; char getKey() { char result=next; next=0; return result; } } keypad;
std::string display[6];
int displayRow=0,displayWrites=0;
void gotoXY(int,int row) { assert(row>=0 && row<6); displayRow=row; }
void lcdString(const char* text) { displayWrites++; display[displayRow]=text; }
void lcdSetBrigtness(int) {}
String lcdTrimMessage(String value) { return value.length()>12 ? value.substring(0,12) : value; }
int mockTone=0;
void buzzerToneStart(int frequency) { mockTone=frequency; }
void buzzerToneStop() { mockTone=0; }

void rtcScheduleWrite() {}
String rtcStatusString() { return "waiting"; }
