#include <LiquidCrystal.h>
#include <SPI.h>
#include <SD.h>

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
  // put your setup code here, to run once:
  pinMode(9, OUTPUT);
  busy();
  pinMode(resetPin, INPUT_PULLUP);
  Serial.begin(9600);
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
  printBeverage("Beer", beers);
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
  String line0 = "Saufautomat";
  String line1 = "v0.1";
  lcd.setCursor(0, 0);
  slowPrint(line0);
  lcd.setCursor(0, 1);
  slowPrint(line1);
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
  if (!SD.begin(53)) {
    Serial.println("Error initializing sd card!");
  }
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
