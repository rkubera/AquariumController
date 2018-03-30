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
  gotoXY(0,2);
  lcdStringX("AQUA:");
  
  //Temperature
  gotoXY(0,3);
  lcdDegree();
  lcdString("C=");
  int tmpInt1 = round(tempAqua);
  if (tempAqua==0) {
    sprintf (buffer, "-- ");
  }
  else {
    sprintf (buffer, "%02d ", tmpInt1);
  }
  lcdString(buffer);
  
  //PH
  tmpInt1 = phAqua;                  // Get the integer (678).
  float tmpFrac = phAqua - tmpInt1;      // Get fraction (0.12).
  int tmpInt2 = trunc(tmpFrac * 10);  // Turn into integer (12)
  if (phAqua==0) {
    sprintf (buffer, "PH=-.-");
  }
  else {
    sprintf (buffer, "PH=%d.%01d", tmpInt1, tmpInt2);
  }
  lcdString(buffer);

  //Aquarium
  gotoXY(0,4);
  lcdStringX("HOME:");
  //Temperature
  gotoXY(0,5);
  lcdDegree();
  lcdString("C=");
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


