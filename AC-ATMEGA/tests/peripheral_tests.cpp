// Integration through the actual input calibration and regulator assignment.
void testPeripheralInputs(){
 CONFIG=FakeConfigImage();megaFeatures={15,4,6,8};megaConfigReady=true;
 sensorsInit();for(byte i=0;i<SENSORS_COUNT;i++)mySensors[i].sensorType=SENSOR_TYPE_NONE;
 for(byte type=AcPeripheral::Dht11;type<=AcPeripheral::Ds18b20;type++){
  String name=peripheralDeviceName(type);
  assert(peripheralsApplySetting(sensorField(1,SensorField::Device),name));
  assert(peripheralInputDevice(1)==type);
  auto& s=mySensors[0];s.minValue=s.criticalMinValue=-FLT_MAX;s.maxValue=s.criticalMaxValue=FLT_MAX;
  assert(s.calibRawRead1==0&&s.calibRawRead2==1&&s.calibValue2==1);
  sensorsDigitalReading(1,-12.5,type==AcPeripheral::Ds18b20?16:10);
  assert(s.isValid()&&std::fabs(s.Value+12.5)<0.001);
  ticks+=2100;assert(s.isValid());ticks+=8000;assert(!s.isValid());
  if(AcPeripheral::isDht(type)){
   peripheralsApplySetting(sensorField(1,SensorField::Measurement),"humidity");
   assert(peripheralInputMeasurement(1)==1);
   sensorsDigitalReading(1,54.5,10);assert(std::fabs(s.Value-54.5)<0.001);
   peripheralsApplySetting(sensorField(1,SensorField::Measurement),"temperature");
  }
 }
 sensorsAssignInput(1,RELAYS_COUNT);assert(sensorsGetOutputInput(1,OUTPUT_TYPE_PWM)==1);
 sensorsDigitalReading(1,20,16);assert(std::fabs(sensorsGetSensorsValue(1,OUTPUT_TYPE_PWM)-20)<0.001);
 sensorsSetSensor(1,SENSORS_VALUE_CALIB_RAW_VALUE1,-10);assert(mySensors[0].calibRawRead1==-10);
 mySensors[0].calibValue1=-10;mySensors[0].calibRawRead2=100;mySensors[0].calibValue2=100;
 sensorsDeviceChanged(1,false);sensorsDigitalReading(1,30,16);assert(mySensors[0].Value==30);
 mySensors[0].criticalMaxValue=25;sensorsDigitalReading(1,30,16);assert(!mySensors[0].isValid());
 assert(!isfinite(sensorsGetSensorsValue(1,OUTPUT_TYPE_PWM)));
 peripheralsApplySetting(sensorField(1,SensorField::Device),"disabled");
 assert(mySensors[0].sensorType==SENSOR_TYPE_NONE);
 sensorsAssignInput(1,0);assert(mySensors[0].sensorType==SENSOR_TYPE_NONE);
 peripheralsApplySetting(sensorField(1,SensorField::Device),"analog");
 assert(mySensors[0].calibRawRead2==1023);
 peripheralsApplySetting(pwmField(1,PWMOutputField::Device),"servo");assert(peripheralOutputDevice(1)==AcPeripheral::Servo);
 peripheralsApplySetting(pwmField(1,PWMOutputField::Device),"unknown");assert(peripheralOutputDevice(1)==AcPeripheral::Servo);
 peripheralsApplySetting(pwmField(1,PWMOutputField::Device),"pwm");assert(peripheralOutputDevice(1)==AcPeripheral::Pwm);
 // Function first, with independently stored thermometer/humidity models.
 peripheralsApplySetting(sensorField(1,SensorField::TemperatureModel),"ds18b20");
 peripheralsApplySetting(sensorField(1,SensorField::Function),"temperature");assert(peripheralInputDevice(1)==AcPeripheral::Ds18b20);
 peripheralsApplySetting(sensorField(1,SensorField::HumidityModel),"dht22");
 peripheralsApplySetting(sensorField(1,SensorField::Function),"humidity");assert(peripheralInputDevice(1)==AcPeripheral::Dht22&&peripheralInputMeasurement(1)==1);
 peripheralsApplySetting(sensorField(1,SensorField::HumidityModel),"ds18b20");assert(peripheralInputDevice(1)==AcPeripheral::Dht22);
 peripheralsApplySetting(sensorField(1,SensorField::Function),"flow");assert(peripheralInputDevice(1)==AcPeripheral::Dht22); // S1 has no pin-change interrupt.
 peripheralsApplySetting(sensorField(5,SensorField::Function),"flow");assert(peripheralInputDevice(5)==AcPeripheral::Flow);
 peripheralsApplySetting(sensorField(5,SensorField::PulsesPerLitre),"1000");assert(peripheralPulsesPerLitre(5)==1000);
 peripheralsApplySetting(sensorField(5,SensorField::PulsesPerLitre),"0");assert(peripheralPulsesPerLitre(5)==1000);
 peripheralsApplySetting(sensorField(1,SensorField::Function),"ph");assert(mySensors[0].calibValue2==14&&mySensors[0].calibRawRead2==1023);mySensors[0].finishReading(512*SENSOR_ADC_SAMPLES);assert(fabs(mySensors[0].Value-14.0*512/1023)<0.001);
 peripheralsApplySetting(sensorField(1,SensorField::Function),"level");sensorsDigitalReading(1,0,1);assert(sensorsPeripheralAlarm(1));sensorsDigitalReading(1,1,1);assert(!sensorsPeripheralAlarm(1));
 peripheralsApplySetting(sensorField(1,SensorField::Function),"leak");sensorsDigitalReading(1,1,1);assert(sensorsPeripheralAlarm(1));sensorsDigitalReading(1,0,1);assert(!sensorsPeripheralAlarm(1));
 // Disabling optional devices preserves assignments but restores normal ADC/PWM.
 peripheralsApplySetting(pwmField(1,PWMOutputField::Device),"servo");
 byte savedFlags=megaFeatures.flags;megaFeatures.flags|=ControllerFeatures::NoPeripherals;
 assert(peripheralInputDevice(1)==AcPeripheral::Analog);
 assert(peripheralOutputDevice(1)==AcPeripheral::Pwm);
 megaFeatures.flags=savedFlags;
 assert(peripheralInputDevice(1)==AcPeripheral::Leak);
 assert(peripheralOutputDevice(1)==AcPeripheral::Servo);
 peripheralsApplySetting(pwmField(1,PWMOutputField::Device),"pwm");
 puts("PASS: peripheral selection, signed digital values, calibration, stale/fault fail-safe, output assignment, disabled inputs and PWM device selection.");
}
