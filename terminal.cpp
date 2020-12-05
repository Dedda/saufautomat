#include "terminal.hpp";
#include "config.hpp"
#include "HardwareSerial.h"
#include <SD.h>

extern Config *config;

String readLine();
String formatFileOrDirectory(File);

// Config commands:
void runConfigCommand(String);
void configList();
void configSet(String);
void configSetRotationTime(String);
void configSetSplashTime(String);
void configSetGhAdTime(String);
void configSetExportInfoTime(String);
void configSetWowTime(String);

void saveConfig();

Terminal::Terminal() {
    _cwd = "/";
}

void Terminal::run() {
    Serial.setTimeout(60*1000);
    while (true)
    {
        Serial.println(_cwd + "> ");
        String command = readLine();
        if (command.equals("exit")) {
            Serial.println("Bye!");
            break;
        } else if (command.equals("ls")) {
            _ls("");
        } else if (command.startsWith("ls ")) {
            _ls(command.substring(3));
        } else if (command.startsWith("cd ")) {
            _cd(command.substring(3));
        } else if (command.startsWith("rm ")) {
            _rm(command.substring(3));
        } else if (command.startsWith("mkdir ")) {
            _mkdir(command.substring(6));
        } else if (command.startsWith("cat ")) {
            _cat(command.substring(4));
        } else if (command.startsWith("config")) {
            runConfigCommand(command.substring(6));
        }
    }
    
}

String Terminal::_path(String file) {
    if (file[0] != '/') {
        file = _cwd + file;
    }
    return file;
}

void Terminal::_cd(String dir) {
    dir = _path(dir);
    File file = SD.open(dir);
    if (file) {
        if (file.isDirectory()) {
            _cwd = dir;
        } else {
            Serial.println(dir + " is not a directory");
        }
    } else {
        Serial.println(dir + " does not exist");
    }
}

void Terminal::_ls(String path) {
    path = _path(path);
    File file = SD.open(path);
    if (file) {
        Serial.println(path + ":");
        if (file.isDirectory()) {
            while (true) {
                File entry = file.openNextFile();
                if (!entry) {
                    break;
                }
                Serial.println(formatFileOrDirectory(entry));
                entry.close();
            }
        } else {
            Serial.println("is file");
            Serial.println("  " + path);
        }
        file.close();
    }
    Serial.flush();
}

void Terminal::_rm(String path) {
    path = _path(path);
    File file = SD.open(path);
    if (file) {
        if (file.isDirectory()) {
            SD.rmdir(path);
        } else {
            SD.remove(path);
        }
        file.close();
    }
}

void Terminal::_mkdir(String path) {
    path = _path(path);
    SD.mkdir(path);
}

void Terminal::_cat(String path) {
    path = _path(path);
    File file = SD.open(path);
    if (file) {
        if (!file.isDirectory()) {
            while (file.available()) {
                Serial.write(file.read());
            }
            Serial.print('\n');
        } else {
            Serial.println("Cannot cat directory!");
        }
        file.close();
    } else {
        Serial.println(path + " does not exist");
    }
}

String formatFileOrDirectory(File f) {
    if (f) {
        String name = String(f.name());
        if (f.isDirectory()) {
            return "d " + name;
        } else {
            return "  " + name;
        }
    }
}

String readLine() {
    String line = "";
    char c;
    while (true) {
        if (Serial.available()) {
            c = Serial.read();
            if (c == '\n') {
                break;
            }
            line += c;
        }
        delay(3);
    };
    return line;
}

void runConfigCommand(String command) {
    if (command.length() == 0) {
        Serial.println("Config commands:");
        Serial.println("---------------");
        Serial.println("  list                   show the current configuration");
        Serial.println("  set default            reset all config values to default");
        Serial.println("  set rotTime [0-..]     set speed for beverage rotation");
        Serial.println("  set splashTime [0-..]  set time for splash screen");
        Serial.println("  set ghAdTime [0..]     set time for GitHub commercial break");
        Serial.println("  set infoTime [0..]     set time for info text about export file");
        Serial.println("  set wowTime [0..]      set time for congratulations on finished drinks");
    } else if (command.equals(" list")) {
        configList();
    } else if(command.startsWith(" set ")) {
        configSet(command.substring(5));
    }
}

void configList() {
    config->list();
}

void configSet(String command) {
    if (command.equals("default")) {
        config->setDefault();
        saveConfig();
    } else if (command.startsWith("rotTime ")) {
        configSetRotationTime(command.substring(8));
    } else if (command.startsWith("splashTime ")) {
        configSetSplashTime(command.substring(11));
    } else if (command.startsWith("ghAdTime ")) {
        configSetGhAdTime(command.substring(9));
    } else if (command.startsWith("infoTime ")) {
        configSetExportInfoTime(command.substring(9));
    } else if (command.startsWith("wowTime ")) {
        configSetWowTime(command.substring(8));
    }
}

bool isNumeric(String s) {
    for (int i = 0; i < s.length(); i++) {
        if (!isDigit(s[i])) {
            return false;
        }
    }
    return true;
}

void configSetRotationTime(String amount) {
    if (isNumeric(amount)) {
        unsigned int number = amount.toInt();
        if (number >= 500) {
            config->rotationTime = number;
            saveConfig();
        }
    }
}

void configSetSplashTime(String amount) {
    if (isNumeric(amount)) {
        unsigned int number = amount.toInt();
        config->splashTime = number;
        saveConfig();
    }
}

void configSetGhAdTime(String amount) {
    if (isNumeric(amount)) {
        unsigned int number = amount.toInt();
        config->gitHubAdTime = number;
        saveConfig();
    }
}

void configSetExportInfoTime(String amount) {
    if (isNumeric(amount)) {
        unsigned int number = amount.toInt();
        config->exportInfoTime = number;
        saveConfig();
    }
}

void configSetWowTime(String amount) {
    if (isNumeric(amount)) {
        unsigned int number = amount.toInt();
        config->wowTime = number;
        saveConfig();
    }
}