void sample(byte index,float value,byte target) {
  auto& s=mySensors[index];
  s.sensorType=target;
  s.resetReadings();
  s.calibRawRead1=0; s.calibRawRead2=1000;
  s.calibValue1=value; s.calibValue2=value+100;
  adc[s.analogPin]=0;
  s.finishReading(SENSOR_ADC_SAMPLES*adc[s.analogPin]);
  assert(s.isValid());
}
void checkClose(double actual,double expected) { assert(std::fabs(actual-expected)<0.001); }
int main() {
  TemporaryState choice; uint8_t selected=0;
  assert(choice.resolve(true,1,1,selected)); selected=1;
  assert(!choice.resolve(true,1,1,selected));
  assert(!choice.resolve(true,1,1,selected));
  assert(choice.resolve(true,1,2,selected) && selected==0);
  selected=2; assert(choice.resolve(false,1,2,selected));
  assert(!choice.resolve(false,1,2,selected,false) && selected==0);

  sensorsInit();
  assert(gpioMode[58]==INPUT);
  auto& sensor=mySensors[0];
  sensor.sensorType=0;
  sensor.calibValue1=0; sensor.calibValue2=100;
  sensor.calibRawRead1=0; sensor.calibRawRead2=1000;
  adc[sensor.analogPin]=250;
  for(int i=0;i<30;i++) { sensor.finishReading(SENSOR_ADC_SAMPLES*adc[sensor.analogPin]); checkClose(sensor.Value,25); checkClose(sensor.rawValue,250); }
  sample(0,0,0); checkClose(sensor.Value,0);
  sample(0,-5,0); checkClose(sensor.Value,-5);
  sensor.numReadings=255; sensor.Init(0); sensor.finishReading(SENSOR_ADC_SAMPLES*adc[sensor.analogPin]); //Buffer bounds.
  sensor.numReadings=SENSOR_FILTER_WINDOW;
  sample(0,10,0); sample(1,30,0); sample(2,-10,0);
  // Legacy multiple assignments cannot silently become an average or pick one.
  assert(sensorsGetOutputInput(1,OUTPUT_TYPE_RELAY)==0);
  assert(std::isnan(sensorsGetSensorsValue(1,OUTPUT_TYPE_RELAY)));
  sensorsSetOutputInput(1,OUTPUT_TYPE_RELAY,2);
  assert(mySensors[0].sensorType==SENSOR_TYPE_NONE && mySensors[2].sensorType==SENSOR_TYPE_NONE);
  assert(sensorsGetOutputInput(1,OUTPUT_TYPE_RELAY)==2);
  assert(mySensors[1].isValid());sample(1,30,0);
  checkClose(sensorsGetSensorsValue(1,OUTPUT_TYPE_RELAY),30);
  sensorsSetSensor(1,SENSORS_VALUE_TYPE,0); // Input-side edits enforce the same rule.
  assert(mySensors[1].sensorType==SENSOR_TYPE_NONE);sample(0,-10,0);
  checkClose(sensorsGetSensorsValue(1,OUTPUT_TYPE_RELAY),-10);
  mySensors[0].Value=FLT_MAX;assert(sensorsGetSensorsValue(1,OUTPUT_TYPE_RELAY)==FLT_MAX);
  sensorsSetOutputInput(1,OUTPUT_TYPE_PWM,1); // Moving an input disconnects its old output.
  assert(sensorsGetOutputInput(1,OUTPUT_TYPE_RELAY)==0);
  assert(sensorsGetOutputInput(1,OUTPUT_TYPE_PWM)==1);
  sample(0,0,RELAYS_COUNT);checkClose(sensorsGetSensorsValue(1,OUTPUT_TYPE_PWM),0);
  sensorsSetOutputInput(1,OUTPUT_TYPE_PWM,SENSORS_COUNT+1);assert(sensorsGetOutputInput(1,OUTPUT_TYPE_PWM)==1);
  ticks+=SENSOR_MAX_AGE_MILLIS+1;assert(std::isnan(sensorsGetSensorsValue(1,OUTPUT_TYPE_PWM)));
  sensorsSetOutputInput(1,OUTPUT_TYPE_PWM,0);assert(std::isnan(sensorsGetSensorsValue(1,OUTPUT_TYPE_PWM)));
  for(byte i=0;i<SENSORS_COUNT;i++) mySensors[i].sensorType=SENSOR_TYPE_NONE;
  sample(0,25,0);
  sensor.criticalMaxValue=24; sensor.finishReading(SENSOR_ADC_SAMPLES*adc[sensor.analogPin]); assert(!sensor.isValid());
  sensor.criticalMaxValue=FLT_MAX; sample(0,25,0);
  sensorsSetSensor(1,SENSORS_VALUE_CALIB_VALUE1,20); assert(!sensor.isValid());
  sample(0,25,0);
  sensor.calibRawRead2=sensor.calibRawRead1; sensor.finishReading(SENSOR_ADC_SAMPLES*adc[sensor.analogPin]); assert(!sensor.isValid());
  sample(0,20,0);
  relaysInit(); auto& relay=myRelays[0];
  assert(gpioMode[RELAY_PIN_1]==OUTPUT);
  relay.relayControlMode=CONTROL_MODE_TRESHOLD; relay.relaySensorsSetpoint=25; relay.maxDeviation=1;
  relay.ControlEvent(); assert(gpioLevel[RELAY_PIN_1]==HIGH);
  sample(0,25.5,0); relay.ControlEvent(); assert(relay.relayState);
  sample(0,27,0); relay.ControlEvent(); assert(!relay.relayState);
  sample(0,25,0); relay.ControlEvent(); assert(!relay.relayState);
  sample(0,23,0); relay.ControlEvent(); assert(relay.relayState);
  int before=CONFIG.writes;
  relaysSetRelay(1,RELAY_MANUAL_ONOFF,RELAY_MANUAL_ONOFF_OFF);
  relay.ControlEvent(); relay.ControlEvent(); assert(!relay.relayState);
  assert(CONFIG.writes==before);
  relaysSetRelay(1,RELAY_MANUAL_ONOFF,RELAY_MANUAL_ONOFF_AUTO);
  relay.ControlEvent(); assert(relay.relayState);
  relay.relayDirection=PID_REVERSE; relay.ControlEvent(); assert(!relay.relayState);
  sample(0,27,0); relay.ControlEvent(); assert(relay.relayState);
  sensor.valid=false; relay.ControlEvent(); assert(!relay.relayState);
  relaysSetRelay(1,RELAY_MANUAL_ONOFF,RELAY_MANUAL_ONOFF_ON); assert(!relay.relayState);
  sample(0,20,RELAYS_COUNT);
  pwmOutputsInit(); auto& pwm=myPwmOutputs[0];
  pwm.pwmOutputControlMode=CONTROL_MODE_PID; pwm.pwmOutputSensorsSetpoint=25;
  pwm.pwmOutputPID.SetTunings(10,0,0);
  ticks+=ANALOG_CONTROL_INTERVAL_MS+1; pwm.ControlEvent();
  checkClose(pwm.pwmOutputPIDInput,20); assert(pwm.pwmOutputValue==50);
  checkClose(pwmValue[DIGITAL_PWM_OUT_PIN_1],50);
  sample(0,30,RELAYS_COUNT);
  ticks+=ANALOG_CONTROL_INTERVAL_MS+1; pwm.ControlEvent(); assert(pwm.pwmOutputValue==0);
  sample(0,20,RELAYS_COUNT);
  ticks+=ANALOG_CONTROL_INTERVAL_MS+1; pwm.ControlEvent(); assert(pwm.pwmOutputValue==50);
  before=CONFIG.writes; ticks+=ANALOG_CONTROL_INTERVAL_MS+1;pwm.ControlEvent();
  assert(pwm.pwmOutputValue==50 && pwm.pwmOutputPID.GetMode()==PID_AUTOMATIC && CONFIG.writes==before);
  pwm.pwmOutputPID.SetMode(PID_MANUAL); pwm.pwmOutputPID.SetControllerDirection(PID_REVERSE); pwm.pwmOutputPIDOutput=0; pwm.pwmOutputPID.SetMode(PID_AUTOMATIC);
  sample(0,30,RELAYS_COUNT);
  ticks+=ANALOG_CONTROL_INTERVAL_MS+1; pwm.ControlEvent(); assert(pwm.pwmOutputValue==50);
  mySensors[0].valid=false; pwm.ControlEvent(); assert(pwm.pwmOutputValue==0);
  sample(0,30,RELAYS_COUNT);
  pwm.pwmOutputControlMode=CONTROL_MODE_TRESHOLD; pwm.pwmOutputDirection=CONTROL_DIRECT;
  pwm.maxDeviation=1;
  sample(0,20,RELAYS_COUNT); pwm.ControlEvent(); assert(pwm.pwmOutputValue==PWM_MAX);
  pwm.ControlEvent(); assert(pwm.pwmOutputValue==PWM_MAX);
  before=CONFIG.writes;
  pwm.pwmOutputReportAll(); relay.relayReportAll(); assert(CONFIG.writes==before);
  pwm.pwmOutputControlMode=CONTROL_MODE_MANUAL; pwm.pwmOutputManualMode=71;
  pwm.pwmOutputApplyValue(); assert(pwm.pwmOutputValue==71);
  assert(floatToString(-1.25)=="-1.25"); assert(floatToString(1.999)=="2.00");
  assert(std::isnan(stringToFloat("abc"))); assert(std::isnan(stringToFloat("12x"))); assert(std::isnan(stringToFloat("nan")));
  checkClose(stringToFloat(" -1.25 "),-1.25); assert(longToString(0xFFFFFFFFUL)=="4294967295");
  CONFIG=FakeConfigImage();
  CONFIG.write(CONFIG_pwm_outputs_addr+43,0xA5); pwm.saveConfig(CONFIG_pwm_outputs_addr);
  byte highPadding=CONFIG.read(CONFIG_pwm_outputs_addr+43);
  byte nextName[15]; std::memcpy(nextName,CONFIG.bytes+CONFIG_pwm_outputs_addr+44,15);
  pwm.maxDeviation=3.25; pwm.saveConfig(CONFIG_pwm_outputs_addr);
  assert(std::memcmp(nextName,CONFIG.bytes+CONFIG_pwm_outputs_addr+44,15)==0);
  configSaveString("123456789012345",CONFIG_pwm_outputs_addr,NAME_LENGTH);
  assert(CONFIG.read(CONFIG_pwm_outputs_addr+43)==highPadding);
  pwm.loadConfig(CONFIG_pwm_outputs_addr); checkClose(pwm.maxDeviation,3.25);
  CONFIG=FakeConfigImage(); pwm.loadConfig(CONFIG_pwm_outputs_addr);
  assert(pwm.pwmOutputControlMode==CONTROL_MODE_MANUAL && pwm.pwmOutputManualMode==0);
  //Overflow is rejected before a nonfinite value can reach PWM conversion.
  PID overflowPid; double extremeInput=0,extremeOutput=25,extremeSetpoint=100;
  overflowPid.Init(&extremeInput,&extremeOutput,&extremeSetpoint,std::numeric_limits<double>::max()/16,0,0,PID_P_ON_E,CONTROL_DIRECT);
  overflowPid.SetMode(PID_AUTOMATIC); ticks+=1000;
  assert(!overflowPid.Compute() && !overflowPid.GetArithmeticValid() && extremeOutput==0);
  overflowPid.SetTunings(1,0,0); ticks+=1000;
  assert(overflowPid.Compute() && overflowPid.GetArithmeticValid() && extremeOutput==100);
  // Runtime capability removal also overrides direct local/manual operations.
  megaFeatures={0,0,0,0};
  relay.relayControlMode=CONTROL_MODE_MANUAL;relay.relayManualOnOff=RELAY_MANUAL_ONOFF_ON;
  relay.ControlEvent();assert(gpioLevel[RELAY_PIN_1]==LOW);
  pwm.pwmOutputControlMode=CONTROL_MODE_MANUAL;pwm.pwmOutputManualMode=65535;
  pwm.pwmOutputApplyValue();assert(pwmValue[DIGITAL_PWM_OUT_PIN_1]==0);
  testAcquisition();
  puts("PASS: ADC averaging, negative/zero readings, bounds, single input, duplicate fail-safe, reassignment, validity, stale inputs, hysteresis, GPIO, PID, manual overrides, LINK state, numeric parsing, CONFIG configuration isolation.");
}
