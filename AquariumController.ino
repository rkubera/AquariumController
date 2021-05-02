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
#define SERIAL_TX_BUFFER_SIZE 10
#define SERIAL_RX_BUFFER_SIZE 10

#define _DEGUB_NONE     0
#define _DEBUG_ERROR    1
#define _DEBUG_WARNING  2
#define _DEBUG_NOTICE   4
#define _DEBUG_MQTT     8

//#define DEBUG_LEVEL _DEBUG_ERROR + _DEBUG_WARNING + _DEBUG_MQTT + _DEBUG_NOTICE

#define DEBUG_LEVEL 0

#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Timezone.h>       // https://github.com/JChristensen/Timezone
#include <Keypad.h>         // https://playground.arduino.cc/Code/Keypad#Download Must be patched. Replace: #define OPEN LOW with #define KBD_OPEN LOW, #define CLOSED HIGH with #define KBD_CLOSED HIGH in Key.h and Keypad.h. Replace OPEN with KBD_OPEN, CLOSE with KBD_CLOSE in Keypad.cpp 
#include <QuickStats.h>     // https://github.com/dndubins/QuickStats
#include <dht.h>            // https://github.com/RobTillaart/Arduino/tree/master/libraries/DHTlib/


//********************************
//PINS
//********************************
#define BUZZER_PIN 6

#define DIGITAL_PWM_12V_OUT_PIN_1 4
#define DIGITAL_PWM_12V_OUT_PIN_2 5

#define DIGITAL_PWM_OUT_PIN_1  7
#define DIGITAL_PWM_OUT_PIN_2  8
#define DIGITAL_PWM_OUT_PIN_3  9
#define DIGITAL_PWM_OUT_PIN_4  10

#define ANALOG_IN_PIN_0 4
#define ANALOG_IN_PIN_1 5
#define ANALOG_IN_PIN_2 6
#define ANALOG_IN_PIN_3 7
#define ANALOG_IN_PIN_4 8
#define ANALOG_IN_PIN_5 9
#define ANALOG_IN_PIN_6 10
#define ANALOG_IN_PIN_7 11

#define LED_RED_PIN 11
#define LED_GREEN_PIN 12
#define LED_BLUE_PIN 13

#define LCD_PIN_SCE   49  // ZMIANA Z 33
#define LCD_PIN_RESET 51  // ZMIANA Z 34
#define LCD_PIN_DC    53  // ZMIANA Z 35
#define LCD_PIN_SDIN  52  // ZMIANA Z 36
#define LCD_PIN_SCLK  50  // ZMIANA Z 37
#define LCD_PIN_LED   2 //Must be PWM Output

#define RELAY_PIN_1 44
#define RELAY_PIN_2 45
#define RELAY_PIN_3 46
#define RELAY_PIN_4 47

#define DHT_PIN 32

#define FAN_PIN 3

//********************************
//Keyboard
//********************************
const byte KBD_ROWS = 5; //four rows
const byte KBD_COLS = 4; //three columns
char keys[KBD_ROWS][KBD_COLS] = {
  {'A', 'B', '#', '*'},
  {'1', '2', '3', 'U'},
  {'4', '5', '6', 'D'},
  {'7', '8', '9', 'R'},
  {'L', '0', 'R', 'E'}
};
//byte KBD_ROW_PINS[KBD_ROWS] = {30, 29, 28, 27, 26}; //connect to the row pinouts of the keypad
//byte KBD_COL_PINS[KBD_COLS] = {22, 23, 24, 25}; //connect to the column pinouts of the keypad

//byte KBD_ROW_PINS[KBD_ROWS] = {23, 25, 27, 29, 31}; //connect to the row pinouts of the keypad
//byte KBD_COL_PINS[KBD_COLS] = {39, 37, 35, 33}; //connect to the column pinouts of the keypad

byte KBD_ROW_PINS[KBD_ROWS] = {39, 37, 35, 33, 31}; //connect to the row pinouts of the keypad
byte KBD_COL_PINS[KBD_COLS] = {23, 25, 27, 29}; //connect to the column pinouts of the keypad

