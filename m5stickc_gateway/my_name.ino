#define NAME_LENGTH 32

char gMyName[NAME_LENGTH];

char* getMyName() {
  return gMyName;
}

void initMyName() {
  // Redraw screen
  M5.Lcd.fillScreen(TFT_BLACK);
  drawHeader();
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(0, 18);
  M5.Lcd.setTextColor(TFT_WHITE);
  
  Serial.print("\nReading name .. ");
  M5.Lcd.print("Reading name .. ");
  char newName[NAME_LENGTH];
  String stringName = getConfigName();
  stringName.toCharArray(newName, NAME_LENGTH);

  Serial.println("[ok]");
  M5.Lcd.println("[ok]");

  // Print Name
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(TFT_GREEN);
  
  Serial.print("  I am ");
  Serial.println(newName);
  M5.Lcd.println("I am:");
  M5.Lcd.println(newName);

  // Keep in Memory
  strcpy(gMyName, newName);
}
