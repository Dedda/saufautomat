void checkReset() {
  if (digitalRead(resetPin) == LOW) {
    lcd.setCursor(0, 0);
    clearLine();
    lcd.setCursor(0, 1);
    clearLine();
    while (digitalRead(resetPin) == LOW) {}
    lcd.setCursor(0, 0);
    lcd.print("Reset? Hold");
    lcd.setCursor(0, 1);
    lcd.print("reset (2s)");
    bool abort = false;
    for (int i = 0; i < 80; i++) {
      delay(25);
      if (digitalRead(resetPin) == LOW) {
        for (int i = 0; i < 80; i++) {
          delay(25);
          if (digitalRead(resetPin) == HIGH) {
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
  for (int i = 0; i < 100; i++) {
    bool bPin = digitalRead(beverages[BEER].pin);
    bool sPin = digitalRead(beverages[SHOT].pin);
    bool nPin = digitalRead(beverages[NON_ALCOHOL].pin);
    bool lPin = digitalRead(beverages[LONGDRINK].pin);
    bool save = false;
    if (bPin == HIGH && beverages[BEER].state == LOW) {
      beverages[BEER].count++;
      newCountDisclaimer(BEER);
      save = true;
    }
    if (sPin == HIGH && beverages[SHOT].state == LOW) {
      beverages[SHOT].count++;
      newCountDisclaimer(SHOT);
      save = true;
    }
    if (nPin == HIGH && beverages[NON_ALCOHOL].state == LOW) {
      beverages[NON_ALCOHOL].count++;
      newCountDisclaimer(NON_ALCOHOL);
      save = true;
    }
    if (lPin == HIGH && beverages[LONGDRINK].state == LOW) {
      beverages[LONGDRINK].count++;
      newCountDisclaimer(LONGDRINK);
      save = true;
    }
    beverages[BEER].state = bPin;
    beverages[SHOT].state = sPin;
    beverages[NON_ALCOHOL].state = nPin;
    beverages[LONGDRINK].state = lPin;
    if (save) {
      saveProgress();
      break;
    }
    delay(25);
  }
}
