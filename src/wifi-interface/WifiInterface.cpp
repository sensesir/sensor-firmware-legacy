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

// Function prototypes
void setupStaticSensorIP();

// Implementation

void startWifiCredAcquisition(const char wifiPin){
	// Wrapper method to start 
	Serial.println("Initiating WiFi set up");

	// For now, just start infinite loop and flash wifi light on and off
	while(1){
		digitalWrite(wifiPin, HIGH);
		delay(1000);
		digitalWrite(wifiPin, LOW);
		delay(1000);
	}
}

IPAddress connectToWifi(const char* ssid, const char* password){
	// Uses the currently set instance vars 
	// Attempt to create static IP
	WiFi.mode(WIFI_STA);

	// Attempt to create static IP
	WiFi.begin(ssid, password);
	setupStaticSensorIP();

	// Wait for a connection
	Serial.println("WIFI INTERFACE: Attempting to connect to WiFi");
	while (WiFi.status() != WL_CONNECTED){
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.print("WIFI INTERFACE: Connected to ");
	Serial.println(ssid);

	// State all info
	delay(1000);
	IPAddress ipAddress = WiFi.localIP();
	Serial.print("WIFI INTERFACE: Assigned IP address = ");
	Serial.println(ipAddress);
	Serial.print("WIFI INTERFACE: Gateway IP = ");
	Serial.println(WiFi.gatewayIP());
	Serial.print("WIFI INTERFACE: Subnet mask = ");
	Serial.println(WiFi.subnetMask());

	return ipAddress;
}

void setupStaticSensorIP(){
	// Create stati IP - 192.168.1.105
	IPAddress ip(192, 168, 8, 250);					// Virtual server: 192.168.8.105
	IPAddress subnet(255, 255, 255, 0);
	IPAddress gateway(192, 168, 8, 1);				// Router IP - gateway(192, 168, 8, 1); 	

	Serial.print("WIFI INTERFACE: Configuring static IP address => ");
	Serial.println(ip);
	WiFi.config(ip, gateway, subnet, gateway);
}




