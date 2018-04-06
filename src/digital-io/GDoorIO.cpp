/*
*	GPIO Class to handle all the pins and ISR triggers
*	Source file	
*
*	Author: Josh Perry
*	Copyright 2018
*
*/

#include "GDoorIO.hpp"

// Implementation
GDoorIO::GDoorIO(){
	// Fill the constructor with something
	dummyVar = "O";
}

void GDoorIO::setupGPIOPins(){
  Serial.println("Setting GPIO Pins to correct IO state");
  
  // pinMode(doorSensorPin, INPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
  // pinMode(wifiLEDPin, OUTPUT);
}

void GDoorIO::actuateDoor(){
	Serial.println("DOOR IO: Actuating door");
	digitalWrite(relayPin, HIGH);

	// Wait for 1.5 seconds to properly pulse
	delay(pulseLength);

	// Set pin low again
	digitalWrite(relayPin, LOW);
	Serial.println("DOOR IO: Completed door pulse");
}

/*
*							Door State
*
*	 Returns the door state as according to the pins - need to 
*    check the pin staying high for 1 fulle second (to filter 
*    noice and stray capacitance)
*
*/

DoorState assessDoorState(){
	// Coming
}

