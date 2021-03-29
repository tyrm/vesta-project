#include <NTPClient.h>
#include <TimeLib.h>
#include <WiFiUdp.h>

#include "eeprom_locations.h"

void initTime() {

  // NTP Server
  char ntpServer[SERVER_URL_LENGTH];
  String stringData = getConfigNTPServer();
  stringData.toCharArray(ntpServer, SERVER_URL_LENGTH);

  // Init Client
  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP, ntpServer);
  timeClient.begin();

  Serial.println("Getting Time");
  if (!timeClient.update()) {
    Serial.println("  Error getting time. ");
    M5.Lcd.println("  Restarting...");
    delay(5000);
    ESP.restart();
  }
  
  Serial.print("  Got Time: ");
  unsigned long t = timeClient.getEpochTime();

  char buff[32];
  sprintf(buff, "%04d-%02d-%02d %02d:%02d:%02d", year(t), month(t), day(t), hour(t), minute(t), second(t));
  Serial.println(buff);
  
  timeClient.end();
}
