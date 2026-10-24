/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/

 //********************************
//Serials
//********************************
#define SERIAL_TX_BUFFER_SIZE 64
#define SERIAL_RX_BUFFER_SIZE 64

#define _DEGUB_NONE     0
#define _DEBUG_ERROR    1
#define _DEBUG_WARNING  2
#define _DEBUG_NOTICE   4
#define _DEBUG_LINK     8

#define TELEMETRY_MIN_INTERVAL_MS 5000

#define DEBUG_LEVEL 0

#include <avr/wdt.h>
#include "RuntimeHealth.h"
#include "ConfigStorage.h"
#include "PeripheralDevices.h"
#include "ControllerFeatures.h"
#include "LcdMenuLayout.h"
ControllerFeatures megaFeatures={0,0,0,0};
bool megaFeaturesKnown=false;
#include "RestoreFields.h"
#include "FeatureFields.h"
#include "ReliableUart.h"
#include "WireQueue.h"
#include "WireFields.h"
#include "ReportField.h"
#include "TemporaryState.h"
#include "SchedulePlan.h"
void megaWireReceive(uint8_t,const uint8_t*,uint8_t);
void megaWireConnection(bool);
extern bool megaConfigReady;
AcWire::Link wireLink(megaWireReceive,megaWireConnection);
WireQueue<512> linkWireQueue;
extern ConfigStorage configStorage;
#include <math.h>
#include <float.h>
#include <Timezone.h>       // https://github.com/JChristensen/Timezone
#include <Keypad.h>         // https://playground.arduino.cc/Code/Keypad#Download Must be patched. Replace: #define OPEN LOW with #define KBD_OPEN LOW, #define CLOSED HIGH with #define KBD_CLOSED HIGH in Key.h and Keypad.h. Replace OPEN with KBD_OPEN, CLOSE with KBD_CLOSE in Keypad.cpp 



//********************************
//PINS
//********************************
#define BUZZER_PIN 22

#define DIGITAL_PWM_12V_OUT_PIN_1 11
#define DIGITAL_PWM_12V_OUT_PIN_2 12

#define DIGITAL_PWM_OUT_PIN_1  5
#define DIGITAL_PWM_OUT_PIN_2  44
#define DIGITAL_PWM_OUT_PIN_3  45
#define DIGITAL_PWM_OUT_PIN_4  46

#define ANALOG_IN_PIN_0 4
#define ANALOG_IN_PIN_1 5
#define ANALOG_IN_PIN_2 6
#define ANALOG_IN_PIN_3 7
#define ANALOG_IN_PIN_4 8
#define ANALOG_IN_PIN_5 9
#define ANALOG_IN_PIN_6 10
#define ANALOG_IN_PIN_7 11

#define PWM_MAX 65535UL
#define CONFIG_pwm_highbits_addr 944
#define LED_PWM_MAX 65535UL
#define LED_BRIGHTNESS_MAX 100UL
#define LED_RED_PIN 6
#define LED_GREEN_PIN 7
#define LED_BLUE_PIN 8

#define LCD_PIN_SCE 42
#define LCD_PIN_RESET 40
#define LCD_PIN_DC 38
#define LCD_PIN_SDIN 36
#define LCD_PIN_SCLK 34
#define LCD_PIN_LED   2 //Must be PWM Output
#define LCD_BACKLIGHT_ACTIVE_LOW 0 //This module lights with HIGH; LOW switches off.

#define RELAY_PIN_1 47
#define RELAY_PIN_2 49
#define RELAY_PIN_3 51
#define RELAY_PIN_4 53

#define DHT_PIN 32

#define FAN_PIN 4 //8-bit Timer0 PWM; D3 remains available for 16-bit PWM.
#define FAN_PWM_MAX 255U

//********************************
//Keyboard
//********************************
const byte KBD_ROWS = 4; //four rows 
const byte KBD_COLS = 4; //four columns
char keys[KBD_ROWS][KBD_COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte KBD_ROW_PINS[KBD_ROWS] = {43, 41, 39, 37}; //connect to the row pinouts of the keypad
byte KBD_COL_PINS[KBD_COLS] = {35, 33, 31, 29}; //connect to the column pinouts of the keypad

//byte KBD_ROW_PINS[KBD_ROWS] = {23, 25, 27, 29, 31}; //connect to the row pinouts of the keypad
//byte KBD_COL_PINS[KBD_COLS] = {39, 37, 35, 33}; //connect to the column pinouts of the keypad

//byte KBD_ROW_PINS[KBD_ROWS] = {39, 37, 35, 33, 31}; //connect to the row pinouts of the keypad
//byte KBD_COL_PINS[KBD_COLS] = {23, 25, 27, 29}; //connect to the column pinouts of the keypad

//********************************
//Main variables
//********************************
const char daysOfTheWeek[7][10] PROGMEM = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const char monthsOfYear[12][10] PROGMEM = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };






