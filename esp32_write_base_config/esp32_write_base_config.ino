#include "EEPROM.h"
#include "rom/crc.h"

#include "config.h"
#include "eeprom_locations.h"

char Version[VERSION_LENGTH] = "VST001";

char DeviceName[DEVICE_NAME_LENGTH] = DEVICE_NAME;
uint16_t DeviceType = DEVICE_TYPE;
char WifiSSID[WIFI_SSID_LENGTH] = WIFI_SSID;
char WifiPass[WIFI_PASS_LENGTH] = WIFI_PASS;
char ServerURL[SERVER_URL_LENGTH] = SERVER_URL;
char ServerUsername[SERVER_USERNAME_LENGTH] = SERVER_USERNAME;
char ServerPassword[SERVER_PASSWORD_LENGTH] = SERVER_PASSWORD;
char OtaPassword[OTA_PASSWORD_LENGTH] = OTA_PASSWORD;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\nBeginning EEPROM");
  if (!EEPROM.begin(EEPROM_SIZE)) {
    Serial.println("Failed to initialise EEPROM");
    Serial.println("Restarting...");
    delay(1000);
    ESP.restart();
  }

  Serial.println("\nCalculating CRC16");
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

  Serial.print("CRC: 0x");
  Serial.print(crc, HEX);
  Serial.println();

  Serial.println("\nWriting config to EEPROM");

  Serial.println("  Writing Version");
  EEPROM.writeString(VERSION_START, Version);

  Serial.println("  Writing Name");
  EEPROM.writeString(DEVICE_NAME_START, DeviceName);

  Serial.println("  Writing DeviceType");
  EEPROM.writeUShort(DEVICE_TYPE_START, DeviceType);

  Serial.println("  Writing WifiSSID");
  EEPROM.writeString(WIFI_SSID_START, WifiSSID);

  Serial.println("  Writing WifiPass");
  EEPROM.writeString(WIFI_PASS_START, WifiPass);

  Serial.println("  Writing ServerURL");
  EEPROM.writeString(SERVER_URL_START, ServerURL);

  Serial.println("  Writing ServerUsername");
  EEPROM.writeString(SERVER_USERNAME_START, ServerUsername);

  Serial.println("  Writing ServerPassword");
  EEPROM.writeString(SERVER_PASSWORD_START, ServerPassword);

  Serial.println("  Writing OtaPassword");
  EEPROM.writeString(OTA_PASSWORD_START, OtaPassword);

  Serial.print("  Writing CRC16: 0x");
  Serial.println(crc, HEX);
  EEPROM.writeUShort(CRC_START, crc);

  EEPROM.commit();
  Serial.println("\nDone!");
}

void loop() {
  // put your main code here, to run repeatedly:

}
