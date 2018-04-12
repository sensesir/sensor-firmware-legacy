/*
*	User data struct for storage of user related data
*	Header file	
*
*	Author: Josh Perry
*	Copyright 2018
*/

#ifndef GDoorUser_h
#define GDoorUser_h

#include <Arduino.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include "../constants/Constants.h"

class GDoorUser{
	// User properties
	
	public:	
		// Constants 
		const char* firmwareVersion = "1.0.0";
		const byte CR = 13;
		const byte LF = 10;

		// Public properties
		const char* uid;
		const char* ssid;
		const char* password;
		IPAddress currentIPAddress; 

		DoorState currentDoorState;

		// Public methods
		GDoorUser();
		bool loadUserData();
		void persistUserDataToDisk();

	private:
		// Private methods
		void readUserDataFromDisk();
		const char* readDataIntoCharPointer(int* addrPointer);
		void writeCharArrayToDisk(const char* data, int* addrPointer);
};



#endif