//********************************
//Main variables
//********************************
const char daysOfTheWeek[7][10] PROGMEM = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const char monthsOfYear[12][10] PROGMEM = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

const char getWifiStatus[] PROGMEM = "get wifistatus";
const char getMqttStatus[] PROGMEM = "get mqttstatus";
const char getHostname[] PROGMEM = "get hostname";
const char getSsid[] PROGMEM = "get ssid";
const char getMqttserver[] PROGMEM = "get mqttserver";

const char mqttSubscribeCommand[] PROGMEM = "subscribe ";
const char mqttPublishretainedCommand[] PROGMEM = "publishretained ";
const char mqttPublishCommand[] PROGMEM = "publish ";

const char charSetSensor[] PROGMEM = "set/Sensor";
const char charGetSensor[] PROGMEM = "get/Sensor";

const char charSetRelay[] PROGMEM = "set/Relay";
const char charGetRelay[] PROGMEM = "get/Relay";

const char charSetPwmOutput[] PROGMEM = "set/PWMOutput";
const char charGetPwmOutput[] PROGMEM = "get/PWMOutput";

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

const char charCustom1[] PROGMEM = "custom1";
const char charCustom2[] PROGMEM = "custom2";

const char charManual[] PROGMEM = "manual";
const char charPartofday[] PROGMEM = "partofday";
const char charTreshold[] PROGMEM = "treshold";
const char charPid[] PROGMEM = "pid";

const char charSensornone[] PROGMEM = "notconnected";
const char charRelay[] PROGMEM = "relay";
const char charPWMOutput[] PROGMEM = "pwmoutput";

const char charValue[] PROGMEM = "Value";
const char charRawValue[] PROGMEM = "RawValue";
const char charSensorType[] PROGMEM = "Type";
const char charSensorCalibValue1[] PROGMEM = "CalibValue1";
const char charSensorCalibRawValue1[] PROGMEM = "CalibRawValue1";
const char charSensorCalibValue2[] PROGMEM = "CalibValue2";
const char charSensorCalibRawValue2[] PROGMEM = "CalibRawValue2";
const char charName[] PROGMEM = "Name";

const char charControlMode[] PROGMEM = "ControlMode";
const char charMorningMode[] PROGMEM = "MorningMode";
const char charAfternoonMode[] PROGMEM = "AfternoonMode";
const char charEveningMode[] PROGMEM = "EveningMode";
const char charNightMode[] PROGMEM = "NightMode";
const char charManualMode[] PROGMEM = "ManualMode";

const char charPidKp[] PROGMEM = "PidKp";
const char charPidKi[] PROGMEM = "PidKi";
const char charPidKd[] PROGMEM = "PidKd";
const char charSensorsSetpoint[] PROGMEM = "SensorsSetpoint";

const char charControlDirection[] PROGMEM = "ControlDirection";
const char charDirect[] PROGMEM = "direct";
const char charReverse[] PROGMEM = "reverse";

const char charSensorsValue[] PROGMEM = "SensorsValue";

const char charState[] PROGMEM = "State";

const char charMaxDeviation[] PROGMEM = "MaxDeviation";


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

#define MQTT_MIN_REFRESH_MILLIS         5000
QuickStats stats;

//********************************
//Fan
//********************************
byte fanStartTemperature = 30;
byte fanMaxSpeedTemperature = 35;
byte maxInternalTemperature = 50;

//********************************
//PWMOutputs
//********************************
#define PWMOUTPUTS_PWMOUTPUT_EEPROM_BYTES  40
#define PWMOUTPUTS_COUNT                   6

#define PWMOUTPUT_MODE_OFF                 0
#define PWMOUTPUT_MODE_ON                  1
#define PWMOUTPUT_MODE_NONE                3

#define PWMOUTPUT_MANUAL_ONOFF_AUTO        0
#define PWMOUTPUT_MANUAL_ONOFF_OFF         1
#define PWMOUTPUT_MANUAL_ONOFF_ON          2

