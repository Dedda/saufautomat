#include <ArduinoJson.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <SD.h>
#include <avr/power.h>
#include "config.hpp"
#include "terminal.hpp"

LiquidCrystal lcd(7, 6, 2, 3, 4, 5);
Config *config = new Config();

const byte iconBeer[] = {
  B00000,
  B11100,
  B11110,
  B11101,
  B11101,
  B11101,
  B11110,
  B11100,
};

const byte iconShot[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B01110,
  B01110,
  B01110,
  B01110,
};

const byte iconLongdrink[] = {
  B00000,
  B01110,
  B01110,
  B01110,
  B01110,
  B01110,
  B01110,
  B01110,
};

const byte iconCocktail[] = {
  B00000,
  B11111,
  B01110,
  B00100,
  B00100,
  B00100,
  B00100,
  B11111,
};

const byte iconNonAlcohol[] = {
  B01110,
  B10011,
  B10011,
  B10101,
  B10101,
  B11001,
  B11001,
  B01110,
};

enum beverageType: byte {
  BEER = 0,
  SHOT,
  LONGDRINK,
  COCKTAIL,
  NON_ALCOHOL
};

class Beverage {
  public:
    byte pin;
    bool state;
    String printName;
    byte count;
    byte type;
    Beverage(byte type, String label, byte pin);
};

Beverage::Beverage(byte type, String label, byte pin) {
  this->pin = pin;
  this->type = type;
  this->printName = label;
  this->state = HIGH;
  this->count = 0;
}

const byte N_BEV_TYPES = NON_ALCOHOL + 1;
const byte resetPin = 49;
const byte terminalPin = 10;

Beverage beverages[N_BEV_TYPES] = {
  Beverage(BEER, "Beers", 16),
  Beverage(SHOT, "Shots", 15),
  Beverage(LONGDRINK, "Longdrinks", 14),
  Beverage(COCKTAIL, "Cocktails", 17),
  Beverage(NON_ALCOHOL, "Non alcoholic", 8)
};

byte currentBeverage = 0;

void setup() {

  pinMode(terminalPin, INPUT_PULLUP);
  bool maintenanceMode = digitalRead(terminalPin) == LOW;
  if (maintenanceMode) {
    lcd.begin(16, 2);
    printLoadingBar("Enable Serial", 0);
    Serial.begin(9600);
    while (!Serial) {}
  } else {
    Serial.println("Skipping debug terminal...");
    enable_power_saver();
    lcd.begin(16, 2);
  }
  lcd.createChar(BEER, iconBeer);
  lcd.createChar(SHOT, iconShot);
  lcd.createChar(LONGDRINK, iconLongdrink);
  lcd.createChar(COCKTAIL, iconCocktail);
  lcd.createChar(NON_ALCOHOL, iconNonAlcohol);
  printLoadingBar("Initialize SD", 30);
  initSD();
  loadConfig();
  if (maintenanceMode) {
    clearScreen();
    lcd.setCursor(0, 0);
    lcd.print("Maintenance Mode");    
    Terminal *terminal = new Terminal();
    terminal->run();
    delete terminal;
  }
  printLoadingBar("Load Progress", 60);
  pinMode(resetPin, INPUT_PULLUP);
  checkReset();
  loadProgress();
  printLoadingBar("Setting Pins", 90);
  for (byte i = 0; i < N_BEV_TYPES; i++) {
    pinMode(beverages[i].pin, INPUT_PULLUP);
  }
  bootAnimation();
  exportFileNameDisclaimer();
}

void loop() {
  currentBeverage++;
  if (currentBeverage == N_BEV_TYPES) {
    currentBeverage = 0;
  }
  printBeverage(currentBeverage);
  awaitInput();
}
