/*
*	Wrapper class to work with the ESP8266 Wifi 
*   library to establish a connection to a local wifi network
*	Header file
*
*	Author: Josh Perry
*	Copyright 2018
*
*/

#ifndef WIFIINTERFACE_H
#define WIFIINTERFACE_H

#include <ESP8266WiFi.h>
#include <Arduino.h>

// Static IP 

class GDoorWifi{
	public:
		void startWifiCredAcquisition(const char wifiPin);
		IPAddress initialWiFiConnection(const char* ssid, const char* password, const int* gatewayIPArr, const int* subnetIPArr, const int staticOctet, const char ledPin);
		IPAddress setWiFiReconnectingState();

	private:
		// Private constants
		int wifiLED;
		const char* currentSsid;
		const char* currentPassword;
		const int* currentGatewayIPArr;
		const int* currentSubnetIPArr;
		int currentStaticOctet;

		IPAddress connectToWifiWithStaticIP(const char* ssid, const char* password, const int* gatewayIPArr, const int* subnetIPArr, const int staticOctet);
		void setupStaticSensorIP(const int* gatewayIPArr, const int* subnetIPArr, const int staticOctet);
		IPAddress connectToWifi(const char* ssid, const char* password, const int* gatewayIPArr, const int* subnetIPArr, const int staticOctet);
		void toggleLED();
};




#endif