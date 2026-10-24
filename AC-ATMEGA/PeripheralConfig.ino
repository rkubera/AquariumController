#include "PeripheralDevices.h"
byte peripheralInputDevice(byte channel){
 if(!channel||channel>SENSORS_COUNT)return AcPeripheral::Disabled;
 if(megaFeatures.has(ControllerFeatures::NoPeripherals))return AcPeripheral::Analog;
 byte type=configStorage.read(AcPeripheral::InputConfig+(channel-1)*2);
 return type<=AcPeripheral::Ph?type:byte(AcPeripheral::Analog);
}
byte peripheralInputMeasurement(byte channel){
 return channel&&channel<=SENSORS_COUNT&&AcPeripheral::isDht(peripheralInputDevice(channel))&&configStorage.read(AcPeripheral::InputConfig+(channel-1)*2+1)==1?1:0;
}
byte peripheralOutputDevice(byte channel){return !megaFeatures.has(ControllerFeatures::NoPeripherals)&&channel&&channel<=PWMOUTPUTS_COUNT&&configStorage.read(AcPeripheral::OutputConfig+channel-1)==1?1:0;}
String peripheralDeviceName(byte type){
 switch(type){case AcPeripheral::Dht11:return F("dht11");case AcPeripheral::Dht12:return F("dht12");case AcPeripheral::Dht21:return F("dht21");case AcPeripheral::Dht22:return F("dht22");case AcPeripheral::Ds18b20:return F("ds18b20");case AcPeripheral::Digital:return F("digital");case AcPeripheral::Level:return F("level");case AcPeripheral::Leak:return F("leak");case AcPeripheral::Flow:return F("flow");case AcPeripheral::Ph:return F("ph");case AcPeripheral::Disabled:return F("disabled");default:return F("analog");}
}
bool peripheralsApplySetting(uint16_t field,const String& value){
 if(peripheralsApplyFunction(field,value)||peripheralsApplyOptions(field,value))return true;
 byte channel=sensorFieldChannel(field);
 if(channel&&(field==sensorField(channel,SensorField::Device)||field==sensorField(channel,SensorField::Measurement))){
  if(channel>megaFeatures.inputs)return true;
  if(field==sensorField(channel,SensorField::Device)){
   byte type=255;for(byte i=0;i<=AcPeripheral::Ph;i++)if(value==peripheralDeviceName(i)){type=i;break;}
   if(type==255||(type==AcPeripheral::Flow&&channel<5))return true;
   if(type!=peripheralInputDevice(channel)){
    peripheralsInputChanged(channel);
    configStorage.update(AcPeripheral::InputConfig+(channel-1)*2,type);
    if(!AcPeripheral::isDht(type))configStorage.update(AcPeripheral::InputConfig+(channel-1)*2+1,0);
    sensorsDeviceChanged(channel,megaConfigReady);
    if(type==AcPeripheral::Disabled)sensorsAssignInput(channel,SENSOR_TYPE_NONE);
    if(megaConfigReady){reportField(sensorField(channel,SensorField::Measurement),peripheralInputMeasurement(channel)?"humidity":"temperature");}
   }
  }else{
   if(value!="temperature"&&value!="humidity")return true;
   if(value=="humidity"&&!AcPeripheral::isDht(peripheralInputDevice(channel)))return true;
   byte measurement=value=="humidity";
   if(measurement!=peripheralInputMeasurement(channel)){
    peripheralsInputChanged(channel);configStorage.update(AcPeripheral::InputConfig+(channel-1)*2+1,measurement);
    sensorsDeviceChanged(channel,megaConfigReady);
   }
  }
  reportField(sensorField(channel,SensorField::Device),peripheralDeviceName(peripheralInputDevice(channel)));
  reportField(sensorField(channel,SensorField::Measurement),peripheralInputMeasurement(channel)?"humidity":"temperature");
  return true;
 }
 channel=pwmFieldChannel(field);
 if(channel&&field==pwmField(channel,PWMOutputField::Device)){
  if(channel>megaFeatures.pwm||(value!="pwm"&&value!="servo"))return true;
  configStorage.update(AcPeripheral::OutputConfig+channel-1,value=="servo");
  pwmOutputsRefreshChannel(channel);reportField(field,peripheralOutputDevice(channel)?"servo":"pwm");return true;
 }
 return false;
}

