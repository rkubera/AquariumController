#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Timezone.h>       // https://github.com/JChristensen/Timezone
#include <Keypad.h>         // https://playground.arduino.cc/Code/Keypad#Download Must be patched. Replace: #define OPEN LOW with #define KBD_OPEN LOW, #define CLOSED HIGH with #define KBD_CLOSED HIGH in Key.h and Keypad.h. Replace OPEN with KBD_OPEN, CLOSE with KBD_CLOSE in Keypad.cpp 
#include <ELClient.h>       // https://github.com/jeelabs/el-client
#include <ELClientCmd.h>    // https://github.com/jeelabs/el-client
#include <ELClientMqtt.h>   // https://github.com/jeelabs/el-client
#include <RTClib.h>         // https://github.com/adafruit/RTClib
#include <QuickStats.h>     // https://github.com/dndubins/QuickStats
#include <dht.h>            // https://github.com/RobTillaart/Arduino/tree/master/libraries/DHTlib/

//********************************
//PINS
//********************************
#define BUZZER_PIN 6

#define DIGITAL_PWM_OUT_PIN_1  7
#define DIGITAL_PWM_OUT_PIN_2  8
#define DIGITAL_PWM_OUT_PIN_3  10

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
#define LED_BLUE_PIN 10

#define LCD_PIN_SCE   33
#define LCD_PIN_RESET 34
#define LCD_PIN_DC    35
#define LCD_PIN_SDIN  36
#define LCD_PIN_SCLK  37
#define LCD_PIN_LED   9 //Must be PWM Output

#define PUMP_PIN_1 5
#define PUMP_PIN_2 4

#define RELAY_PIN_1 44
#define RELAY_PIN_2 45

#define DHT_PIN 2

#define FAN_PIN 3

const byte KBD_ROWS = 5; //four rows
const byte KBD_COLS = 4; //three columns
char keys[KBD_ROWS][KBD_COLS] = {
{'A','B','#','*'},
{'1','2','3','U'},
{'4','5','6','D'},
{'7','8','9','R'},
{'L','0','R','E'}
};
byte KBD_ROW_PINS[KBD_ROWS] = {30, 29, 28, 27, 26}; //connect to the row pinouts of the keypad
byte KBD_COL_PINS[KBD_COLS] = {22, 23, 24, 25}; //connect to the column pinouts of the keypad

//********************************
//Main variables
//********************************
const char daysOfTheWeek[7][10] PROGMEM = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const char monthsOfYear[12][10] PROGMEM = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

const char charSetSensor[] PROGMEM = "set/Sensor";
const char charGetSensor[] PROGMEM = "get/Sensor";

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

const char charManual[] PROGMEM = "manual";
const char charPartofday[] PROGMEM = "partofday";
const char charTemperature[] PROGMEM = "temperature";
const char charPh[] PROGMEM = "ph";
//const char charAquawaterlevel[] PROGMEM = "aquawaterlevel";
//const char charTankwaterlevel[] PROGMEM = "tankwaterlevel";
const char charPhsensor1[] PROGMEM = "phsensor1";
const char charPhsensor2[] PROGMEM = "phsensor2";
const char charThermometer1[] PROGMEM = "thermometer1";
const char charThermometer2[] PROGMEM = "thermometer2";
const char charWaterlevelsensor1[] PROGMEM = "waterlevelsensor1";
const char charWaterlevelsensor2[] PROGMEM = "waterlevelsensor2";
const char charWaterlevelsensor3[] PROGMEM = "waterlevelsensor3";
const char charWaterlevelsensor4[] PROGMEM = "waterlevelsensor4";

const char charSensornone[] PROGMEM = "notconnected";
const char charPhsensor[] PROGMEM = "phsensor";
const char charThermometer[] PROGMEM = "thermometer";
const char charAquawaterlevel[] PROGMEM = "aquawaterlevel";
const char charTankwaterlevel[] PROGMEM = "tankwaterlevel";

