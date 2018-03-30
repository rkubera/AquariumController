void pumpsInit () {
  pinMode(PUMP_PIN_1, OUTPUT);
  pinMode(PUMP_PIN_2, OUTPUT);

  analogWrite (PUMP_PIN_1, 0);
  analogWrite (PUMP_PIN_2, 0);
}


