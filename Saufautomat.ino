#include <ArduinoJson.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <SD.h>
#include <avr/power.h>
#include "pins.h"
#include "config.hpp"
#include "terminal.hpp"

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
Config *config = new Config();

const byte iconBeer[8] = {
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

Beverage beverages[N_BEV_TYPES] = {
  Beverage(BEER, "Beers", BTN_BEER),
  Beverage(SHOT, "Shots", BTN_SHOTS),
  Beverage(LONGDRINK, "Longdrinks", BTN_LONGDRINK),
  Beverage(COCKTAIL, "Cocktails", BTN_COCKTAIL),
  Beverage(NON_ALCOHOL, "Non alcoholic", BTN_NON_ALCOHOL)
};

byte currentBeverage = 0;

void setup() {
  bool maintenanceMode = maintenanceStartupCheck();  
  createBeverageChars();
  printLoadingBar("Initialize SD", 30);
  initSD();
  loadConfig();
  if (maintenanceMode) {
    runMaintenanceTerminal();
  }
  printLoadingBar("Load Progress", 60);
  pinMode(BTN_RESET, INPUT_PULLUP);
  checkReset();
  loadProgress();
  setupBeveragePins();
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

// Real logic
bool maintenanceStartupCheck() {
  pinMode(BTN_TERMINAL, INPUT_PULLUP);
  bool maintenanceMode = digitalRead(terminalPin) == LOW;
  if (maintenanceMode) {
    lcd.begin(16, 2);
    printLoadingBar("Enable Serial", 0);
    startSerial();
  } else {
    Serial.println("Skipping debug terminal...");
    enable_power_saver();
    lcd.begin(16, 2);
  }
  return maintenanceMode;
}

void startSerial() {
  Serial.begin(9600);
  while (!Serial) {}
}

void createBeverageChars() {
  lcd.createChar(BEER, (uint8_t *) iconBeer);
  lcd.createChar(SHOT, (uint8_t *) iconShot);
  lcd.createChar(LONGDRINK, (uint8_t *) iconLongdrink);
  lcd.createChar(COCKTAIL, (uint8_t *) iconCocktail);
  lcd.createChar(NON_ALCOHOL, (uint8_t *) iconNonAlcohol);
}

void runMaintenanceTerminal() {
  clearScreen();
  lcd.setCursor(0, 0);
  lcd.print("Maintenance Mode");      
  Terminal *terminal = new Terminal();
  terminal->run();
  delete terminal;  
}

void setupBeveragePins() {
  printLoadingBar("Setting Pins", 90);
  for (byte i = 0; i < N_BEV_TYPES; i++) {
    pinMode(beverages[i].pin, INPUT_PULLUP);
  }
}