const char charSensorValue[] PROGMEM = "Value";
const char charSensorRawValue[] PROGMEM = "RawValue";
const char charSensorType[] PROGMEM = "Type";
const char charSensorCalibValue1[] PROGMEM = "CalibValue1";
const char charSensorCalibRawValue1[] PROGMEM = "CalibRawValue1";
const char charSensorCalibValue2[] PROGMEM = "CalibValue2";
const char charSensorCalibRawValue2[] PROGMEM = "CalibRawValue2";
const char charSensorMinValue[] PROGMEM = "MinValue";
const char charSensorMaxValue[] PROGMEM = "MaxValue";
const char charSensorCriticalMinValue[] PROGMEM = "CriticalMinValue";
const char charSensorCriticalMaxValue[] PROGMEM = "CriticalMaxValue";

#define CONTROL_MODE_MANUAL             0
#define CONTROL_MODE_PART_OF_DAY        1
#define CONTROL_MODE_TEMPERATURE        2
#define CONTROL_MODE_PH                 3
/*
#define CONTROL_MODE_SENSOR1            4
#define CONTROL_MODE_SENSOR2            5
#define CONTROL_MODE_SENSOR3            6
#define CONTROL_MODE_SENSOR4            7
#define CONTROL_MODE_SENSOR5            8
#define CONTROL_MODE_SENSOR6            9
#define CONTROL_MODE_SENSOR7            10
#define CONTROL_MODE_SENSOR8            11
*/

#define CONTROL_MODE_AQUAWATERLEVEL     4
#define CONTROL_MODE_TANKWATERLEVEL     5
#define CONTROL_MODE_PHSENSOR1          6
#define CONTROL_MODE_PHSENSOR2          7
#define CONTROL_MODE_THERMOMETER1       8
#define CONTROL_MODE_THERMOMETER2       9
#define CONTROL_MODE_WATERLEVELSENSOR1  10
#define CONTROL_MODE_WATERLEVELSENSOR2  11
#define CONTROL_MODE_WATERLEVELSENSOR3  12
#define CONTROL_MODE_WATERLEVELSENSOR4  13

byte fanStartTemperature = 30;
byte fanMaxSpeedTemperature = 35;
byte maxInternalTemperature = 50;

double timerMillisEventDate;
double timerSecondEventDate;
double timerMinuteEventDate;
double timerHourEventDate;
const int bufferSize = 100;
char buffer[bufferSize];
uint32_t boot_time = 0;
QuickStats stats;

//********************************
//Sensors
//********************************
#define SENSORS_COUNT                     8

#define SENSOR_TYPE_NONE                  0
#define SENSOR_TYPE_PH                    1
#define SENSOR_TYPE_THERMOMETER           2
#define SENSOR_TYPE_AQUA_WATER_LEVEL      3
#define SENSOR_TYPE_TANK_WATER_LEVEL      4

#define SENSORS_VALUE                     1
#define SENSORS_VALUE_RAW                 2
#define SENSORS_VALUE_TYPE                3
#define SENSORS_VALUE_MIN                 4
#define SENSORS_VALUE_MAX                 5
#define SENSORS_VALUE_CRITICAL_MIN        6
#define SENSORS_VALUE_CRITICAL_MAX        7
#define SENSORS_VALUE_CALIB_VALUE1        8
#define SENSORS_VALUE_CALIB_RAW_VALUE1    9
#define SENSORS_VALUE_CALIB_VALUE2        10
#define SENSORS_VALUE_CALIB_RAW_VALUE2    11

//********************************
//Wifi
//********************************
#define WIFI_STATUS_CONNECTED     1
#define WIFI_STATUS_DISCONNECTED  0
char wifiStatus = 0;

//********************************
//Errors
//********************************
bool errorClock = false;
bool errorTemperature = false;
bool errorPH = false;

byte errorsCount = 0;

