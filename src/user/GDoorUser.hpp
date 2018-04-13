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
		const byte CR = 13;
		const byte LF = 10;

		// Public properties
		char uid[29];
		char ssid[40];
		char password[40];
		DoorState currentDoorState;

		// Networking props
		IPAddress currentIPAddress; 
		int gatewayIPArr[4];		
		int subnetMaskIpArr[4];
		int espStaticOctet = 250;				// May have to make this dynamic in future (The irony - haha)
		char gatewayIPStr[20];
		char assignedIPStr[20];

		// Public methods
		GDoorUser();
		bool loadUserData();
		void persistUserDataToDisk();
		void createIPStrings();

	private:
		// Private methods
		void readUserDataFromDisk();
		void readDataIntoCharPointer(int* addrPointer, char* target);
		void readDataIntoIntPointer(int* addrPointer, int* target);
		void writeCharArrayToDisk(const char* data, int* addrPointer);
		void writeIntArrayToDisk(int* data, int* addrPointer, int arrayLength);
		void printIPIntArray(int* data);
};



#endif
