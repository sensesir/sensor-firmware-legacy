/*
*	HTTP Client Library for use with ESP8266
*	Source file	
*
*	Author: Josh Perry
*	Copyright 2018
*
*/

// Includes
#include "HTTPInterface.hpp"

// Function prototypes
const char* createHttpJson(const char* stateStr, const char* uid);

/*
 *                      Update Door State
 *  Function takes in a door state, creates an HTTP POST request 
 *  for the cloud functions API endpoint & logs the response
*/

void sendUpdateForState(DoorState newState, const char* senderUID){
   // Set the string to send with the http
   const char* statusStr = (newState == DOOR_STATE_OPEN) ? "00" : "01";
   Serial.print("Creating http request to change door state to: ");
   Serial.println(statusStr);
   
   // Create the HTTP client class
   HTTPClient httpReq;

   // Payload creation
   String payload = createHttpJson(statusStr, senderUID);                                                                           // "{\"uid\":\"2XsXatJ4DgdltTMcIcrC146aUNZ2\",\"statusUpdate\":\"open\"}";

   httpReq.begin("http://us-central1-iot-za.cloudfunctions.net/doorStatusUpdate");                  // "https://us-central1-iot-za.cloudfunctions.net/doorStatusUpdate"
   httpReq.addHeader("Content-Type", "text/plain");
   
   // Send the post
   int resCode = httpReq.POST(payload);      // {'uid':'2XsXatJ4DgdltTMcIcrC146aUNZ2','statusUpdate':'open'}

   // Look for the response & examine 
   String resString = httpReq.getString();
   Serial.println();
   Serial.print("Receieved response to HTTP POST:  ");
   Serial.println(resCode);
   Serial.println(resString);

   // Close connection
   httpReq.end();
}

// Utility functions

const char* createHttpJson(const char* stateStr, const char* uid){
  // Multiple concats to achieve;
  // Target with backslashes: "{\"uid\":\"XXXXXXXXXXXXXXXXXXXXXXXXXXXX\",\"statusUpdate\":\"00\"}" Length = 58
  Serial.print("Creating payload string for user with uid:");
  Serial.println(uid);

  // Create dummy array to mutate
  char *payload = "{\"uid\":\"XXXXXXXXXXXXXXXXXXXXXXXXXXXX\",\"statusUpdate\":\"00\"}";
  for (int i = 0; i < strlen(uid); i++){
    // Need to overwrite UID placeholder:
    payload[i + 8] = uid[i];
  }

  // Overwrite status bits
  payload[54] = stateStr[0]; 
  payload[55] = stateStr[1]; 

  Serial.print("Sending json payload:");
  Serial.println(payload);
  return payload;
}













