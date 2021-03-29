#include "EEPROM.h"
#include "rom/crc.h"

#include "config.h"
#include "eeprom_locations.h"

char NTPServer[NTP_SERVER_LENGTH] = NTP_SERVER;

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

  uint8_t* crcData = (uint8_t*)NTPServer;
  crc = crc16_be(crc, crcData, NTP_SERVER_START);

  crc = ~crc;

  Serial.print("CRC: 0x");
  Serial.print(crc, HEX);
  Serial.println();

  Serial.println("\nWriting config to EEPROM");

  Serial.println("  Writing NTPServer");
  EEPROM.writeString(NTP_SERVER_START, NTPServer);

  Serial.print("  Writing CRC16: 0x");
  Serial.println(crc, HEX);
  EEPROM.writeUShort(DEVICE_CRC_START, crc);

  EEPROM.commit();
  Serial.println("\nDone!");
}

void loop() {
  // put your main code here, to run repeatedly:

}
