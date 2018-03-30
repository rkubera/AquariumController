void keyboardInit() {
  
}

void keyboardCheck() {
  char key = keypad.getKey();
  if (key) {
    lastKey=key;
  }
}


