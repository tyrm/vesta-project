#define TFT_GREY 0x5AEB

void drawHeader() {
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(1, 0);
  M5.Lcd.setTextColor(TFT_CYAN);
  M5.Lcd.println("Vesta Gateway");
}

void drawStarupScreen() {
  M5.Lcd.fillScreen(TFT_BLACK);
  drawHeader();
  M5.Lcd.setTextSize(0);
  M5.Lcd.setCursor(0, 18);
  M5.Lcd.setTextColor(TFT_WHITE);
}
