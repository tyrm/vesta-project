void beeperLoop() {
  static unsigned long lastBeep = 0;
  unsigned long currentMillis = millis();

  if(currentMillis - lastBeep > 5000) {
    lastBeep = currentMillis;  
    
    Serial.print("Beep Boop. I am ");
    Serial.println(getMyName());
  }
}
