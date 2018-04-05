/*
*	Class to handle Web Server creation 
*   and maintenance on ESP8266 chip
*	Source file	
*
*	Author: Josh Perry
*	Copyright 2018
*/

// Include
#include "WebServer.hpp"

// Implementation
WebServer::WebServer() {
	// Empty constructure - ntohing to do here
}


void WebServer::startHttpServer(const char* uid, IPAddress assignedIP){
	// Instantiate the server
	ESP8266WebServer server(80);

	// Constructor
	localIP = assignedIP;
	Serial.print("WEB SERVER: Listening on port 80 with IP:");
	Serial.println(localIP);

	// Setup up the callback functions
	char* targetURL = "/XXXXXXXXXXXXXXXXXXXXXXXXXXXX";	
	for (int i = 1; i < 29; i++){
	 	targetURL[i] = uid[i];
	} 

	// Configure the URL to listen for
	server.on("/", [&server]() {   //Define the handling function for the path
    	Serial.println("WEB SERVER: Received http req");
    	server.send(200, "text / plain", "Garage door listening loud and clear!");
  	});
	
	// Spin up the bugger!
	server.begin();
}


void WebServer::initiateDoorAction() {
	// Handler function triggered for incoming http req
	// Testing: 80/192.168.8.101

	Serial.println("WEB SERVER: Received http req");
}