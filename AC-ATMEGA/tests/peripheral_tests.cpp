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
 // Concrete profiles configure electrical behavior but survive manual overrides.
 peripheralsApplySetting(sensorField(1,SensorField::LeakModel),"grove101020018");
 assert(peripheralProfile(1,AcPeripheral::Leak)==1&&peripheralOptions(1)==1&&peripheralDebounce(1)==50);
 peripheralsApplySetting(sensorField(1,SensorField::Pullup),"on");
 assert(peripheralProfile(1,AcPeripheral::Leak)==1&&peripheralOptions(1)==3);
 peripheralsApplySetting(sensorField(1,SensorField::LeakModel),"grove101020018");assert(peripheralOptions(1)==3);
 peripheralsApplySetting(sensorField(1,SensorField::LevelModel),"sen0204");
 peripheralsApplySetting(sensorField(1,SensorField::Function),"level");assert(peripheralOptions(1)==0);
 peripheralsApplySetting(sensorField(1,SensorField::Inverted),"on");assert(peripheralProfile(1,AcPeripheral::Level)==1&&peripheralProfile(1,AcPeripheral::Leak)==1);
 peripheralsApplySetting(sensorField(1,SensorField::FlowModel),"sen0217");assert(peripheralProfile(1,AcPeripheral::Flow)==0);
 peripheralsApplySetting(sensorField(5,SensorField::FlowModel),"sen0217");assert(peripheralPulsesPerLitre(5)==450&&peripheralOptions(5)==0);
 peripheralsApplySetting(sensorField(5,SensorField::PulsesPerLitre),"470");
 peripheralsApplySetting(sensorField(5,SensorField::FlowModel),"sen0217");assert(peripheralPulsesPerLitre(5)==470);
 peripheralsApplySetting(sensorField(1,SensorField::PhModel),"sen0169v2");
 peripheralsApplySetting(sensorField(1,SensorField::Function),"ph");
 auto& ph=mySensors[0];assert(ph.calibValue1==7&&ph.calibValue2==4);
 ph.finishReading(307*SENSOR_ADC_SAMPLES);assert(ph.isValid()&&fabs(ph.Value-7)<0.01);
 ph.resetReadings();ph.finishReading(416*SENSOR_ADC_SAMPLES);assert(fabs(ph.Value-4)<0.01);
 ph.calibRawRead1=310;ph.calibRawRead2=420;ph.calibValue1=7;ph.calibValue2=4;
 peripheralsApplySetting(sensorField(1,SensorField::PhModel),"sen0161v2");assert(ph.calibRawRead1==310&&ph.calibRawRead2==420);
 // Boot replay stores profiles without overwriting restored electrical/calibration settings.
 megaConfigReady=false;
 peripheralsApplySetting(sensorField(1,SensorField::PhModel),"sen0169v2");assert(ph.calibRawRead1==310);
 peripheralsApplySetting(sensorField(1,SensorField::LeakModel),"generic");
 assert(peripheralProfile(1,AcPeripheral::Ph)==2&&peripheralProfile(1,AcPeripheral::Level)==1);
 megaConfigReady=true;
 peripheralsApplySetting(sensorField(1,SensorField::Function),"leak");
 // Active generic pH adopts nominal points; invalid physical pH fails safe.
 peripheralsApplySetting(sensorField(2,SensorField::Function),"ph");
 peripheralsApplySetting(sensorField(2,SensorField::PhModel),"sen0161v2");
 assert(mySensors[1].calibValue1==7&&mySensors[1].calibValue2==4);
 mySensors[1].finishReading(0);assert(!mySensors[1].isValid());
 mySensors[1].finishReading(1023*SENSOR_ADC_SAMPLES);assert(!mySensors[1].isValid());
 peripheralsApplySetting(sensorField(2,SensorField::PhModel),"sen0204");assert(peripheralProfile(2,AcPeripheral::Ph)==1);
 peripheralsApplySetting(sensorField(2,SensorField::PhModel),"generic");assert(mySensors[1].calibRawRead2==1023&&mySensors[1].calibValue2==14);
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
