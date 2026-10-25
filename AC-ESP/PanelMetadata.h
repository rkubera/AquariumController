#pragma once
#include "PanelWireIds.h"
struct PanelField { byte group,channel,property; };
static const char PANEL_PROPERTY_0[] PROGMEM = "Name";
static const char PANEL_PROPERTY_1[] PROGMEM = "Function";
static const char PANEL_PROPERTY_2[] PROGMEM = "TemperatureModel";
static const char PANEL_PROPERTY_3[] PROGMEM = "HumidityModel";
static const char PANEL_PROPERTY_4[] PROGMEM = "LevelModel";
static const char PANEL_PROPERTY_5[] PROGMEM = "LeakModel";
static const char PANEL_PROPERTY_6[] PROGMEM = "FlowModel";
static const char PANEL_PROPERTY_7[] PROGMEM = "PhModel";
static const char PANEL_PROPERTY_8[] PROGMEM = "Inverted";
static const char PANEL_PROPERTY_9[] PROGMEM = "Pullup";
static const char PANEL_PROPERTY_10[] PROGMEM = "DebounceMs";
static const char PANEL_PROPERTY_11[] PROGMEM = "PulsesPerLitre";
static const char PANEL_PROPERTY_12[] PROGMEM = "Type";
static const char PANEL_PROPERTY_13[] PROGMEM = "CalibValue1";
static const char PANEL_PROPERTY_14[] PROGMEM = "CalibRawValue1";
static const char PANEL_PROPERTY_15[] PROGMEM = "CalibValue2";
static const char PANEL_PROPERTY_16[] PROGMEM = "CalibRawValue2";
static const char PANEL_PROPERTY_17[] PROGMEM = "MinValue";
static const char PANEL_PROPERTY_18[] PROGMEM = "MaxValue";
static const char PANEL_PROPERTY_19[] PROGMEM = "CriticalMinValue";
static const char PANEL_PROPERTY_20[] PROGMEM = "CriticalMaxValue";
static const char PANEL_PROPERTY_21[] PROGMEM = "Value";
static const char PANEL_PROPERTY_22[] PROGMEM = "RawValue";
static const char PANEL_PROPERTY_23[] PROGMEM = "Humidity";
static const char PANEL_PROPERTY_24[] PROGMEM = "ControlMode";
static const char PANEL_PROPERTY_25[] PROGMEM = "State";
static const char PANEL_PROPERTY_26[] PROGMEM = "ControlDirection";
static const char PANEL_PROPERTY_27[] PROGMEM = "Input";
static const char PANEL_PROPERTY_28[] PROGMEM = "SensorsSetpoint";
static const char PANEL_PROPERTY_29[] PROGMEM = "MaxDeviation";
static const char PANEL_PROPERTY_30[] PROGMEM = "MorningMode";
static const char PANEL_PROPERTY_31[] PROGMEM = "AfternoonMode";
static const char PANEL_PROPERTY_32[] PROGMEM = "EveningMode";
static const char PANEL_PROPERTY_33[] PROGMEM = "NightMode";
static const char PANEL_PROPERTY_34[] PROGMEM = "SensorsValue";
static const char PANEL_PROPERTY_35[] PROGMEM = "Device";
static const char PANEL_PROPERTY_36[] PROGMEM = "ManualMode";
static const char PANEL_PROPERTY_37[] PROGMEM = "PidKp";
static const char PANEL_PROPERTY_38[] PROGMEM = "PidKi";
static const char PANEL_PROPERTY_39[] PROGMEM = "PidKd";
static const char PANEL_PROPERTY_40[] PROGMEM = "LedState";
static const char PANEL_PROPERTY_41[] PROGMEM = "LedControlMode";
static const char PANEL_PROPERTY_42[] PROGMEM = "LedManualMode";
static const char PANEL_PROPERTY_43[] PROGMEM = "CurrentBrightness";
static const char PANEL_PROPERTY_44[] PROGMEM = "CustomColor";
static const char PANEL_PROPERTY_45[] PROGMEM = "LedColorMorning";
static const char PANEL_PROPERTY_46[] PROGMEM = "MorningBrightness";
static const char PANEL_PROPERTY_47[] PROGMEM = "LedColorAfternoon";
static const char PANEL_PROPERTY_48[] PROGMEM = "AfternoonBrightness";
static const char PANEL_PROPERTY_49[] PROGMEM = "LedColorEvening";
static const char PANEL_PROPERTY_50[] PROGMEM = "EveningBrightness";
static const char PANEL_PROPERTY_51[] PROGMEM = "LedColorNight";
static const char PANEL_PROPERTY_52[] PROGMEM = "NightBrightness";
static const char PANEL_PROPERTY_53[] PROGMEM = "ScheduleMode";
static const char PANEL_PROPERTY_54[] PROGMEM = "MorningTime";
static const char PANEL_PROPERTY_55[] PROGMEM = "AfternoonTime";
static const char PANEL_PROPERTY_56[] PROGMEM = "EveningTime";
static const char PANEL_PROPERTY_57[] PROGMEM = "NightTime";
static const char PANEL_PROPERTY_58[] PROGMEM = "WeekdayMorningTime";
static const char PANEL_PROPERTY_59[] PROGMEM = "WeekdayAfternoonTime";
static const char PANEL_PROPERTY_60[] PROGMEM = "WeekdayEveningTime";
static const char PANEL_PROPERTY_61[] PROGMEM = "WeekdayNightTime";
static const char PANEL_PROPERTY_62[] PROGMEM = "WeekendMorningTime";
static const char PANEL_PROPERTY_63[] PROGMEM = "WeekendAfternoonTime";
static const char PANEL_PROPERTY_64[] PROGMEM = "WeekendEveningTime";
static const char PANEL_PROPERTY_65[] PROGMEM = "WeekendNightTime";
static const char PANEL_PROPERTY_66[] PROGMEM = "ActualPartOfDay";
static const char PANEL_PROPERTY_67[] PROGMEM = "ActualDate";
static const char PANEL_PROPERTY_68[] PROGMEM = "ActualTime";
static const char PANEL_PROPERTY_69[] PROGMEM = "TimezoneRule1Week";
static const char PANEL_PROPERTY_70[] PROGMEM = "TimezoneRule1DayOfWeek";
static const char PANEL_PROPERTY_71[] PROGMEM = "TimezoneRule1Month";
static const char PANEL_PROPERTY_72[] PROGMEM = "TimezoneRule1Hour";
static const char PANEL_PROPERTY_73[] PROGMEM = "TimezoneRule1Offset";
static const char PANEL_PROPERTY_74[] PROGMEM = "TimezoneRule2Week";
static const char PANEL_PROPERTY_75[] PROGMEM = "TimezoneRule2DayOfWeek";
static const char PANEL_PROPERTY_76[] PROGMEM = "TimezoneRule2Month";
static const char PANEL_PROPERTY_77[] PROGMEM = "TimezoneRule2Hour";
static const char PANEL_PROPERTY_78[] PROGMEM = "TimezoneRule2Offset";
static const char PANEL_PROPERTY_79[] PROGMEM = "ClockRtcStatus";
static const char PANEL_PROPERTY_80[] PROGMEM = "ActualTimezoneOffset";
static const char PANEL_PROPERTY_81[] PROGMEM = "ActualDayOfWeek";
static const char PANEL_PROPERTY_82[] PROGMEM = "FanStartTemp";
static const char PANEL_PROPERTY_83[] PROGMEM = "FanMaxSpeedTemp";
static const char PANEL_PROPERTY_84[] PROGMEM = "MaxIntTemp";
static const char PANEL_PROPERTY_85[] PROGMEM = "InternalTemperature";
static const char PANEL_PROPERTY_86[] PROGMEM = "InternalHumidity";
static const char PANEL_PROPERTY_87[] PROGMEM = "FanPWMValue";
static const char PANEL_PROPERTY_88[] PROGMEM = "BuzzerOnStart";
static const char PANEL_PROPERTY_89[] PROGMEM = "BuzzerOnErrors";
static const char PANEL_PROPERTY_90[] PROGMEM = "BuzzerMorning";
static const char PANEL_PROPERTY_91[] PROGMEM = "BuzzerAfternoon";
static const char PANEL_PROPERTY_92[] PROGMEM = "BuzzerEvening";
static const char PANEL_PROPERTY_93[] PROGMEM = "BuzzerNight";
static const char PANEL_PROPERTY_94[] PROGMEM = "ErrorCount";
static const char PANEL_PROPERTY_95[] PROGMEM = "AlarmLast1";
static const char PANEL_PROPERTY_96[] PROGMEM = "AlarmLast2";
static const char PANEL_PROPERTY_97[] PROGMEM = "AlarmLast3";
static const char PANEL_PROPERTY_98[] PROGMEM = "MegaLastBoot";
static const char* const PANEL_PROPERTIES[] PROGMEM={PANEL_PROPERTY_0,PANEL_PROPERTY_1,PANEL_PROPERTY_2,PANEL_PROPERTY_3,PANEL_PROPERTY_4,PANEL_PROPERTY_5,PANEL_PROPERTY_6,PANEL_PROPERTY_7,PANEL_PROPERTY_8,PANEL_PROPERTY_9,PANEL_PROPERTY_10,PANEL_PROPERTY_11,PANEL_PROPERTY_12,PANEL_PROPERTY_13,PANEL_PROPERTY_14,PANEL_PROPERTY_15,PANEL_PROPERTY_16,PANEL_PROPERTY_17,PANEL_PROPERTY_18,PANEL_PROPERTY_19,PANEL_PROPERTY_20,PANEL_PROPERTY_21,PANEL_PROPERTY_22,PANEL_PROPERTY_23,PANEL_PROPERTY_24,PANEL_PROPERTY_25,PANEL_PROPERTY_26,PANEL_PROPERTY_27,PANEL_PROPERTY_28,PANEL_PROPERTY_29,PANEL_PROPERTY_30,PANEL_PROPERTY_31,PANEL_PROPERTY_32,PANEL_PROPERTY_33,PANEL_PROPERTY_34,PANEL_PROPERTY_35,PANEL_PROPERTY_36,PANEL_PROPERTY_37,PANEL_PROPERTY_38,PANEL_PROPERTY_39,PANEL_PROPERTY_40,PANEL_PROPERTY_41,PANEL_PROPERTY_42,PANEL_PROPERTY_43,PANEL_PROPERTY_44,PANEL_PROPERTY_45,PANEL_PROPERTY_46,PANEL_PROPERTY_47,PANEL_PROPERTY_48,PANEL_PROPERTY_49,PANEL_PROPERTY_50,PANEL_PROPERTY_51,PANEL_PROPERTY_52,PANEL_PROPERTY_53,PANEL_PROPERTY_54,PANEL_PROPERTY_55,PANEL_PROPERTY_56,PANEL_PROPERTY_57,PANEL_PROPERTY_58,PANEL_PROPERTY_59,PANEL_PROPERTY_60,PANEL_PROPERTY_61,PANEL_PROPERTY_62,PANEL_PROPERTY_63,PANEL_PROPERTY_64,PANEL_PROPERTY_65,PANEL_PROPERTY_66,PANEL_PROPERTY_67,PANEL_PROPERTY_68,PANEL_PROPERTY_69,PANEL_PROPERTY_70,PANEL_PROPERTY_71,PANEL_PROPERTY_72,PANEL_PROPERTY_73,PANEL_PROPERTY_74,PANEL_PROPERTY_75,PANEL_PROPERTY_76,PANEL_PROPERTY_77,PANEL_PROPERTY_78,PANEL_PROPERTY_79,PANEL_PROPERTY_80,PANEL_PROPERTY_81,PANEL_PROPERTY_82,PANEL_PROPERTY_83,PANEL_PROPERTY_84,PANEL_PROPERTY_85,PANEL_PROPERTY_86,PANEL_PROPERTY_87,PANEL_PROPERTY_88,PANEL_PROPERTY_89,PANEL_PROPERTY_90,PANEL_PROPERTY_91,PANEL_PROPERTY_92,PANEL_PROPERTY_93,PANEL_PROPERTY_94,PANEL_PROPERTY_95,PANEL_PROPERTY_96,PANEL_PROPERTY_97,PANEL_PROPERTY_98};
static const PanelField panelFields[] PROGMEM = {
{0,1,0},
{0,1,1},
{0,1,2},
{0,1,3},
{0,1,4},
{0,1,5},
{0,1,6},
{0,1,7},
{0,1,8},
{0,1,9},
{0,1,10},
{0,1,11},
{0,1,12},
{0,1,13},
{0,1,14},
{0,1,15},
{0,1,16},
{0,1,17},
{0,1,18},
{0,1,19},
{0,1,20},
{0,1,21},
{0,1,22},
{0,1,23},
{0,2,0},
{0,2,1},
{0,2,2},
{0,2,3},
{0,2,4},
{0,2,5},
{0,2,6},
{0,2,7},
{0,2,8},
{0,2,9},
{0,2,10},
{0,2,11},
{0,2,12},
{0,2,13},
{0,2,14},
{0,2,15},
{0,2,16},
{0,2,17},
{0,2,18},
{0,2,19},
{0,2,20},
{0,2,21},
{0,2,22},
{0,2,23},
{0,3,0},
{0,3,1},
{0,3,2},
{0,3,3},
{0,3,4},
{0,3,5},
{0,3,6},
{0,3,7},
{0,3,8},
{0,3,9},
{0,3,10},
{0,3,11},
{0,3,12},
{0,3,13},
{0,3,14},
{0,3,15},
{0,3,16},
{0,3,17},
{0,3,18},
{0,3,19},
{0,3,20},
{0,3,21},
{0,3,22},
{0,3,23},
{0,4,0},
{0,4,1},
{0,4,2},
{0,4,3},
{0,4,4},
{0,4,5},
{0,4,6},
{0,4,7},
{0,4,8},
{0,4,9},
{0,4,10},
{0,4,11},
{0,4,12},
{0,4,13},
{0,4,14},
{0,4,15},
{0,4,16},
{0,4,17},
{0,4,18},
{0,4,19},
{0,4,20},
{0,4,21},
{0,4,22},
{0,4,23},
{0,5,0},
{0,5,1},
{0,5,2},
{0,5,3},
{0,5,4},
{0,5,5},
{0,5,6},
{0,5,7},
{0,5,8},
{0,5,9},
{0,5,10},
{0,5,11},
{0,5,12},
{0,5,13},
{0,5,14},
{0,5,15},
{0,5,16},
{0,5,17},
{0,5,18},
{0,5,19},
{0,5,20},
{0,5,21},
{0,5,22},
{0,5,23},
{0,6,0},
{0,6,1},
{0,6,2},
{0,6,3},
{0,6,4},
{0,6,5},
{0,6,6},
{0,6,7},
{0,6,8},
{0,6,9},
{0,6,10},
{0,6,11},
{0,6,12},
{0,6,13},
{0,6,14},
{0,6,15},
{0,6,16},
{0,6,17},
{0,6,18},
{0,6,19},
{0,6,20},
{0,6,21},
{0,6,22},
{0,6,23},
{0,7,0},
{0,7,1},
{0,7,2},
{0,7,3},
{0,7,4},
{0,7,5},
{0,7,6},
{0,7,7},
{0,7,8},
{0,7,9},
{0,7,10},
{0,7,11},
{0,7,12},
{0,7,13},
{0,7,14},
{0,7,15},
{0,7,16},
{0,7,17},
{0,7,18},
{0,7,19},
{0,7,20},
{0,7,21},
{0,7,22},
{0,7,23},
{0,8,0},
{0,8,1},
{0,8,2},
{0,8,3},
{0,8,4},
{0,8,5},
{0,8,6},
{0,8,7},
{0,8,8},
{0,8,9},
{0,8,10},
{0,8,11},
{0,8,12},
{0,8,13},
{0,8,14},
{0,8,15},
{0,8,16},
{0,8,17},
{0,8,18},
{0,8,19},
{0,8,20},
{0,8,21},
{0,8,22},
{0,8,23},
{1,1,0},
{1,1,24},
{1,1,25},
{1,1,26},
{1,1,27},
{1,1,28},
{1,1,29},
{1,1,30},
{1,1,31},
{1,1,32},
{1,1,33},
{1,1,34},
{1,2,0},
{1,2,24},
{1,2,25},
{1,2,26},
{1,2,27},
{1,2,28},
{1,2,29},
{1,2,30},
{1,2,31},
{1,2,32},
{1,2,33},
{1,2,34},
{1,3,0},
{1,3,24},
{1,3,25},
{1,3,26},
{1,3,27},
{1,3,28},
{1,3,29},
{1,3,30},
{1,3,31},
{1,3,32},
{1,3,33},
{1,3,34},
{1,4,0},
{1,4,24},
{1,4,25},
{1,4,26},
{1,4,27},
{1,4,28},
{1,4,29},
{1,4,30},
{1,4,31},
{1,4,32},
{1,4,33},
{1,4,34},
{2,1,0},
{2,1,35},
{2,1,24},
{2,1,26},
{2,1,27},
{2,1,28},
{2,1,29},
{2,1,30},
{2,1,31},
{2,1,32},
{2,1,33},
{2,1,36},
{2,1,37},
{2,1,38},
{2,1,39},
{2,1,21},
{2,1,34},
{2,2,0},
{2,2,35},
{2,2,24},
{2,2,26},
{2,2,27},
{2,2,28},
{2,2,29},
{2,2,30},
{2,2,31},
{2,2,32},
{2,2,33},
{2,2,36},
{2,2,37},
{2,2,38},
{2,2,39},
{2,2,21},
{2,2,34},
{2,3,0},
{2,3,35},
{2,3,24},
{2,3,26},
{2,3,27},
{2,3,28},
{2,3,29},
{2,3,30},
{2,3,31},
{2,3,32},
{2,3,33},
{2,3,36},
{2,3,37},
{2,3,38},
{2,3,39},
{2,3,21},
{2,3,34},
{2,4,0},
{2,4,35},
{2,4,24},
{2,4,26},
{2,4,27},
{2,4,28},
{2,4,29},
{2,4,30},
{2,4,31},
{2,4,32},
{2,4,33},
{2,4,36},
{2,4,37},
{2,4,38},
{2,4,39},
{2,4,21},
{2,4,34},
{2,5,0},
{2,5,35},
{2,5,24},
{2,5,26},
{2,5,27},
{2,5,28},
{2,5,29},
{2,5,30},
{2,5,31},
{2,5,32},
{2,5,33},
{2,5,36},
{2,5,37},
{2,5,38},
{2,5,39},
{2,5,21},
{2,5,34},
{2,6,0},
{2,6,35},
{2,6,24},
{2,6,26},
{2,6,27},
{2,6,28},
{2,6,29},
{2,6,30},
{2,6,31},
{2,6,32},
{2,6,33},
{2,6,36},
{2,6,37},
{2,6,38},
{2,6,39},
{2,6,21},
{2,6,34},
{3,1,40},
{3,1,41},
{3,1,42},
{3,1,43},
{3,1,44},
{3,1,45},
{3,1,46},
{3,1,47},
{3,1,48},
{3,1,49},
{3,1,50},
{3,1,51},
{3,1,52},
{4,1,53},
{4,1,54},
{4,1,55},
{4,1,56},
{4,1,57},
{4,1,58},
{4,1,59},
{4,1,60},
{4,1,61},
{4,1,62},
{4,1,63},
{4,1,64},
{4,1,65},
{4,1,66},
{5,1,67},
{5,1,68},
{5,1,69},
{5,1,70},
{5,1,71},
{5,1,72},
{5,1,73},
{5,1,74},
{5,1,75},
{5,1,76},
{5,1,77},
{5,1,78},
{5,1,79},
{5,1,80},
{5,1,81},
{6,1,82},
{6,1,83},
{6,1,84},
{6,1,85},
{6,1,86},
{6,1,87},
{7,1,88},
{7,1,89},
{7,1,90},
{7,1,91},
{7,1,92},
{7,1,93},
{8,1,94},
{8,1,95},
{8,1,96},
{8,1,97},
{8,1,98},
};
static PanelField panelField(uint16_t i) { PanelField f; memcpy_P(&f,&panelFields[i],sizeof(f)); return f; }
static String panelFieldName(uint16_t i) { return acFieldName(panelWireId(i)); }
static String panelFieldProperty(uint16_t i) { const char* property; memcpy_P(&property,PANEL_PROPERTIES+panelField(i).property,sizeof(property)); return String(FPSTR(property)); }
static const uint16_t PANEL_FIELD_COUNT=sizeof(panelFields)/sizeof(panelFields[0]);
static const uint8_t PANEL_COMMAND_ENABLED[] PROGMEM={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,0};
static String panelCommandField(uint16_t i) { return i<PANEL_FIELD_COUNT && pgm_read_byte(PANEL_COMMAND_ENABLED+i)?panelFieldName(i):String(); }
