/*
*	HTTP Client Library for use with ESP8266
*	Header file	
*
*	Author: Josh Perry
*	Copyright 2018
*
*/

#ifndef HTTPInterface_h
#define HTTPInterface_h

// Includes
#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include "../constants/Constants.h"                    // ../constants/

// Global constants
// const char* remoteIPQuery = "checkip.dyndns.org";

// Function declarations
void sendUpdateForState(DoorState newState, const char* senderUID);
void uploadBootInfo(const char* portNum, const char* recordedUID, const char* firmwareVersion, const char* targetStaticIP, const char* assignedLocalIP);
void sendHealthCheckUpdate(unsigned long currentMillis, const char* senderUID);

#endif
