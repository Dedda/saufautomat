void checkReset() {
  if (digitalRead(BTN_RESET) == LOW) {
    clearScreen();
    while (digitalRead(BTN_RESET) == LOW) {}
    lcd.setCursor(0, 0);
    lcd.print("Reset? Hold");
    lcd.setCursor(0, 1);
    lcd.print("reset (2s)");
    bool abort = false;
    for (byte i = 0; i < 80; i++) {
      delay(25);
      if (digitalRead(BTN_RESET) == LOW) {
        for (byte i = 0; i < 80; i++) {
          delay(25);
          if (digitalRead(BTN_RESET) == HIGH) {
            abort = true;
            break;
          }
        }
        if (!abort) {
          nextFile();
          abort = true;
        }
      }
      if (abort) {
        break;
      }
    }
  }
}

void awaitInput() {
  for (int i = 0; i < (config->rotationTime) / 25; i++) {
    bool pins[N_BEV_TYPES];
    for (byte i = 0; i < N_BEV_TYPES; i++) {
      pins[i] = digitalRead(beverages[i].pin);
    }
    bool save = false;
    for (byte i = 0; i < N_BEV_TYPES; i++) {
      if (pins[i] == HIGH && beverages[i].state == LOW) {
        beverages[i].count++;
        newCountDisclaimer(i);
        save = true;
      }
      beverages[i].state = pins[i];
    }

    if (save) {
      saveProgress();
      break;
    }
    delay(25);
  }
}
