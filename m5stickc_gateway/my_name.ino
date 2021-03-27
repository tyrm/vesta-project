#include "eeprom_locations.h"

char gMyName[NAME_LENGTH];

char* getMyName() {
  return gMyName;
}

void initMyName() {
  // Redraw screen
  drawStarupScreen();
  
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
