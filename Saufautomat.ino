#include <ArduinoJson.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <SD.h>
#include <avr/power.h>

LiquidCrystal lcd(7, 6, 2, 3, 4, 5);

enum beverageType {
  BEER = 0,
  SHOT,
  LONGDRINK,
  NON_ALCOHOL
};

class Beverage {
  public:
    int pin;
    bool state;
    String printName;
    int count;
    int type;
    Beverage(int type, String label, int pin);
};

Beverage::Beverage(int type, String label, int pin) {
  this->pin = pin;
  this->type = type;
  this->printName = label;
  this->state = HIGH;
  this->count = 0;
}

const int N_BEV_TYPES = NON_ALCOHOL + 1;

const int resetPin = 49;

const int displayTime = 2500;

Beverage beverages[N_BEV_TYPES] = {
  Beverage(BEER, "Beers", 16),
  Beverage(SHOT, "Shots", 15),
  Beverage(LONGDRINK, "Longdrinks", 14),
  Beverage(NON_ALCOHOL, "Non alcoholic", 8)
};

int currentBeverage = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
  // enable_power_saver();

  pinMode(9, OUTPUT);
  busy();
  pinMode(resetPin, INPUT_PULLUP);
  lcd.begin(16, 2);
  bootAnimation();
  initSD();
  checkReset();
  loadProgress();
  exportFileNameDisclaimer();
  pinMode(beverages[BEER].pin, INPUT_PULLUP);
  pinMode(beverages[SHOT].pin, INPUT_PULLUP);
  pinMode(beverages[LONGDRINK].pin, INPUT_PULLUP);
  pinMode(beverages[NON_ALCOHOL].pin, INPUT_PULLUP);
  idle();
}

void loop() {
  currentBeverage++;
  if (currentBeverage == N_BEV_TYPES) {
    currentBeverage = 0;
  }
  printBeverage(currentBeverage);
  awaitInput();
}

void busy() {
  digitalWrite(9, HIGH);
}

void idle() {
  digitalWrite(9, LOW);
}
