void testAcquisition() {
  megaFeatures={0,0,0,8};megaConfigReady=true;
  for(byte i=0;i<SENSORS_COUNT;++i){
    auto& s=mySensors[i];s.Init(i);s.sensorType=i%2?0:RELAYS_COUNT;
    s.calibRawRead1=0;s.calibRawRead2=1023;s.calibValue1=0;s.calibValue2=1;
    s.minValue=s.criticalMinValue=-FLT_MAX;s.maxValue=s.criticalMaxValue=FLT_MAX;
    s.numReadings=SENSOR_FILTER_WINDOW;adc[s.analogPin]=123+i;
  }
  ticks=1000;sensorsResetAcquisition();
  auto drive=[](){for(unsigned i=0;i<200;++i){unsigned before=adcReadCalls;sensorsMillisEvent();assert(adcReadCalls-before<=1);}};
  unsigned before=adcReadCalls;drive();assert(adcReadCalls-before==SENSORS_COUNT*(SENSOR_ADC_SAMPLES+1));
  for(byte i=0;i<8;++i){assert(mySensors[i].isValid());assert(mySensors[i].readingCount==1);assert(std::fabs(mySensors[i].rawValue-(123+i))<0.001);}
  // Preserve fractional calibration for the regulator, not rounded tenths.
  assert(mySensors[0].Value>0.12&&mySensors[0].Value<0.121);
  before=adcReadCalls;ticks+=ANALOG_CONTROL_INTERVAL_MS-1;drive();assert(adcReadCalls==before);
  ++ticks;drive();assert(adcReadCalls-before==8*(SENSOR_ADC_SAMPLES+1));assert(mySensors[0].readingCount==2);
  adc[mySensors[0].analogPin]=1000;
  for(unsigned round=0;round<3;++round){ticks+=ANALOG_CONTROL_INTERVAL_MS;drive();}
  assert(mySensors[0].readingCount==3&&std::fabs(mySensors[0].rawValue-1000)<0.001);
  // Fresh critical violations bypass the averaging filter.
  mySensors[0].criticalMaxValue=0.5;ticks+=ANALOG_CONTROL_INTERVAL_MS;drive();assert(!mySensors[0].isValid());
  megaFeatures.inputs=2;mySensors[0].sensorType=SENSOR_TYPE_NONE;mySensors[1].sensorType=255;
  ticks+=ANALOG_CONTROL_INTERVAL_MS;before=adcReadCalls;drive();assert(adcReadCalls==before&&!mySensors[1].isValid());
  megaConfigReady=false;sensorsResetAcquisition();drive();assert(adcReadCalls==before);
  megaConfigReady=true;megaFeatures.inputs=1;mySensors[0].sensorType=RELAYS_COUNT;mySensors[0].criticalMaxValue=FLT_MAX;
  ticks=0xfffffff0UL;sensorsResetAcquisition();drive();assert(mySensors[0].isValid());
  before=adcReadCalls;ticks=0x54;drive();assert(adcReadCalls-before==SENSOR_ADC_SAMPLES+1); // 100 ms across uint32 rollover.
  ticks+=SENSOR_MAX_AGE_MILLIS+1;assert(!mySensors[0].isValid());
  // No normal telemetry burst on fast changes or invalid/valid transitions.
  auto& sensor=mySensors[0];ticks=10000;sensor.lastValueSendMillis=9000;sensor.validityPublished=false;
  sensor.valid=true;sensor.lastReadingMillis=ticks;sensor.Value=0.123;sensor.rawValue=123;
  publications.clear();sensor.publishValidity();assert(publications.size()==2);
  sensor.Value=0.456;sensor.rawValue=456;ticks+=100;sensor.publishValidity();assert(publications.size()==2);
  sensor.resetReadings();assert(publications.size()==2);ticks=10999;sensor.publishValidity();assert(publications.size()==2);
  ticks=11000;sensor.publishValidity();assert(publications.size()==4);assert(publications.back().payload=="invalid");
  sensor.valid=true;sensor.Value=0.75;sensor.rawValue=750;sensor.lastReadingMillis=ticks;sensor.publishValidity();assert(publications.size()==4);
  ticks=12000;sensor.lastReadingMillis=ticks;sensor.publishValidity();assert(publications.size()==6);sensor.publishValidity();assert(publications.size()==6);
  puts("PASS: input telemetry is limited to 1 second, including invalid/valid transitions.");
  puts("PASS: 100 ms acquisition for eight inputs, one ADC operation per service, channel discard, full calibration precision, 3-result filter, critical limits, unassigned/disabled inputs and millis rollover.");
}
