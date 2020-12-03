void slowPrint(String line) {
  for (int i = 0; i < line.length(); i++) {
      lcd.print(line[i]);
      delay(50);
  }
}

void bootAnimation() {
  lcd.setCursor(0, 0);
  slowPrint("Saufautomat");
  lcd.setCursor(0, 1);
  slowPrint("v1.0");
  delay(1500);
  lcd.setCursor(0, 0);
  slowPrint("Follow me on");
  lcd.setCursor(0, 1);
  slowPrint("Github: @Dedda");
  delay(1500);
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

void printBeverage(int bev) {
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
  clearScreen();
  lcd.setCursor(0, 0);
  lcd.print("Export file:");
  lcd.setCursor(0, 1);
  lcd.print(currentFileName());
  delay(2000);
}

void newCountDisclaimer(int beverage) {
  Beverage bev = beverages[beverage];
  clearScreen();
  lcd.setCursor(0, 0);
  lcd.print(bev.printName + ":");
  lcd.setCursor(0, 1);
  lcd.print(bev.count);
  lcd.setCursor(11, 1);
  lcd.print("WOW!");
  delay(1500);
}
