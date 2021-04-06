static BLEUUID lywsd03mmcTempServiceUUID("ebe0ccb0-7a0a-4b0c-8a1a-6ff2997da3a6");
static BLEUUID lywsd03mmcTempCharUUID("ebe0ccc1-7a0a-4b0c-8a1a-6ff2997da3a6");

static boolean lywsd03mmcDoConnect = false;
static boolean lywsd03mmcConnected = false;
static boolean closeClient = false;

static BLEClient* pClient;
static BLEAdvertisedDevice* clientDevice;
static BLEAddress* searchAddress;
static BLERemoteCharacteristic* pRemoteCharacteristic;

static void lywsd03mmcNotifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    Serial.print("Notify callback for characteristic ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print(" of data length ");
    Serial.println(length);
    Serial.print("data: '");
    Serial.print((char*)pData);
    Serial.println("'");

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
    lywsd03mmcConnected = false;
    Serial.print("ble discconnected from ");
    Serial.println(pclient->getPeerAddress().toString().c_str());
  }
}; // Lywsd03mmcClientCallback

void bleLoop() {
  if (lywsd03mmcDoConnect == true) {
    lywsd03mmcConnect();
    lywsd03mmcDoConnect = false;
  }
  if (closeClient == true) {
    pClient->disconnect();
    closeClient = false;
  }
}

void lywsd03mmcConnect() {
    Serial.print("connecting to ");
    Serial.println(clientDevice->getAddress().toString().c_str());
  
    pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");
    
    pClient->setClientCallbacks(new Lywsd03mmcClientCallback());
    pClient->connect(clientDevice);
    Serial.println(" - Connected to server");

    BLERemoteService* pRemoteService = pClient->getService(lywsd03mmcTempServiceUUID);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(lywsd03mmcTempServiceUUID.toString().c_str());
      pClient->disconnect();
      return;
    }

    pRemoteCharacteristic = pRemoteService->getCharacteristic(lywsd03mmcTempCharUUID);
    if (pRemoteCharacteristic == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(lywsd03mmcTempCharUUID.toString().c_str());
      pClient->disconnect();
      return;
    }
    Serial.println(" - Found our characteristic");

    if(pRemoteCharacteristic->canNotify())
      pRemoteCharacteristic->registerForNotify(lywsd03mmcNotifyCallback);
    
    lywsd03mmcConnected = true;
}

void getDataFromLywsd03mmc(const char* mac) {
  searchAddress = new BLEAddress(mac);

  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new Lywsd03mmcAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(15, false);
}

void initBle() {
  BLEDevice::init("");
}
