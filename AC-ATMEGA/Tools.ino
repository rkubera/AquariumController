/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/
 

String getStringControlModeFromValue(byte Value) {
  switch (Value) {
    case CONTROL_MODE_MANUAL:
        return setBufferFromFlash(charManual);
        break;
    case CONTROL_MODE_PART_OF_DAY:
        return setBufferFromFlash(charPartofday);
        break;
     case CONTROL_MODE_TRESHOLD:
        return setBufferFromFlash(charTreshold);
        break;
     case CONTROL_MODE_PID:
        return setBufferFromFlash(charPid);
        break;
  }
  return "";
}

String readConfigName(int addr, int maxSize) {
  int index;
  for (index = 0; index < maxSize; index++) {
    bufferOut[index] = configStorage.read(addr+index);
    if (bufferOut[index]==0) {
      return (String) bufferOut;
    } 
  }
  bufferOut[maxSize] = 0;
  return (String) bufferOut;
}

String setBufferFromFlash(const char * what) {
  char z;
  int index;
  for (index = 0; index < bufferOutSize; index++) {
    z=pgm_read_byte(&(what[index]));
    bufferOut[index] = z;
    if (z==0) {
      return (String) bufferOut;
    } 
  }
  bufferOut[bufferOutSize-1]=0;
  return (String) bufferOut;
}

int parseYearFromString(String value){return value.substring(0,4).toInt();}
byte parseMonthFromString(String value){return value.substring(5,7).toInt();}
byte parseDayFromString(String value){return value.substring(8,10).toInt();}

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

String floatToString(float value) {
  if (!isfinite(value)) return "invalid";
  return String(value,2);
}

String doubleToString(double value) {
  if (!isfinite(value)) return "invalid";
  return String(value,4);
}

float stringToFloat(String str) {
  str.trim();
  char* end;
  double value = strtod(str.c_str(),&end);
  if (end == str.c_str() || *end != '\0' || !isfinite(value)) return NAN;
  return value;
}

double stringToDouble(String str) {
  return stringToFloat(str); //On AVR, double and float are both 32-bit.
}

int stringToInt(String str) {
  return str.toInt();
}

unsigned long stringToLong(String str) {
  return atol(str.c_str());
}

String longToString (unsigned long value) {
  char buff[30];
  snprintf(buff,sizeof(buff),"%lu",value);
  return (String) buff;
}

String intToString (int value) {
  snprintf(bufferOut,bufferOutSize,"%d",value);
  return (String) bufferOut;
}