const char charOn[] PROGMEM = "on";
const char charOff[] PROGMEM = "off";

const char charBlack[] PROGMEM = "black";
const char charRed[] PROGMEM = "red";
const char charGreen[] PROGMEM = "green";
const char charBlue[] PROGMEM = "blue";
const char charCyan[] PROGMEM = "cyan";
const char charMagenta[] PROGMEM = "magenta";
const char charYellow[] PROGMEM = "yellow";
const char charWhite[] PROGMEM = "white";
const char charWave[] PROGMEM = "wave";

const char charCustom[] PROGMEM = "custom";

const char charManual[] PROGMEM = "manual";
const char charPartofday[] PROGMEM = "partofday";
const char charTreshold[] PROGMEM = "treshold";
const char charPid[] PROGMEM = "pid";

const char charSensornone[] PROGMEM = "notconnected";
const char charRelay[] PROGMEM = "relay";
const char charPWMOutput[] PROGMEM = "pwmoutput";




const char charDirect[] PROGMEM = "direct";
const char charReverse[] PROGMEM = "reverse";





//********************************
//Control
//********************************

#define CONTROL_MODE_MANUAL             0
#define CONTROL_MODE_PART_OF_DAY        1
#define CONTROL_MODE_TRESHOLD           2
#define CONTROL_MODE_PID                3
#define CONTROL_TRESHOLD_DIRECTION      253

#define NAME_LENGTH                     15
#define OUTPUT_TYPE_PWM                 1
#define OUTPUT_TYPE_RELAY               2


//********************************
//Fan
//********************************
byte fanStartTemperature = 30;
byte fanMaxSpeedTemperature = 35;
byte maxInternalTemperature = 50;

//********************************
//PWMOutputs
//********************************
#define PWMOUTPUTS_PWMOUTPUT_CONFIG_BYTES  44
#define PWMOUTPUTS_COUNT                   6



#define PWMOUTPUT_CONTROL_MODE             2
#define PWMOUTPUT_MODE_MORNING             3
#define PWMOUTPUT_MODE_AFTERNOON           4
#define PWMOUTPUT_MODE_EVENING             5
#define PWMOUTPUT_MODE_NIGHT               6
#define PWMOUTPUT_MANUAL_MODE              8

#define PWMOUTPUT_CONTROL_DIRECTION        9
#define PWMOUTPUT_SENSORS_SETPOINT         10
#define PWMOUTPUT_PID_KP                   11
#define PWMOUTPUT_PID_KI                   12
#define PWMOUTPUT_PID_KD                   13
#define PWMOUTPUT_MAX_DEVIATION            14
#define PWMOUTPUT_OUTPUT_VALUE             15
#define PWMOUTPUT_PID_VALID                17

#define PID_AUTOMATIC                      1
#define PID_MANUAL                         0
#define CONTROL_DIRECT                     0
#define PID_REVERSE                        1
#define PID_P_ON_M                         0
#define PID_P_ON_E                         1

//********************************
//Relays
//********************************
#define RELAYS_RELAY_CONFIG_BYTES         30
#define RELAYS_COUNT                      4

#define RELAY_MODE_OFF                    0
#define RELAY_MODE_ON                     1
#define RELAY_MODE_NONE                   3

#define RELAY_MANUAL_ONOFF_AUTO           0
#define RELAY_MANUAL_ONOFF_OFF            1
#define RELAY_MANUAL_ONOFF_ON             2

#define RELAY_STATE                       1
#define RELAY_CONTROL_MODE                2
#define RELAY_MODE_MORNING                3
#define RELAY_MODE_AFTERNOON              4
#define RELAY_MODE_EVENING                5
#define RELAY_MODE_NIGHT                  6
#define RELAY_MANUAL_ONOFF                7
#define RELAY_MAX_DEVIATION               8
#define RELAY_SENSORS_SETPOINT            9
#define RELAY_CONTROL_DIRECTION           10

