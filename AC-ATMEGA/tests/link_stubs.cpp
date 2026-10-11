String clockTimezoneCodeToWeek(int value) {
  const char* names[]={"last","first","second","third","fourth"}; return names[value];
}
String clockTimezoneCodeToDoW(int value) { return daysOfTheWeek[value-1]; }
String clockTimezoneCodeToMonth(int value) { return monthsOfYear[value-1]; }

void lcdSetContrast(byte contrast) { lcdContrast=contrast; }

void lcdBacklightWake() {}
void lcdBacklightService() {}
