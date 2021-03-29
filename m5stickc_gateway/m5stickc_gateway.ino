#include <M5StickC.h>

// Make sure to set partition scheme to: Minimal SPIFFS

void setup() {
  // Startup
  initHardware();
  delay(100);
  verifyBaseConfig();
  delay(100);
  initMyName();
  delay(100);
  initWifi();
  delay(100);
  initOTA();
  delay(100);
  initMq();
}

void loop() {
  mqLoop();
  beeperLoop();
  otaLoop();
}
