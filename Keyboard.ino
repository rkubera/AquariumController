/**************************************************
 *                                                *
 * AquaController by Radek Kubera (rkubera)       *
 * all rights reserved                            *
 * free of charge for non-commercial use only     *
 * https://github.com/rkubera/AquariumController  *
 *                                                *
 * ************************************************/
 
void keyboardInit() {
  
}

void keyboardCheck() {
  char key = keypad.getKey();
  if (key) {
    lastKey=key;
  }
}

