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

  RTC_TimeTypeDef TimeStruct;
  TimeStruct.Hours   = hour(t);
  TimeStruct.Minutes = minute(t);
  TimeStruct.Seconds = second(t);
  M5.Rtc.SetTime(&TimeStruct);

  RTC_DateTypeDef DateStruct;
  DateStruct.WeekDay = weekday(t)-1;
  DateStruct.Month = month(t);
  DateStruct.Date = day(t);
  DateStruct.Year = year(t);
  M5.Rtc.SetData(&DateStruct);
  
  timeClient.end();
}

char* getTimestamp() {
  RTC_TimeTypeDef RTC_TimeStruct;
  RTC_DateTypeDef RTC_DateStruct;
  
  M5.Rtc.GetTime(&RTC_TimeStruct);
  M5.Rtc.GetData(&RTC_DateStruct);
  
  static char buff[32];
  sprintf(buff, "%04d-%02d-%02d %02d:%02d:%02d", RTC_DateStruct.Year, RTC_DateStruct.Month,RTC_DateStruct.Date, RTC_TimeStruct.Hours, RTC_TimeStruct.Minutes, RTC_TimeStruct.Seconds);

  return buff;
}
