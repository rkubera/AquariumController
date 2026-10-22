//PCD8544: 84x48 pixels, 12 characters x 6 rows at the existing 7px font.
//Five data rows and one navigation row; rows are padded to erase old values.
#include <stdarg.h>
static byte menuPage=0, menuChannel=0;
static char menuDesiredRows[6][13],menuDisplayedRows[6][13];
static bool menuRowValid[6]={false,false,false,false,false,false};
static byte menuNextRow=0;

//Bound each LCD transfer to one row; unchanged rows generate no traffic.
void menuLcdService() {
  for (byte checked=0;checked<6;checked++) {
    byte row=menuNextRow;
    menuNextRow=(menuNextRow+1)%6;
    if (menuRowValid[row] && strcmp(menuDesiredRows[row],menuDisplayedRows[row])==0) continue;
    gotoXY(0,row);
    lcdString(menuDesiredRows[row]);
    memcpy(menuDisplayedRows[row],menuDesiredRows[row],13);
    menuRowValid[row]=true;
    return;
  }
}

void menuLine(byte row,const char* text) {
  if(row>=6)return;
  if(!text)text="";
  byte length=0;
  while(length<12 && text[length]){menuDesiredRows[row][length]=text[length];++length;}
  if(length==12 && text[12])menuDesiredRows[row][11]='.';
  for(byte i=length;i<12;i++)menuDesiredRows[row][i]=' ';
  menuDesiredRows[row][12]=0;
}

void menuLine(byte row,const String& text) {menuLine(row,text.c_str());}

void menuLineFlash(byte row,const __FlashStringHelper* text) {
  char line[14]={};
  for(byte i=0;i<13;i++){line[i]=pgm_read_byte(reinterpret_cast<const char*>(text)+i);if(!line[i])break;}
  menuLine(row,line);
}

void menuLineFormat(byte row,const char* format,...) {
  char line[48];
  va_list args;va_start(args,format);vsnprintf_P(line,sizeof(line),format,args);va_end(args);
  menuLine(row,line);
}

void menuLineFloat(byte row,const char* format,float value) {
  // A finite AVR float needs at most 43 characters with two decimals and sign.
  char number[48];
  if(isfinite(value))dtostrf(value,0,2,number);
  else strcpy_P(number,PSTR("invalid"));
  menuLineFormat(row,format,number);
}

void menuLineName(byte row,int address) {
  char name[14]={};
  for(byte i=0;i<13 && i<NAME_LENGTH;i++){name[i]=configStorage.read(address+i);if(!name[i])break;}
  menuLine(row,name);
}

byte menuChannelCount() {
  return AcLcdMenu::channels(menuPage,megaFeatures);
}

const char* menuMode(byte mode) {
  if (mode==CONTROL_MODE_PART_OF_DAY) return "day";
  if (mode==CONTROL_MODE_TRESHOLD) return "hyst";
  if (mode==CONTROL_MODE_PID) return "pid";
  return "manual";
}

void menuInit() {
  menuPage=0; menuChannel=0; lastKeyMillis=millis(); menuNextRow=0;
  for (byte row=0;row<6;row++) {
    memset(menuDesiredRows[row],' ',12); menuDesiredRows[row][12]=0; menuRowValid[row]=false;
  }
}

