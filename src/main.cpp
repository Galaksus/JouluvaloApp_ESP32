#include <Arduino.h>
#include <BLEHandler.h>
#include <GlobalConstants.h>

#define ledPin 13//15 //BUILTIN_LED // Määritä tähän haluttu pinni

#ifdef ESP_PLATFORM
  // Code specific to ESP platforms
  #ifdef ESP32
    // Code specific to ESP32
    #ifdef BOARD_ESP32CAM
      #define MONITOR_SPEED 115200
    #elif defined(BOARD_ESP32DOIT_DEVKIT_V1)
      #define MONITOR_SPEED 9600
    #else
      #define MONITOR_SPEED_DEFAULT 9600
    #endif
  #endif
#endif

#ifndef MONITOR_SPEED
  // Default monitor speed if not specified for a board
  #define MONITOR_SPEED MONITOR_SPEED_DEFAULT
#endif
const int ledChannel = 0; // PWM channel
const int resolution = 8; // PWM resolution (8 bits)

int lightShowMode = 1;

unsigned long previousMillis = 0;
int brightness = 255;
int autoPWMbrightness = 0;
int fadeAmount = 1; // How much it fades each cycle
int fadeDelay = 10; // Delay in milliseconds

bool lightON = false;
// Construct BLE class
BLEHandler* BLE;

void setup() {
  // Start the serial communication
  Serial.begin(MONITOR_SPEED);
   while(!Serial);
  BLE = new BLEHandler();

  pinMode(ledPin, OUTPUT); // Määritä oikee pinni
  
  // Set up PWM on the LED pin
  ledcSetup(ledChannel, 5000, resolution); // Configure PWM settings
  ledcAttachPin(ledPin, ledChannel); // Attach the PWM channel to the pin
}


void LEDdimmer(int dimmingValue) {
    // Säädettävä dimmeri
    ledcWrite(ledChannel, dimmingValue);
    brightness = dimmingValue;
}

void loop()
{
  // if (lightShowMode == 0) {return;}
  unsigned long currentMillis = millis();

  switch (lightShowMode)
  {
  case 0:
    //digitalWrite(ledPin, LOW);
    ledcWrite(ledChannel, 0);
    lightON = false;
    break;

  case 1:
    //digitalWrite(ledPin, HIGH);
    if (!lightON) {
      ledcWrite(ledChannel, brightness);
      lightON = true;
    }
    break;
  case 2:

    if (currentMillis - previousMillis >= fadeDelay)
    {
      previousMillis = currentMillis;

      // Update the brightness
      autoPWMbrightness += fadeAmount;

      // Reverse the fade direction when reaching the limits
      if (autoPWMbrightness <= 0 || autoPWMbrightness >= brightness)
      {
        fadeAmount = -fadeAmount;
      }

      // Apply the brightness to the LED
      ledcWrite(ledChannel, autoPWMbrightness);
    case 3:
      // LEDdimmer(currentDimmingValue);
      break;

    case 4:

      break;

    case 5:
      break;

    default:
      digitalWrite(ledPin, HIGH);
      break;
    }
  }
}