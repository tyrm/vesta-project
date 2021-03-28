#include <EEPROM.h>
#include <rom/crc.h>

#include "eeprom_locations.h"

String getConfigName() {
  return EEPROM.readString(DEVICE_NAME_START);
}

String getConfigOtaPassword() {
  return EEPROM.readString(OTA_PASSWORD_START);
}

String getConfigServerPassword() {
  return EEPROM.readString(SERVER_PASSWORD_START);
}

String getConfigServerUsername() {
  return EEPROM.readString(SERVER_USERNAME_START);
}

String getConfigServerURL() {
  return EEPROM.readString(SERVER_URL_START);
}

String getConfigWifiSSID() {
  return EEPROM.readString(WIFI_SSID_START);
}

String getConfigWifiPass() {
  return EEPROM.readString(WIFI_PASS_START);
}

void verifyConfig() {
  char Version[VERSION_LENGTH];
  char DeviceName[DEVICE_NAME_LENGTH];
  uint16_t DeviceType;
  char WifiSSID[WIFI_SSID_LENGTH];
  char WifiPass[WIFI_PASS_LENGTH];
  char ServerURL[SERVER_URL_LENGTH];
  char ServerUsername[SERVER_USERNAME_LENGTH];
  char ServerPassword[SERVER_PASSWORD_LENGTH];
  char OtaPassword[OTA_PASSWORD_LENGTH];

  uint16_t storedCrc;

  String stringData;
  
  // Redraw screen
  drawStarupScreen();

  // Clear Memory
  Serial.print("Clearing Memory .. ");
  M5.Lcd.print("Clearing Memory .. ");

  for (int i = 0; i < VERSION_LENGTH; i++) {
    Version[i] = '\0';
  }
  for (int i = 0; i < DEVICE_NAME_LENGTH; i++) {
    DeviceName[i] = '\0';
  }
  for (int i = 0; i < WIFI_SSID_LENGTH; i++) {
    WifiSSID[i] = '\0';
  }
  for (int i = 0; i < WIFI_PASS_LENGTH; i++) {
    WifiPass[i] = '\0';
  }
  for (int i = 0; i < SERVER_URL_LENGTH; i++) {
    ServerURL[i] = '\0';
  }
  for (int i = 0; i < SERVER_USERNAME_LENGTH; i++) {
    ServerUsername[i] = '\0';
  }
  for (int i = 0; i < SERVER_PASSWORD_LENGTH; i++) {
    ServerPassword[i] = '\0';
  }
  for (int i = 0; i < OTA_PASSWORD_LENGTH; i++) {
    OtaPassword[i] = '\0';
  }
  
  Serial.println("[ok]");
  M5.Lcd.println("[ok]");

  // Read Data into Memory
  Serial.print("Reading EEPROM .. ");
  M5.Lcd.print("Reading EEPROM .. ");

  // Version
  stringData = EEPROM.readString(VERSION_START);
  stringData.toCharArray(Version, VERSION_LENGTH);

  // Name
  stringData = getConfigName();
  stringData.toCharArray(DeviceName, DEVICE_NAME_LENGTH);

  // DeviceType
  DeviceType = EEPROM.readUShort(DEVICE_TYPE_START);

  // WifiSSID
  stringData = getConfigWifiSSID();
  stringData.toCharArray(WifiSSID, WIFI_SSID_LENGTH);

  // WifiPass
  stringData = getConfigWifiPass();
  stringData.toCharArray(WifiPass, WIFI_PASS_LENGTH);

  // ServerURL
  stringData = getConfigServerURL();
  stringData.toCharArray(ServerURL, SERVER_URL_LENGTH);

  // ServerUsername
  stringData = getConfigServerUsername();
  stringData.toCharArray(ServerUsername, SERVER_USERNAME_LENGTH);

  // ServerPassword
  stringData = getConfigServerPassword();
  stringData.toCharArray(ServerPassword, SERVER_PASSWORD_LENGTH);

  // OtaPassword
  stringData = getConfigOtaPassword();
  stringData.toCharArray(OtaPassword, OTA_PASSWORD_LENGTH);

  // Stored CRC
  storedCrc = EEPROM.readUShort(CRC_START);

  Serial.println("[ok]");
  M5.Lcd.println("[ok]");
  
  // Calculate CRC
  Serial.print("Calculating CRC .. ");
  M5.Lcd.print("Calculating CRC .. ");
  
  uint16_t crc = ~0x0000;

  uint8_t* crcData = (uint8_t*)Version;
  crc = crc16_be(crc, crcData, VERSION_LENGTH);

  crcData = (uint8_t*)DeviceName;
  crc = crc16_be(crc, crcData, DEVICE_NAME_LENGTH);
  
  byte low = DeviceType;
  byte high = DeviceType >> 8;
  uint8_t deviceTypeBytes[] = {high, low};
  crc = crc16_be(crc, deviceTypeBytes, DEVICE_TYPE_LENGTH);

  crcData = (uint8_t*)WifiSSID;
  crc = crc16_be(crc, crcData, WIFI_SSID_LENGTH);

  crcData = (uint8_t*)WifiPass;
  crc = crc16_be(crc, crcData, WIFI_PASS_LENGTH);

  crcData = (uint8_t*)ServerURL;
  crc = crc16_be(crc, crcData, SERVER_URL_LENGTH);

  crcData = (uint8_t*)ServerUsername;
  crc = crc16_be(crc, crcData, SERVER_USERNAME_LENGTH);

  crcData = (uint8_t*)ServerPassword;
  crc = crc16_be(crc, crcData, SERVER_PASSWORD_LENGTH);

  crcData = (uint8_t*)OtaPassword;
  crc = crc16_be(crc, crcData, OTA_PASSWORD_LENGTH);

  crc = ~crc;

  Serial.println("[ok]");
  M5.Lcd.println("[ok]");
  
  Serial.print(" Stored CRC: 0x");
  M5.Lcd.print(" Stored CRC: 0x");
  Serial.println(storedCrc, HEX);
  M5.Lcd.println(storedCrc, HEX);
  
  Serial.print(" Calcld CRC: 0x");
  M5.Lcd.print(" Calcld CRC: 0x");
  Serial.println(crc, HEX);
  M5.Lcd.println(crc, HEX);
  
  if (storedCrc != crc) {
    M5.Lcd.setTextColor(TFT_RED);
    Serial.println("\nCRCs don't match");
    M5.Lcd.println("CRCs don't match");
    Serial.println("Restarting...");
    M5.Lcd.println("Restarting...");
    delay(5000);
    ESP.restart();
  }
}
