int fileCounter = 1;

void initSD() {
  SD.begin(53);
  fileCounter = latestFileCounter();
}

String currentFileName() {
  return fileNameForCounter(fileCounter);
}

String fileNameForCounter(int counter) {
  return "Alc_" + String(counter) + ".txt";
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
    StaticJsonDocument<1024> doc;
    for (int beverageId = 0; beverageId <= N_BEV_TYPES; beverageId++) {
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
    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, myFile);
    if (error) {
      return;
    }
    for (int bevId = 0; bevId <= N_BEV_TYPES; bevId++) {
      beverages[bevId].count = doc[beverages[bevId].printName] | 0;
    }
    myFile.close();
  }
  idle();
}
