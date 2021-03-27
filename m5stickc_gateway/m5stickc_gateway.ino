#include <M5StickC.h>

void setup() {
  // Startup
  initHardware();
  delay(100);
  verifyConfig();
  delay(100);
  initMyName();
  delay(1000);
  initWifi();
  
}

void loop() {
  Serial.print("Beep Boop. I am ");
  Serial.println(getMyName());
  delay(5000);
}