#define PWMOUTPUT_STATE                    1
#define PWMOUTPUT_CONTROL_MODE             2
#define PWMOUTPUT_MODE_MORNING             3
#define PWMOUTPUT_MODE_AFTERNOON           4
#define PWMOUTPUT_MODE_EVENING             5
#define PWMOUTPUT_MODE_NIGHT               6
#define PWMOUTPUT_MANUAL_ONOFF             7
#define PWMOUTPUT_MANUAL_MODE              8

#define PWMOUTPUT_CONTROL_DIRECTION        9
#define PWMOUTPUT_SENSORS_SETPOINT         10
#define PWMOUTPUT_PID_KP                   11
#define PWMOUTPUT_PID_KI                   12
#define PWMOUTPUT_PID_KD                   13
#define PWMOUTPUT_MAX_DEVIATION            14
#define PWMOUTPUT_OUTPUT_VALUE             15

#define PID_AUTOMATIC                      1
#define PID_MANUAL                         0
#define CONTROL_DIRECT                     0
#define PID_REVERSE                        1
#define PID_P_ON_M                         0
#define PID_P_ON_E                         1

//********************************
//Relays
//********************************
#define RELAYS_RELAY_EEPROM_BYTES         30
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
#define SENSORS_SENSOR_EEPROM_BYTES       40

#define SENSORS_COUNT                     8

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

uint32_t clockLastSynchro = 7200000;

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
double lastCriticalEvent =0;
#define CRITICAL_EVENT_MIN_MILLIS 30
double timerMillisEventDate;
double timerTenSecondsEventDate;
double timerSecondEventDate;
double timerMinuteEventDate;
double timerHourEventDate;

//********************************
//Buzzer
//********************************
#define BUZZER_ON 1
#define BUZZER_OFF 0

byte buzzerOnStart = 1;
byte buzzerOnErrors = 1;

//********************************
//LED
//********************************
#define _LED_MIN_VALUE 0
#define _LED_MAX_VALUE 255
#define _LED_RANGE_VALUES _LED_MAX_VALUE - _LED_MIN_VALUE

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
#define LED_MODE_CUSTOM1 10
#define LED_MODE_CUSTOM2 11

#define LED_FADEMODE_DEFAULT 0
#define LED_FADEMODE_FADEIN 1
#define LED_FADEMODE_FADEOUT 2


#define LED_BRIGHTNESS_AUTO -1

byte ledControlMode = CONTROL_MODE_MANUAL;
byte ledManualMode = LED_MODE_NONE;

unsigned long ledCustomColor1 = 0xFFFFFF;
unsigned long ledCustomColor2 = 0xFFFFFF;

int ledBrightness = 0;
int ledActualBrightness = 0;

int ledMorningBrightness = 0;
int ledAfternoonBrightness = 0;
int ledEveningBrightness = 0;
int ledNightBrightness = 0;
int ledManualBrightness = 0;

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

byte ledTransitionTime = ledStepSwitchColorSeconds;

byte ledWaveIdx;
byte ledLastMode = LED_MODE_NONE;

#define LED_MANUAL_ONOFF_AUTO 0
#define LED_MANUAL_ONOFF_OFF 1
#define LED_MANUAL_ONOFF_ON 2

int ledActualState = LED_MANUAL_ONOFF_OFF;

byte ledManualOnOff = LED_MANUAL_ONOFF_AUTO;

double ledRedLevel, ledGreenLevel, ledBlueLevel;

unsigned long lastEventMillis;

//********************************
//LCD
//********************************
//Configuration for the LCD
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
//MQTT
//********************************

String wifi_ssid = "";
String wifi_pass = "";
String mqtt_server = "";
int mqtt_port = 1883;
String mqtt_user = "ac";
String mqtt_pass = "";

String mqttElDeviceName = "";
bool hostnameReceived = false;
bool mqttserverReceived = false;
bool subscriptionSent = false;

const char published[] PROGMEM = "published";

#define MQTT_STATUS_CONNECTED     1
#define MQTT_STATUS_DISCONNECTED  0

byte mqttStatus = MQTT_STATUS_DISCONNECTED;

