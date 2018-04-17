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
// const char* staticIP = "192.168.1.105";

// Methods
void startWifiCredAcquisition(const char wifiPin);
IPAddress connectToWifi(const char* ssid, const char* password, const int* gatewayIPArr, const int* subnetIPArr, const int staticOctet, const char ledPin);
IPAddress setWiFiReconnectingState();


#endif