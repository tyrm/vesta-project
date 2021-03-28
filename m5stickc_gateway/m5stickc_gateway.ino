#include <M5StickC.h>

void setup() {
  // Startup
  initHardware();
  delay(1000);
  verifyConfig();
  delay(1000);
  initMyName();
  delay(1000);
  initWifi();
  delay(1000);
  initMq();
  delay(1000);
  initOTA();
}

void loop() {
  mqLoop();
  beeperLoop();
  otaLoop();
}