void menuShowCMillis() {
  static unsigned long lastRender=0;
  byte previousPage=menuPage,previousChannel=menuChannel;
  AcLcdMenu::normalize(menuPage,menuChannel,megaFeatures);
  if (millis()-lastRender<200UL && !lastKey && previousPage==menuPage && previousChannel==menuChannel) { menuLcdService(); return; }
  lastRender=millis();
  char key=lastKey;
  lastKey=0;
  if (key>='1' && key<='9') {
    byte selected=AcLcdMenu::at(key-'1',megaFeatures);
    if(selected<AcLcdMenu::PageCount){menuPage=selected;menuChannel=0;}
  }
  if (key=='0') { menuPage=0; menuChannel=0; }
  //The home page has no channel selection: A/B tune contrast.
  if (menuPage==0 && (key=='A' || key=='B')) {
    int contrast=constrain((int)lcdContrast+(key=='A' ? 2 : -2),0,127);
    lcdSetContrast((byte)contrast);
  }
  byte count=menuChannelCount();
  if (key=='A') menuChannel=(menuChannel+count-1)%count;
  if (key=='B') menuChannel=(menuChannel+1)%count;
  if (previousPage!=menuPage || previousChannel!=menuChannel) menuNextRow=0;
  byte channel=menuChannel+1;
  //C toggles an output; D releases its temporary manual override.
  if (menuPage==4 && (key=='C' || key=='D')) {
    byte value=key=='D' ? RELAY_MANUAL_ONOFF_AUTO :
      relaysGetRelay(channel,RELAY_STATE) ? RELAY_MANUAL_ONOFF_OFF : RELAY_MANUAL_ONOFF_ON;
    relaysSetRelay(channel,RELAY_MANUAL_ONOFF,value);
    reportField(relayField(channel,RelayField::State),relaysGetRelay(channel,RELAY_STATE) ? "on" : "off");
  }
  if (menuPage==6 && (key=='D' || key=='C')) {
    ledManualOnOff=key=='D' ? LED_MANUAL_ONOFF_AUTO : ledActualState==LED_MANUAL_ONOFF_ON ? LED_MANUAL_ONOFF_OFF : LED_MANUAL_ONOFF_ON;
    ledSetActualMode(); ledPublishState(true);
  }
  // Format directly into the fixed LCD rows; no temporary String array.
  for(byte row=0;row<5;row++)menuLine(row,"");
  if (menuPage==0) {
    time_t local=clockGetLocalTime();
    menuLineFormat(0,PSTR("%04d/%02d/%02d"),year(local),month(local),day(local));
    menuLineFormat(1,PSTR("%02d:%02d:%02d"),hour(local),minute(local),second(local));
    menuLineFormat(3,PSTR("WiFi:%s"),wifiStatus==WIFI_STATUS_CONNECTED ? "on" : "off");
    menuLineFormat(4,PSTR("Broker:%s"),brokerStatus==BROKER_STATUS_CONNECTED ? "on" : "off");
  }
  else if (menuPage==1) {
    menuLineFlash(0,F("NETWORK"));menuLine(1,deviceName);menuLine(2,wifi_ssid);
    menuLine(3,brokerServer);
    menuLineFormat(4,PSTR("Broker:%s"),brokerStatus==BROKER_STATUS_CONNECTED ? "on" : "off");
  }
  else if (menuPage==2) {
    time_t local=clockGetLocalTime();
    menuLineFlash(0,F("CLOCK"));
    menuLineFormat(1,PSTR("%04d/%02d/%02d"),year(local),month(local),day(local));
    menuLineFormat(2,PSTR("%02d:%02d:%02d"),hour(local),minute(local),second(local));
    if(megaFeatures.has(ControllerFeatures::Rtc))menuLineFormat(3,PSTR("RTC:%s"),rtcStatusString());
    menuLine(4,clockSyncStatusString());
  }
  else if (menuPage==3) {
    menuLineFormat(0,PSTR("SENSOR %u"),unsigned(channel));
    menuLineName(1,CONFIG_sensors_addr+menuChannel*SENSORS_SENSOR_CONFIG_BYTES);
    bool valid=sensorsGetSensor(channel,SENSORS_VALUE_VALID);
    menuLineFloat(2,PSTR("V:%s"),valid ? sensorsGetSensor(channel,SENSORS_VALUE) : NAN);
    menuLineFloat(3,PSTR("Raw:%s"),sensorsGetSensor(channel,SENSORS_VALUE_RAW));
    int assignment=sensorsGetSensor(channel,SENSORS_VALUE_TYPE);
    bool available=assignment<RELAYS_COUNT ? assignment<megaFeatures.relays : assignment-RELAYS_COUNT<megaFeatures.pwm;
    if(assignment==SENSOR_TYPE_NONE || !available)menuLineFlash(4,F("To:none"));
    else if(assignment<RELAYS_COUNT)menuLineFormat(4,PSTR("To:Relay%d"),assignment+1);
    else menuLineFormat(4,PSTR("To:PWM%d"),assignment-RELAYS_COUNT+1);
  }
  else if (menuPage==4 || menuPage==5) {
    bool relay=menuPage==4;
    menuLineFormat(0,relay ? PSTR("RELAY %u") : PSTR("PWM %u"),unsigned(channel));
    menuLineName(1,relay ? CONFIG_relays_addr+menuChannel*RELAYS_RELAY_CONFIG_BYTES : CONFIG_pwm_outputs_addr+menuChannel*PWMOUTPUTS_PWMOUTPUT_CONFIG_BYTES);
    byte mode=relay ? relaysGetRelay(channel,RELAY_CONTROL_MODE) : pwmOutputsGetPwmOutput(channel,PWMOUTPUT_CONTROL_MODE);
    byte override=relay ? relaysGetRelay(channel,RELAY_MANUAL_ONOFF) : 0;
    if(relay)menuLineFormat(2,PSTR("%s:%s"),menuMode(mode),override==0 ? "auto" : override==1 ? "off" : "on");
    else menuLine(2,menuMode(mode));
    if(relay)menuLineFormat(3,PSTR("State:%s"),relaysGetRelay(channel,RELAY_STATE) ? "on" : "off");
    else menuLineFloat(3,PSTR("V:%s"),pwmOutputsGetPwmOutputDouble(channel,PWMOUTPUT_OUTPUT_VALUE));
    if(megaFeatures.inputs)menuLineFloat(4,PSTR("In:%s"),sensorsGetSensorsValue(channel,relay ? OUTPUT_TYPE_RELAY : OUTPUT_TYPE_PWM));
  }
  else if (menuPage==6) {
    menuLineFormat(0,PSTR("LED:%s"),ledActualState==LED_MANUAL_ONOFF_ON ? "on" : "off");
    menuLineFormat(1,PSTR("Bright:%u%%"),unsigned(ledBrightness));
    menuLineFormat(2,PSTR("R:%u"),unsigned(uint16_t(ledActualRed)));
    menuLineFormat(3,PSTR("G:%u"),unsigned(uint16_t(ledActualGreen)));
    menuLineFormat(4,PSTR("B:%u"),unsigned(uint16_t(ledActualBlue)));
  }
  else if (menuPage==7) {
    menuLineFlash(0,megaFeatures.has(ControllerFeatures::Fan) ? F("INTERNAL") : F("DHT"));
    bool valid=dhtHasValidReading();
    menuLineFloat(1,valid ? PSTR("T:%sC") : PSTR("T:%s"),valid ? dhtTemperature : NAN);
    menuLineFloat(2,valid ? PSTR("H:%s%%") : PSTR("H:%s"),valid ? dhtHumidity : NAN);
    if(megaFeatures.has(ControllerFeatures::Fan)){
      menuLineFormat(3,PSTR("Limit:%dC"),int(maxInternalTemperature));
      menuLineFormat(4,PSTR("Fan:%u"),unsigned(fanPWM));
    }
  }
  else {
    menuLineFlash(0,F("ERRORS"));
    menuLineFormat(1,PSTR("Count:%d"),int(errorsCount));
    menuLineFormat(2,PSTR("Flags:%d"),int(errorsReportStatus));
    menuLineFormat(3,PSTR("CRC:%lu"),static_cast<unsigned long>(serialCrcErrors));
    menuLineFormat(4,PSTR("Long:%lu"),static_cast<unsigned long>(serialOversizedFrames));
  }
  if (menuPage==0) menuLineFlash(5,F("A/B kontrast"));
  else if(count>1)menuLineFormat(5,PSTR("%u/%u %u/%u A/B"),unsigned(AcLcdMenu::number(menuPage,megaFeatures)),unsigned(AcLcdMenu::count(megaFeatures)),unsigned(channel),unsigned(count));
  else menuLineFormat(5,PSTR("%u/%u"),unsigned(AcLcdMenu::number(menuPage,megaFeatures)),unsigned(AcLcdMenu::count(megaFeatures)));
  menuLcdService();
}
