#include <M5Core2.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer* pServer = nullptr;
BLECharacteristic* pCharacteristic = nullptr;
bool deviceConnected = false;
const char* serviceUUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
const char* charUUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8";

class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};

void setup() {
  M5.begin();
  Serial.begin(115200);

  // Initialize BLE
  BLEDevice::init("M5Stack_Core2");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService* pService = pServer->createService(serviceUUID);
  pCharacteristic = pService->createCharacteristic(
                      charUUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE
                    );
  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();
  pServer->getAdvertising()->start();
  Serial.println("Waiting for a client connection...");
}

void loop() {
  if (deviceConnected) {
    // Add your communication logic here
    Serial.println("Device connected");
    delay(1000);
  }
}
