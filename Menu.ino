/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/

typedef int (*menuCallbackFunction) (int arg1);
menuCallbackFunction menu_callbacks [7] {menuMainMenu, menuNetwork, menuTime, doAction1, doAction1, doAction1, doAction1};

static int menu_actual_menu = 0;
static int menu_actual_submenu = 0;

void menuInit() {
  
}

void doAction1(int key) {
  if (key) {
    lcdClear();
  }
  gotoXY(0,0);
  lcdStringX((char*)"SCREEN:");
  sprintf(bufferOut,"%02d", menu_actual_menu);
  gotoXY(0,1);
  lcdString(bufferOut);
}

void menuShowCMillis() {
  static unsigned long lastMenuMillis = millis();
  int menuElements = sizeof(menu_callbacks) / sizeof(menu_callbacks[0]);
  if (abs(millis()-lastKeyMillis)<5000) {
    lcdSetBrigtness(1000);
  }
  else {
    double brightness = millis()-lastKeyMillis-5000;
    if (brightness>1000) {
      brightness = 1000;
      lastKeyMillis = 0;
    }
    brightness = 1000-brightness;
    lcdSetBrigtness(brightness);
  }
   
  if (abs(lastMenuMillis-millis())>100) {
    lastMenuMillis = millis();
    //FullMenu();
    if (lastKey) {
      if (lastKey=='L') {
        menu_actual_menu = menu_actual_menu-1;
        if (menu_actual_menu<0) menu_actual_menu = menuElements-1;
        menu_actual_submenu = 0;
      }
      else if (lastKey=='R') {
        menu_actual_menu = menu_actual_menu+1;
        if (menu_actual_menu>=menuElements) menu_actual_menu = 0;
        menu_actual_submenu = 0;
      }
    }
    menu_callbacks[menu_actual_menu](lastKey);
    lastKey = char(0);
  }
}

void menuNetwork(int key) {
  if (key) {
    lcdClear();
  }
  String message;
  gotoXY(0,0);
  message = "NETWORK";
  lcdStringX(message.c_str());

  gotoXY(0,1);
  message = lcdTrimMessage("NAME:"+mqttElDeviceName);
  lcdString(message.c_str());

  gotoXY(0,2);
  message="NET:";
  if (wifiStatus == WIFI_STATUS_CONNECTED) {
    message = lcdTrimMessage( message + "+" + wifi_ssid);
  }
  else message = lcdTrimMessage( message + "-" + wifi_ssid);
  lcdString(message.c_str());

  gotoXY(0,3);
  message = "MQTT:";
  if (mqttStatus == MQTT_STATUS_CONNECTED) {
    message = lcdTrimMessage( message + "+" + mqtt_server);
  }
  else {
    message = lcdTrimMessage( message + "-" + mqtt_server);
  }
  lcdString(message.c_str());
}

void menuTime(int key) {
  if (key) {
    lcdClear();
  }
}

void menuMainMenu(int key) {
  if (key) {
    lcdClear();
  }
  //Date and time
  time_t local = clockGetLocalTime();
  int mymin = minute(local);
  int myhour = hour(local);
  int mymonth = month(local);
  int myday = day(local);

  gotoXY(0,0);
  if ((round(millis()/500)%4)==0) {
    sprintf(bufferOut,"%02d/%02d  %02d %02d", mymonth, myday, myhour, mymin);
  }
  else {
    sprintf(bufferOut,"%02d/%02d  %02d:%02d", mymonth, myday, myhour, mymin);
  }
  lcdString(bufferOut);
  
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
    message = lcdTrimMessage(message+wifi_ssid);
    gotoXY(0,2);
    lcdString(message.c_str());

    message = lcdTrimMessage(mqttElDeviceName);
   
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
