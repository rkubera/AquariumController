/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/
 
void(* resetController) (void) = 0;

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

String setBufferFromEeprom(int addr, int maxSize) {
  int index;
  for (index = 0; index < maxSize; index++) {
    bufferOut[index] = EEPROM.read(addr+index);
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
  sprintf (bufferOut, "%d.%02d", tmpInt1, tmpInt2); 
  return (String) bufferOut;   
}

String doubleToString (double value) {
  int tmpInt1 = value;
  double tmpFrac = value - tmpInt1;
  int tmpInt2 = round(tmpFrac * 10000);
  
  value = (double)tmpInt1+((double)tmpInt2/10000);
  sprintf (bufferOut, "%d.%04d", tmpInt1, tmpInt2); 
  return (String) bufferOut;   
}

float stringToFloat(String str) {
  return str.toFloat();
}

double stringToDouble (String str) {
  return str.toDouble();
}

int stringToInt(String str) {
  return str.toInt();
}

String intToString (int value) {
  sprintf (bufferOut, "%d", value);
  return (String) bufferOut;
}
