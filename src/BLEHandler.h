#ifndef BLEHandler_H
#define BLEHandler_H

#include <Arduino.h>
#include <GlobalConstants.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Extern variables from main.cpp
extern int fadeDelay;
extern int lightShowMode;
extern int dimmingValue;

class BLEHandler
{
public:
    BLEHandler();  // Constructor
    ~BLEHandler(); // Destructor
    void startAdvertising();
    void setLedBasedOnBluetoothConnection(bool isConnected);

private:
    bool deviceConnected = false;
    bool isAdvertising = false; // Advertising state

    BLEServer *pServer;
    BLEAdvertising *pAdvertising;

    /* CallBack classes related to BLE*/

    // ConnectionCallbacks class definition
    class ConnectionCallbacks : public BLEServerCallbacks
    {
    private:
        BLEHandler &bleHandler; // Reference to the parent BLEHandler instance

    public:
        ConnectionCallbacks(BLEHandler &handler) : bleHandler(handler) {}

        void onConnect(BLEServer *server)
        {
            // ets_printf("Client connected.\n");
            Serial.println("Client connected.");
            bleHandler.deviceConnected = true;
            // set inbuilt LED on/off accordingly
            bleHandler.setLedBasedOnBluetoothConnection(bleHandler.deviceConnected);
        }

        void onDisconnect(BLEServer *server)
        {
            // ets_printf("Client disconnected.\n");
            Serial.println("Client disconnected.");
            bleHandler.deviceConnected = false;
            bleHandler.startAdvertising(); // Restart advertising using the parent instance
            // set inbuilt LED on/off accordingly
            bleHandler.setLedBasedOnBluetoothConnection(bleHandler.deviceConnected);
        }
    };

    // CharacteristicEventHandler class definition
    class CharacteristicEventHandler : public BLECharacteristicCallbacks
    {
    private:
        BLEHandler &bleHandler; // Reference to the parent BLEHandler instance

        static constexpr size_t MAX_DATA_SIZE = 100; // Adjust the size as needed (currently 100 characters)

        char receivedData[MAX_DATA_SIZE]; // data buffer

    public:
        // Constructor with a reference to BLEHandler
        CharacteristicEventHandler(BLEHandler &handler) : bleHandler(handler) {}

        void onWrite(BLECharacteristic *pCharacteristic)
        {
            // Retrieve the data written to the characteristic
            std::string value = pCharacteristic->getValue();
            int intValue;
            try
            {
                intValue = std::stoi(value);
            }
            catch (const std::invalid_argument &e)
            {
                Serial.println("Invalid argument. Conversion to int failed.");
            }
            catch (const std::out_of_range &e)
            {
                Serial.println("Out of range. Conversion to int failed.");
            }

            Serial.print("LightShowMode value: ");
            Serial.println(lightShowMode);

            if (pCharacteristic->getUUID().equals(BLEUUID(COMMON_CHARACTERISTIC_UUID)))
            {
                Serial.print("COMMON_CHARACTERISTIC_UUID Received data: ");
                Serial.println(intValue);
                lightShowMode = intValue;

            }
            else if (pCharacteristic->getUUID().equals(BLEUUID(DIMMER_CHARACTERISTIC_UUID)))
            {
                Serial.print("DIMMER_CHARACTERISTIC_UUID Received data as integer: ");
                Serial.println(intValue);
                LEDdimmer(intValue);
            }
            else if (pCharacteristic->getUUID().equals(BLEUUID(DELAY_CHARACTERISTIC_UUID)))
            {
                Serial.print("DELAY_CHARACTERISTIC_UUID Received data as integer: ");
                Serial.println(intValue);
                fadeDelay = intValue;
            }

            else
            {
                Serial.println("Unknown UUID");
            }
        }

        void onRead(BLECharacteristic *pCharacteristic)
        {
            /*
            * Sends lightShowMode, brightness and fadeDelay to corresponding UUIDs on read request
            */
            Serial.println("Read request received");
            if (pCharacteristic->getUUID().equals(BLEUUID(COMMON_CHARACTERISTIC_UUID)))
            {

            }
            else if (pCharacteristic->getUUID().equals(BLEUUID(DIMMER_CHARACTERISTIC_UUID)))
            {

            }
            else if (pCharacteristic->getUUID().equals(BLEUUID(DELAY_CHARACTERISTIC_UUID)))
            {

            }
        }
    };
};

#endif // BLEHandler_H