//********************************
//Sensors
//********************************
#define SENSORS_SENSOR_CONFIG_BYTES       40

#define SENSORS_COUNT                     8
#define SENSOR_PUBLISH_INTERVAL_MS        1000UL
#define ANALOG_CONTROL_INTERVAL_MS        100UL
#define SENSOR_ADC_SAMPLES                 10
#define SENSOR_FILTER_WINDOW               3
#define SENSOR_MAX_AGE_MILLIS             (5UL*ANALOG_CONTROL_INTERVAL_MS)
#define SENSORS_VALUE_MIN                 12
#define SENSORS_VALUE_MAX                 13
#define SENSORS_VALUE_CRITICAL_MIN        14
#define SENSORS_VALUE_CRITICAL_MAX        15
#define SENSORS_VALUE_VALID               16

#define SENSOR_TYPE_NONE                  254

#define SENSORS_VALUE                     1
#define SENSORS_VALUE_RAW                 2
#define SENSORS_VALUE_TYPE                3
#define SENSORS_VALUE_CALIB_VALUE1        8
#define SENSORS_VALUE_CALIB_RAW_VALUE1    9
#define SENSORS_VALUE_CALIB_VALUE2        10
#define SENSORS_VALUE_CALIB_RAW_VALUE2    11

//********************************
//Wifi
//********************************
#define WIFI_STATUS_CONNECTED     1
#define WIFI_STATUS_DISCONNECTED  0
char wifiStatus = WIFI_STATUS_DISCONNECTED;

//********************************
//Errors
//********************************

int errorsCount = 0;
int errorsReportStatus;
int errorsLastReportStatus;

//********************************
//TimeZone
//********************************
#define TIMEZONE_WEEK_FIRST   First
#define TIMEZONE_WEEK_SECOND  Second
#define TIMEZONE_WEEK_THIRD   Third
#define TIMEZONE_WEEK_FOURTH  Fourth
#define TIMEZONE_WEEK_LAST    Last

#define TIMEZONE_DOW_SUNDAY     Sun
#define TIMEZONE_DOW_MONDAY     Mon
#define TIMEZONE_DOW_TUESDAY    Tue
#define TIMEZONE_DOW_WEDNESDAY  Wed
#define TIMEZONE_DOW_FOURSDAY   Thu
#define TIMEZONE_DOW_FRIDAY     Fri
#define TIMEZONE_DOW_SATURDAY   Sat

#define TIMEZONE_MONTH_JANUARY    Jan
#define TIMEZONE_MONTH_FEBRUARY   Feb
#define TIMEZONE_MONTH_MARCH      Mar
#define TIMEZONE_MONTH_APRIL      Apr
#define TIMEZONE_MONTH_MAY        May
#define TIMEZONE_MONTH_JUNE       Jun
#define TIMEZONE_MONTH_JULY       Jul
#define TIMEZONE_MONTH_AUGUST     Aug
#define TIMEZONE_MONTH_SEPTEMBER  Sep
#define TIMEZONE_MONTH_OCTOBER    Oct
#define TIMEZONE_MONTH_NOVEMBER   Nov
#define TIMEZONE_MONTH_DECEMBER   Dec

const char timezoneWeekFirst[] PROGMEM = "first";
const char timezoneWeekSecond[] PROGMEM = "second";
const char timezoneWeekThird[] PROGMEM = "third";
const char timezoneWeekFourth[] PROGMEM = "fourth";
const char timezoneWeekLast[] PROGMEM = "last";

char timezoneActualOffset = 1;
byte timezoneRule1Week;
byte timezoneRule2Week;
byte timezoneRule1DayOfWeek;
byte timezoneRule2DayOfWeek;
byte timezoneRule1Hour;
byte timezoneRule2Hour;
char timezoneRule1Offset;
char timezoneRule2Offset;
byte timezoneRule1Month;
byte timezoneRule2Month;

//********************************
//Clock
//********************************

TimeChangeRule myDST;
TimeChangeRule mySTD;
Timezone myTZ (myDST, mySTD);
TimeChangeRule *tcr;        //pointer to the time change rule, use to get TZ abbrev

byte globalSecond = 0;
byte globalMinute = 43;
byte globalHour = 22; //24 hour time
byte globalWeekDay = 6; //0-6 -> sunday – Saturday
byte globalMonthDay = 10;
byte globalMonth = 2;
int globalYear = 18;

