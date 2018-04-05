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

// Function declarations
void sendUpdateForState(DoorState newState, const char* senderUID);

#endif
