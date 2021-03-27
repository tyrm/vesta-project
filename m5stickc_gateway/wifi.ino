#include <WiFi.h>

#include "eeprom_locations.h"

void initWifi() {
  // Redraw screen
  drawStarupScreen();

  Serial.print("\nReading Wifi config . ");
  M5.Lcd.print("Reading Wifi config . ");

  String stringData;
  
  // WifiSSID
  char wifiSSID[WIFI_SSID_LENGTH];
  stringData = getWifiSSID();
  stringData.toCharArray(wifiSSID, WIFI_SSID_LENGTH);

  // WifiPass
  char wifiPass[WIFI_PASS_LENGTH];
  stringData = getWifiPass();
  stringData.toCharArray(wifiPass, WIFI_PASS_LENGTH);
  Serial.println("[ok]");
  M5.Lcd.println("[ok]");

  
  Serial.print("  SSID: ");
  M5.Lcd.print("  SSID: ");
  Serial.println(wifiSSID);
  M5.Lcd.println(wifiSSID);

  Serial.print("Connecting ");
  M5.Lcd.print("Connecting ");
  
  WiFi.setHostname(getMyName());
  WiFi.begin(wifiSSID, wifiPass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println(" [ok]");
  M5.Lcd.println(" [ok]");
  
  Serial.print("  IP: ");
  M5.Lcd.print("  IP: ");
  Serial.println(WiFi.localIP());
  M5.Lcd.println(WiFi.localIP());
  
}
