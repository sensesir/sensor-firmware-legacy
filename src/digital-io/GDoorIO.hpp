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
		int doorSensorPin = 12;
		int relayPin = 2;
		int wifiLEDPin = 14;
		int pulseLength = 1500;		// Milis

		const char* dummyVar;

		// Public methods
		GDoorIO();

		void setupGPIOPins();
		void actuateDoor();
};



#endif
