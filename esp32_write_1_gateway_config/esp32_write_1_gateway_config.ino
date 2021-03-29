#include "EEPROM.h"
#include "rom/crc.h"

#include "config.h"
#include "eeprom_locations.h"

void setup() {
  uint16_t storedCrc;
  String stringData;
  
  Serial.begin(115200);
  delay(1000);

  // Clear Memory
  Serial.println("Clearing Memory .. ");
  char ntpServer[NTP_SERVER_LENGTH];

  for (int i = 0; i < NTP_SERVER_LENGTH; i++) {
    ntpServer[i] = '\0';
  }

  Serial.println("Populating Memory .. ");
  stringData = NTP_SERVER;
  stringData.toCharArray(ntpServer, NTP_SERVER_LENGTH);

  Serial.println("\nBeginning EEPROM");
  if (!EEPROM.begin(EEPROM_SIZE)) {
    Serial.println("Failed to initialise EEPROM");
    Serial.println("Restarting...");
    delay(1000);
    ESP.restart();
  }

  Serial.println("\nCalculating CRC16");
  uint16_t crc = ~0x0000;

  uint8_t* crcData = (uint8_t*)ntpServer;
  crc = crc16_be(crc, crcData, NTP_SERVER_LENGTH);

  crc = ~crc;

  Serial.print("CRC: 0x");
  Serial.print(crc, HEX);
  Serial.println();

  Serial.println("\nWriting config to EEPROM");

  Serial.print("  Writing NTPServer: ");
  Serial.println(ntpServer);
  EEPROM.writeString(NTP_SERVER_START, ntpServer);

  Serial.print("  Writing CRC16: 0x");
  Serial.println(crc, HEX);
  EEPROM.writeUShort(DEVICE_CRC_START, crc);

  EEPROM.commit();
  Serial.println("\nDone!");
}

void loop() {
  // put your main code here, to run repeatedly:

}
