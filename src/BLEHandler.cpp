#include <Arduino.h>
#include "BLEHandler.h"

BLEHandler::BLEHandler()
{
    // Start the BLE server
    BLEDevice::init("ESP32 Service");
    pServer = BLEDevice::createServer();

    // Set the server callbacks
    pServer->setCallbacks(new ConnectionCallbacks(*this));

    // Create service
    BLEService *pService = pServer->createService(SERVICE_UUID);
            Serial.println("Service created.");

    // Create Characteristics
    BLECharacteristic *pCharacteristic_DELAY = pService->createCharacteristic(
                                        DELAY_CHARACTERISTIC_UUID,
                                        BLECharacteristic::PROPERTY_READ |
                                            BLECharacteristic::PROPERTY_WRITE);
    pCharacteristic_DELAY->setCallbacks(new CharacteristicEventHandler(*this));

    BLECharacteristic *pCharacteristic_DIMMER = pService->createCharacteristic(
                                            DIMMER_CHARACTERISTIC_UUID,
                                            BLECharacteristic::PROPERTY_READ |
                                            BLECharacteristic::PROPERTY_WRITE
                                        );
    pCharacteristic_DIMMER->setCallbacks(new CharacteristicEventHandler(*this));

    BLECharacteristic *pCharacteristic_COMMON = pService->createCharacteristic(
                                            COMMON_CHARACTERISTIC_UUID,
                                            BLECharacteristic::PROPERTY_READ |
                                            BLECharacteristic::PROPERTY_WRITE
                                        );
    pCharacteristic_COMMON->setCallbacks(new CharacteristicEventHandler(*this));

    // Add all the characteristics to the service
  pService->addCharacteristic(pCharacteristic_DELAY);
  pService->addCharacteristic(pCharacteristic_DIMMER);
  pService->addCharacteristic(pCharacteristic_COMMON);


    // Start the service
    pService->start();

    // Get advertising instance
    pAdvertising = pServer->getAdvertising();

    // Start advertising after setting up the service
    startAdvertising();
}

BLEHandler::~BLEHandler()
{
    // Destructor implementation
}

void BLEHandler::startAdvertising()
{
    // Stop advertising if it is running
    if (isAdvertising)
    {
        pAdvertising->stop();
        isAdvertising = false;
    }
    // Configure advertising parameters
    BLEAdvertisementData advertisingData;
    advertisingData.setFlags(0x04); // General Discoverable mode
    advertisingData.setCompleteServices(BLEUUID(SERVICE_UUID));

    // Start advertising
    pAdvertising->setAdvertisementData(advertisingData);
    pAdvertising->start();
    isAdvertising = true;
    Serial.println("ESP32 device is advertising the \"" + String(SERVICE_UUID) + "\" service");
}

// Function to set the inbuilt LED based on Bluetooth connection status
void BLEHandler::setLedBasedOnBluetoothConnection(bool isConnected) {
    if (isConnected) {
        digitalWrite(LED_BUILTIN, HIGH);  // Set LED HIGH when connected
    } else {
        digitalWrite(LED_BUILTIN, LOW);   // Set LED LOW when disconnected
    }
}