#define EEPROM_SIZE 512

void initHardware() {
  // Start Serial
  Serial.begin(115200);
  delay(10);

  // Start M5 Hardware
  M5.begin();

  // Configure Screen
  M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(TFT_BLACK);
  drawHeader();
  M5.Lcd.setTextSize(0);
  M5.Lcd.setCursor(0, 18);
  M5.Lcd.setTextColor(TFT_WHITE);
  
  // Start EEPROM
  Serial.print("\nBeginning EEPROM .. ");
  M5.Lcd.print("Beginning EEPROM .. ");
  if (!EEPROM.begin(EEPROM_SIZE)) {
    M5.Lcd.setTextColor(TFT_RED);
    Serial.println("\nFailed to init EEPROM");
    M5.Lcd.println("\nFailed to init EEPROM");
    Serial.println("Restarting...");
    M5.Lcd.println("Restarting...");
    delay(5000);
    ESP.restart();
  }
  Serial.println("[ok]");
  M5.Lcd.println("[ok]");
}
