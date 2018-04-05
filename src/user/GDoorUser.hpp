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
#include <ESP8266WiFi.h>
#include "../constants/Constants.h"

class GDoorUser{
	// User properties
	
	public:	
		// Public properties
		const char* uid;
		const char* ssid;
		const char* password;
		IPAddress currentIPAddress; 

		DoorState currentDoorState;

		// Public methods
		GDoorUser();
};



#endif
