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
      sprintf(buffer,"%02d/%02d  %02d %02d", mymonth, myday, myhour, mymin);
    }
    else {
      sprintf(buffer,"%02d/%02d  %02d:%02d", mymonth, myday, myhour, mymin);
    }
    lcdString(buffer);
  //}

  //Aquarium
  gotoXY(0,4);
  lcdStringX("HOME:");
  //Temperature
  gotoXY(0,5);
  lcdDegree();
  lcdString("C=");

  int tmpInt1;
  if (dhtTemperature==0) {
    sprintf (buffer, "-- ", tmpInt1);    
  }
  else {
    tmpInt1 = round(dhtTemperature);
    sprintf (buffer, "%d ", tmpInt1);
  }
  lcdString(buffer);

  //Humidity
  if (dhtHumidity==0) {
    sprintf (buffer, "Hu=--", tmpInt1);
  }
  else {
    tmpInt1 = round(dhtHumidity);
    sprintf (buffer, "Hu=%d", tmpInt1); 
  }
  lcdString(buffer);
  lcdString ("%");
}


