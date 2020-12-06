int fileCounter = 1;
String configFile = "config.txt";
String counterFileNamePrefix = "Alc_";
String counterFileExtension = ".txt";

void initSD() {
  SD.begin(53);
  fileCounter = latestFileCounter();
}

String currentFileName() {
  return fileNameForCounter(fileCounter);
}

String fileNameForCounter(int counter) {
  return counterFileNamePrefix + String(counter) + counterFileExtension;
}

int latestFileCounter() {
  int fileCounter = 2;
  for (;;fileCounter++) {
    if (!SD.exists(fileNameForCounter(fileCounter))) {
      return fileCounter-1;
    }
  }
  return 0;
}

void nextFile() {
  fileCounter++;
}

void saveProgress() {
  busy();
  String fileName = currentFileName();
  SD.remove(fileName);
  File myFile = SD.open(fileName, FILE_WRITE);
  if (myFile) {
    StaticJsonDocument<512> doc;
    for (int beverageId = 0; beverageId < N_BEV_TYPES; beverageId++) {
      Beverage bev = beverages[beverageId];
      doc[bev.printName] = bev.count;
    }
    serializeJsonPretty(doc, myFile);
    myFile.flush();
    myFile.close();
  }
  idle();
}

void loadProgress() {
  busy();
  File myFile = SD.open(currentFileName());
  if (myFile) {
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, myFile);
    if (error) {
      myFile.close();
      idle();
      return;
    }
    for (int bevId = 0; bevId < N_BEV_TYPES; bevId++) {
      beverages[bevId].count = doc[beverages[bevId].printName] | 0;
    }
    myFile.close();
  }
  idle();
}

void loadConfig() {
  busy();
  File file = SD.open(configFile);
  if (file) {
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, file);
    if (error) {
      file.close();
      idle();
      return;
    }
    config->rotationTime = doc["rotTime"];
    config->splashTime = doc["splashTime"];
    config->gitHubAdTime = doc["ghAdTime"];
    config->exportInfoTime = doc["infoTime"];
    config->wowTime = doc["wowTime"];
    file.close();
  }
  idle();
}

void saveConfig() {
  busy();
  SD.remove(configFile);
  File file = SD.open(configFile, FILE_WRITE);
  if (file) {
    StaticJsonDocument<512> doc;
    doc["rotTime"] = config->rotationTime;
    doc["splashTime"] = config->splashTime;
    doc["ghAdTime"] = config->gitHubAdTime;
    doc["infoTime"] = config->exportInfoTime;
    doc["wowTime"] = config->wowTime;
    serializeJsonPretty(doc, file);
    file.flush();
    file.close();
  }
  idle();
}