uint32_t clockLastSynchro = 0;
uint32_t clockLastNtpTime = 0;
byte clockSyncStatus = 0;
byte clockRtcStatus = 0;
uint32_t clockTimeGeneration = 0;
#define CLOCK_MIN_VALID_NTP_TIME 1577836800UL //2020-01-01 UTC
#define CLOCK_MAX_NTP_STEP 300UL
#define CLOCK_SYNC_INTERVAL_MILLIS 10000UL

//********************************
//Scheduler
//********************************
const char schedulerMorning[] PROGMEM = "morning";
const char schedulerAfternoon[] PROGMEM = "afternoon";
const char schedulerEvening[] PROGMEM = "evening";
const char schedulerNight[] PROGMEM = "night";

#define SCHEDULER_MODE_NONE 0
#define SCHEDULER_MODE_MORNING 1
#define SCHEDULER_MODE_AFTERNOON 2
#define SCHEDULER_MODE_EVENING 3
#define SCHEDULER_MODE_NIGHT 4
#define SCHEDULER_MODE_INITIAL 254

byte schedulerStartMorningHour = 9;
byte schedulerStartMorningMinute = 0;

byte schedulerStartAfternoonHour = 10;
byte schedulerStartAfternoonMinute = 0;

byte schedulerStartEveningHour = 19;
byte schedulerStartEveningMinute = 0;

byte schedulerStartNightHour = 22;
byte schedulerStartNightMinute = 00;

//********************************
//Events
//********************************
unsigned long lastCriticalEvent =0;
#define CRITICAL_EVENT_MIN_MILLIS 30
unsigned long timerMillisEventDate;
unsigned long timerTenSecondsEventDate;
unsigned long timerSecondEventDate;
unsigned long timerMinuteEventDate;
unsigned long timerHourEventDate;

//********************************
//Buzzer
//********************************
#define BUZZER_ON 1
#define BUZZER_OFF 0

byte buzzerOnStart = BUZZER_OFF;
byte buzzerOnErrors = BUZZER_OFF;
byte buzzerPhaseMask=7;

//********************************
//LED
//********************************
#define _LED_MIN_VALUE 0
#define _LED_MAX_VALUE 255
#define _LED_RANGE_VALUES (_LED_MAX_VALUE - _LED_MIN_VALUE)

#define LED_MODE_NONE    0
#define LED_MODE_WHITE   1
#define LED_MODE_WAVE    2
#define LED_MODE_RED     3
#define LED_MODE_GREEN   4
#define LED_MODE_BLUE    5
#define LED_MODE_CYAN    6
#define LED_MODE_MAGENTA 7
#define LED_MODE_YELLOW  8
#define LED_MODE_BLACK   9
#define LED_MODE_CUSTOM 10

/*
*/

byte ledControlMode = CONTROL_MODE_MANUAL;
byte ledManualMode = LED_MODE_NONE;

uint16_t ledCurrentBrightness=0;
bool ledScheduleBrightnessReset=true;
unsigned long ledCustomColor = 0xFFFFFF;

uint16_t ledBrightness = 0;

uint16_t ledMorningBrightness = 0;
uint16_t ledAfternoonBrightness = 0;
uint16_t ledEveningBrightness = 0;
uint16_t ledNightBrightness = 0;
uint16_t ledManualBrightness = 0;

byte ledMode = LED_MODE_NONE;
byte ledModeMorning = LED_MODE_NONE;
byte ledModeAfternoon = LED_MODE_NONE;
byte ledModeEvening = LED_MODE_NONE;
byte ledModeNight = LED_MODE_NONE;

double ledActualRed = _LED_MIN_VALUE;
double ledActualGreen = _LED_MIN_VALUE;
double ledActualBlue = _LED_MIN_VALUE;

int ledRed = _LED_MIN_VALUE;
int ledGreen = _LED_MIN_VALUE;
int ledBlue = _LED_MIN_VALUE;

byte ledStepSwitchColorSeconds = 4;
byte ledStepWaveSeconds = 10;
int ledFadeInFromBlackSeconds = 120;

uint16_t ledTransitionTime = ledStepSwitchColorSeconds;

byte ledWaveIdx;
byte ledLastMode = LED_MODE_NONE;

#define LED_MANUAL_ONOFF_AUTO 0
#define LED_MANUAL_ONOFF_OFF 1
#define LED_MANUAL_ONOFF_ON 2

int ledActualState = LED_MANUAL_ONOFF_OFF;

