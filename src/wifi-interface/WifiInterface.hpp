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


// Methods
bool connectToWifi(const char* ssid, const char* password);


#endif