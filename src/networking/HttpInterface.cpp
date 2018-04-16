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
void addKeyValueJSONStringEntry(const char* key, const char* value, char* target, int* index);

/*
 *                            Upload Boot Info
 *
 *    Sends required info to server to be passed to DB for communication 
 *    with ESP JSON string is loaded individually function by function.
 *    
 *    Information sent:
 *
 *      1) Server port number
 *      2) Sensor recorderd UID
 *      3) Sensor firmware version
 *  
*/

void uploadBootInfo(const char* portNum, const char* recordedUID, const char* firmwareVersion, const char* targetStaticIP, const char* assignedLocalIP){
  // Create the POST request
  Serial.print("HTTP INTERFACE: Sending boot info to server.");

  // Create the JSON string
  char payload[250];
  sprintf(payload, "{\"serverPort\":\"%s\",\"uid\":\"%s\",\"firmwareVersion\":\"%s\",\"targetStaticIP\":\"%s\",\"assignedLocalIP\":\"%s\"}", portNum, recordedUID, firmwareVersion, targetStaticIP, assignedLocalIP);

  // Have a look at the end product
  Serial.print("HTTP INTERFACE: JSON payload = ");
  Serial.println(payload);

  // Create the HTTP post req
  HTTPClient httpReq;
  httpReq.begin("http://us-central1-iot-za.cloudfunctions.net/sensorBootData");  
  httpReq.addHeader("Content-Type", "text/plain");

  int resCode = httpReq.POST(payload);

  // Look for the response & examine 
   String resString = httpReq.getString();
   Serial.println();
   Serial.print("Receieved response to HTTP POST:  ");
   Serial.println(resCode);
   Serial.println(resString);

   // Close connection
   httpReq.end();
}


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

/*
 *                      Update Door Health
 *
 *  Function sends an http post to the server health check API 
 *  endpoint. The payload the current millis count. 
 * 
*/

void sendHealthCheckUpdate(unsigned long currentMillis, const char* senderUID){
  // Create the POST request
  Serial.print("HTTP INTERFACE: Sending health update to server.");

  // Create the JSON string
  char payload[150];
  sprintf(payload, "{\"uid\":\"%s\",\"millis\":\"%lu\"}", senderUID, currentMillis);

  // Have a look at the end product
  Serial.print("HTTP INTERFACE: JSON payload = ");
  Serial.println(payload);

  // Create the HTTP post req
  HTTPClient httpReq;
  httpReq.begin("http://us-central1-iot-za.cloudfunctions.net/sensorHealthUpdate");  
  httpReq.addHeader("Content-Type", "text/plain");

  int resCode = httpReq.POST(payload);

  // Look for the response & examine 
   String resString = httpReq.getString();
   Serial.println();
   Serial.print("Receieved response to HTTP POST:  ");
   Serial.println(resCode);
   Serial.println(resString);

   // Close connection
   httpReq.end();
}

/*
 *                         Health check response
 *
 *  In standard operation, a nominally performing sensor will respond to 
 *  periodic pings from the Firebase server (health checks). In these health 
 *  checks, the chip should respond with 2 things: 1) The door status, 2) The 
 *  timestamp of when it's connection to the internet was established (stored 
 *  on the user object)
 *   
*/



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

void addKeyValueJSONStringEntry(const char* key, const char* value, char* target, int* index){
    // Add the key first
    target[*index] = *"\"";
    *index += 1;

    // Add they key
    for (int i = 0; i < strlen(key); i++){
      target[*index] = key[i];
      *index += 1;
    }

    target[*index] = *"\"";
    target[*index + 1] = *":";
    target[*index + 2] = *"\"";
    *index += 3;

    // Add the value
    for (int j = 0; j < strlen(value); j++){
      target[*index] = value[j];
      *index +=1;
    }

    // Finish entry with "
    target[*index] = *"\"";
    *index += 1;
}