const char setLedColorMorning[] PROGMEM = "set/LedColorMorning";                 //red, green, blue, white, cyan, magenta, yellow, white, wave, black, custom1, custom2
const char setLedColorAfternoon[] PROGMEM = "set/LedColorAfternoon";             //red, green, blue, white, cyan, magenta, yellow, white, wave, black, custom1, custom2
const char setLedColorEvening[] PROGMEM = "set/LedColorEvening";                 //red, green, blue, white, cyan, magenta, yellow, white, wave, black, custom1, custom2
const char setLedColorNight[] PROGMEM = "set/LedColorNight";                     //red, green, blue, white, cyan, magenta, yellow, white, wave, black, custom1, custom2
const char setBrightness[] PROGMEM = "set/Brightness";                           //0-2048
const char setActualTime[] PROGMEM = "set/ActualTime";                           //HH:mm in 24 hours format
const char setActualDate[] PROGMEM = "set/ActualDate";                           //yyyy/mm/dd
const char setMorningTime[] PROGMEM = "set/MorningTime";                         //HH:mm in 24 hours format
const char setAfternoonTime[] PROGMEM = "set/AfternoonTime";                     //HH:mm in 24 hours format
const char setEveningTime[] PROGMEM = "set/EveningTime";                         //HH:mm in 24 hours format
const char setNightTime[] PROGMEM = "set/NightTime";                             //HH:mm in 24 hours format
const char setFanStartTemperature[] PROGMEM = "set/FanStartTemp";                //integer value
const char setFanMaxSpeedTemperature[] PROGMEM = "set/FanMaxSpeedTemp";          //integer value
const char setMaxInternalTemperature[] PROGMEM = "set/MaxIntTemp";               //integer value
const char setLedControlMode[] PROGMEM = "set/LedControlMode";                   //manual, partofday
const char setLedManualMode[] PROGMEM = "set/LedManualMode";                     //red, green, blue, white, cyan, magenta, yellow, white, wave, black, custom1, custom2
const char setBuzzerOnStart[] PROGMEM = "set/BuzzerOnStart";                     //on, off
const char setBuzzerOnErrors[] PROGMEM = "set/BuzzerOnErrors";                   //on, off
const char setTimezoneRule1Week[] PROGMEM = "set/TimezoneRule1Week";             //first, second, third, fourth, last
const char setTimezoneRule2Week[] PROGMEM = "set/TimezoneRule2Week";             //first, second, third, fourth, last
const char setTimezoneRule1DayOfWeek[] PROGMEM = "set/TimezoneRule1DayOfWeek";   //sunday, monday, tuesday, wednesday, thursday, friday, saturday
const char setTimezoneRule2DayOfWeek[] PROGMEM = "set/TimezoneRule2DayOfWeek";   //sunday, monday, tuesday, wednesday, thursday, friday, saturday
const char setTimezoneRule1Hour[] PROGMEM = "set/TimezoneRule1Hour";             //0..23
const char setTimezoneRule2Hour[] PROGMEM = "set/TimezoneRule2Hour";             //0..23
const char setTimezoneRule1Offset[] PROGMEM = "set/TimezoneRule1Offset";         //+1, +2, -1, -2 etc
const char setTimezoneRule2Offset[] PROGMEM = "set/TimezoneRule2Offset";         //+1, +2, -1, -2 etc
const char setTimezoneRule1Month[] PROGMEM = "set/TimezoneRule1Month";           //january, february, march, april, may, june, july, august, september, october, november, december
const char setTimezoneRule2Month[] PROGMEM = "set/TimezoneRule2Month";           //january, february, march, april, may, june, july, august, september, october, november, december
const char setLedState[] PROGMEM = "set/LedState";                               //on, off
const char setCustomColor1[] PROGMEM = "set/CustomColor1";
const char setCustomColor2[] PROGMEM = "set/CustomColor2";

