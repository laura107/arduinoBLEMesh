#include <M5Atom.h>
#include <BLEDevice.h>
#include <BLEClient.h>
#include <BLEUtils.h>

static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
static BLEUUID charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");
static boolean doConnect = false;
static boolean connected = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    // Check if the advertised device is the one we're looking for
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {
      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
    }
  }
};

void setup() {
  M5.begin();
  Serial.begin(115200);

  // Initialize BLE
  BLEDevice::init("");
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(30);
}

void loop() {
  if (doConnect) {
    BLEClient* pClient = BLEDevice::createClient();
    if (pClient->connect(myDevice)) {
      Serial.println("Connected to server");
      pRemoteCharacteristic = pClient->getService(serviceUUID)->getCharacteristic(charUUID);
      if (pRemoteCharacteristic->canRead()) {
        Serial.println(pRemoteCharacteristic->readValue().c_str());
      }
      connected = true;
    } else {
      Serial.println("Failed to connect to server");
    }
    doConnect = false;
  }
  if (connected) {
    // Add your communication logic here
    delay(1000);
  }
}
