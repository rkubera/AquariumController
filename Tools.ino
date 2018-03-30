void(* resetController) (void) = 0;

#define CONTROL_MODE_MANUAL             0
#define CONTROL_MODE_PART_OF_DAY        1
#define CONTROL_MODE_TEMPERATURE        2
#define CONTROL_MODE_PH                 3
#define CONTROL_MODE_AQUAWATERLEVEL     4
#define CONTROL_MODE_TANKWATERLEVEL     5
#define CONTROL_MODE_PHSENSOR1          6
#define CONTROL_MODE_PHSENSOR2          7
#define CONTROL_MODE_THERMOMETER1       8
#define CONTROL_MODE_THERMOMETER2       9
#define CONTROL_MODE_WATERLEVELSENSOR1  10
#define CONTROL_MODE_WATERLEVELSENSOR2  11
#define CONTROL_MODE_WATERLEVELSENSOR3  12
#define CONTROL_MODE_WATERLEVELSENSOR4  13

String getStringControlModeFromValue(byte Value) {
  switch (Value) {
    case CONTROL_MODE_MANUAL:
        return setBufferFromFlash(charManual);
        break;
    case CONTROL_MODE_PART_OF_DAY:
        return setBufferFromFlash(charPartofday);
        break;
    case CONTROL_MODE_PH:
        return setBufferFromFlash(charPh);
        break;
    case CONTROL_MODE_TEMPERATURE:
        return setBufferFromFlash(charTemperature);
        break;
    case CONTROL_MODE_AQUAWATERLEVEL:
        return setBufferFromFlash(charAquawaterlevel);
        break;
    case CONTROL_MODE_TANKWATERLEVEL:
        return setBufferFromFlash(charTankwaterlevel);
        break;
    case CONTROL_MODE_PHSENSOR1:
        return setBufferFromFlash(charPhsensor1);
        break;
    case CONTROL_MODE_PHSENSOR2:
        return setBufferFromFlash(charPhsensor2);
        break;
    case CONTROL_MODE_THERMOMETER1:
        return setBufferFromFlash(charThermometer1);
        break;
    case CONTROL_MODE_THERMOMETER2:
        return setBufferFromFlash(charThermometer2);
        break;
    case CONTROL_MODE_WATERLEVELSENSOR1:
        return setBufferFromFlash(charWaterlevelsensor1);
        break;
    case CONTROL_MODE_WATERLEVELSENSOR2:
        return setBufferFromFlash(charWaterlevelsensor2);
        break;
    case CONTROL_MODE_WATERLEVELSENSOR3:
        return setBufferFromFlash(charWaterlevelsensor3);
        break;
    case CONTROL_MODE_WATERLEVELSENSOR4:
        return setBufferFromFlash(charWaterlevelsensor4);
        break;
  }
  return "";
}

String setBufferFromEeprom(int addr, int maxSize) {
  int index;
  for (index = 0; index < maxSize; index++) {
    buffer[index] = EEPROM.read(addr+index);
    if (buffer[index]==0) {
      return (String) buffer;
    } 
  }
  return (String) buffer;
}

String setBufferFromFlash(const char * what) {
  char z;
  int index;
  for (index = 0; index < bufferSize; index++) {
    z=pgm_read_byte(&(what[index]));
    buffer[index] = z;
    if (z==0) {
      return (String) buffer;
    } 
  }
  return (String) buffer;
}

int parseYearFromString(String dateString) {
  int endIndex = dateString.indexOf('/');
  String temp = dateString.substring(0,endIndex);
  return temp.toInt();
}

byte parseMonthFromString(String dateString) {
  int endIndex = dateString.indexOf('/');
  String temp = dateString.substring(endIndex+1);
  endIndex = temp.indexOf('/');
  temp = temp.substring(0,endIndex);
  return temp.toInt();
}

byte parseDayFromString(String dateString) {
  int endIndex = dateString.indexOf('/');
  String temp = dateString.substring(endIndex+1);
  endIndex = temp.indexOf('/');
  temp = temp.substring(endIndex+1);
  return temp.toInt();
}

byte parseHourFromString(String dateString) {
  int endIndex = dateString.indexOf(':');
  String temp = dateString.substring(0,endIndex);
  return temp.toInt();
}

byte parseMinuteFromString(String dateString) {
  int endIndex = dateString.indexOf(':');
  String temp = dateString.substring(endIndex+1);
  return temp.toInt();
}

String floatToString (float value) {
  int tmpInt1 = value;
  float tmpFrac = value - tmpInt1;
  int tmpInt2 = round(tmpFrac * 100);
  
  value = (float)tmpInt1+((float)tmpInt2/100);
  sprintf (buffer, "%d.%02d", tmpInt1, tmpInt2); 
  return (String) buffer;   
}

float stringToFloat(String str) {
  return str.toFloat();
}

int stringToInt(String str) {
  return str.toInt();
}

String intToString (int value) {
  sprintf (buffer, "%d", value);
  return (String) buffer;
}


