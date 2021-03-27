#include <PubSubClient.h>
#include <WiFi.h>

#include "eeprom_locations.h"

WiFiClient wifiClient;
PubSubClient mqClient(wifiClient);
char gServerURL[SERVER_URL_LENGTH];

void initWifi() {
  // Redraw screen
  drawStarupScreen();

  Serial.print("\nReading Wifi cfg .. ");
  M5.Lcd.print("Reading Wifi cfg .. ");

  String stringData;
  
  // WifiSSID
  char wifiSSID[WIFI_SSID_LENGTH];
  stringData = getWifiSSID();
  stringData.toCharArray(wifiSSID, WIFI_SSID_LENGTH);

  // WifiPass
  char wifiPass[WIFI_PASS_LENGTH];
  stringData = getWifiPass();
  stringData.toCharArray(wifiPass, WIFI_PASS_LENGTH);
  Serial.println("[ok]");
  M5.Lcd.println("[ok]");

  
  Serial.print("  SSID: ");
  M5.Lcd.print("  SSID: ");
  Serial.println(wifiSSID);
  M5.Lcd.println(wifiSSID);

  Serial.print("Connecting ");
  M5.Lcd.print("Connecting ");
  
  WiFi.setHostname(getMyName());
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiSSID, wifiPass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println(" [ok]");
  M5.Lcd.println(" [ok]");
  
  Serial.print("  IP: ");
  M5.Lcd.print("  IP: ");
  Serial.println(WiFi.localIP());
  M5.Lcd.println(WiFi.localIP());
}

void initMq() {
  // Redraw screen
  drawStarupScreen();

  Serial.print("\nReading server cfg .. ");
  M5.Lcd.print("Reading server cfg .. ");
  
  // ServerURL
  String stringData = getConfigServerURL();
  stringData.toCharArray(gServerURL, SERVER_URL_LENGTH);
  
  Serial.println("[ok]");
  M5.Lcd.println("[ok]");
  
  Serial.print("  Server: ");
  M5.Lcd.print("  Server: ");
  Serial.println(gServerURL);
  M5.Lcd.println(gServerURL);
  
  mqClient.setServer(gServerURL, 1883);
  mqClient.setCallback(mqCallback);
}

void mqCallback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}

void mqLoop() {
  if (!mqClient.connected()) {
    mqReconnect();
  }
  
  mqClient.loop();
}

void mqReconnect() {
    while (!mqClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqClient.connect(getMyName())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic","hello world");
      // ... and resubscribe
      //client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
