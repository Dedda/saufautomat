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
    for (int beverageId = BEER; beverageId <= NON_ALCOHOL; beverageId++) {
      Beverage bev = beverages[beverageId];
      myFile.write(bev.count);
    }
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
    beverages[BEER].count = loadBeverage(myFile);
    beverages[SHOT].count = loadBeverage(myFile);
    beverages[LONGDRINK].count = loadBeverage(myFile);
    beverages[NON_ALCOHOL].count = loadBeverage(myFile);
    myFile.close();
  }
}