const char getBrightness[] PROGMEM = "get/Brightness";
const char getActualTime[] PROGMEM = "get/ActualTime";
const char getActualDate[] PROGMEM = "get/ActualDate";
const char getActualDayOfWeek[] PROGMEM = "get/ActualDayOfWeek";
const char getActualTimezoneOffset[] PROGMEM = "get/ActualTimezoneOffset";
const char getInternalTemperature[] PROGMEM = "get/InternalTemperature";
const char getInternalHumidity[] PROGMEM = "get/InternalHumidity";
const char getMorningTime[] PROGMEM = "get/MorningTime";
const char getAfternoonTime[] PROGMEM = "get/AfternoonTime";
const char getEveningTime[] PROGMEM = "get/EveningTime";
const char getNightTime[] PROGMEM = "get/NightTime";
const char getLedColorMorning[] PROGMEM = "get/LedColorMorning";
const char getLedColorAfternoon[] PROGMEM = "get/LedColorAfternoon";
const char getLedColorEvening[] PROGMEM = "get/LedColorEvening";
const char getLedColorNight[] PROGMEM = "get/LedColorNight";
const char getLedControlMode[] PROGMEM = "get/LedControlMode";
const char getLedManualMode[] PROGMEM = "get/LedManualMode";
const char getActualPartOfDay[] PROGMEM = "get/ActualPartOfDay";
const char getStatus[] PROGMEM = "get/Status";
const char getFanStartTemperature[] PROGMEM = "get/FanStartTemp";
const char getFanMaxSpeedTemperature[] PROGMEM = "get/FanMaxSpeedTemp";
const char getFanPWMValue[] PROGMEM = "get/FanPWMValue";
const char getMaxInternalTemperature[] PROGMEM = "get/MaxIntTemp";
const char getBuzzerOnStart[] PROGMEM = "get/BuzzerOnStart";
const char getBuzzerOnErrors[] PROGMEM = "get/BuzzerOnErrors";
const char getTimezoneRule1Week[] PROGMEM = "get/TimezoneRule1Week";
const char getTimezoneRule2Week[] PROGMEM = "get/TimezoneRule2Week";
const char getTimezoneRule1DayOfWeek[] PROGMEM = "get/TimezoneRule1DayOfWeek";
const char getTimezoneRule2DayOfWeek[] PROGMEM = "get/TimezoneRule2DayOfWeek";
const char getTimezoneRule1Hour[] PROGMEM = "get/TimezoneRule1Hour";
const char getTimezoneRule2Hour[] PROGMEM = "get/TimezoneRule2Hour";
const char getTimezoneRule1Offset[] PROGMEM = "get/TimezoneRule1Offset";
const char getTimezoneRule2Offset[] PROGMEM = "get/TimezoneRule2Offset";
const char getTimezoneRule1Month[] PROGMEM = "get/TimezoneRule1Month";
const char getTimezoneRule2Month[] PROGMEM = "get/TimezoneRule2Month";
const char getLedState[] PROGMEM = "get/LedState";
const char getCustomColor1[] PROGMEM = "get/CustomColor1";
const char getCustomColor2[] PROGMEM = "get/CustomColor2";


//********************************
//EEPROM
//********************************

//Daily values
#define EEPROM_ledModeMorning_addr                          2
#define EEPROM_ledModeAfternoon_addr                        3
#define EEPROM_ledModeEvening_addr                          4
#define EEPROM_ledModeNight_addr                            5

#define EEPROM_schedulerStartMorningHour_addr               6
#define EEPROM_schedulerStartMorningMinute_addr             7
#define EEPROM_schedulerStartAfternoonHour_addr             8
#define EEPROM_schedulerStartAfternoonMinute_addr           9
#define EEPROM_schedulerStartEveningHour_addr               10
#define EEPROM_schedulerStartEveningMinute_addr             11
#define EEPROM_schedulerStartNightHour_addr                 12
#define EEPROM_schedulerStartNightMinute_addr               13

#define EEPROM_ledMorningBrightness_addr                    14
#define EEPROM_ledAfternoonBrightness_addr                  18
#define EEPROM_ledEveningBrightness_addr                    22
#define EEPROM_ledNightBrightness_addr                      26
#define EEPROM_ledManualBrightness_addr                     30

