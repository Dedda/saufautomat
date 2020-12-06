void enable_power_saver() {

    power_adc_disable();
    power_usart0_disable();
    #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    power_usart2_disable();
    power_timer3_disable();
    power_timer4_disable();
    power_timer5_disable();
    #endif
    power_timer1_disable();
    power_timer2_disable();
    power_twi_disable();

    // turn off on board LED to save some power
    turnOffDigitalPins();
    turnOffAnalogPins();

}


void turnOffAnalogPins() {
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(A6, OUTPUT);
  pinMode(A7, OUTPUT);
  #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  pinMode(A8, OUTPUT);
  pinMode(A9, OUTPUT);
  pinMode(A10, OUTPUT);
  pinMode(A11, OUTPUT);
  pinMode(A12, OUTPUT);
  pinMode(A13, OUTPUT);
  pinMode(A14, OUTPUT);
  pinMode(A15, OUTPUT);
  #endif
  
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
  digitalWrite(A4, LOW);
  digitalWrite(A5, LOW);
  digitalWrite(A6, LOW);
  digitalWrite(A7, LOW);
  #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  digitalWrite(A8, LOW);
  digitalWrite(A9, LOW);
  digitalWrite(A10, LOW);
  digitalWrite(A11, LOW);
  digitalWrite(A12, LOW);
  digitalWrite(A13, LOW);
  digitalWrite(A14, LOW);
  digitalWrite(A15, LOW);
  #endif
}

void turnOffDigitalPins() {
  for (int i = 0; i < 54; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
}
