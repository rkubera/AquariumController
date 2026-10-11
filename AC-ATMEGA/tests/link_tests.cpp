void wireSetting(uint32_t op,const String& field,const String& value){uint8_t p[160];AcWire::put32(p,op);AcWire::put16(p+4,acFieldId(field));memcpy(p+6,value.c_str(),value.length());megaWireReceive(AcWire::Setting,p,value.length()+6);}
void menuRefresh() {
  for(int i=0;i<6;i++) {
    int before=displayWrites;
    menuShowCMillis(); assert(displayWrites-before<=1);
  }
}
void drain() { for(int i=0;i<10000 && (i==0 || linkWireQueue.free()!=512 || Serial3.available());i++) linkSerialService(); assert(linkWireQueue.free()==512); }
void dhtReply(const byte* data) {
  //Response low/high, then 40 low/high data pulses at 10us sampling.
  dhtCaptureTick(false);
  for(int i=0;i<8;i++) dhtCaptureTick(false);
  for(int i=0;i<8;i++) dhtCaptureTick(true);
  dhtCaptureTick(false);
  for(int bit=0;bit<40;bit++) {
    for(int i=0;i<5;i++) dhtCaptureTick(false);
    int high=(data[bit/8] & (0x80>>(bit%8))) ? 7 : 3;
    for(int i=0;i<high;i++) dhtCaptureTick(true);
    dhtCaptureTick(false);
  }
}
std::string retained(const std::string& field) {
  drain();
  std::string prefix="value "+field+" ";
  size_t at=Serial3.output.rfind(prefix);
  if(at==std::string::npos) fprintf(stderr,"Missing field: %s\n",field.c_str());
  assert(at!=std::string::npos);
  at+=prefix.size(); return Serial3.output.substr(at,Serial3.output.find('\r',at)-at);
}
void command(const std::string& field,const std::string& value) {
  applySetting(acFieldId(field.c_str()),value.c_str());
  drain();
}
int main() {
  deviceName="test"; hostnameReceived=true; brokerStatus=BROKER_STATUS_CONNECTED;
  wifiStatus=WIFI_STATUS_CONNECTED;
  globalWeekDay=7; globalYear=2026; globalMonth=10; globalMonthDay=6;
  timezoneRule1Week=0; timezoneRule2Week=0;
  timezoneRule1DayOfWeek=1; timezoneRule2DayOfWeek=1;
  timezoneRule1Month=3; timezoneRule2Month=10;
  fanStartTemperature=30; fanMaxSpeedTemperature=35; maxInternalTemperature=50;
  sensorsInit(); relaysInit(); pwmOutputsInit();
  Serial3.output.clear();
  for(int i=1;i<=SENSORS_COUNT;i++) {
    std::string base="Sensor"+std::to_string(i);
    command(""+base+"Name","Input "+std::to_string(i));
    assert(retained(""+base+"Name")=="Input "+std::to_string(i));
    command(""+base+"Type","relay1"); assert(retained(""+base+"Type")=="relay1");
    command(""+base+"CalibValue1","-2.25"); assert(retained(""+base+"CalibValue1")=="-2.25");
    command(""+base+"CalibRawValue1","10"); assert(retained(""+base+"CalibRawValue1")=="10.00");
    command(""+base+"CalibValue2","28.75"); assert(retained(""+base+"CalibValue2")=="28.75");
    command(""+base+"CalibRawValue2","1010"); assert(retained(""+base+"CalibRawValue2")=="1010.00");
    for(const char* limit:{"MinValue","CriticalMinValue"}) {
      command(""+base+limit,"-10"); assert(retained(""+base+limit)=="-10.00");
    }
    for(const char* limit:{"MaxValue","CriticalMaxValue"}) {
      command(""+base+limit,"40"); assert(retained(""+base+limit)=="40.00");
    }
    command(""+base+"MaxValue","off"); assert(retained(""+base+"MaxValue")=="off");
    mySensors[i-1].publishAll();
    for(const char* field:{"Name","Type","Value","RawValue","CalibValue1","CalibRawValue1","CalibValue2","CalibRawValue2","MinValue","MaxValue","CriticalMinValue","CriticalMaxValue"})
      retained(""+base+field);
    command(""+base+"Type","notconnected");
  }
  for(int i=1;i<=RELAYS_COUNT;i++) {
    std::string base="Relay"+std::to_string(i);
    command(""+base+"Name","Relay "+std::to_string(i));
    command(""+base+"ControlMode","manual");
    command(""+base+"SensorsSetpoint","25.25"); assert(retained(""+base+"SensorsSetpoint")=="25.25");
    command(""+base+"MaxDeviation","1.5"); assert(retained(""+base+"MaxDeviation")=="1.50");
    command(""+base+"MaxDeviation","-9"); assert(retained(""+base+"MaxDeviation")=="1.50");
    command(""+base+"Input","sensor1"); assert(retained(""+base+"Input")=="sensor1");
    command(""+base+"ControlDirection","reverse");
    for(const char* period:{"MorningMode","AfternoonMode","EveningMode","NightMode"}) {
      command(""+base+period,"off"); assert(retained(""+base+period)=="off");
      command(""+base+period,"on"); assert(retained(""+base+period)=="on");
      command(""+base+period,"none"); assert(retained(""+base+period)=="on");
    }
    command(""+base+"State","on"); assert(retained(""+base+"State")=="on");
    command(""+base+"State","off"); assert(retained(""+base+"State")=="off");
    myRelays[i-1].relayReportAll();
    for(const char* field:{"Name","ControlMode","ControlDirection","State","SensorsValue","SensorsSetpoint","MaxDeviation","Input","MorningMode","AfternoonMode","EveningMode","NightMode"})
      retained(""+base+field);
  }
  for(int i=1;i<=PWMOUTPUTS_COUNT;i++) {
    std::string base="PWMOutput"+std::to_string(i);
    command(""+base+"Name","PWM "+std::to_string(i));
    command(""+base+"ControlMode","manual");
    command(""+base+"Input","sensor2"); assert(retained(""+base+"Input")=="sensor2");
    command(""+base+"ControlDirection","direct");
    for(const char* period:{"MorningMode","AfternoonMode","EveningMode","NightMode","ManualMode"}) {
      command(""+base+period,"127"); assert(retained(""+base+period)=="127");
      command(""+base+period,"65536"); assert(retained(""+base+period)=="127");
    }
    for(const char* field:{"SensorsSetpoint","MaxDeviation","PidKp","PidKi","PidKd"}) {
      command(""+base+field,"1.25");
      assert(retained(""+base+field)==(std::string(field).find("Pid")==0?"1.2500":"1.25"));
    }
    assert(acFieldId(String((base+"State").c_str()))==0);
    myPwmOutputs[i-1].pwmOutputReportAll();
    for(const char* field:{"Name","ControlMode","ControlDirection","Value","SensorsValue","SensorsSetpoint","MaxDeviation","Input","MorningMode","AfternoonMode","EveningMode","NightMode","ManualMode","PidKp","PidKi","PidKd"})
      retained(""+base+field);
  }
  //All five 16-bit settings survive save/load for every output, including high bits.
  for(byte ch=1;ch<=PWMOUTPUTS_COUNT;ch++){
    for(byte kind:{byte(PWMOUTPUT_MODE_MORNING),byte(PWMOUTPUT_MODE_AFTERNOON),byte(PWMOUTPUT_MODE_EVENING),byte(PWMOUTPUT_MODE_NIGHT),byte(PWMOUTPUT_MANUAL_MODE)})pwmOutputsSetPwmOutput(ch,kind,65535);
    myPwmOutputs[ch-1].loadConfig(CONFIG_pwm_outputs_addr+(ch-1)*PWMOUTPUTS_PWMOUTPUT_CONFIG_BYTES);
    assert(pwmOutputsGetPwmOutput(ch,PWMOUTPUT_MODE_MORNING)==65535 && pwmOutputsGetPwmOutput(ch,PWMOUTPUT_MANUAL_MODE)==65535);
  }
  //Mixed upper bytes must survive save/load without damaging sensor calibration.
  for(byte ch=1;ch<=PWMOUTPUTS_COUNT;ch++){
    auto& out=myPwmOutputs[ch-1];out.pwmOutputModeMorning=32768;out.pwmOutputModeAfternoon=65534;out.pwmOutputModeEvening=1024;out.pwmOutputModeNight=4097;out.pwmOutputManualMode=65000;
    int address=CONFIG_pwm_outputs_addr+(ch-1)*PWMOUTPUTS_PWMOUTPUT_CONFIG_BYTES;out.saveConfig(address);
    mySensors[ch-1].saveConfig(CONFIG_sensors_addr+(ch-1)*SENSORS_SENSOR_CONFIG_BYTES);
    out.loadConfig(address);assert(out.pwmOutputModeMorning==32768 && out.pwmOutputModeAfternoon==65534 && out.pwmOutputModeEvening==1024 && out.pwmOutputModeNight==4097 && out.pwmOutputManualMode==65000);
  }
  for(const char* period:{"Morning","Afternoon","Evening","Night","Current"}) {
    command(std::string("")+period+"Brightness","90");
    assert(retained(std::string("")+period+"Brightness")=="90");
  }
  for(const char* period:{"Morning","Afternoon","Evening","Night"}) {
    command(std::string("")+period+"Time","12:34");
    assert(retained(std::string("")+period+"Time")=="12:34:00");
    command(std::string("LedColor")+period,"green");
    assert(retained(std::string("LedColor")+period)=="green");
  }
  schedulerLoadConfiguration();
  command("ScheduleMode","weekdayweekend");assert(retained("ScheduleMode")=="weekdayweekend");
  command("WeekdayMorningTime","06:15:00");assert(retained("WeekdayMorningTime")=="06:15:00");
  command("WeekendMorningTime","09:45:00");assert(retained("WeekendMorningTime")=="09:45:00");
  schedulerLoadConfiguration();assert(schedulerPlanMode==1 && schedulerPlanTimes[0][0]==6 && schedulerPlanTimes[1][0]==9);
  command("WeekendMorningTime","09:45:01");assert(retained("WeekendMorningTime")=="09:45:00");
  command("ScheduleMode","daily");assert(retained("ScheduleMode")=="daily");
  command("MorningTime","07:25:00");assert(retained("MorningTime")=="07:25:00");
  command("MorningTime","07:25:59");assert(retained("MorningTime")=="07:25:00");
  command("ActualDate","2024-02-29");assert(retained("ActualDate")=="2024-02-29");
  command("ActualDate","2023-02-29");assert(retained("ActualDate")=="2024-02-29");
  command("ActualTime","12:34:42");assert(retained("ActualTime")=="12:34:56" && globalSecond==42);
  command("ActualDate","2026/10/06"); assert(retained("ActualDate")=="2026-10-06");
  command("ActualTime","12:34"); assert(retained("ActualTime").substr(0,5)=="12:34");
  for(int rule=1;rule<=2;rule++) {
    std::string base="TimezoneRule"+std::to_string(rule);
    command(""+base+"Week","last"); assert(retained(""+base+"Week")=="last");
    command(""+base+"DayOfWeek","sunday"); assert(retained(""+base+"DayOfWeek")=="Sunday");
    command(""+base+"Month","march"); assert(retained(""+base+"Month")=="March");
    command(""+base+"Hour","2"); assert(retained(""+base+"Hour")=="2");
    command(""+base+"Offset","+2"); assert(retained(""+base+"Offset")=="+2");
  }
  command("CustomColor","16711680");
  assert(configGetUint32Value(CONFIG_ledCustomColor_addr)==16711680);
  command("LedControlMode","manual"); command("LedManualMode","red");
  command("CurrentBrightness","50"); command("LedState","off"); assert(retained("LedState")=="off");
  command("LedState","on"); assert(retained("LedState")=="on");
  command("LedManualMode","invalid"); assert(ledManualMode==LED_MODE_RED);
  command("LedManualMode","black"); assert(ledManualMode==LED_MODE_BLACK);
  ledManualOnOff=LED_MANUAL_ONOFF_AUTO; ledSetActualMode();
  ledReportAll(); assert(retained("LedState")=="off");
  assert(Serial3.output.find("test/LedPowerState ")==std::string::npos);
  assert(Serial3.output.find("value POWER")==std::string::npos);
  assert(Serial3.output.find("power ")==std::string::npos);
  assert(linkIsTime("23:59") && !linkIsTime("24:00") && !linkIsTime("xx:00"));
  assert(linkIsDate("2024/02/29") && !linkIsDate("2023/02/29") && !linkIsDate("2026/04/31"));
  command("FanStartTemp","32"); assert(retained("FanStartTemp")=="32");
  command("FanMaxSpeedTemp","32"); assert(retained("FanMaxSpeedTemp")=="35");
  command("MaxIntTemp","60"); assert(retained("MaxIntTemp")=="60");
  command("BuzzerOnStart","off"); command("BuzzerOnErrors","off");
  dhtInit(); ticks+=2001; dhtGetDataMillisEvent();
  auto started=ticks; dhtGetDataMillisEvent(); assert(ticks==started && dhtRequestState==1);
  ticks+=18; dhtGetDataMillisEvent();
  byte data[]={50,0,25,0,75}; dhtReply(data); dhtGetDataMillisEvent();
  assert(retained("InternalTemperature")=="25.00");
  ticks+=2001; dhtGetDataMillisEvent(); ticks+=18; dhtGetDataMillisEvent();
  for(int i=0;i<22;i++) dhtCaptureTick(true);
  dhtGetDataMillisEvent();
  assert(retained("InternalTemperature")=="25.00");
  //Measure pulse duration rather than assuming every ISR ran at exactly 10us.
  ticks+=2001; dhtGetDataMillisEvent(); ticks+=18; dhtGetDataMillisEvent();
  dhtCaptureTickElapsed(false,20);
  for(int i=0;i<8;i++) dhtCaptureTickElapsed(false,20);
  for(int i=0;i<8;i++) dhtCaptureTickElapsed(true,20);
  dhtCaptureTickElapsed(false,20);
  for(int bit=0;bit<40;bit++) {
    for(int i=0;i<5;i++) dhtCaptureTickElapsed(false,20);
    dhtCaptureTickElapsed(true,20);
    if(data[bit/8] & (0x80>>(bit%8))) { dhtCaptureTickElapsed(true,60); dhtCaptureTickElapsed(true,60); }
    else dhtCaptureTickElapsed(true,40);
    dhtCaptureTickElapsed(false,20);
  }
  dhtGetDataMillisEvent(); assert(dhtValid && dhtTemperature==25 && dhtHumidity==50);
  //Repeated no-response failures reinitialize the receiver without blocking.
  for(int attempt=0;attempt<6;attempt++) {
    ticks+=6000; dhtGetDataMillisEvent(); ticks+=18; dhtGetDataMillisEvent();
    for(int i=0;i<22;i++) dhtCaptureTick(true);
    dhtGetDataMillisEvent();
  }
  auto reinitTime=ticks; dhtGetDataMillisEvent();
  assert(ticks==reinitTime && dhtFailures==0 && dhtRequestState==0 && dhtCaptureState==0);
  assert(gpioMode[DHT_PIN]==INPUT_PULLUP && !dhtValid);
  dhtGetDataMillisEvent(); assert(dhtRequestState==0);
  ticks+=2001; dhtGetDataMillisEvent(); ticks+=18; dhtGetDataMillisEvent();
  dhtReply(data); dhtGetDataMillisEvent(); assert(dhtValid);
  //Snapshot emits one field per step; no oversized per-channel UART bursts.
  publishValue=0;
  for(int i=0;i<1000 && publishValue<11;i++) { linkSnapshotStep(); drain(); }
  assert(publishValue==11);
  for(const char* field:{"FanStartTemp","FanMaxSpeedTemp","FanPWMValue","MaxIntTemp","InternalTemperature","InternalHumidity","BuzzerOnStart","BuzzerOnErrors","MorningTime","AfternoonTime","EveningTime","NightTime","ActualPartOfDay","ActualDate","ActualTime","ActualDayOfWeek","ActualTimezoneOffset","Status","ErrorCount","Hostname","Ssid","BrokerServer","WifiStatus","BrokerStatus"}) retained(field);
  assert(retained("ActualDayOfWeek")=="Saturday"); //TimeLib weekdays are 1..7.
  eventBrokerDisconnected();assert(brokerStatus==BROKER_STATUS_DISCONNECTED);
  eventBrokerConnected();assert(brokerStatus==BROKER_STATUS_CONNECTED&&publishValue==0);
  brokerServerReceived=false;eventBrokerServerReceived();assert(brokerServerReceived);drain();

  //A saturated hardware UART must not wait or emit partial software frames.
  drain(); Serial3.writable=0;
  auto before=ticks;
  std::string oldOutput=Serial3.output;
  for(int i=0;i<30;i++) wireValue(acFieldId("CustomColor"),"12345678901234567890");
  assert(ticks==before && Serial3.output==oldOutput && serialTxDroppedCommands>0);
  Serial3.writable=63; drain();
  std::string suffix=Serial3.output.substr(oldOutput.size());
  const std::string complete="value CustomColor 12345678901234567890\r\n";
  while(!suffix.empty()) { assert(suffix.find(complete)==0); suffix.erase(0,complete.size()); }

  // PID retains proportional/integral behavior and rescaling at sample changes.
  double pidInput=1,pidOutput=0,pidSetpoint=3;PID ramPid;
  ramPid.Init(&pidInput,&pidOutput,&pidSetpoint,2,1,0,PID_P_ON_E,CONTROL_DIRECT);
  ramPid.SetOutputLimits(0,PWM_MAX);ramPid.SetMode(PID_AUTOMATIC);
  assert(ramPid.Compute()&&fabs(pidOutput-4.2)<0.0001);
  ramPid.SetSampleTime(200);ticks+=100;assert(!ramPid.Compute());
  ticks+=100;assert(ramPid.Compute()&&fabs(pidOutput-4.6)<0.0001);

  //Brightness fades preserve color ratios and finish all channels together.
  ledControlMode=CONTROL_MODE_MANUAL; ledManualOnOff=LED_MANUAL_ONOFF_AUTO;
  ledManualMode=LED_MODE_CUSTOM; ledCustomColor=0x7f00ff; ledManualBrightness=100;
  ledStepSwitchColorSeconds=10; ledActualRed=0; ledActualGreen=0; ledActualBlue=0;
  ledSetActualMode(); ticks+=10000; ledSetActualMode();
  assert(fabs(ledActualRed-double(LED_PWM_MAX)*127/255)<0.01 && ledActualBlue==LED_PWM_MAX);
  ledManualBrightness=0; ledSetActualMode(); ticks+=5000; ledSetActualMode();
  assert(fabs(ledActualRed-double(LED_PWM_MAX)*127/510)<0.01 && fabs(ledActualBlue-LED_PWM_MAX/2.0)<0.01);
  assert(retained("LedState")=="off"); //Target is off while RGB is still fading.
  ticks+=5000; ledSetActualMode(); ledSetActualMode();
  assert(ledActualRed==0 && ledActualBlue==0 && retained("LedState")=="off");
  ledStepSwitchColorSeconds=0; ledManualBrightness=100; ledSetActualMode();
  assert(fabs(ledActualRed-double(LED_PWM_MAX)*127/255)<0.01 && ledActualBlue==LED_PWM_MAX);

  // Color changes use 1 second; shutdown still uses the configured 10 seconds.
  ledStepSwitchColorSeconds=10;
  ledCustomColor=0x00ff00;ledSetActualMode();assert(ledFadeDuration==1000);
  ticks+=1000;ledFadeService();assert(ledActualRed==0&&ledActualGreen==LED_PWM_MAX&&ledActualBlue==0);
  ledManualBrightness=50;ledSetActualMode();assert(ledFadeDuration==1000);
  ticks+=1000;ledFadeService();assert(fabs(ledActualGreen-LED_PWM_MAX/2.0)<0.1);
  ledManualBrightness=0;ledSetActualMode();assert(ledFadeDuration==10000);
  ticks+=10000;ledFadeService();
  ledManualBrightness=100;ledStepSwitchColorSeconds=0;ledSetActualMode();

  //Wave must start even when the preceding custom color is not a built-in RGB corner.
  ledManualMode=LED_MODE_WAVE; ledStepWaveSeconds=1;
  ledSetActualMode(); assert(ledRed==255 && ledGreen==0 && ledBlue==0);
  ticks+=1000; ledSetActualMode(); ledMicrosEvent();
  assert(ledRed==0 && ledGreen==255 && ledBlue==0);

  //Wave at non-full brightness still takes the configured 1 second per color.
  ledManualBrightness=50; ledManualMode=LED_MODE_RED; ledStepSwitchColorSeconds=0; ledSetActualMode();
  ledManualMode=LED_MODE_WAVE; ledSetActualMode(); ledMicrosEvent();
  ticks+=500; ledSetActualMode();
  assert(fabs(ledActualRed-LED_PWM_MAX/4.0)<0.1 && fabs(ledActualGreen-LED_PWM_MAX/4.0)<0.1);
  ticks+=500; ledSetActualMode(); ledMicrosEvent();
  assert(ledRed==0 && ledGreen==0 && ledBlue==255);

  //Percent brightness maps to high-resolution RGB PWM, validates bounds, and keeps endpoints exact.
  applySetting(FieldId::CurrentBrightness,"100"); assert(ledManualBrightness==100);
  applySetting(FieldId::CurrentBrightness,"50"); assert(ledManualBrightness==50);
  applySetting(FieldId::CurrentBrightness,"101"); assert(ledManualBrightness==50);
  applySetting(FieldId::CurrentBrightness,"-1"); assert(ledManualBrightness==50);
  ledManualMode=LED_MODE_WHITE; ledStepSwitchColorSeconds=0;
  ledManualBrightness=100; ledSetActualMode();
  assert(ledActualRed==LED_PWM_MAX && ledActualGreen==LED_PWM_MAX && ledActualBlue==LED_PWM_MAX);
  ledManualBrightness=0; ledSetActualMode();
  assert(ledActualRed==0 && ledActualGreen==0 && ledActualBlue==0);

  //At 2% the fade has hundreds of monotonic levels and never exceeds its target.
  ledManualBrightness=2; ledManualMode=LED_MODE_WHITE; ledStepSwitchColorSeconds=0;
  ledManualOnOff=LED_MANUAL_ONOFF_AUTO; ledSetActualMode();
  double lowTarget=LED_PWM_MAX*0.02; assert(fabs(ledActualRed-lowTarget)<0.01);
  ledStepSwitchColorSeconds=2; ledManualOnOff=LED_MANUAL_ONOFF_OFF; ledSetActualMode();
  int distinct=0;uint16_t previous=uint16_t(ledActualRed+0.5);
  for(int step=0;step<400;step++){
    ticks+=5;ledFadeService();uint16_t current=uint16_t(ledActualRed+0.5);
    assert(current<=previous && current<=uint16_t(lowTarget+0.5));
    if(current!=previous)++distinct;previous=current;
  }
  assert(distinct>250 && ledActualRed==0 && pwmValue[LED_RED_PIN]==0);
  ledManualOnOff=LED_MANUAL_ONOFF_ON;ledSetActualMode();ticks+=500;ledFadeService();
  double partial=ledActualRed; assert(partial>0 && partial<lowTarget);
  ledManualOnOff=LED_MANUAL_ONOFF_OFF;ledSetActualMode();
  assert(fabs(ledActualRed-partial)<0.01);ticks+=2000;ledFadeService();assert(ledActualRed==0);

  ledManualOnOff=LED_MANUAL_ONOFF_AUTO;ledManualBrightness=0;ledStepSwitchColorSeconds=0;ledSetActualMode();

  // Unknown numeric field IDs are ignored; no field parser exists.
  unsigned long priorColor=ledCustomColor;uint8_t unknown[7]={};AcWire::put32(unknown,91);AcWire::put16(unknown+4,65535);unknown[6]='1';megaWireReceive(AcWire::Setting,unknown,7);assert(ledCustomColor==priorColor);

  //NTP: reject invalid timestamps, require confirmation for large jumps.
  clockLastNtpTime=0;
  setTime(0); clockNTPSynchronize_cb(0); assert(clockSyncStatus==3 && now()==0);
  clockNTPSynchronize_cb(1770000000UL); assert(clockSyncStatus==2 && now()==0);
  ticks+=10000; clockNTPSynchronize_cb(1770000010UL);
  assert(clockSyncStatus==1 && now()==1770000010UL);
  assert(configGetUint32Value(CONFIG_unix_timestamp_addr)==1770000010UL);
  clockNTPSynchronize_cb(1780000000UL); assert(clockSyncStatus==2 && now()==1770000010UL);
  ticks+=10000; clockNTPSynchronize_cb(1780000010UL); assert(now()==1780000010UL);
  clockNTPSynchronize_cb(UINT32_MAX); assert(clockSyncStatus==3 && now()==1780000010UL);
  assert(configGetUint32Value(CONFIG_unix_timestamp_addr)==1780000010UL);
  int clockWrites=CONFIG.writes;
  clockNTPSynchronize_cb(1780000011UL); assert(now()==1780000011UL);
  assert(CONFIG.writes==clockWrites);
  uint32_t parsed=0; assert(linkParseUint32("4294967295",parsed) && parsed==UINT32_MAX);
  assert(!linkParseUint32("4294967296",parsed));

  //DHT checksum failures, timeout and valid decimal temperature.
  ticks+=2001; dhtGetDataMillisEvent(); ticks+=18; dhtGetDataMillisEvent();
  byte corrupt[]={50,0,25,0,74}; dhtReply(corrupt); dhtGetDataMillisEvent(); assert(!dhtValid);
  ticks+=2001; dhtGetDataMillisEvent(); ticks+=18; dhtGetDataMillisEvent();
  byte decimal[]={50,0,25,5,80}; dhtReply(decimal); dhtGetDataMillisEvent();
  assert(dhtValid && fabs(dhtTemperature-25.5)<0.01);

  //Melodies advance only on later loop calls, without consuming simulated time.
  buzzerOnStart=BUZZER_ON; before=ticks; buzzerInit();
  assert(ticks==before && buzzerSequence==1 && mockTone>0);
  for(int i=0;i<30;i++) { ticks+=1000; buzzerMillisEvent(); }
  assert(buzzerSequence==0 && mockTone==0);

  // The alarm threshold is independent of fan operating temperatures.
  fanStartTemperature=21;fanMaxSpeedTemperature=26;maxInternalTemperature=50;
  command("MaxIntTemp","20");assert(maxInternalTemperature==20);
  command("FanMaxSpeedTemp","40");assert(fanMaxSpeedTemperature==40);
  command("MaxIntTemp","0");assert(maxInternalTemperature==0);
  command("FanMaxSpeedTemp","21");assert(fanMaxSpeedTemperature==40);
  // Explicit phase settings apply to every error, including high temperature.
  int oldPart=partOfDay;partOfDay=SCHEDULER_MODE_NIGHT;
  buzzerOnErrors=BUZZER_ON;errorsCount=1;errorsReportStatus=4;buzzerPhaseMask=7;
  beepErrors();assert(buzzerSequence==0);
  command("BuzzerNight","on");assert(buzzerPhaseMask==15 && configGetValue(CONFIG_buzzerPhases_addr)==0xaf);
  beepErrors();assert(buzzerSequence==2);
  command("BuzzerNight","off");buzzerMillisEvent();assert(buzzerSequence==0 && mockTone==0);
  command("BuzzerNight","on");errorsReportStatus=8;ticks+=5001;beepErrors();assert(buzzerSequence==2);
  errorsReportStatus=1;buzzerMillisEvent();assert(buzzerSequence==0 && mockTone==0);
  command("BuzzerNight","off");assert(buzzerPhaseMask==7);
  errorsReportStatus=4;beepErrors();assert(buzzerSequence==0);
  buzzerOnErrors=BUZZER_OFF;beepErrors();assert(buzzerSequence==0);
  partOfDay=SCHEDULER_MODE_MORNING;buzzerOnErrors=BUZZER_ON;
  errorsReportStatus=8;ticks+=5001;beepErrors();assert(buzzerSequence==2);
  // Stop a resolved ordinary error, even if another cause is still active.
  errorsReportStatus=1;buzzerMillisEvent();assert(buzzerSequence==0 && mockTone==0);
  ticks+=5001;beepErrors();assert(buzzerSequence==2);
  errorsCount=0;errorsReportStatus=0;buzzerMillisEvent();assert(buzzerSequence==0 && mockTone==0);
  errorsCount=1;errorsReportStatus=1;ticks+=5001;beepErrors();assert(buzzerSequence==2);
  buzzerOnErrors=BUZZER_OFF;buzzerMillisEvent();assert(buzzerSequence==0 && mockTone==0);
  partOfDay=oldPart;errorsCount=0;errorsReportStatus=0;maxInternalTemperature=100;

  //A schedule crossing midnight must not let the morning interval swallow night.
  schedulerStartMorningHour=18; schedulerStartMorningMinute=0;
  schedulerStartAfternoonHour=6; schedulerStartAfternoonMinute=0;
  schedulerStartEveningHour=20; schedulerStartEveningMinute=0;
  schedulerStartNightHour=22; schedulerStartNightMinute=0;
  assert(schedulerGetPartOfDay(19,0,false)==SCHEDULER_MODE_MORNING);
  assert(schedulerGetPartOfDay(21,0,false)==SCHEDULER_MODE_EVENING);
  assert(schedulerGetPartOfDay(23,0,false)==SCHEDULER_MODE_NIGHT);
  assert(schedulerGetPartOfDay(1,0,false)==SCHEDULER_MODE_NIGHT);
  assert(schedulerGetPartOfDay(7,0,false)==SCHEDULER_MODE_AFTERNOON);

  //All LCD rows fit, navigation wraps and output keys share LINK control rules.
  menuInit(); ticks+=201; menuRefresh();
  assert(display[0].find("2026/10/06")==0 && display[1].find("12:34:56")==0);
  assert(display[2]==std::string(12,' ') && display[5]=="A/B kontrast");
  assert(display[3].find("WiFi:")==0 && display[4].find("Broker:")==0);
  for(int page=1;page<9;page++) {
    keypad.next='1'+page; keyboardCheckMillisEvent(); menuRefresh(); assert(menuPage==page && menuChannel==0);
    for(auto &line:display) assert(line.size()==12);
  }
  lastKey='0'; ticks+=201; menuRefresh(); assert(menuPage==0);
  menuPage=4; menuChannel=0;
  relaysSetRelay(1,RELAY_CONTROL_MODE,CONTROL_MODE_MANUAL);
  relaysSetRelay(1,RELAY_MANUAL_ONOFF,RELAY_MANUAL_ONOFF_OFF);
  lastKey='C'; ticks+=201; menuRefresh(); assert(relaysGetRelay(1,RELAY_STATE));
  lastKey='D'; ticks+=201; menuRefresh(); assert(relaysGetRelay(1,RELAY_MANUAL_ONOFF)==0);
  lastKey='A'; ticks+=201; menuRefresh(); assert(menuChannel==RELAYS_COUNT-1);
  lastKey='B'; ticks+=201; menuRefresh(); assert(menuChannel==0);
  ledManualBrightness=50; ledManualMode=LED_MODE_RED; ledSetActualMode();
  keypad.next='*'; keyboardCheckMillisEvent(); assert(ledManualOnOff==LED_MANUAL_ONOFF_OFF);
  keypad.next='#'; keyboardCheckMillisEvent(); assert(ledManualOnOff==LED_MANUAL_ONOFF_AUTO);
  lastKey='0'; ticks+=201; menuRefresh(); assert(menuPage==0);
  lcdContrast=40;
  keypad.next='B'; keyboardCheckMillisEvent(); menuRefresh(); assert(lcdContrast==38 && menuPage==0);
  keypad.next='A'; keyboardCheckMillisEvent(); menuRefresh(); assert(lcdContrast==40 && menuPage==0);
  lcdContrast=0; lastKey='B'; menuRefresh(); assert(lcdContrast==0);
  lcdContrast=127; lastKey='A'; menuRefresh(); assert(lcdContrast==127);
  lcdContrast=40;
  //A stable page is not retransmitted, and a change emits only the dirty row.
  int rowWrites=displayWrites;
  ticks+=201; menuRefresh(); assert(displayWrites==rowWrites);
  const char* changedWifi=display[3].find("WiFi:on")==0 ? "WiFi:off" : "WiFi:on";
  menuLine(3,changedWifi);
  menuLcdService(); assert(displayWrites==rowWrites+1 && display[3].find(changedWifi)==0);
  menuLine(3,changedWifi); menuLcdService(); assert(displayWrites==rowWrites+1);
  //Changing pages during a partial refresh replaces pending old rows.
  lastKey='2'; menuShowCMillis(); assert(menuPage==1);
  lastKey='0'; menuRefresh(); assert(menuPage==0);
  assert(display[2]==std::string(12,' ') && display[5]=="A/B kontrast");

  publishValue=0; linkSnapshotField=3; linkSnapshotChannel=2;
  linkRequestSnapshot();
  assert(publishValue==0 && linkSnapshotField==0 && linkSnapshotChannel==0);
  //Timestamp queries remain queued without a blocking UART write.
  wifiStatus=WIFI_STATUS_CONNECTED;drain();Serial3.output.clear();
  ticks+=CLOCK_SYNC_INTERVAL_MILLIS+1;clockSyncEvent();drain();
  assert(Serial3.output.find("query timestamp\r\n")!=std::string::npos);
  dhtValid=true; dhtLastReadingMillis=ticks;
  fanStartTemperature=30; fanMaxSpeedTemperature=35;
  dhtTemperature=30.5; fanMillisEvent(); assert(fanPWM==83);
  dhtTemperature=31; fanMillisEvent(); assert(fanPWM==102 && pwmValue[FAN_PIN]==102);
  dhtTemperature=30; fanMillisEvent(); assert(fanPWM==0);
  dhtValid=false; fanMillisEvent(); assert(fanPWM==255);
  dhtValid=true; dhtTemperature=29; fanMillisEvent(); assert(fanPWM==0);
  fanMaxSpeedTemperature=30; fanMillisEvent(); assert(fanPWM==255);

  //Local panel commands use the same validated handlers without a broker.
  drain(); Serial3.output.clear(); hostnameReceived=true; brokerStatus=BROKER_STATUS_DISCONNECTED;
  linkLocalPanel=true;linkRequestSnapshot(); assert(linkLocalPanel && publishValue==0);
  // Settings are applied/read back without any controller-side persistence.
  wireSetting(1,"Sensor1Name","local probe"); linkPanelService(); drain();
  assert(Serial3.output.find("result 1 Sensor1Name local probe\r\n")!=std::string::npos);
  Serial3.output.clear();
  wireSetting(2,"Relay1State","off"); linkPanelService(); drain();
  assert(Serial3.output.find("result 2 Relay1State off\r\n")!=std::string::npos);
  float priorGain=pwmOutputsGetPwmOutputDouble(1,PWMOUTPUT_PID_KP);
  Serial3.output.clear(); wireSetting(3,"PWMOutput1PidKp","-1"); linkPanelService(); drain();
  assert(pwmOutputsGetPwmOutputDouble(1,PWMOUTPUT_PID_KP)==priorGain);
  assert(Serial3.output.find("result 3 PWMOutput1PidKp ")!=std::string::npos);

  Serial3.output.clear();wireSetting(4,"MaxIntTemp","20");
  for(int attempt=0;attempt<20 && linkPanelField;attempt++){linkPanelService();drain();}
  assert(maxInternalTemperature==20);
  assert(Serial3.output.find("result 4 MaxIntTemp 20\r\n")!=std::string::npos);
  maxInternalTemperature=100;
  eventWifiDisconnected(); assert(hostnameReceived); eventBrokerDisconnected();
  linkRequestSnapshot(); Serial3.output.clear(); linkSnapshotStep(); drain();
  assert(Serial3.output.find("value ")!=std::string::npos);
  linkSnapshotCapture=true;linkSnapshotFieldCount=0;linkSnapshotField=1;
  int formatted=0;
  reportField(FieldId::CurrentBrightness,(++formatted,String(11)));
  reportField(FieldId::MorningBrightness,(++formatted,String(22)));
  reportField(FieldId::NightBrightness,(++formatted,String(33)));
  assert(formatted==1 && linkSnapshotFieldCount==3);
  linkSnapshotCapture=false;drain();
  // Fade rendering continues without recalculating mode or publishing state.
  ledControlMode=CONTROL_MODE_MANUAL;ledManualOnOff=LED_MANUAL_ONOFF_AUTO;
  ledManualBrightness=100;ledStepSwitchColorSeconds=0;ledManualMode=LED_MODE_RED;ledSetActualMode();
  ledStepSwitchColorSeconds=2;ledManualMode=LED_MODE_BLUE;ledSetActualMode();
  ticks+=500;ledFadeService();
  assert(std::abs(ledActualRed-LED_PWM_MAX/2.0)<1 && std::abs(ledActualBlue-LED_PWM_MAX/2.0)<1);
  ticks+=500;ledFadeService();assert(ledActualRed==0 && ledActualBlue==LED_PWM_MAX);
  //Liveness and duplicate suppression are tested by the binary transport suite.

  // Cold-start interlock applies even to manual ON and sensor-failure fan demand.
  megaConfigReady=false;
  relaysSetRelay(1,RELAY_CONTROL_MODE,CONTROL_MODE_MANUAL);
  relaysSetRelay(1,RELAY_MANUAL_ONOFF,RELAY_MANUAL_ONOFF_ON);relaysMillisEvent();
  assert(!relaysGetRelay(1,RELAY_STATE));
  pwmOutputsSetPwmOutput(1,PWMOUTPUT_CONTROL_MODE,CONTROL_MODE_MANUAL);
  pwmOutputsSetPwmOutput(1,PWMOUTPUT_MANUAL_MODE,255);
  pwmOutputsMillisEvent();
  assert(pwmOutputsGetPwmOutputDouble(1,PWMOUTPUT_OUTPUT_VALUE)==0);
  ledManualMode=LED_MODE_RED;ledManualBrightness=100;ledManualOnOff=LED_MANUAL_ONOFF_ON;ledSetActualMode();
  assert(ledActualRed==0&&ledActualGreen==0&&ledActualBlue==0);
  dhtValid=false;fanMillisEvent();assert(fanPWM==0);
  megaConfigReady=true;
  // Unassigned regulation inputs are configuration, not alarms.
  for(byte channel=1;channel<=SENSORS_COUNT;channel++)sensorsSetSensor(channel,SENSORS_VALUE_TYPE,SENSOR_TYPE_NONE);
  dhtValid=true;dhtLastReadingMillis=ticks;dhtTemperature=25;maxInternalTemperature=100;
  relaysSetRelay(1,RELAY_CONTROL_MODE,CONTROL_MODE_TRESHOLD);
  pwmOutputsSetPwmOutput(1,PWMOUTPUT_CONTROL_MODE,CONTROL_MODE_PID);
  errorReport(false);assert(errorsCount==0);
  sensorsSetOutputInput(1,OUTPUT_TYPE_RELAY,1);errorReport(false);
  assert(errorsCount>=1 && (errorsReportStatus & 2));
  sensorsSetOutputInput(1,OUTPUT_TYPE_RELAY,0);errorReport(false);assert(errorsCount==0);
  sensorsSetOutputInput(1,OUTPUT_TYPE_PWM,2);errorReport(false);
  assert(errorsCount>=1 && (errorsReportStatus & 2) && !(errorsReportStatus & 16));
  sensorsSetOutputInput(1,OUTPUT_TYPE_PWM,0);errorReport(false);assert(errorsCount==0);

  // Enabled RTC faults participate in the common alarm lifecycle.
  clockRtcStatus=2;errorReport(false);
  assert(errorsCount==1 && (errorsReportStatus & 32) && alarmHistory.codes[0]==27);
  clockRtcStatus=3;errorReport(false);assert(errorsCount==1 && alarmHistory.codes[0]==27);
  clockRtcStatus=1;errorReport(false);assert(errorsCount==0 && alarmHistory.count==0);
  clockRtcStatus=0;errorReport(false);assert(errorsCount==0);
  clockRtcStatus=2;megaFeatures.flags &= ~ControllerFeatures::Rtc;
  errorReport(false);assert(errorsCount==0);
  megaFeatures.flags |= ControllerFeatures::Rtc;clockRtcStatus=1;

  // Every writable Portal/HA field must have a supported Mega readback.
  for(const char* name:testCommandNames){
    drain();Serial3.output.clear();linkPanelId=999;linkPanelField=acFieldId(name);
    assert(linkPanelField);linkPanelService();drain();
    if(Serial3.output.find("unsupported")!=std::string::npos){fprintf(stderr,"Missing readback: %s\n",name);abort();}
    assert(Serial3.output.find(std::string("result 999 ")+name+" ")!=std::string::npos);
  }
  // All native defaults must survive the controller's real validation/readback.
  megaConfigReady=false;fanStartTemperature=0;fanMaxSpeedTemperature=35;
  for(byte i=1;i<=SENSORS_COUNT;i++)sensorsSetSensor(i,SENSORS_VALUE_TYPE,SENSOR_TYPE_NONE);
  for(const auto& record:testRestoreRecords){
    drain();Serial3.output.clear();applySetting(acFieldId(record[0]),record[1]);
    drain();Serial3.output.clear();linkPanelId=998;linkPanelField=acFieldId(record[0]);linkPanelService();drain();
    std::string prefix=std::string("result 998 ")+record[0]+" ";auto at=Serial3.output.find(prefix);assert(at!=std::string::npos);
    auto end=Serial3.output.find("\r\n",at);std::string actual=Serial3.output.substr(at+prefix.size(),end-at-prefix.size());
    char *ae=nullptr,*be=nullptr;double av=strtod(actual.c_str(),&ae),bv=strtod(record[1],&be);
    bool numeric=!actual.empty()&&*record[1]&&!*ae&&!*be&&std::isfinite(av)&&std::isfinite(bv)&&fabs(av-bv)<0.00001;
    if(strcasecmp(actual.c_str(),record[1])!=0&&!numeric){fprintf(stderr,"Restore rejected %s: requested [%s], actual [%s]\n",record[0],record[1],actual.c_str());abort();}
  }
  megaConfigReady=true;
  megaFeatures={0,0,0,0};dhtValid=false;dhtTemperature=100;
  errorReport(true);assert(errorsCount==0);
  fanEvent();assert(fanPWM==0);
  assert(!linkFieldWanted(FieldId::InternalTemperature));
  assert(!linkFieldWanted(relayField(1,RelayField::State)));
  assert(linkFieldWanted(FieldId::LedState));
  megaFeatures={15,2,3,0};
  applySetting(relayField(1,RelayField::ControlMode),"treshold");
  assert(relaysGetRelay(1,RELAY_CONTROL_MODE)==CONTROL_MODE_MANUAL);
  applySetting(pwmField(1,PWMOutputField::ControlMode),"pid");
  assert(pwmOutputsGetPwmOutput(1,PWMOUTPUT_CONTROL_MODE)==CONTROL_MODE_MANUAL);
  // Names may legitimately equal a removed mode token.
  applySetting(relayField(1,RelayField::Name),"pid");
  assert(readConfigName(CONFIG_relays_addr,NAME_LENGTH)=="pid");
  // A temporary day-plan brightness never modifies any scheduled brightness.
  megaConfigReady=true;megaFeatures={15,4,6,8};partOfDay=SCHEDULER_MODE_MORNING;
  ledControlMode=CONTROL_MODE_PART_OF_DAY;ledManualOnOff=LED_MANUAL_ONOFF_AUTO;
  ledModeMorning=LED_MODE_CUSTOM;ledModeAfternoon=LED_MODE_RED;
  uint16_t previousManualBrightness=ledManualBrightness;
  ledMorningBrightness=30;ledAfternoonBrightness=45;ledScheduleBrightnessReset=true;
  ledSetActualMode();assert(ledCurrentBrightness==30);
  applySetting(FieldId::CurrentBrightness,"80");
  for(int i=0;i<20;i++)ledSetActualMode();
  assert(ledCurrentBrightness==80&&ledMorningBrightness==30&&ledAfternoonBrightness==45&&ledManualBrightness==previousManualBrightness);
  partOfDay=SCHEDULER_MODE_AFTERNOON;ledSetActualMode();assert(ledCurrentBrightness==45);
  partOfDay=SCHEDULER_MODE_MORNING;ledSetActualMode();assert(ledCurrentBrightness==30);
  applySetting(FieldId::CustomColor,"66051");assert(ledRed==1&&ledGreen==2&&ledBlue==3);
  partOfDay=SCHEDULER_MODE_AFTERNOON;ledSetActualMode();
  applySetting(FieldId::CustomColor,"16777215");assert(ledRed==255&&ledGreen==255&&ledBlue==255&&ledCustomColor==16777215&&ledModeAfternoon==LED_MODE_RED);
  ledControlMode=CONTROL_MODE_MANUAL;ledManualMode=LED_MODE_WHITE;ledSetActualMode();assert(ledCurrentBrightness==previousManualBrightness);
  // Brightness edits explicitly set power, even after a manual off/on click.
  applySetting(FieldId::LedState,"off");assert(ledActualState==LED_MANUAL_ONOFF_OFF);
  applySetting(FieldId::CurrentBrightness,"25");assert(ledActualState==LED_MANUAL_ONOFF_ON&&ledCurrentBrightness==25);
  applySetting(FieldId::CurrentBrightness,"0");assert(ledActualState==LED_MANUAL_ONOFF_OFF&&ledCurrentBrightness==0);
  ledManualMode=LED_MODE_WHITE;applySetting(FieldId::CurrentBrightness,"10");
  ledManualMode=LED_MODE_BLACK;applySetting(FieldId::CurrentBrightness,"10");assert(ledActualState==LED_MANUAL_ONOFF_ON);
  ledControlMode=CONTROL_MODE_PART_OF_DAY;partOfDay=SCHEDULER_MODE_MORNING;ledScheduleBrightnessReset=true;ledSetActualMode();
  applySetting(FieldId::LedState,"off");applySetting(FieldId::CurrentBrightness,"80");assert(ledActualState==LED_MANUAL_ONOFF_ON);
  applySetting(FieldId::CurrentBrightness,"0");assert(ledActualState==LED_MANUAL_ONOFF_OFF&&ledMorningBrightness==30);
  // RGB overrides survive regular calculations without changing saved selectors.
  ledInit();ledControlMode=CONTROL_MODE_MANUAL;ledManualMode=LED_MODE_RED;ledModeMorning=LED_MODE_BLUE;
  applySetting(FieldId::CurrentBrightness,"50");
  applySetting(FieldId::CustomColor,"66051");
  assert(ledManualMode==LED_MODE_RED&&ledModeMorning==LED_MODE_BLUE);
  for(int i=0;i<20;i++)ledSetActualMode();
  assert(ledRed==1&&ledGreen==2&&ledBlue==3);
  ledManualMode=LED_MODE_GREEN;ledSetActualMode();assert(ledRed==0&&ledGreen==255&&ledBlue==0);
  ledControlMode=CONTROL_MODE_PART_OF_DAY;
  ledMorningBrightness=ledAfternoonBrightness=ledEveningBrightness=ledNightBrightness=50;
  ledManualOnOff=LED_MANUAL_ONOFF_AUTO;
  for(byte phase=SCHEDULER_MODE_MORNING;phase<=SCHEDULER_MODE_NIGHT;phase++){
    ledModeMorning=ledModeAfternoon=ledModeEvening=ledModeNight=LED_MODE_RED;partOfDay=phase;
    applySetting(FieldId::CustomColor,"8323327");
    assert(ledModeMorning==LED_MODE_RED&&ledModeAfternoon==LED_MODE_RED&&ledModeEvening==LED_MODE_RED&&ledModeNight==LED_MODE_RED);
    for(int i=0;i<20;i++)ledSetActualMode();
    assert(ledRed==127&&ledGreen==0&&ledBlue==255);
  }
  partOfDay=SCHEDULER_MODE_MORNING;ledSetActualMode();assert(ledRed==255&&ledGreen==0&&ledBlue==0);
  megaConfigReady=false;ledManualMode=LED_MODE_GREEN;ledModeNight=LED_MODE_BLUE;
  applySetting(FieldId::CustomColor,"16711680");assert(ledManualMode==LED_MODE_GREEN&&ledModeNight==LED_MODE_BLUE);
  megaConfigReady=true;
  // Editing scheduled brightness never overrides current brightness mid-phase.
  ledControlMode=CONTROL_MODE_PART_OF_DAY;partOfDay=SCHEDULER_MODE_MORNING;
  ledMorningBrightness=30;ledScheduleBrightnessReset=true;ledSetActualMode();
  applySetting(FieldId::CurrentBrightness,"80");
  applySetting(FieldId::MorningBrightness,"55");assert(ledMorningBrightness==55&&ledCurrentBrightness==80);
  applySetting(FieldId::AfternoonBrightness,"60");assert(ledAfternoonBrightness==60&&ledCurrentBrightness==80);
  for(int i=0;i<20;i++)ledSetActualMode();assert(ledCurrentBrightness==80);
  partOfDay=SCHEDULER_MODE_AFTERNOON;ledSetActualMode();assert(ledCurrentBrightness==60);
  partOfDay=SCHEDULER_MODE_MORNING;ledSetActualMode();assert(ledCurrentBrightness==55);
  puts("PASS: LINK for all 8 sensors, 4 relays and 6 PWM channels; global settings and paced snapshots; proportional LED fades; bounded application queue, DHT and buzzer; clock jump validation; LCD/keypad; numeric wire IDs, spaces, numeric validation, unknown numeric IDs, offline local panel changes/readback, rejection and cold-start output interlocks.");
}
