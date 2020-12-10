byte fileCounter = 1;
const String configFile = "config.txt";
const String counterFileNamePrefix = "Alc_";
const String counterFileExtension = ".txt";

void initSD() {
  SD.begin(53);
  fileCounter = latestFileCounter();
}

String currentFileName() {
  return fileNameForCounter(fileCounter);
}

String fileNameForCounter(byte counter) {
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
  String fileName = currentFileName();
  SD.remove(fileName);
  File myFile = SD.open(fileName, FILE_WRITE);
  if (myFile) {
    StaticJsonDocument<512> doc;
    for (byte beverageId = 0; beverageId < N_BEV_TYPES; beverageId++) {
      Beverage bev = beverages[beverageId];
      doc[bev.printName] = bev.count;
    }
    serializeJsonPretty(doc, myFile);
    myFile.flush();
    myFile.close();
  }
}

void loadProgress() {
  File myFile = SD.open(currentFileName());
  if (myFile) {
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, myFile);
    if (error) {
      myFile.close();
      return;
    }
    for (byte bevId = 0; bevId < N_BEV_TYPES; bevId++) {
      beverages[bevId].count = doc[beverages[bevId].printName] | 0;
    }
    myFile.close();
  }
}

void loadConfig() {
  File file = SD.open(configFile);
  if (file) {
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, file);
    if (error) {
      file.close();
      return;
    }
    config->rotationTime = doc["rotTime"];
    config->splashTime = doc["splashTime"];
    config->gitHubAdTime = doc["ghAdTime"];
    config->exportInfoTime = doc["infoTime"];
    config->wowTime = doc["wowTime"];
    file.close();
  }
}

void saveConfig() {
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
}