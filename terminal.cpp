#include "terminal.hpp";
#include "HardwareSerial.h"
#include <SD.h>

String readLine();
String formatFileOrDirectory(File);


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