byte ledManualOnOff = LED_MANUAL_ONOFF_AUTO;

double ledRedLevel, ledGreenLevel, ledBlueLevel;


//********************************
//LCD
//********************************
//Configuration for the LCD
byte lcdContrast = 40; //PCD8544 Vop, lower values lighten the background.
#define LCD_C     LOW
#define LCD_D     HIGH
#define LCD_CMD   0

// Size of the LCD
#define LCD_X     84
#define LCD_Y     48

//Power
#define LCD_GND   46
#define LCD_POWER 44

//********************************
//LINK
//********************************

String wifi_ssid = "";
String brokerServer = "";

String deviceName = "";
bool hostnameReceived = false;
bool brokerServerReceived = false;

const char published[] PROGMEM = "published";

#define BROKER_STATUS_CONNECTED     1
#define BROKER_STATUS_DISCONNECTED  0

byte brokerStatus = BROKER_STATUS_DISCONNECTED;



//********************************
//CONFIG
//********************************

//Daily values
#define CONFIG_ledModeMorning_addr                          2
#define CONFIG_ledModeAfternoon_addr                        3
#define CONFIG_ledModeEvening_addr                          4
#define CONFIG_ledModeNight_addr                            5

#define CONFIG_schedulerStartMorningHour_addr               6
#define CONFIG_schedulerStartMorningMinute_addr             7
#define CONFIG_schedulerStartAfternoonHour_addr             8
#define CONFIG_schedulerStartAfternoonMinute_addr           9
#define CONFIG_schedulerStartEveningHour_addr               10
#define CONFIG_schedulerStartEveningMinute_addr             11
#define CONFIG_schedulerStartNightHour_addr                 12
#define CONFIG_schedulerStartNightMinute_addr               13

#define CONFIG_ledMorningBrightness_addr                    14
#define CONFIG_ledAfternoonBrightness_addr                  18
#define CONFIG_ledEveningBrightness_addr                    22
#define CONFIG_ledNightBrightness_addr                      26
#define CONFIG_ledManualBrightness_addr                     30

#define CONFIG_ledControlMode_addr                          56
#define CONFIG_ledManualMode_addr                           58

#define CONFIG_buzzerOnStart_addr                           59
#define CONFIG_buzzerOnErrors_addr                          60
#define CONFIG_buzzerPhases_addr                            61

//Global values
#define CONFIG_maxInternalTemperature_addr                  36

#define CONFIG_fanStartTemperature_addr                     34
#define CONFIG_fanMaxSpeedTemperature_addr                  35

#define CONFIG_timezoneRule1Week_addr                       85
#define CONFIG_timezoneRule2Week_addr                       86
#define CONFIG_timezoneRule1DayOfWeek_addr                  87
#define CONFIG_timezoneRule2DayOfWeek_addr                  88
#define CONFIG_timezoneRule1Hour_addr                       89
#define CONFIG_timezoneRule2Hour_addr                       90
#define CONFIG_timezoneRule1Offset_addr                     91
#define CONFIG_timezoneRule2Offset_addr                     92
#define CONFIG_timezoneRule1Month_addr                      93
#define CONFIG_timezoneRule2Month_addr                      94

#define CONFIG_unix_timestamp_addr                          95 //(4 bytes)

#define CONFIG_ledCustomColor_addr                         100 //(4 bytes)

#define CONFIG_config_version_addr                          108
#define CONFIG_sensor_limits_addr                           816
#define SENSOR_LIMITS_CONFIG_BYTES                           16

#define CONFIG_sensors_addr                                 112
#define CONFIG_relays_addr                                  (CONFIG_sensors_addr+(SENSORS_SENSOR_CONFIG_BYTES*SENSORS_COUNT)) //4*20 = 80
#define CONFIG_pwm_outputs_addr                             (CONFIG_relays_addr+(RELAYS_RELAY_CONFIG_BYTES*RELAYS_COUNT))

//********************************
//Keyboard
//********************************
char lastKey;
unsigned long lastKeyMillis = 0;
Keypad keypad = Keypad(makeKeymap(keys), KBD_ROW_PINS, KBD_COL_PINS, KBD_ROWS, KBD_COLS);

//********************************
//Watchdog
//********************************
time_t watchdogupTime;

//********************************
//DHT
//********************************

float dhtHumidity = NAN;
bool dhtValid = false;
byte dhtReadingCount = 0;
unsigned long dhtLastReadingMillis = 0;
float dhtTemperature = NAN;

