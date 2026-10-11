#pragma once
#include "PanelWireIds.h"
struct PanelField { byte group,channel,property; };
static const char PANEL_PROPERTY_0[] PROGMEM = "Name";
static const char PANEL_PROPERTY_1[] PROGMEM = "Type";
static const char PANEL_PROPERTY_2[] PROGMEM = "CalibValue1";
static const char PANEL_PROPERTY_3[] PROGMEM = "CalibRawValue1";
static const char PANEL_PROPERTY_4[] PROGMEM = "CalibValue2";
static const char PANEL_PROPERTY_5[] PROGMEM = "CalibRawValue2";
static const char PANEL_PROPERTY_6[] PROGMEM = "MinValue";
static const char PANEL_PROPERTY_7[] PROGMEM = "MaxValue";
static const char PANEL_PROPERTY_8[] PROGMEM = "CriticalMinValue";
static const char PANEL_PROPERTY_9[] PROGMEM = "CriticalMaxValue";
static const char PANEL_PROPERTY_10[] PROGMEM = "Value";
static const char PANEL_PROPERTY_11[] PROGMEM = "RawValue";
static const char PANEL_PROPERTY_12[] PROGMEM = "ControlMode";
static const char PANEL_PROPERTY_13[] PROGMEM = "State";
static const char PANEL_PROPERTY_14[] PROGMEM = "ControlDirection";
static const char PANEL_PROPERTY_15[] PROGMEM = "Input";
static const char PANEL_PROPERTY_16[] PROGMEM = "SensorsSetpoint";
static const char PANEL_PROPERTY_17[] PROGMEM = "MaxDeviation";
static const char PANEL_PROPERTY_18[] PROGMEM = "MorningMode";
static const char PANEL_PROPERTY_19[] PROGMEM = "AfternoonMode";
static const char PANEL_PROPERTY_20[] PROGMEM = "EveningMode";
static const char PANEL_PROPERTY_21[] PROGMEM = "NightMode";
static const char PANEL_PROPERTY_22[] PROGMEM = "SensorsValue";
static const char PANEL_PROPERTY_23[] PROGMEM = "ManualMode";
static const char PANEL_PROPERTY_24[] PROGMEM = "PidKp";
static const char PANEL_PROPERTY_25[] PROGMEM = "PidKi";
static const char PANEL_PROPERTY_26[] PROGMEM = "PidKd";
static const char PANEL_PROPERTY_27[] PROGMEM = "LedState";
static const char PANEL_PROPERTY_28[] PROGMEM = "LedControlMode";
static const char PANEL_PROPERTY_29[] PROGMEM = "LedManualMode";
static const char PANEL_PROPERTY_30[] PROGMEM = "CurrentBrightness";
static const char PANEL_PROPERTY_31[] PROGMEM = "CustomColor";
static const char PANEL_PROPERTY_32[] PROGMEM = "LedColorMorning";
static const char PANEL_PROPERTY_33[] PROGMEM = "MorningBrightness";
static const char PANEL_PROPERTY_34[] PROGMEM = "LedColorAfternoon";
static const char PANEL_PROPERTY_35[] PROGMEM = "AfternoonBrightness";
static const char PANEL_PROPERTY_36[] PROGMEM = "LedColorEvening";
static const char PANEL_PROPERTY_37[] PROGMEM = "EveningBrightness";
static const char PANEL_PROPERTY_38[] PROGMEM = "LedColorNight";
static const char PANEL_PROPERTY_39[] PROGMEM = "NightBrightness";
static const char PANEL_PROPERTY_40[] PROGMEM = "ScheduleMode";
static const char PANEL_PROPERTY_41[] PROGMEM = "MorningTime";
static const char PANEL_PROPERTY_42[] PROGMEM = "AfternoonTime";
static const char PANEL_PROPERTY_43[] PROGMEM = "EveningTime";
static const char PANEL_PROPERTY_44[] PROGMEM = "NightTime";
static const char PANEL_PROPERTY_45[] PROGMEM = "WeekdayMorningTime";
static const char PANEL_PROPERTY_46[] PROGMEM = "WeekdayAfternoonTime";
static const char PANEL_PROPERTY_47[] PROGMEM = "WeekdayEveningTime";
static const char PANEL_PROPERTY_48[] PROGMEM = "WeekdayNightTime";
static const char PANEL_PROPERTY_49[] PROGMEM = "WeekendMorningTime";
static const char PANEL_PROPERTY_50[] PROGMEM = "WeekendAfternoonTime";
static const char PANEL_PROPERTY_51[] PROGMEM = "WeekendEveningTime";
static const char PANEL_PROPERTY_52[] PROGMEM = "WeekendNightTime";
static const char PANEL_PROPERTY_53[] PROGMEM = "ActualPartOfDay";
static const char PANEL_PROPERTY_54[] PROGMEM = "ActualDate";
static const char PANEL_PROPERTY_55[] PROGMEM = "ActualTime";
static const char PANEL_PROPERTY_56[] PROGMEM = "TimezoneRule1Week";
static const char PANEL_PROPERTY_57[] PROGMEM = "TimezoneRule1DayOfWeek";
static const char PANEL_PROPERTY_58[] PROGMEM = "TimezoneRule1Month";
static const char PANEL_PROPERTY_59[] PROGMEM = "TimezoneRule1Hour";
static const char PANEL_PROPERTY_60[] PROGMEM = "TimezoneRule1Offset";
static const char PANEL_PROPERTY_61[] PROGMEM = "TimezoneRule2Week";
static const char PANEL_PROPERTY_62[] PROGMEM = "TimezoneRule2DayOfWeek";
static const char PANEL_PROPERTY_63[] PROGMEM = "TimezoneRule2Month";
static const char PANEL_PROPERTY_64[] PROGMEM = "TimezoneRule2Hour";
static const char PANEL_PROPERTY_65[] PROGMEM = "TimezoneRule2Offset";
static const char PANEL_PROPERTY_66[] PROGMEM = "ClockRtcStatus";
static const char PANEL_PROPERTY_67[] PROGMEM = "ActualTimezoneOffset";
static const char PANEL_PROPERTY_68[] PROGMEM = "ActualDayOfWeek";
static const char PANEL_PROPERTY_69[] PROGMEM = "FanStartTemp";
static const char PANEL_PROPERTY_70[] PROGMEM = "FanMaxSpeedTemp";
static const char PANEL_PROPERTY_71[] PROGMEM = "MaxIntTemp";
static const char PANEL_PROPERTY_72[] PROGMEM = "InternalTemperature";
static const char PANEL_PROPERTY_73[] PROGMEM = "InternalHumidity";
static const char PANEL_PROPERTY_74[] PROGMEM = "FanPWMValue";
static const char PANEL_PROPERTY_75[] PROGMEM = "BuzzerOnStart";
static const char PANEL_PROPERTY_76[] PROGMEM = "BuzzerOnErrors";
static const char PANEL_PROPERTY_77[] PROGMEM = "BuzzerMorning";
static const char PANEL_PROPERTY_78[] PROGMEM = "BuzzerAfternoon";
static const char PANEL_PROPERTY_79[] PROGMEM = "BuzzerEvening";
static const char PANEL_PROPERTY_80[] PROGMEM = "BuzzerNight";
static const char PANEL_PROPERTY_81[] PROGMEM = "ErrorCount";
static const char PANEL_PROPERTY_82[] PROGMEM = "AlarmLast1";
static const char PANEL_PROPERTY_83[] PROGMEM = "AlarmLast2";
static const char PANEL_PROPERTY_84[] PROGMEM = "AlarmLast3";
static const char* const PANEL_PROPERTIES[] PROGMEM={PANEL_PROPERTY_0,PANEL_PROPERTY_1,PANEL_PROPERTY_2,PANEL_PROPERTY_3,PANEL_PROPERTY_4,PANEL_PROPERTY_5,PANEL_PROPERTY_6,PANEL_PROPERTY_7,PANEL_PROPERTY_8,PANEL_PROPERTY_9,PANEL_PROPERTY_10,PANEL_PROPERTY_11,PANEL_PROPERTY_12,PANEL_PROPERTY_13,PANEL_PROPERTY_14,PANEL_PROPERTY_15,PANEL_PROPERTY_16,PANEL_PROPERTY_17,PANEL_PROPERTY_18,PANEL_PROPERTY_19,PANEL_PROPERTY_20,PANEL_PROPERTY_21,PANEL_PROPERTY_22,PANEL_PROPERTY_23,PANEL_PROPERTY_24,PANEL_PROPERTY_25,PANEL_PROPERTY_26,PANEL_PROPERTY_27,PANEL_PROPERTY_28,PANEL_PROPERTY_29,PANEL_PROPERTY_30,PANEL_PROPERTY_31,PANEL_PROPERTY_32,PANEL_PROPERTY_33,PANEL_PROPERTY_34,PANEL_PROPERTY_35,PANEL_PROPERTY_36,PANEL_PROPERTY_37,PANEL_PROPERTY_38,PANEL_PROPERTY_39,PANEL_PROPERTY_40,PANEL_PROPERTY_41,PANEL_PROPERTY_42,PANEL_PROPERTY_43,PANEL_PROPERTY_44,PANEL_PROPERTY_45,PANEL_PROPERTY_46,PANEL_PROPERTY_47,PANEL_PROPERTY_48,PANEL_PROPERTY_49,PANEL_PROPERTY_50,PANEL_PROPERTY_51,PANEL_PROPERTY_52,PANEL_PROPERTY_53,PANEL_PROPERTY_54,PANEL_PROPERTY_55,PANEL_PROPERTY_56,PANEL_PROPERTY_57,PANEL_PROPERTY_58,PANEL_PROPERTY_59,PANEL_PROPERTY_60,PANEL_PROPERTY_61,PANEL_PROPERTY_62,PANEL_PROPERTY_63,PANEL_PROPERTY_64,PANEL_PROPERTY_65,PANEL_PROPERTY_66,PANEL_PROPERTY_67,PANEL_PROPERTY_68,PANEL_PROPERTY_69,PANEL_PROPERTY_70,PANEL_PROPERTY_71,PANEL_PROPERTY_72,PANEL_PROPERTY_73,PANEL_PROPERTY_74,PANEL_PROPERTY_75,PANEL_PROPERTY_76,PANEL_PROPERTY_77,PANEL_PROPERTY_78,PANEL_PROPERTY_79,PANEL_PROPERTY_80,PANEL_PROPERTY_81,PANEL_PROPERTY_82,PANEL_PROPERTY_83,PANEL_PROPERTY_84};
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
{1,1,0},
{1,1,12},
{1,1,13},
{1,1,14},
{1,1,15},
{1,1,16},
{1,1,17},
{1,1,18},
{1,1,19},
{1,1,20},
{1,1,21},
{1,1,22},
{1,2,0},
{1,2,12},
{1,2,13},
{1,2,14},
{1,2,15},
{1,2,16},
{1,2,17},
{1,2,18},
{1,2,19},
{1,2,20},
{1,2,21},
{1,2,22},
{1,3,0},
{1,3,12},
{1,3,13},
{1,3,14},
{1,3,15},
{1,3,16},
{1,3,17},
{1,3,18},
{1,3,19},
{1,3,20},
{1,3,21},
{1,3,22},
{1,4,0},
{1,4,12},
{1,4,13},
{1,4,14},
{1,4,15},
{1,4,16},
{1,4,17},
{1,4,18},
{1,4,19},
{1,4,20},
{1,4,21},
{1,4,22},
{2,1,0},
{2,1,12},
{2,1,14},
{2,1,15},
{2,1,16},
{2,1,17},
{2,1,18},
{2,1,19},
{2,1,20},
{2,1,21},
{2,1,23},
{2,1,24},
{2,1,25},
{2,1,26},
{2,1,10},
{2,1,22},
{2,2,0},
{2,2,12},
{2,2,14},
{2,2,15},
{2,2,16},
{2,2,17},
{2,2,18},
{2,2,19},
{2,2,20},
{2,2,21},
{2,2,23},
{2,2,24},
{2,2,25},
{2,2,26},
{2,2,10},
{2,2,22},
{2,3,0},
{2,3,12},
{2,3,14},
{2,3,15},
{2,3,16},
{2,3,17},
{2,3,18},
{2,3,19},
{2,3,20},
{2,3,21},
{2,3,23},
{2,3,24},
{2,3,25},
{2,3,26},
{2,3,10},
{2,3,22},
{2,4,0},
{2,4,12},
{2,4,14},
{2,4,15},
{2,4,16},
{2,4,17},
{2,4,18},
{2,4,19},
{2,4,20},
{2,4,21},
{2,4,23},
{2,4,24},
{2,4,25},
{2,4,26},
{2,4,10},
{2,4,22},
{2,5,0},
{2,5,12},
{2,5,14},
{2,5,15},
{2,5,16},
{2,5,17},
{2,5,18},
{2,5,19},
{2,5,20},
{2,5,21},
{2,5,23},
{2,5,24},
{2,5,25},
{2,5,26},
{2,5,10},
{2,5,22},
{2,6,0},
{2,6,12},
{2,6,14},
{2,6,15},
{2,6,16},
{2,6,17},
{2,6,18},
{2,6,19},
{2,6,20},
{2,6,21},
{2,6,23},
{2,6,24},
{2,6,25},
{2,6,26},
{2,6,10},
{2,6,22},
{3,1,27},
{3,1,28},
{3,1,29},
{3,1,30},
{3,1,31},
{3,1,32},
{3,1,33},
{3,1,34},
{3,1,35},
{3,1,36},
{3,1,37},
{3,1,38},
{3,1,39},
{4,1,40},
{4,1,41},
{4,1,42},
{4,1,43},
{4,1,44},
{4,1,45},
{4,1,46},
{4,1,47},
{4,1,48},
{4,1,49},
{4,1,50},
{4,1,51},
{4,1,52},
{4,1,53},
{5,1,54},
{5,1,55},
{5,1,56},
{5,1,57},
{5,1,58},
{5,1,59},
{5,1,60},
{5,1,61},
{5,1,62},
{5,1,63},
{5,1,64},
{5,1,65},
{5,1,66},
{5,1,67},
{5,1,68},
{6,1,69},
{6,1,70},
{6,1,71},
{6,1,72},
{6,1,73},
{6,1,74},
{7,1,75},
{7,1,76},
{7,1,77},
{7,1,78},
{7,1,79},
{7,1,80},
{8,1,81},
{8,1,82},
{8,1,83},
{8,1,84},
};
static PanelField panelField(uint16_t i) { PanelField f; memcpy_P(&f,&panelFields[i],sizeof(f)); return f; }
static String panelFieldName(uint16_t i) { return acFieldName(panelWireId(i)); }
static String panelFieldProperty(uint16_t i) { const char* property; memcpy_P(&property,PANEL_PROPERTIES+panelField(i).property,sizeof(property)); return String(FPSTR(property)); }
static const uint16_t PANEL_FIELD_COUNT=sizeof(panelFields)/sizeof(panelFields[0]);
static const uint8_t PANEL_COMMAND_ENABLED[] PROGMEM={1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0};
static String panelCommandField(uint16_t i) { return i<PANEL_FIELD_COUNT && pgm_read_byte(PANEL_COMMAND_ENABLED+i)?panelFieldName(i):String(); }
