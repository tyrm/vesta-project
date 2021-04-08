#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include <BLEDevice.h>
#include <EEPROM.h>
#include <M5StickC.h>
#include <NTPClient.h>
#include <PubSubClient.h>
#include <rom/crc.h>
#include <TimeLib.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// Make sure to set partition scheme to: Minimal SPIFFS

void setup() {
  // Startup
  initHardware();
  delay(100);
  verifyBaseConfig();
  delay(100);
  verifyGatewayConfig();
  delay(100);
  initMyName();
  delay(100);
  initWifi();
  delay(100);
  initTime();
  delay(100);
  initOTA();
  delay(100);
  initMq();
  delay(100);
  initBle();
  delay(100);
  getDataFromLywsd03mmc("a4:c1:38:20:0e:1b");
}

void loop() {
  mqLoop();
  beeperLoop();
  otaLoop();
  bleLoop();
}
