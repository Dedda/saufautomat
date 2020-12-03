int fileCounter = 1;

void initSD() {
  SD.begin(53);
  fileCounter = latestFileCounter();
  Serial.print("Last file counter: ");
  Serial.println(fileCounter);
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

byte loadBeverage(File f) {
  if (f.available()) {
    return f.read();
  }
  return 0;
}

void saveProgress() {
  busy();
  String fileName = fileNameForCounter(fileCounter);
  SD.remove(fileNameForCounter(fileCounter));
  File myFile = SD.open(fileName, FILE_WRITE);
  Serial.println("store, File: " + fileName);
  if (myFile) {
    Serial.println("yupes");
    myFile.seek(0);
    StaticJsonDocument<1024> doc;
    for (int beverageId = BEER; beverageId <= NON_ALCOHOL; beverageId++) {
      Beverage bev = beverages[beverageId];
      doc[bev.printName] = bev.count;
    }
    serializeJson(doc, myFile);
    myFile.flush();
    myFile.close();
  }
  idle();
}

void loadProgress() {
  Serial.println("read, File: " + fileNameForCounter(fileCounter));
  File myFile = SD.open(fileNameForCounter(fileCounter));
  if (myFile) {
    Serial.println("yuper");
    myFile.seek(0);
    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, myFile);
    if (error) {
      return;
    }
    for (int bevId = BEER; bevId <= NON_ALCOHOL; bevId++) {
      beverages[bevId].count = doc[beverages[bevId].printName] | 0;
    }
    myFile.close();
  }
}
