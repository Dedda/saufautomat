#include <ArduinoJson.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <SD.h>
#include <avr/power.h>
#include "terminal.hpp"

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
const int terminalPin = 10;

const int displayTime = 2500;

const int BUSY_LED = 9;

Beverage beverages[N_BEV_TYPES] = {
  Beverage(BEER, "Beers", 16),
  Beverage(SHOT, "Shots", 15),
  Beverage(LONGDRINK, "Longdrinks", 14),
  Beverage(NON_ALCOHOL, "Non alcoholic", 8)
};

int currentBeverage = 0;

void setup() {

  pinMode(terminalPin, INPUT_PULLUP);
  if (digitalRead(terminalPin) == LOW) {
    lcd.begin(16, 2);
    printLoadingBar("Enable Serial", 0);
    Serial.begin(9600);
    while (!Serial) {}
    clearScreen();
    lcd.setCursor(0, 0);
    lcd.print("Maintenance Mode");
    Terminal *terminal = new Terminal();
    terminal->run();
  } else {
    Serial.println("Skipping debug terminal...");
    enable_power_saver();
    lcd.begin(16, 2);
  }
  printLoadingBar("Initialize SD", 30);
  // terminal();
  pinMode(9, OUTPUT);
  // busy();
  initSD();
  printLoadingBar("Load Progress", 60);
  pinMode(resetPin, INPUT_PULLUP);
  checkReset();
  loadProgress();
  printLoadingBar("Setting Pins", 90);
  for (int i = 0; i < N_BEV_TYPES; i++) {
    pinMode(beverages[i].pin, INPUT_PULLUP);
  }
  bootAnimation();
  exportFileNameDisclaimer();
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
  digitalWrite(BUSY_LED, HIGH);
}

void idle() {
  digitalWrite(BUSY_LED, LOW);
}