int errorsNumReadings = 20;
float errorsTemparature[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

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
const char timezoneWeekLast[] PROGMEM ="last";

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

RTC_DS1307 clockRtc;

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
const char schedulerMorning[] PROGMEM="morning";
const char schedulerAfternoon[] PROGMEM="afternoon";
const char schedulerEvening[] PROGMEM="evening";
const char schedulerNight[] PROGMEM="night";

#define SCHEDULER_MODE_NONE 0
#define SCHEDULER_MODE_MORNING 1
#define SCHEDULER_MODE_AFTERNOON 2
#define SCHEDULER_MODE_EVENING 3
#define SCHEDULER_MODE_NIGHT 4

byte schedulerStartMorningHour = 9;
byte schedulerStartMorningMinute = 0;

byte schedulerStartAfternoonHour = 10;
byte schedulerStartAfternoonMinute = 0;

byte schedulerStartEveningHour = 19;
byte schedulerStartEveningMinute = 0;

byte schedulerStartNightHour = 22;
byte schedulerStartNightMinute = 00;

//********************************
//Buzzer
//********************************
#define BUZZER_ON 1
#define BUZZER_OFF 0

byte buzzerOnStart = 1;
byte buzzerOnErrors = 1;

//********************************
//PH
//********************************
float phAqua = 7.0;

float phValue1 = 7.0;
float phRawRead1 = 520;

float phValue2 = 4.0;
float phRawRead2 = 647;

int phNumReadings = 20;
float phReadings[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float phRawReadings[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

float phMin = 5.2;
float phMax = 8.8;

int phLastPhRawReading;
float phLastPhAqua;

//********************************
//Temperature
//********************************
float tempAqua = 0;

float tempValue1 = 27;
float tempRawRead1 = 748;

float tempValue2 = 20;
float tempRawRead2 = 510;

int tempNumReadings = 20;
float tempReadings[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float tempRawReadings[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

float tempMin = 26;
float tempMax = 28;

//********************************
//LED
//********************************
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

#define LED_BRIGHTNESS_AUTO -1

byte ledControlMode = CONTROL_MODE_MANUAL;
byte ledManualMode = LED_MODE_NONE;

byte ledTimer = 100;
byte ledActualTimer = 0;
byte ledBrightness = 0;
int ledActualBrightness = 0;

int ledMorningBrightness = 0;
int ledAfternoonBrightness = 0;
int ledEveningBrightness = 0;
int ledNightBrightness = 0;
int ledManualBrightness = 0;

int ledAutoBrightness = 255;
byte ledMode = LED_MODE_NONE;
byte ledModeMorning = LED_MODE_NONE;
byte ledModeAfternoon = LED_MODE_NONE;
byte ledModeEvening = LED_MODE_NONE;
byte ledModeNight = LED_MODE_NONE;

int ledActualRed = 0;
int ledActualGreen = 0;
int ledActualBlue = 0;

byte ledRed = 0;
byte ledGreen = 0;
byte ledBlue = 0;

byte ledStep = 10;
byte ledStepSwitchColor = 10;
byte ledStepWave = 1;

int ledFadeInFromBlackSeconds = 120;

byte ledWaveIdx;
byte ledLastMode = LED_MODE_NONE;

#define LED_MANUAL_ONOFF_AUTO 0
#define LED_MANUAL_ONOFF_OFF 1
#define LED_MANUAL_ONOFF_ON 2

byte ledManualOnOff = LED_MANUAL_ONOFF_AUTO;

double ledRedLevel, ledGreenLevel, ledBlueLevel;

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

//********************************
//MQTTEL
//********************************
#define MQTT_STATUS_CONNECTED     1
#define MQTT_STATUS_DISCONNECTED  0

byte mqttStatus = MQTT_STATUS_DISCONNECTED;

ELClient mqttElesp(&Serial, &Serial);
ELClientCmd mqttElcmd(&mqttElesp);
ELClientMqtt mqttEl(&mqttElesp);

char mqttElDeviceName[] = "ac";

bool mqttElconnected;

const char setLedColorMorning[] PROGMEM = "set/LedColorMorning";                 //red, green, blue, white, cyan, magenta, yellow, white, wave, black
const char setLedColorAfternoon[] PROGMEM = "set/LedColorAfternoon";             //red, green, blue, white, cyan, magenta, yellow, white, wave, black
const char setLedColorEvening[] PROGMEM = "set/LedColorEvening";                 //red, green, blue, white, cyan, magenta, yellow, white, wave, black
const char setLedColorNight[] PROGMEM = "set/LedColorNight";                     //red, green, blue, white, cyan, magenta, yellow, white, wave, black
const char setBrightness[] PROGMEM = "set/Brightness";                           //0-255
const char setActualTime[] PROGMEM = "set/ActualTime";                           //HH:mm in 24 hours format
const char setActualDate[] PROGMEM="set/ActualDate";                             //yyyy/mm/dd
const char setMorningTime[] PROGMEM = "set/MorningTime";                         //HH:mm in 24 hours format
const char setAfternoonTime[] PROGMEM = "set/AfternoonTime";                     //HH:mm in 24 hours format
const char setEveningTime[] PROGMEM = "set/EveningTime";                         //HH:mm in 24 hours format
const char setNightTime[] PROGMEM = "set/NightTime";                             //HH:mm in 24 hours format
const char setRelay1State[] PROGMEM = "set/Relay1State";                         //ON,OFF
const char setRelay2State[] PROGMEM = "set/Relay2State";                         //ON,OFF
const char setRelay1ControlMode[] PROGMEM = "set/Relay1ControlMode";             //manual, partofday, temperature, ph, aquawaterlevel, tankwaterlevel, phsensor1, phsensor2, thermometer1, thermometer2, waterlevelsensor1, waterlevelsensor2, waterlevelsensor3, waterlevelsensor4
const char setRelay2ControlMode[] PROGMEM = "set/Relay2ControlMode";             //manual, partofday, temperature, ph, aquawaterlevel, tankwaterlevel, phsensor1, phsensor2, thermometer1, thermometer2, waterlevelsensor1, waterlevelsensor2, waterlevelsensor3, waterlevelsensor4
const char setRelay1MorningMode[] PROGMEM = "set/Relay1MorningMode";             //ON,OFF,AUTO
const char setRelay1AfternoonMode[] PROGMEM = "set/Relay1AfternoonMode";         //ON,OFF,AUTO
const char setRelay1EveningMode[] PROGMEM = "set/Relay1EveningMode";             //ON,OFF,AUTO
const char setRelay1NightMode[] PROGMEM = "set/Relay1NightMode";                 //ON,OFF,AUTO
const char setRelay2MorningMode[] PROGMEM = "set/Relay2MorningMode";             //ON,OFF,AUTO
const char setRelay2AfternoonMode[] PROGMEM = "set/Relay2AfternoonMode";         //ON,OFF,AUTO
const char setRelay2EveningMode[] PROGMEM = "set/Relay2EveningMode";             //ON,OFF,AUTO
const char setRelay2NightMode[] PROGMEM = "set/Relay2NightMode";                 //ON,OFF,AUTO
const char setPHMin[] PROGMEM="set/PHMin";                                       //Float value 0.0..14.0
const char setPHMax[] PROGMEM="set/PHMax";                                       //Float value 0.0..14.0
const char setPH1Calib[] PROGMEM="set/PH1Calib";                                 //Float value 0.0..14.0
const char setPH2Calib[] PROGMEM="set/PH2Calib";                                 //Float value 0.0..14.0
const char setPH1RawCalib[] PROGMEM="set/PH1RawCalib";                           //integer 0..1023
const char setPH2RawCalib[] PROGMEM="set/PH2RawCalib";                           //integer 0..1023
const char setFanStartTemperature[] PROGMEM="set/FanStartTemp";                  //integer value
const char setFanMaxSpeedTemperature[] PROGMEM="set/FanMaxSpeedTemp";            //integer value
const char setMaxInternalTemperature[] PROGMEM="set/MaxIntTemp";                 //integer value
const char setLedControlMode[] PROGMEM = "set/LedControlMode";                   //manual, partofday
const char setLedManualMode[] PROGMEM = "set/LedManualMode";                     //red, green, blue, white, cyan, magenta, yellow, white, wave, black
const char setBuzzerOnStart[] PROGMEM = "set/BuzzerOnStart";                     //on, off
const char setBuzzerOnErrors[] PROGMEM = "set/BuzzerOnErrors";                   //on, off
const char setTempMin[] PROGMEM="set/TempMin";                                   //Float value
const char setTempMax[] PROGMEM="set/TempMax";                                   //Float value
const char setTemp1Calib[] PROGMEM="set/Temp1Calib";                             //Float value
const char setTemp2Calib[] PROGMEM="set/Temp2Calib";                             //Float value
const char setTemp1RawCalib[] PROGMEM="set/Temp1RawCalib";                       //integer 0..1023
const char setTemp2RawCalib[] PROGMEM="set/Temp2RawCalib";                       //integer 0..1023
const char setTimezoneRule1Week[] PROGMEM="set/TimezoneRule1Week";               //first, second, third, fourth, last
const char setTimezoneRule2Week[] PROGMEM="set/TimezoneRule2Week";               //first, second, third, fourth, last
const char setTimezoneRule1DayOfWeek[] PROGMEM="set/TimezoneRule1DayOfWeek";     //sunday, monday, tuesday, wednesday, thursday, friday, saturday
const char setTimezoneRule2DayOfWeek[] PROGMEM="set/TimezoneRule2DayOfWeek";     //sunday, monday, tuesday, wednesday, thursday, friday, saturday
const char setTimezoneRule1Hour[] PROGMEM="set/TimezoneRule1Hour";               //0..23
const char setTimezoneRule2Hour[] PROGMEM="set/TimezoneRule2Hour";               //0..23
const char setTimezoneRule1Offset[] PROGMEM="set/TimezoneRule1Offset";           //+1, +2, -1, -2 etc
const char setTimezoneRule2Offset[] PROGMEM="set/TimezoneRule2Offset";           //+1, +2, -1, -2 etc
const char setTimezoneRule1Month[] PROGMEM="set/TimezoneRule1Month";             //january, february, march, april, may, june, july, august, september, october, november, december
const char setTimezoneRule2Month[] PROGMEM="set/TimezoneRule2Month";             //january, february, march, april, may, june, july, august, september, october, november, december
const char setLedState[] PROGMEM="set/LedState";                                 //on, off

const char getBrightness[] PROGMEM = "get/Brightness";
const char getActualTime[] PROGMEM = "get/ActualTime";
const char getActualDate[] PROGMEM="get/ActualDate";
const char getActualDayOfWeek[] PROGMEM="get/ActualDayOfWeek";
const char getActualTimezoneOffset[] PROGMEM = "get/ActualTimezoneOffset";
const char getInternalTemperature[] PROGMEM = "get/InternalTemperature"; 
const char getInternalHumidity[] PROGMEM = "get/InternalHumidity";   
const char getMorningTime[] PROGMEM = "get/MorningTime";
const char getAfternoonTime[] PROGMEM = "get/AfternoonTime";
const char getEveningTime[] PROGMEM = "get/EveningTime";
const char getNightTime[] PROGMEM = "get/NightTime";
const char getRelay1State[] PROGMEM = "get/Relay1State";
const char getRelay2State[] PROGMEM = "get/Relay2State";
const char getRelay1ControlMode[] PROGMEM = "get/Relay1ControlMode";
const char getRelay2ControlMode[] PROGMEM = "get/Relay2ControlMode";
const char getRelay1MorningMode[] PROGMEM = "get/Relay1MorningMode";
const char getRelay1AfternoonMode[] PROGMEM = "get/Relay1AfternoonMode";
const char getRelay1EveningMode[] PROGMEM = "get/Relay1EveningMode";
const char getRelay1NightMode[] PROGMEM = "get/Relay1NightMode";
const char getRelay2MorningMode[] PROGMEM = "get/Relay2MorningMode";
const char getRelay2AfternoonMode[] PROGMEM = "get/Relay2AfternoonMode";
const char getRelay2EveningMode[] PROGMEM = "get/Relay2EveningMode";
const char getRelay2NightMode[] PROGMEM = "get/Relay2NightMode";
const char getLedColorMorning[] PROGMEM = "get/LedColorMorning";
const char getLedColorAfternoon[] PROGMEM = "get/LedColorAfternoon";
const char getLedColorEvening[] PROGMEM = "get/LedColorEvening";
const char getLedColorNight[] PROGMEM = "get/LedColorNight";
const char getLedControlMode[] PROGMEM = "get/LedControlMode";
const char getLedManualMode[] PROGMEM = "get/LedManualMode";
const char getActualPartOfDay[] PROGMEM = "get/ActualPartOfDay";
const char getPH[] PROGMEM="get/PH";
const char getRawPH[] PROGMEM="get/RawPH";
const char getPH1Calib[] PROGMEM="get/PH1Calib";
const char getPH2Calib[] PROGMEM="get/PH2Calib";
const char getPH1RawCalib[] PROGMEM="get/PH1RawCalib";
const char getPH2RawCalib[] PROGMEM="get/PH2RawCalib";
const char getPHMin[] PROGMEM="get/PHMin";
const char getPHMax[] PROGMEM="get/PHMax";
const char getStatus[] PROGMEM="get/Status";
const char getFanStartTemperature[] PROGMEM="get/FanStartTemp";
const char getFanMaxSpeedTemperature[] PROGMEM="get/FanMaxSpeedTemp";
const char getFanPWMValue[] PROGMEM="get/FanPWMValue";
const char getMaxInternalTemperature[] PROGMEM="get/MaxIntTemp";
const char getBuzzerOnStart[] PROGMEM = "get/BuzzerOnStart";
const char getBuzzerOnErrors[] PROGMEM = "get/BuzzerOnErrors";
const char getTempMin[] PROGMEM="get/TempMin";
const char getTempMax[] PROGMEM="get/TempMax";
const char getTemp1Calib[] PROGMEM="get/Temp1Calib";
const char getTemp2Calib[] PROGMEM="get/Temp2Calib";
const char getTemp1RawCalib[] PROGMEM="get/Temp1RawCalib";
const char getTemp2RawCalib[] PROGMEM="get/Temp2RawCalib";
const char getTemperature[] PROGMEM="get/Temperature";
const char getRawTemperature[] PROGMEM="get/RawTemperature";
const char getTimezoneRule1Week[] PROGMEM="get/TimezoneRule1Week";
const char getTimezoneRule2Week[] PROGMEM="get/TimezoneRule2Week";
const char getTimezoneRule1DayOfWeek[] PROGMEM="get/TimezoneRule1DayOfWeek";
const char getTimezoneRule2DayOfWeek[] PROGMEM="get/TimezoneRule2DayOfWeek";
const char getTimezoneRule1Hour[] PROGMEM="get/TimezoneRule1Hour";
const char getTimezoneRule2Hour[] PROGMEM="get/TimezoneRule2Hour";
const char getTimezoneRule1Offset[] PROGMEM="get/TimezoneRule1Offset";
const char getTimezoneRule2Offset[] PROGMEM="get/TimezoneRule2Offset";
const char getTimezoneRule1Month[] PROGMEM="get/TimezoneRule1Month";
const char getTimezoneRule2Month[] PROGMEM="get/TimezoneRule2Month";
const char getLedState[] PROGMEM="get/LedState"; 

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
#define EEPROM_ledAfternoonBrightness_addr                  15
#define EEPROM_ledEveningBrightness_addr                    16
#define EEPROM_ledNightBrightness_addr                      17
#define EEPROM_ledManualBrightness_addr                     18

#define EEPROM_relay1ModeMorning_addr                       22
#define EEPROM_relay1ModeAfternoon_addr                     23
#define EEPROM_relay1ModeEvening_addr                       24
#define EEPROM_relay1ModeNight_addr                         25

#define EEPROM_relay2ModeMorning_addr                       26
#define EEPROM_relay2ModeAfternoon_addr                     27
#define EEPROM_relay2ModeEvening_addr                       28
#define EEPROM_relay2ModeNight_addr                         29

#define EEPROM_ledControlMode_addr                          57
#define EEPROM_ledManualMode_addr                           58

#define EEPROM_buzzerOnStart_addr                           59
#define EEPROM_buzzerOnErrors_addr                          60

//Global values
#define EEPROM_maxInternalTemperature_addr                  31

#define EEPROM_phMin_addr                                   32    //float - 4 bytes
#define EEPROM_phMax_addr                                   36    //float - 4 bytes

#define EEPROM_fanStartTemperature_addr                     30
#define EEPROM_fanMaxSpeedTemperature_addr                  40

#define EEPROM_phValue1_addr                                41    //float - 4 bytes
#define EEPROM_phRawRead1_addr                              45    //float - 4 bytes
#define EEPROM_phValue2_addr                                49    //float - 4 bytes
#define EEPROM_phRawRead2_addr                              53    //float - 4 bytes

#define EEPROM_tempMin_addr                                 61    //float - 4 bytes
#define EEPROM_tempMax_addr                                 65    //float - 4 bytes
#define EEPROM_tempValue1_addr                              69    //float - 4 bytes
#define EEPROM_tempRawRead1_addr                            73    //float - 4 bytes
#define EEPROM_tempValue2_addr                              77    //float - 4 bytes
#define EEPROM_tempRawRead2_addr                            81    //float - 4 bytes

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

#define EEPROM_relay1ControlMode_addr                       95
#define EEPROM_relay2ControlMode_addr                       96

#define EEPROM_sensors_addr                                 97      //8*33 = 264

//********************************
//Keyboard
//********************************
char lastKey;
Keypad keypad = Keypad(makeKeymap(keys), KBD_ROW_PINS, KBD_COL_PINS, KBD_ROWS, KBD_COLS);

//********************************
//Pumps
//********************************

//********************************
//Watchdog
//********************************
time_t watchdogStartTime;
time_t watchdogupTime;

//********************************
//Relays
//********************************
#define RELAY_MODE_OFF 0
#define RELAY_MODE_ON 1
#define RELAY_MODE_NONE 3

bool relay1State = false;
bool relay2State = false;

byte relay1ControlMode = CONTROL_MODE_MANUAL;
byte relay2ControlMode = CONTROL_MODE_MANUAL;

byte relay1ModeMorning = RELAY_MODE_NONE;
byte relay1ModeAfternoon = RELAY_MODE_NONE;
byte relay1ModeEvening = RELAY_MODE_NONE;
byte relay1ModeNight = RELAY_MODE_NONE;

byte relay2ModeMorning = RELAY_MODE_NONE;
byte relay2ModeAfternoon = RELAY_MODE_NONE;
byte relay2ModeEvening = RELAY_MODE_NONE;
byte relay2ModeNight = RELAY_MODE_NONE;

#define RELAY_MANUAL_ONOFF_AUTO 0
#define RELAY_MANUAL_ONOFF_OFF 1
#define RELAY_MANUAL_ONOFF_ON 2

byte relay1ManualOnOff = RELAY_MANUAL_ONOFF_AUTO;
byte relay2ManualOnOff = RELAY_MANUAL_ONOFF_AUTO;

byte relay1LastMode, relay2LastMode;
byte relayLastPartOfDay1, relayLastPartOfDay2;
byte relay1Mode, relay2Mode;

bool relayLastRelay2State;
bool relayLastRelay1State;
//********************************
//DHT
//********************************
dht DHT;
float dhtHumidity = 0;
float dhtTemperature = 0;

int dhtNumReadings = 20;
float dhtTemparatureReadings[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float dhtHumidityReadings[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//********************************
//Fan
//********************************
byte fanPWM = 255;

//********************************
//Main program
//********************************

void setup() {

  //LCD
  lcdInit();
  gotoXY(0,1);
  lcdString("Initializing");
  
  //Init
  Serial.begin(115200);
  Wire.begin();

  //Config
  configLoad();
  
  //Fan
  initFan();
  
  //Keyboard
  keyboardInit();

  //Menu
  menuInit();

  //MQTTEL
  mqttElInit();
  
  //Scheduler
  schedulerInit();

  //LED
  ledInit();
    
  //Clock
  clockInit();

  //DHT
  dhtInit();

  //Relays
  relaysInit();

  //Pumps
  pumpsInit();
  
  lcdClear();  
  //clockSetLocalTime();

  //PH
  phInit();

  //Thermometer
  thermometerInit();

  //Sensors
  sensorsInit();

  //Errors
  errorsInit();
  
  //Events
  timerMillisEventDate = millis();
  timerSecondEventDate = millis();
  timerMinuteEventDate = millis();
  timerHourEventDate = millis();

  //Watchdog
  watchdogInit();

  //Buzzer
  buzzerInit();
}

void loop() {
  if (abs(millis()-timerMillisEventDate)>1) {
    eventTimerMillis();
    timerMillisEventDate = millis();
  }

   if (abs(millis()-timerSecondEventDate)>1000) {
    eventTimerSecond();
    timerSecondEventDate = millis();
  }

   if (abs(millis()-timerMinuteEventDate)>60000) {
    eventTimerMinute();
    timerMinuteEventDate = millis();
  }

   if (abs(millis()-timerHourEventDate)>(3600000)) {
    eventTimerHour();
    timerHourEventDate = millis();
  }
    //Keyvboard
  keyboardCheck();

  //Menu
  menuShow();
  
  //DHT
  dhtGetData(); 

  //MQTTEL
  mqttElCheck();
  
}
