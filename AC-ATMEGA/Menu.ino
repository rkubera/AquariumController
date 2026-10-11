//PCD8544: 84x48 pixels, 12 characters x 6 rows at the existing 7px font.
//Five data rows and one navigation row; rows are padded to erase old values.
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

void menuLine(byte row,String text) {
  text=lcdTrimMessage(text);
  for (byte i=0;i<12;i++) menuDesiredRows[row][i]=i<text.length() ? text[i] : ' ';
  menuDesiredRows[row][12]=0;
}

byte menuChannelCount() {
  return AcLcdMenu::channels(menuPage,megaFeatures);
}

String menuMode(byte mode) {
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
  String lines[5];
  if (menuPage==0) {
    time_t local=clockGetLocalTime();
    sprintf(bufferOut,"%04d/%02d/%02d",year(local),month(local),day(local)); lines[0]=bufferOut;
    sprintf(bufferOut,"%02d:%02d:%02d",hour(local),minute(local),second(local)); lines[1]=bufferOut;
    lines[3]=String("WiFi:")+(wifiStatus==WIFI_STATUS_CONNECTED ? "on" : "off");
    lines[4]=String("Broker:")+(brokerStatus==BROKER_STATUS_CONNECTED ? "on" : "off");
  }
  else if (menuPage==1) {
    lines[0]="NETWORK"; lines[1]=deviceName; lines[2]=wifi_ssid;
    lines[3]=brokerServer; lines[4]=String("Broker:")+(brokerStatus==BROKER_STATUS_CONNECTED ? "on" : "off");
  }
  else if (menuPage==2) {
    time_t local=clockGetLocalTime();
    lines[0]="CLOCK";
    sprintf(bufferOut,"%04d/%02d/%02d",year(local),month(local),day(local)); lines[1]=bufferOut;
    sprintf(bufferOut,"%02d:%02d:%02d",hour(local),minute(local),second(local)); lines[2]=bufferOut;
    if(megaFeatures.has(ControllerFeatures::Rtc))lines[3]="RTC:"+rtcStatusString();
    lines[4]=clockSyncStatusString();
  }
  else if (menuPage==3) {
    lines[0]="SENSOR "+intToString(channel);
    lines[1]=readConfigName(CONFIG_sensors_addr+menuChannel*SENSORS_SENSOR_CONFIG_BYTES,NAME_LENGTH);
    bool valid=sensorsGetSensor(channel,SENSORS_VALUE_VALID);
    lines[2]=valid ? "V:"+floatToString(sensorsGetSensor(channel,SENSORS_VALUE)) : "V:invalid";
    lines[3]="Raw:"+floatToString(sensorsGetSensor(channel,SENSORS_VALUE_RAW));
    int assignment=sensorsGetSensor(channel,SENSORS_VALUE_TYPE);
    bool available=assignment<RELAYS_COUNT ? assignment<megaFeatures.relays : assignment-RELAYS_COUNT<megaFeatures.pwm;
    lines[4]=assignment==SENSOR_TYPE_NONE || !available ? "To:none" : assignment<RELAYS_COUNT ? "To:Relay"+intToString(assignment+1) : "To:PWM"+intToString(assignment-RELAYS_COUNT+1);
  }
  else if (menuPage==4 || menuPage==5) {
    bool relay=menuPage==4;
    lines[0]=(relay ? "RELAY " : "PWM ")+intToString(channel);
    lines[1]=readConfigName((relay ? CONFIG_relays_addr+menuChannel*RELAYS_RELAY_CONFIG_BYTES : CONFIG_pwm_outputs_addr+menuChannel*PWMOUTPUTS_PWMOUTPUT_CONFIG_BYTES),NAME_LENGTH);
    byte mode=relay ? relaysGetRelay(channel,RELAY_CONTROL_MODE) : pwmOutputsGetPwmOutput(channel,PWMOUTPUT_CONTROL_MODE);
    byte override=relay ? relaysGetRelay(channel,RELAY_MANUAL_ONOFF) : 0;
    lines[2]=relay ? menuMode(mode)+":"+(override==0 ? "auto" : override==1 ? "off" : "on") : menuMode(mode);
    lines[3]=relay ? String("State:")+(relaysGetRelay(channel,RELAY_STATE) ? "on" : "off") : "V:"+floatToString(pwmOutputsGetPwmOutputDouble(channel,PWMOUTPUT_OUTPUT_VALUE));
    if(megaFeatures.inputs){
      float input=sensorsGetSensorsValue(channel,relay ? OUTPUT_TYPE_RELAY : OUTPUT_TYPE_PWM);
      lines[4]=isfinite(input) ? "In:"+floatToString(input) : "In:invalid";
    }
  }
  else if (menuPage==6) {
    lines[0]=String("LED:")+(ledActualState==LED_MANUAL_ONOFF_ON ? "on" : "off");
    lines[1]="Bright:"+String(ledBrightness)+"%";
    lines[2]="R:"+String((uint16_t)ledActualRed);
    lines[3]="G:"+String((uint16_t)ledActualGreen);
    lines[4]="B:"+String((uint16_t)ledActualBlue);
  }
  else if (menuPage==7) {
    lines[0]=megaFeatures.has(ControllerFeatures::Fan) ? "INTERNAL" : "DHT";
    lines[1]=dhtHasValidReading() ? "T:"+floatToString(dhtTemperature)+"C" : "T:invalid";
    lines[2]=dhtHasValidReading() ? "H:"+floatToString(dhtHumidity)+"%" : "H:invalid";
    if(megaFeatures.has(ControllerFeatures::Fan)){
      lines[3]="Limit:"+intToString(maxInternalTemperature)+"C";
      lines[4]="Fan:"+intToString(fanPWM);
    }
  }
  else {
    lines[0]="ERRORS"; lines[1]="Count:"+intToString(errorsCount);
    lines[2]="Flags:"+intToString(errorsReportStatus);
    lines[3]="CRC:"+String(serialCrcErrors);
    lines[4]="Long:"+String(serialOversizedFrames);
  }
  for (byte i=0;i<5;i++) menuLine(i,lines[i]);
  if (menuPage==0) menuLine(5,"A/B kontrast");
  else {
    String navigation=intToString(AcLcdMenu::number(menuPage,megaFeatures))+"/"+intToString(AcLcdMenu::count(megaFeatures));
    if(count>1)navigation+=" "+intToString(channel)+"/"+intToString(count)+" A/B";
    menuLine(5,navigation);
  }
  menuLcdService();
}
