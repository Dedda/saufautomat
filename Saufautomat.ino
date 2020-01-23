#include <LiquidCrystal.h>
#include <SPI.h>
#include <SD.h>
#include <avr/power.h>

LiquidCrystal lcd(7, 6, 2, 3, 4, 5);

const String fileName = "bevs";
const int displayTime = 2500;

const int beerPin = 16;
bool beerState = HIGH;
const int shotsPin = 15;
bool shotsState = HIGH;
const int nonAlcPin = 14;
bool nonAlcState = HIGH;
const int longdrinkPin = 8;
bool longdrinkState = HIGH;
const int resetPin = 49;

byte beers = 0;
byte shots = 0;
byte non_alcoholics = 0;
byte longdrinks = 0;

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
  pinMode(beerPin, INPUT_PULLUP);
  pinMode(shotsPin, INPUT_PULLUP);
  pinMode(nonAlcPin, INPUT_PULLUP);
  pinMode(longdrinkPin, INPUT_PULLUP);
  idle();
}

void loop() {
  // put your main code here, to run repeatedly:  
  printBeverage("Beers", beers);
  awaitInput();
  printBeverage("Shots", shots);
  awaitInput();
  printBeverage("Longdrinks", longdrinks);
  awaitInput();
  printBeverage("Non alcoholic", non_alcoholics);
  awaitInput();
}

inline void clearLine() {
  lcd.print("                ");
}

void printBeverage(String name, byte amount) {
  lcd.setCursor(0, 0);
  clearLine();
  lcd.setCursor(0, 0);
  lcd.print(name);
  lcd.setCursor(name.length(), 0);
  lcd.print(":");
  lcd.setCursor(0, 1);
  clearLine();
  lcd.setCursor(0, 1);
  lcd.print(amount);
}

void awaitInput() {
  for (int i = 0; i < 100; i++) {
    bool bPin = digitalRead(beerPin);
    bool sPin = digitalRead(shotsPin);
    bool nPin = digitalRead(nonAlcPin);
    bool lPin = digitalRead(longdrinkPin);
    if (bPin == HIGH && beerState == LOW) {      
      beers++;
      saveProgress();
    }
    if (sPin == HIGH && shotsState == LOW) {
      shots++;
      saveProgress();
    }
    if (nPin == HIGH && nonAlcState == LOW) {
      non_alcoholics++;
      saveProgress();
    }
    if (lPin == HIGH && longdrinkState == LOW) {
      longdrinks++;
      saveProgress();
    }
    beerState = bPin;
    shotsState = sPin;
    nonAlcState = nPin;
    longdrinkState = lPin;
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
    myFile.write(beers);
    myFile.write(shots);
    myFile.write(longdrinks);
    myFile.write(non_alcoholics);
    myFile.flush();
    myFile.close();
  }
  idle();
}

void loadProgress() {
  File myFile = SD.open(fileName);
  if (myFile) {
    myFile.seek(0);
    beers = loadBeverage(myFile);
    shots = loadBeverage(myFile);
    longdrinks = loadBeverage(myFile);
    non_alcoholics = loadBeverage(myFile);   
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