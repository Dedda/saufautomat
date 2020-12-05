#include "config.hpp"
#include "HardwareSerial.h"

#define ms Serial.println("ms");

int rotationWaitTimes[3] = {
    1000,
    1500,
    2000
};

Config::Config() {
    setDefault();
};

void Config::setDefault() {
    rotationSpeed = 1;
    splashTime = 1500;
    gitHubAdTime = 1500;
    exportInfoTime = 2000;
    wowTime = 1500;
}

int Config::rotationWaitTime() {
    return rotationWaitTimes[rotationSpeed];
}

void Config::list() {
    Serial.println("Config:");
    Serial.println("=======");
    
    Serial.print("Rotation speed: ");
    Serial.print(rotationSpeed);
    Serial.print(", ");
    Serial.print(rotationWaitTime());
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