#define EEPROM_ledControlMode_addr                          56
#define EEPROM_ledManualMode_addr                           58

#define EEPROM_buzzerOnStart_addr                           59
#define EEPROM_buzzerOnErrors_addr                          60

//Global values
#define EEPROM_maxInternalTemperature_addr                  31

#define EEPROM_fanStartTemperature_addr                     30
#define EEPROM_fanMaxSpeedTemperature_addr                  40

#define EEPROM_timezoneRule1Week_addr                       85
#define EEPROM_timezoneRule2Week_addr                       86
#define EEPROM_timezoneRule1DayOfWeek_addr                  87
#define EEPROM_timezoneRule2DayOfWeek_addr                  88
#define EEPROM_timezoneRule1Hour_addr                       89
#define EEPROM_timezoneRule2Hour_addr                       90
#define EEPROM_timezoneRule1Offset_addr                     91
#define EEPROM_timezoneRule2Offset_addr                     92
#define EEPROM_timezoneRule1Month_addr                      93
#define EEPROM_timezoneRule2Month_addr                      94

#define EEPROM_unix_timestamp_addr                          95 //(4 bytes)

#define EEPROM_ledCustomColor1_addr                         100 //(4 bytes)
#define EEPROM_ledCustomColor2_addr                         104 //(4 bytes)

#define EEPROM_sensors_addr                                 2048      //8*50 = 400
#define EEPROM_relays_addr                                  EEPROM_sensors_addr+(SENSORS_SENSOR_EEPROM_BYTES*SENSORS_COUNT) //4*20 = 80
#define EEPROM_pwm_outputs_addr                             EEPROM_relays_addr+(RELAYS_RELAY_EEPROM_BYTES*RELAYS_COUNT)

//********************************
//Keyboard
//********************************
char lastKey;
double lastKeyMillis = 0;
Keypad keypad = Keypad(makeKeymap(keys), KBD_ROW_PINS, KBD_COL_PINS, KBD_ROWS, KBD_COLS);

//********************************
//Watchdog
//********************************
time_t watchdogStartTime;
time_t watchdogupTime;

//********************************
//DHT
//********************************
dht DHT;
float dhtHumidity = 0;
float dhtTemperature = 0;

int dhtNumReadings = 20;
float dhtTemparatureReadings[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float dhtHumidityReadings[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

//********************************
//Fan
//********************************
byte fanPWM = 255;

//********************************
//Main program
//********************************

const int bufferOutSize = 100;
char bufferOut[bufferOutSize];
int publishValue = -1;

void setup() {

    //Init
  Serial.begin(115200);
  Wire.begin();

  //Watchdog
  watchdogInit();
  
  //LCD
  lcdInit();
  gotoXY(0, 1);
  lcdString((char *)"Initializing");

  //Config
  configLoad();

  //Clock
  clockInit();

  //LED
  ledInit();

  //Fan
  initFan();

  //Keyboard
  keyboardInit();

  //MQTT
  mqttInit();

  //Scheduler
  schedulerInit();
  
  //DHT
  dhtInit();
  
  //Relays
  relaysInit();

  //PWMOutputs
  pwmOutputsInit();

  //clockSetLocalTime();

  //Sensors
  sensorsInit();

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
  buzzerInit();

  String command = setBufferFromFlash(getHostname);
  mqttSendCommand(command);

}

void loop() {
  if (abs(millis() - timerMillisEventDate) > 1) {
    eventTimerMillis();
    timerMillisEventDate = millis();
  }

  if (abs(millis() - timerSecondEventDate) > 1000) {
    eventTimerSecond();
    timerSecondEventDate = millis();
  }

  if (abs(millis() - timerTenSecondsEventDate) > 10000) {
    eventTimerTenSeconds();
    timerTenSecondsEventDate = millis();
  }
  
  if (abs(millis() - timerMinuteEventDate) > 60000) {
    eventTimerMinute();
    timerMinuteEventDate = millis();
  }

  if (abs(millis() - timerHourEventDate) > (3600000)) {
    eventTimerHour();
    timerHourEventDate = millis();
  }
  
}
