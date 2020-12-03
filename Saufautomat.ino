#include <ArduinoJson.h>
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
  BEER = 0,
  SHOT,
  LONGDRINK,
  NON_ALCOHOL
};

const int resetPin = 49;

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

int currentBeverage = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
  // enable_power_saver();

  pinMode(9, OUTPUT);
  busy();
  pinMode(resetPin, INPUT_PULLUP);
  lcd.begin(16, 2);
  // bootAnimation();
  initSD();
  checkReset();
  loadProgress();
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
