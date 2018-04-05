/*
*	Class to handle Web Server creation 
*   and maintenance on ESP8266 chip
*	Header file	
*
*	Author: Josh Perry
*	Copyright 2018
*/

#ifndef WebServer_h
#define WebServer_h

// Includes
#include <ESP8266WebServer.h>
#include <Arduino.h>

class WebServer {
	// Class interface
	public:
		// Instance variables
		ESP8266WebServer server;
		IPAddress localIP;

		// constructor 
		WebServer();

		// General functions
		void startHttpServer(const char* uid, IPAddress assignedIP);

	private:
		void initiateDoorAction();
};



#endif
