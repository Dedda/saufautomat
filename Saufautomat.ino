#include <LiquidCrystal.h>
#include <SPI.h>
#include <SD.h>
#include <avr/power.h>

LiquidCrystal lcd(7, 6, 2, 3, 4, 5);

const int N_BEV_TYPES = 4;

struct Beverage {
  int pin;
  bool state;
  String printName;
  byte count;
};

enum beverage{
  BEER,
  SHOT,
  LONGDRINK,
  NON_ALCOHOL
};

const String fileName = "bevs";
const int displayTime = 2500;

Beverage beverages[N_BEV_TYPES] = {
  {
    16,
    HIGH,
    "Beers",
    0
  },
  {
    15,
    HIGH,
    "Shots",
    0
  },
  {
    14,
    HIGH,
    "Longdrinks",
    0
  },
  {
    8,
    HIGH,
    "Non alcoholic",
    0
  }
};

const int resetPin = 49;

void setup() {

  power_adc_disable();
  power_usart0_disable();
  power_usart2_disable();
  power_timer1_disable();
  power_timer2_disable();
  power_timer3_disable();
  power_timer4_disable();
  power_timer5_disable();
  power_twi_disable();

  // turn off on board LED to save some power
  turnOffDigitalPins();
  turnOffAnalogPins();

  pinMode(9, OUTPUT);
  busy();
  pinMode(resetPin, INPUT_PULLUP);
  lcd.begin(16, 2);
  bootAnimation();
  initSD();
  checkReset();
  loadProgress();
  pinMode(beverages[BEER].pin, INPUT_PULLUP);
  pinMode(beverages[SHOT].pin, INPUT_PULLUP);
  pinMode(beverages[LONGDRINK].pin, INPUT_PULLUP);
  pinMode(beverages[NON_ALCOHOL].pin, INPUT_PULLUP);
  idle();
}

int currentBeverage = 0;

void loop() {
  // put your main code here, to run repeatedly:
  currentBeverage++;
  if (currentBeverage == N_BEV_TYPES) {
    currentBeverage = 0;
  }
  printBeverage(currentBeverage);
  awaitInput();
}

inline void clearLine() {
  lcd.print("                ");
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

void awaitInput() {
  for (int i = 0; i < 100; i++) {
    bool bPin = digitalRead(beverages[BEER].pin);
    bool sPin = digitalRead(beverages[SHOT].pin);
    bool nPin = digitalRead(beverages[NON_ALCOHOL].pin);
    bool lPin = digitalRead(beverages[LONGDRINK].pin);
    if (bPin == HIGH && beverages[BEER].state == LOW) {      
      beverages[BEER].count++;
      saveProgress();
    }
    if (sPin == HIGH && beverages[SHOT].state == LOW) {
      beverages[SHOT].count++;
      saveProgress();
    }
    if (nPin == HIGH && beverages[NON_ALCOHOL].state == LOW) {
      beverages[NON_ALCOHOL].count++;
      saveProgress();
    }
    if (lPin == HIGH && beverages[LONGDRINK].state == LOW) {
      beverages[LONGDRINK].count++;
      saveProgress();
    }
    beverages[BEER].state = bPin;
    beverages[SHOT].state = sPin;
    beverages[NON_ALCOHOL].state = nPin;
    beverages[LONGDRINK].state = lPin;
    delay(25);
  }
}

void resetProgress() {
  busy();
  SD.remove(fileName);
  idle();
}

void saveProgress() {
  resetProgress();
  busy();
  File myFile = SD.open(fileName, FILE_WRITE);
  if (myFile) {
    myFile.seek(0);
    myFile.write(beverages[BEER].count);
    myFile.write(beverages[SHOT].count);
    myFile.write(beverages[LONGDRINK].count);
    myFile.write(beverages[NON_ALCOHOL].count);
    myFile.flush();
    myFile.close();
  }
  idle();
}

void loadProgress() {
  File myFile = SD.open(fileName);
  if (myFile) {
    myFile.seek(0);
    beverages[BEER].count = loadBeverage(myFile);
    beverages[SHOT].count = loadBeverage(myFile);
    beverages[LONGDRINK].count = loadBeverage(myFile);
    beverages[NON_ALCOHOL].count = loadBeverage(myFile);   
    myFile.close();
  }
}

void busy() {
  digitalWrite(9, HIGH);
}

void idle() {
  digitalWrite(9, LOW);
}

void bootAnimation() {
  lcd.setCursor(0, 0);
  slowPrint("Saufautomat");
  lcd.setCursor(0, 1);
  slowPrint("v0.1");
  delay(1500);
  lcd.setCursor(0, 0);
  slowPrint("Follow me on");
  lcd.setCursor(0, 1);
  slowPrint("Github: @Dedda");
  delay(1500);
}

void slowPrint(String line) {
  for (int i = 0; i < line.length(); i++) {
      lcd.print(line[i]);
      delay(50);
  } 
}

byte loadBeverage(File f) {
  if (f.available()) {
    return f.read();
  }
  return 0;
}

void initSD() {
  SD.begin(53);
}

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
          resetProgress();
          abort = true;
        }
      }
      if (abort) {
        break;
      }
    }
  }
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
  pinMode(A8, OUTPUT);
  pinMode(A9, OUTPUT);
  pinMode(A10, OUTPUT);
  pinMode(A11, OUTPUT);
  pinMode(A12, OUTPUT);
  pinMode(A13, OUTPUT);
  pinMode(A14, OUTPUT);
  pinMode(A15, OUTPUT);
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
  digitalWrite(A4, LOW);
  digitalWrite(A5, LOW);
  digitalWrite(A6, LOW);
  digitalWrite(A7, LOW);
  digitalWrite(A8, LOW);
  digitalWrite(A9, LOW);
  digitalWrite(A10, LOW);
  digitalWrite(A11, LOW);
  digitalWrite(A12, LOW);
  digitalWrite(A13, LOW);
  digitalWrite(A14, LOW);
  digitalWrite(A15, LOW);
}

void turnOffDigitalPins() {
  for (int i = 0; i < 54; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
}
