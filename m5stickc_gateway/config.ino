#include "EEPROM.h"
#include "rom/crc.h"

// EEPROM Locations
#define VERSION_START 0
#define VERSION_LENGTH 8

#define NAME_START 8
#define NAME_LENGTH 32

#define WIFI_SSID_START 40
#define WIFI_SSID_LENGTH 32

#define WIFI_PASS_START 72
#define WIFI_PASS_LENGTH 32

#define SERVER_URL_START 104
#define SERVER_URL_LENGTH 32

#define DEVICE_TYPE_START 136
#define DEVICE_TYPE_LENGTH 2

#define CRC_START 254
#define CRC_LENGTH 2

void verifyConfig() {
  char Version[VERSION_LENGTH];
  char Name[NAME_LENGTH];
  char WifiSSID[WIFI_SSID_LENGTH];
  char WifiPass[WIFI_PASS_LENGTH];
  char ServerURL[SERVER_URL_LENGTH];
  uint16_t DeviceType;

  uint16_t storedCrc;

  String stringData;
  
  // Redraw screen
  M5.Lcd.fillScreen(TFT_BLACK);
  drawHeader();
  M5.Lcd.setTextSize(0);
  M5.Lcd.setCursor(0, 18);
  M5.Lcd.setTextColor(TFT_WHITE);

  // Clear Memory
  Serial.print("Clearing Memory .. ");
  M5.Lcd.print("Clearing Memory .. ");

  for (int i = 0; i <= VERSION_LENGTH; i++) {
    Version[i] = '\0';
  }
  for (int i = 0; i <= NAME_LENGTH; i++) {
    Name[i] = '\0';
  }
  for (int i = 0; i <= WIFI_SSID_LENGTH; i++) {
    WifiSSID[i] = '\0';
  }
  for (int i = 0; i <= WIFI_PASS_LENGTH; i++) {
    WifiPass[i] = '\0';
  }
  for (int i = 0; i <= SERVER_URL_LENGTH; i++) {
    ServerURL[i] = '\0';
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
  stringData = EEPROM.readString(NAME_START);
  stringData.toCharArray(Name, NAME_LENGTH);

  // WifiSSID
  stringData = EEPROM.readString(WIFI_SSID_START);
  stringData.toCharArray(WifiSSID, WIFI_SSID_LENGTH);

  // WifiPass
  stringData = EEPROM.readString(WIFI_PASS_START);
  stringData.toCharArray(WifiPass, WIFI_PASS_LENGTH);

  // ServerURL
  stringData = EEPROM.readString(SERVER_URL_START);
  stringData.toCharArray(ServerURL, SERVER_URL_LENGTH);

  // DeviceType
  DeviceType = EEPROM.readUShort(DEVICE_TYPE_START);

  // Stored CRC
  storedCrc = EEPROM.readUShort(CRC_START);

  Serial.println("[ok]");
  M5.Lcd.println("[ok]");

  Serial.print("  Version: ");
  Serial.println(Version);
  Serial.print("  Name: ");
  Serial.println(Name);
  Serial.print("  WifiSSID: ");
  Serial.println(WifiSSID);
  Serial.print("  WifiPass: ");
  Serial.println(WifiPass);
  Serial.print("  ServerURL: ");
  Serial.println(ServerURL);
  Serial.print("  DeviceType: ");
  Serial.println(DeviceType, DEC);
  Serial.print("  Stored CRC: 0x");
  Serial.println(storedCrc, HEX);
  
  // Calculate CRC
  Serial.print("Calculating CRC .. ");
  M5.Lcd.print("Calculating CRC .. ");
  
  uint16_t crc = ~0x0000;

  uint8_t* crcData = (uint8_t*)Version;
  crc = crc16_be(crc, crcData, VERSION_LENGTH);

  crcData = (uint8_t*)Name;
  crc = crc16_be(crc, crcData, NAME_LENGTH);

  crcData = (uint8_t*)WifiSSID;
  crc = crc16_be(crc, crcData, WIFI_SSID_LENGTH);

  crcData = (uint8_t*)WifiPass;
  crc = crc16_be(crc, crcData, WIFI_PASS_LENGTH);

  crcData = (uint8_t*)ServerURL;
  crc = crc16_be(crc, crcData, SERVER_URL_LENGTH);

  byte low = DeviceType;
  byte high = DeviceType >> 8;
  uint8_t deviceTypeBytes[] = {high, low};
  crc = crc16_be(crc, deviceTypeBytes, DEVICE_TYPE_LENGTH);
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
}
