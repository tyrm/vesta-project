#include "eeprom_locations.h"

static WiFiClient wifiClient;
static PubSubClient mqClient(wifiClient);
static char gServerURL[SERVER_URL_LENGTH];
static char gServerUsername[SERVER_USERNAME_LENGTH];
static char gServerPassword[SERVER_PASSWORD_LENGTH];

void initWifi() {
  // Redraw screen
  drawStarupScreen();

  Serial.print("\nReading Wifi cfg .. ");
  M5.Lcd.print("Reading Wifi cfg .. ");

  String stringData;
  
  // WifiSSID
  char wifiSSID[WIFI_SSID_LENGTH];
  stringData = getConfigWifiSSID();
  stringData.toCharArray(wifiSSID, WIFI_SSID_LENGTH);

  // WifiPass
  char wifiPass[WIFI_PASS_LENGTH];
  stringData = getConfigWifiPass();
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
  
  // ServerUsername
  stringData = getConfigServerUsername();
  stringData.toCharArray(gServerUsername, SERVER_USERNAME_LENGTH);
  
  // ServerPassword
  stringData = getConfigServerPassword();
  stringData.toCharArray(gServerPassword, SERVER_PASSWORD_LENGTH);
  
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
    if (mqClient.connect(getMyName(), gServerUsername, gServerPassword)) {
      Serial.println("connected");
      
      String nameString = getMyName();
      String namedSubString = "gateway.cmd." + nameString;
      char namedSub[namedSubString.length() + 1];
      namedSubString.toCharArray(namedSub, namedSubString.length() + 1);
      mqClient.subscribe(namedSub);
      
      mqClient.subscribe("gateway.cmd.all");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void mqSendSensorBat(const char* sensorID, int percent) {
  DynamicJsonDocument doc(1024);
  
  doc["data"]["bat_p"] = percent;
  doc["from"] = getMyName();
  doc["sid"] = sensorID;
  doc["ts"] = getTimestamp();

  char jsonStr[200];
  serializeJson(doc, jsonStr);
  mqClient.publish("sensor.reading.bat",jsonStr);
}

void mqSendSensorEnv(const char* sensorID, float temp, int hum, float volt) {
  DynamicJsonDocument doc(1024);
  
  doc["data"]["temp"] = temp;
  doc["data"]["hum"] = hum;
  doc["data"]["bat_v"] = volt;
  doc["from"] = getMyName();
  doc["sid"] = sensorID;
  doc["ts"] = getTimestamp();

  char jsonStr[200];
  serializeJson(doc, jsonStr);
  mqClient.publish("sensor.reading.env",jsonStr);
}
