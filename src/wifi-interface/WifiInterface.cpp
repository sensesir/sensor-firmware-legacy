/*
*	Wrapper class to work with the ESP8266 Wifi 
*   library to establish a connection to a local wifi network
*	Source file	
*
*	Author: Josh Perry
*	Copyright 2018
*
*/


#include "WifiInterface.hpp"

// Implementation

bool connectToWifi(const char* ssid, const char* password){
	// Uses the currently set instance vars 
	WiFi.begin(ssid, password);
	Serial.println("WIFI INTERFACE: Attempting to connect to WiFi");

	// Wait for a connection
	while (WiFi.status() != WL_CONNECTED){
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.print("WIFI INTERFACE: Connected to ");
	Serial.println(ssid);

	return true;
}