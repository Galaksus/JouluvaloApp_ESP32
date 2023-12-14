#ifndef GLOBALCONSTANTS_H
#define GLOBALCONSTANTS_H

// define pins
// ESP32 cam specifc remapping of SDA and SCL to 14 and 15 GPIO pins
// as the default SDA and SCP pins are used by the camera
// These I2C pins are used by magnetometer sensor
#define I2C_SDA_PIN 13      // GPIO 13 for SDA
#define I2C_SCL_PIN 15      // GPIO 15 for SCL
#define LIMIT_SWITCH_PIN 0 // Limit switch pin
#define dirPin 14    // Stepper motor dir pin // default 3 // debug setup 0
#define pulPin 12    // Stepper motor pul pin  // default 1 // debug setup 12
#define PWM 4 // PWM pin of MD20A DC motor driver
#define DIR 2 // DIR pin of MD20A DC motor driver

/*/ BLE related constants */
//  The service UUID that the ESP32 device will advertise
#define SERVICE_UUID "58ecb6f1-887b-487d-a378-0f9048c505da"
// The characteristic UUIDs for reading data from the client
#define DELAY_CHARACTERISTIC_UUID    "e0a432d7-8e2c-4380-b4b2-1568aa0412a3"
#define DIMMER_CHARACTERISTIC_UUID    "20e88205-d8cd-42a9-bcfa-4b599484d362" 
#define COMMON_CHARACTERISTIC_UUID       "f53de08c-1c0c-459a-a6d5-cd26a1523060"


// Functions

void setLightOnOff(int state);
void LEDdimmer(int dimmingValue);


#endif // GLOBALCONSTANTS_H
