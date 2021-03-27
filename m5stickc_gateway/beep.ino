unsigned long lastBeep = 0;

void beeperLoop() {
  unsigned long currentMillis = millis();

  if(currentMillis - lastBeep > 5000) {
    lastBeep = currentMillis;  
    
    Serial.print("Beep Boop. I am ");
    Serial.println(getMyName());
  }
}