byte peripheralOptions(byte ch){return configStorage.read(AcPeripheral::OptionsConfig+(ch-1)*7)&3;}
uint16_t peripheralDebounce(byte ch){uint16_t v;configStorage.get(AcPeripheral::OptionsConfig+(ch-1)*7+1,v);return v<=5000?v:50;}
float peripheralPulsesPerLitre(byte ch){float v;configStorage.get(AcPeripheral::OptionsConfig+(ch-1)*7+3,v);return isfinite(v)&&v>=1&&v<=1000000?v:450;}
void peripheralsReportSettings(byte ch){
 reportField(sensorField(ch,SensorField::Inverted),peripheralOptions(ch)&1?"on":"off");
 reportField(sensorField(ch,SensorField::Pullup),peripheralOptions(ch)&2?"on":"off");
 reportField(sensorField(ch,SensorField::DebounceMs),intToString(peripheralDebounce(ch)));
 reportField(sensorField(ch,SensorField::PulsesPerLitre),floatToString(peripheralPulsesPerLitre(ch)));
}
bool peripheralsApplyOptions(uint16_t field,const String& value){
 byte ch=sensorFieldChannel(field);if(!ch||ch>megaFeatures.inputs)return false;
 int a=AcPeripheral::OptionsConfig+(ch-1)*7;
 if(field==sensorField(ch,SensorField::Inverted)||field==sensorField(ch,SensorField::Pullup)){
  if(value!="on"&&value!="off")return true;
  byte mask=field==sensorField(ch,SensorField::Inverted)?1:2,flags=peripheralOptions(ch);
  configStorage.update(a,value=="on"?flags|mask:flags&~mask);
 }else if(field==sensorField(ch,SensorField::DebounceMs)){
  float number=stringToFloat(value);if(!isfinite(number)||number<0||number>5000||number!=uint16_t(number))return true;
  configStorage.put(a+1,uint16_t(number));
 }else if(field==sensorField(ch,SensorField::PulsesPerLitre)){
  float number=stringToFloat(value);if(!isfinite(number)||number<1||number>1000000)return true;
  configStorage.put(a+3,number);
 }else return false;
 peripheralsInputChanged(ch);sensorsDeviceChanged(ch,false);peripheralsReportSettings(ch);return true;
}

String peripheralFunctionName(byte ch){
 byte type=peripheralInputDevice(ch);
 if(AcPeripheral::isDht(type))return peripheralInputMeasurement(ch)?F("humidity"):F("temperature");
 if(type==AcPeripheral::Ds18b20)return F("temperature");
 return peripheralDeviceName(type);
}
byte peripheralSavedModel(byte ch,bool humidity){
 byte v=configStorage.read(AcPeripheral::ModelConfig+(ch-1)*2+humidity);
 return v>=AcPeripheral::Dht11&&v<=(humidity?AcPeripheral::Dht22:AcPeripheral::Ds18b20)?v:byte(AcPeripheral::Dht11);
}
void peripheralsReportFunction(byte ch){
 reportField(sensorField(ch,SensorField::Function),peripheralFunctionName(ch));
 reportField(sensorField(ch,SensorField::TemperatureModel),peripheralDeviceName(peripheralSavedModel(ch,false)));
 reportField(sensorField(ch,SensorField::HumidityModel),peripheralDeviceName(peripheralSavedModel(ch,true)));
}
bool peripheralsApplyFunction(uint16_t field,const String& value){
 byte ch=sensorFieldChannel(field);if(!ch||ch>megaFeatures.inputs)return false;
 if(field==sensorField(ch,SensorField::TemperatureModel)||field==sensorField(ch,SensorField::HumidityModel)){
  bool humidity=field==sensorField(ch,SensorField::HumidityModel);byte model=0;
  for(byte type=AcPeripheral::Dht11;type<=AcPeripheral::Ds18b20;type++)if(value==peripheralDeviceName(type))model=type;
  if(!model||(humidity&&model==AcPeripheral::Ds18b20))return true;
  if(peripheralSavedModel(ch,humidity)==model){peripheralsReportFunction(ch);return true;}
  configStorage.update(AcPeripheral::ModelConfig+(ch-1)*2+humidity,model);
  if(peripheralFunctionName(ch)==(humidity?"humidity":"temperature")){
   peripheralsInputChanged(ch);configStorage.update(AcPeripheral::InputConfig+(ch-1)*2,model);sensorsDeviceChanged(ch,false);
  }
 }else if(field==sensorField(ch,SensorField::Function)){
  byte type=255;bool humidity=value=="humidity";
  if(value=="temperature"||humidity)type=peripheralSavedModel(ch,humidity);
  else for(byte t=0;t<=AcPeripheral::Ph;t++)if(value==peripheralDeviceName(t)&&!AcPeripheral::isDigital(t))type=t;
  if(type==255||(type==AcPeripheral::Flow&&ch<5))return true;
  bool changed=value!=peripheralFunctionName(ch);
  if(!changed){peripheralsReportFunction(ch);return true;}
  peripheralsInputChanged(ch);configStorage.update(AcPeripheral::InputConfig+(ch-1)*2,type);
  configStorage.update(AcPeripheral::InputConfig+(ch-1)*2+1,humidity);
  sensorsDeviceChanged(ch,changed&&megaConfigReady);
  if(type==AcPeripheral::Disabled)sensorsAssignInput(ch,SENSOR_TYPE_NONE);
 }else return false;
 peripheralsReportFunction(ch);return true;
}
