#include "dump.h"
#include "HardwareSerial.h"
#include <SD.h>

String fileNameForCounter(int counter);

void dumpSerial() {
    int counter = 1;
    while(true) {
        String fileName = fileNameForCounter(counter);
        File file = SD.open(fileName);
        if (file) {
            Serial.println(fileName + "$");
            while (file.available()) {
                Serial.write(file.read());
            }
            Serial.print("%");
        } else {
            break;
        }
        counter++;
    };
    Serial.print("$");
}