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

// Private constants
int wifiLED;

// Function prototypes
void setupStaticSensorIP(const int* gatewayIPArr, const int* subnetIPArr, const int staticOctet);
void toggleLED();

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

IPAddress connectToWifi(const char* ssid, const char* password, const int* gatewayIPArr, const int* subnetIPArr, const int staticOctet, const char ledPin){
	// Set the WiFi LED on
	wifiLED = ledPin;

	// Attempt to create static IP
	WiFi.mode(WIFI_STA);

	// Attempt to create static IP
	WiFi.begin(ssid, password);
	setupStaticSensorIP(gatewayIPArr, subnetIPArr, staticOctet);

	// Wait for a connection
	Serial.print("WIFI INTERFACE: Attempting to connect to WiFi => ");
	Serial.println(ssid);
	while (WiFi.status() != WL_CONNECTED){
		delay(500);
		Serial.print(".");
		toggleLED();
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

	// Indicate connected state on LED
	digitalWrite(wifiLED, HIGH);
	return ipAddress;
}

void setupStaticSensorIP(const int* gatewayIPArr, const int* subnetIPArr, const int staticOctet){
	// Create stati IP - 192.168.1.105
	IPAddress ip(gatewayIPArr[0], gatewayIPArr[1], gatewayIPArr[2], staticOctet);					// Virtual server: 192.168.8.105
	IPAddress subnet(subnetIPArr[0], subnetIPArr[1], subnetIPArr[2], subnetIPArr[3]);
	IPAddress gateway(gatewayIPArr[0], gatewayIPArr[1], gatewayIPArr[2], gatewayIPArr[3]);				// Router IP - gateway(192, 168, 8, 1); 	

	Serial.print("WIFI INTERFACE: Configuring static IP address => ");
	Serial.println(ip);
	WiFi.config(ip, gateway, subnet, gateway);
}

IPAddress setWiFiReconnectingState(){
	// Start a ticker at interval 500ms
	Serial.println("WIFI INTERFACE: WiFi connection dropped, attempting to reconnect");
	while (WiFi.status() != WL_CONNECTED){
		delay(500);
		Serial.print(".");
		toggleLED();
	}

	// Reconnected
	Serial.println("WIFI INTERFACE: Successfully reconnected");
	digitalWrite(wifiLED, HIGH);

	// Print and return the assigned local IP on reconnection
	delay(1000);
	IPAddress ipAddress = WiFi.localIP();
	Serial.print("WIFI INTERFACE: Assigned LAN IP address = ");
	Serial.println(ipAddress);
	return ipAddress;
}

void toggleLED(){
	int state = digitalRead(wifiLED);
	digitalWrite(wifiLED, !state);
}




