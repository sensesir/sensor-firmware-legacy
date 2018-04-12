/*
*	GPIO Class to handle all the pins and ISR triggers
*	Header file	
*
*	Author: Josh Perry
*	Copyright 2018
*
*/

#ifndef GDoorIO_h
#define DGoorIO_h

// Includes
#include <Arduino.h>
#include "../constants/Constants.h" 

// Class declaration

class GDoorIO{
	public:
		int doorSensorPin = 14;
		int relayPin 	  = 4;
		int wifiLEDPin 	  = 5;
		int hardResetPin  = 15;
		int pulseLength   = 1500;		// Milis

		// Unused exposed pins
		int uPin16 = 16;
		int uPin12 = 12;
		int uPin13 = 13;
		int uPin2  = 2;

		// Public methods
		GDoorIO();

		void setupGPIOPins();
		void actuateDoor();
		DoorState assessDoorState();
};



#endif
