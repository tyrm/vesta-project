static BLEUUID genericBatteryServiceUUID((uint16_t)0x180F);
static BLEUUID genericBatteryCharUUID((uint16_t)0x2A19);

static BLEUUID lywsd03mmcTempServiceUUID("ebe0ccb0-7a0a-4b0c-8a1a-6ff2997da3a6");
static BLEUUID lywsd03mmcTempCharUUID("ebe0ccc1-7a0a-4b0c-8a1a-6ff2997da3a6");

static boolean bleConnected = false;
static boolean closeClient = false;

static boolean lywsd03mmcDoConnect = false;

static BLEClient* pClient;
static BLEAdvertisedDevice* clientDevice;
static BLEAddress* searchAddress;
static BLERemoteCharacteristic* pRemoteCharacteristic;

typedef union {
  short num;
  byte b [sizeof(short)];
} shortBytes;

static void lywsd03mmcNotifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    shortBytes tempBytes;
    tempBytes.b[0] = pData[0];
    tempBytes.b[1] = pData[1];
    float temp = tempBytes.num / 100.0;
    
    shortBytes voltageBytes;
    voltageBytes.b[0] = pData[3];
    voltageBytes.b[1] = pData[4];
    float voltage = voltageBytes.num / 1000.0;
    
    Serial.print("Notify callback for characteristic ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print(" of data length ");
    Serial.println(length);
    Serial.print("data: '");
    Serial.print(temp);
    Serial.print(", ");
    Serial.print(pData[2], DEC);
    Serial.print(", ");
    Serial.print(voltage);
    Serial.println("'");

    mqSendSensorEnv(clientDevice->getAddress().toString().c_str(), temp, pData[2], voltage);

    closeClient = true;
}

class Lywsd03mmcAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.getAddress().equals(*searchAddress)) {
      BLEDevice::getScan()->stop();
      clientDevice = new BLEAdvertisedDevice(advertisedDevice);
      lywsd03mmcDoConnect = true;
    } // Found our server
  } // onResult
}; // Lywsd03mmcAdvertisedDeviceCallbacks

class Lywsd03mmcClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
    Serial.print("ble cconnected to ");
    Serial.println(pclient->getPeerAddress().toString().c_str());
  }

  void onDisconnect(BLEClient* pclient) {
    bleConnected = false;
    Serial.print("ble discconnected from ");
    Serial.println(pclient->getPeerAddress().toString().c_str());
  }
}; // Lywsd03mmcClientCallback

void bleLoop() {
  if (lywsd03mmcDoConnect == true) {
    lywsd03mmcConnect();
    lywsd03mmcDoConnect = false;
  } else if (closeClient == true) {
    pClient->disconnect();
    
    closeClient = false;
  }
}

void lywsd03mmcConnect() {
  Serial.print("connecting to ");
  Serial.println(clientDevice->getAddress().toString().c_str());

  pClient = BLEDevice::createClient();
  
  pClient->setClientCallbacks(new Lywsd03mmcClientCallback());
  if (!pClient->connect(clientDevice)) {
    Serial.println(" - Connection failed");
    pClient->disconnect();
    return;
  }

  Serial.println(" - Connected to server");
  bleConnected = true;

  BLERemoteService* pService;

  // get battery level
  pService = pClient->getService(genericBatteryServiceUUID);
  if (pService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(genericBatteryServiceUUID.toString().c_str());
    pClient->disconnect();
    return;
  }

  pRemoteCharacteristic = pService->getCharacteristic(genericBatteryCharUUID);
  if (pRemoteCharacteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(genericBatteryCharUUID.toString().c_str());
    pClient->disconnect();
    return;
  }
  Serial.println(" - Found battery characteristic");

  if(pRemoteCharacteristic->canRead()) {
    std::string value = pRemoteCharacteristic->readValue();
    Serial.print("The characteristic value was: '");
    Serial.print(value[0], DEC);
    Serial.println("'");

    mqSendSensorBat(clientDevice->getAddress().toString().c_str(), (int)value[0]);
  }

  // setup notify to get temp
  pService = pClient->getService(lywsd03mmcTempServiceUUID);
  if (pService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(lywsd03mmcTempServiceUUID.toString().c_str());
    pClient->disconnect();
    return;
  }

  pRemoteCharacteristic = pService->getCharacteristic(lywsd03mmcTempCharUUID);
  if (pRemoteCharacteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(lywsd03mmcTempCharUUID.toString().c_str());
    pClient->disconnect();
    return;
  }
  Serial.println(" - Found temp characteristic");

  if(pRemoteCharacteristic->canNotify())
    pRemoteCharacteristic->registerForNotify(lywsd03mmcNotifyCallback);
}

void getDataFromLywsd03mmc(const char* mac) {
  searchAddress = new BLEAddress(mac);

  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new Lywsd03mmcAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(30, false);
}

void initBle() {
  BLEDevice::init(getMyName());
}
