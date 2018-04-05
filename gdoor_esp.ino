/*
*  Main file for controlling code and execution on 
*  the ESP8266 for the garage sensor project
*  Project file
*
* Author: Josh Perry
* Copyright 2018
*
*/

// Extern libs
#include "ESP8266WiFi.h"

// Internal files
#include "src/constants/Constants.h"
#include "src/wifi-interface/WifiInterface.hpp"
#include "src/user/GDoorUser.hpp"   
#include "src/networking/HttpInterface.hpp"   

// Global vars
GDoorUser user;
char doorSensorPinState = HIGH;
int doorSensorPin = 2;

void setup() {
  delay(3000);
  Serial.begin(115200);
  Serial.println("Firing up Esp!!");

  // Set up GPIO pins
  setupGPIOPins();

  // Connect to wifi
  connectToWifi(user.ssid, user.password);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  assessDoorState();
}


/*
 *                GPIO Handling
 * 
 * This section contains all code related to GPIO 
 * pins and their handling
 * 
 */

void setupGPIOPins(){
  Serial.println("Setting GPIO Pins to correct IO state");
  pinMode(doorSensorPin, INPUT);
}

void assessDoorState() {
  // Only read the pin state if the previous value was high
  if (doorSensorPinState == HIGH){
    doorSensorPinState = digitalRead(doorSensorPin);
    
    if (doorSensorPinState == LOW){
      // Switch is in actuated state - update door status locally and in DB
      
      if (user.currentDoorState == DOOR_STATE_OPEN){
        Serial.println("MAIN: Door status changed to CLOSED");
        user.currentDoorState = DOOR_STATE_CLOSED;
        sendUpdateForState(DOOR_STATE_CLOSED, user.uid);
      }

      else{
        Serial.println("MAIN: Door status changed to OPEN");
        user.currentDoorState = DOOR_STATE_OPEN;
        sendUpdateForState(DOOR_STATE_OPEN, user.uid);
      }
    }
  }

  else{
    // Update the pin state only (without a message)
    doorSensorPinState = digitalRead(doorSensorPin);
  }
}









