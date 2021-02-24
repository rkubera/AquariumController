/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/
 
void menuInit() {
  
}

void menuShow() {
  static unsigned long lastMenuMillis = millis();
  if (abs(lastMenuMillis-millis())>100) {
    lastMenuMillis = millis();
    FullMenu();
  }
}

void FullMenu() {
  menuStatsuWindow();
}

void menuStatsuWindow() {
  //Date and time
  time_t local = clockGetLocalTime();
  int mymin = minute(local);
  int myhour = hour(local);
  int mymonth = month(local);
  int myday = day(local);

  //if (mymin<60 && myhour<=24 && mymonth<=12 && myday<=31) {
    gotoXY(0,0);
    if ((round(millis()/500)%4)==0) {
      sprintf(bufferOut,"%02d/%02d  %02d %02d", mymonth, myday, myhour, mymin);
    }
    else {
      sprintf(bufferOut,"%02d/%02d  %02d:%02d", mymonth, myday, myhour, mymin);
    }
    lcdString(bufferOut);
  //}
  gotoXY(0,1);
  lcdStringX((char*)"NET:");
  if (wifiStatus == WIFI_STATUS_CONNECTED) {
    String message = "";
    if (mqttStatus == MQTT_STATUS_CONNECTED) {
      message ="(c)";
    }
    else {
      message ="( )";
    }
    message = message+wifi_ssid;

    if (message.length()>12) {
      message = message.substring(0,9)+"...";
    }
    gotoXY(0,2);
    lcdString(message.c_str());

    message = mqttElDeviceName;
    if (message.length()>12) {
      message = message.substring(0,9)+"...";
    }
    gotoXY(0,3);
    lcdString(message.c_str());
  }

  //Aquarium
  gotoXY(0,4);
  lcdStringX((char*)"HOME:");
  //Temperature
  gotoXY(0,5);
  lcdDegree();
  lcdString((char*)"C=");

  int tmpInt1;
  if (dhtTemperature==0) {
    sprintf (bufferOut, "-- ");    
  }
  else {
    tmpInt1 = round(dhtTemperature);
    sprintf (bufferOut, "%d ", tmpInt1);
  }
  lcdString(bufferOut);

  //Humidity
  if (dhtHumidity==0) {
    sprintf (bufferOut, "Hu=--");
  }
  else {
    tmpInt1 = round(dhtHumidity);
    sprintf (bufferOut, "Hu=%d", tmpInt1); 
  }
  lcdString(bufferOut);
  lcdString ((char*)"%");
}
