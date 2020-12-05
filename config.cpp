#include "config.hpp"
#include "HardwareSerial.h"

#define ms Serial.println("ms");

Config::Config() {
    setDefault();
};

void Config::setDefault() {
    rotationTime = 2500;
    splashTime = 1500;
    gitHubAdTime = 1500;
    exportInfoTime = 2000;
    wowTime = 1500;
}

void Config::list() {
    Serial.println("Config:");
    Serial.println("=======");
    
    Serial.print("Rotation time: ");
    Serial.print(rotationTime);
    ms
    Serial.print("Splash screen time: ");
    Serial.print(splashTime);
    ms
    Serial.print("GitHub ad time: ");
    Serial.print(gitHubAdTime);
    ms
    Serial.print("Export file info time: ");
    Serial.print(exportInfoTime);
    ms
    Serial.print("Wow time: ");
    Serial.print(wowTime);
    ms
}