byte dhtNumReadings = 20;
uint16_t dhtTemparatureReadings[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
uint16_t dhtHumidityReadings[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

//********************************
//Fan
//********************************
byte fanPWM = FAN_PWM_MAX;

//********************************
//Main program
//********************************

const int bufferOutSize = 32; // Names <=15 bytes; date/time prefixes <=25 bytes including NUL.
char bufferOut[bufferOutSize];
int publishValue = -1;
uint32_t serialPartialTimeouts=0;
uint32_t serialCrcErrors = 0;
uint32_t serialOversizedFrames = 0;
uint32_t serialMalformedFrames = 0;
uint32_t serialValidFrames = 0;


//Complete UART frames are queued atomically; hardware transmission never waits.

uint32_t serialTxDroppedCommands=0;
bool linkSnapshotCapture=false,linkSnapshotSelectedSent=false,linkSnapshotRetry=false;
uint16_t linkSnapshotField=0,linkSnapshotFieldCount=0;
byte linkSnapshotChannel=0;

bool linkLocalPanel=false,linkPanelReading=false;
uint16_t linkPanelField=0;
uint32_t linkPanelId=0;
String linkPanelReadValue;
bool linkPanelFound=false;

byte dhtBusPin=DHT_PIN;
bool dhtPeripheralActive=false;
volatile bool peripheralOneWireActive=false;

void setup() {
    //Init
  Serial.begin(115200);

  //Watchdog
  watchdogInit();
  
  controlTimersInit();

  //LCD
  lcdInit();
  gotoXY(0, 1);
  lcdString((char *)"Initializing");

  //Config
  configLoad();

  //Clock
  clockInit();
  if(megaFeatures.has(ControllerFeatures::Rtc))rtcInit();

  //LED
  ledInit();

  //Fan
  initFan();

  //Keyboard

  //LINK
  linkInit();

  //Scheduler
  
  //DHT
  if(megaFeatures.has(ControllerFeatures::Dht))dhtInit();
  
  //Acquire inputs cooperatively; output interlocks wait for valid readings.
  sensorsInit();
  sensorsResetAcquisition();

  //Relays
  relaysInit();

  //PWMOutputs
  pwmOutputsInit();

  //clockSetLocalTime();


  //Errors
  errorsInit();

  //Events
  timerMillisEventDate = millis();
  timerSecondEventDate = millis();
  timerTenSecondsEventDate = millis();
  timerMinuteEventDate = millis();
  timerHourEventDate = millis();

  //Menu
  lcdClear();
  menuInit();
  
  //Buzzer
  // Startup melody runs after the verified ESP configuration is applied.

  wireRequest(AcWire::Hostname);

}

void loop() {
  uint32_t usbLoopStarted=micros();
  ledFadeService();
  runtimeMark(RuntimeConfig);lcdContrastStorageService();
  runtimeMark(RuntimeRtc);rtcService();
  runtimeMark(RuntimeConfig);configStorageMillisEvent();
  runtimeMark(RuntimeLink);linkSerialService();
  runtimeMark(RuntimeAdc);sensorsMillisEvent();peripheralsService();
  runtimeMark(RuntimeSnapshot);uint32_t snapshotAt=micros();linkSnapshotStep();usbDiagnosticStage(8,micros()-snapshotAt);

  if (millis() - timerMillisEventDate > 1) {
    runtimeMark(RuntimeEvents);eventTimerMillis();
    timerMillisEventDate = millis();
  }

  if (millis() - timerSecondEventDate > 1000) {
    runtimeMark(RuntimeEvents);eventTimerSecond();
    timerSecondEventDate = millis();
  }

  if (millis() - timerTenSecondsEventDate > 10000) {
    runtimeMark(RuntimeEvents);eventTimerTenSeconds();
    timerTenSecondsEventDate = millis();
  }
  
  if (millis() - timerMinuteEventDate > 60000) {
    runtimeMark(RuntimeEvents);eventTimerMinute();
    timerMinuteEventDate = millis();
  }

  if (millis() - timerHourEventDate > (3600000)) {
    runtimeMark(RuntimeEvents);eventTimerHour();
    timerHourEventDate = millis();
  }
  
  runtimeMark(RuntimeDiagnostics);usbDiagnosticLoop(micros()-usbLoopStarted);
  runtimeMark(RuntimeIdle);

}
