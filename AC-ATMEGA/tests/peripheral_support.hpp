#include "PeripheralDevices.h"
void peripheralWritePwm(byte,byte pin,uint16_t value,bool healthy){pwmWrite16(pin,healthy?value:0);}
void peripheralsInputChanged(byte){}
void peripheralsDisable(){}
String peripheralHumidityString(byte){return "invalid";}
bool peripheralsBusBusy(){return false;}
void dhtSelectPin(byte){}
