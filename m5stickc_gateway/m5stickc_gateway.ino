#include <M5StickC.h>

void setup() {
  // Startup
  initHardware();
  delay(100);
  verifyConfig();
  delay(100);
}

void loop() {
  Serial.println("Beep Boop.");
  delay(5000);
}
