void slowPrint(String line) {
  for (byte i = 0; i < line.length(); i++) {
      lcd.print(line[i]);
      delay(50);
  }
}

void bootAnimation() {
  clearScreen();
  if (config->splashTime) {
    lcd.setCursor(0, 0);
    slowPrint("Saufautomat");
    lcd.setCursor(0, 1);
    slowPrint("v1.0");
    delay(config->splashTime);
  }
  if (config->gitHubAdTime) {
    lcd.setCursor(0, 0);
    slowPrint("Follow me on");
    lcd.setCursor(0, 1);
    slowPrint("Github: @Dedda");
    delay(config->gitHubAdTime);
  }
}

inline void clearLine() {
  lcd.print("                ");
}

void clearScreen() {
  lcd.setCursor(0, 0);
  clearLine();
  lcd.setCursor(0, 1);
  clearLine();
}

void printLoadingBar(String label, byte percentage) {
  percentage = min(percentage, 100);
  byte boxes = percentage * 16 / 100;
  clearScreen();
  lcd.setCursor(0, 0);
  lcd.print(label);
  lcd.setCursor(0, 1);
  for (byte i = 0; i < boxes; i++) {
    lcd.print(char(255));
  }
}

void printBeverage(byte bev) {
  lcd.setCursor(0, 0);
  clearLine();
  lcd.setCursor(0, 0);
  lcd.print(beverages[bev].printName);
  lcd.setCursor(beverages[bev].printName.length(), 0);
  lcd.print(":");
  lcd.setCursor(0, 1);
  clearLine();
  lcd.setCursor(0, 1);
  lcd.print(beverages[bev].count);
}

void exportFileNameDisclaimer() {
  if (config->exportInfoTime) {
    clearScreen();
    lcd.setCursor(0, 0);
    lcd.print("Export file:");
    lcd.setCursor(0, 1);
    lcd.print(currentFileName());
    delay(config->exportInfoTime);
  }
}

void newCountDisclaimer(byte beverage) {  
  if (config->wowTime) {
    Beverage bev = beverages[beverage];
    clearScreen();
    lcd.setCursor(0, 0);
    lcd.print(bev.printName + ":");
    lcd.setCursor(0, 1);
    lcd.print(bev.count);
    lcd.setCursor(9, 1);
    lcd.write(beverage);
    lcd.print(" WOW!");
    delay(config->wowTime);
  }
}
