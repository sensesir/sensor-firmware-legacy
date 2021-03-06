/*
*  Main file for controlling code and execution on 
*  the ESP8266 for the garage sensor project
*  Project file
*
* Author: Josh Perry
* Copyright 2018
*
*/

// Extern libs
#include "ESP8266WebServer.h"

// Internal files
#include "src/constants/Constants.h"
#include "src/wifi-interface/WifiInterface.hpp"
#include "src/user/GDoorUser.hpp"   
#include "src/networking/HttpInterface.hpp"  
#include "src/digital-io/GDoorIO.hpp"   
// #include "src/networking/WebServer.hpp"        
// --> Web Server class doesn't work, could be that strong reference isn't maintained to server object

// Firmware constants
const char* firmWVersion = "1.0.0";   // Weird name because of namespace conflicts
const int portNumber = 6969;
const char* portNumberStr = "6969";
const unsigned long healthCheckInterval = 900000; // 15 mins in milliseconds (for health ping)

// Global vars
GDoorUser user;
GDoorIO doorIO;
GDoorWifi wifiInterface;
ESP8266WebServer server(portNumber);
unsigned long currentMillis = 1;        //make sure they don't equate on first check
unsigned long previousMillis = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Firing up Esp!!");

  // Set up GPIO pins
  doorIO.setupGPIOPins();
  delay(3000);

  // Attempt to load user data from disk
  bool loadSuccess = user.loadUserData();
  if (!loadSuccess){
    // Start WiFi setup mode
    wifiInterface.startWifiCredAcquisition(doorIO.wifiLEDPin);
  }
 
  // Connect to wifi
  user.currentIPAddress = wifiInterface.initialWiFiConnection(user.ssid, user.password, user.gatewayIPArr, user.subnetMaskIpArr, user.espStaticOctet, doorIO.wifiLEDPin);                       
  delay(500); 

  // Write boot updates to DB & initial door state
  user.createIPStrings();
  uploadBootInfo(portNumberStr, user.uid, firmWVersion, user.gatewayIPStr, user.assignedIPStr);
  delay(500);  
  sendInitialDoorState();

  // Set up the server
  serverSetup();
  
  // Take a breath before we enter the loop
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  assessDoorState();
  server.handleClient();
  healthCheckTimeQuery();

  // Check connectivity state
  if (WiFi.status() != WL_CONNECTED){
    handleWifiReconProcedure();
  }
}

/*
 *                Health Ping Update
 * 
 *  Handler function to assess the current millies period  
 *  - accounting for roll over. If the current period exceeds 
 *  the set interval, a health ping is sent to the server.
 * 
 */

void healthCheckTimeQuery(){
  currentMillis = millis();
  unsigned long millisDiff = currentMillis - previousMillis;

  if(millisDiff > healthCheckInterval) {
    // Need to do a health update  
    Serial.println("MAIN: Health check time - hitting API");

    // Update server
    sendHealthCheckUpdate(currentMillis, user.uid);

    // Update the previous time checkpoint
    previousMillis = currentMillis;
  } 

  else if(currentMillis < previousMillis){
    // Roll over --> Need to reset
    Serial.println("MAIN: Millis rollover - resetting counters");
    previousMillis = 0;
  }
}

void handleWifiReconProcedure(){
  user.currentIPAddress = wifiInterface.setWiFiReconnectingState();
  user.createIPStrings();
  delay(500); 

  // Reset millis - no need for a health check immediately after recon not
  previousMillis = millis();
  
  // Indicate to the server that the sensor is back online
  sendReconnectionNotification(user.uid, user.assignedIPStr);
  delay(500); 
}

/*
 *                GPIO Handling
 * 
 * This section contains all code related to GPIO 
 * pins and their handling
 * 
 */

void sendInitialDoorState(){
  // Update the user data
  DoorState currentState = doorIO.assessDoorState();
  user.currentDoorState = currentState;
  
  if (currentState == DOOR_STATE_OPEN){
    Serial.println("MAIN: Door status changed to OPEN");
    sendUpdateForState(DOOR_STATE_OPEN, user.uid);
  }

  else{
    Serial.println("MAIN: Door status changed to CLOSED");
    sendUpdateForState(DOOR_STATE_CLOSED, user.uid);
  }
}
  
void assessDoorState() {
  // Only read the pin state if the previous value was high
  DoorState currentState = doorIO.assessDoorState();
  if (currentState == user.currentDoorState){
      // No update required
      return;
  }

  // Update the user data
  user.currentDoorState = currentState;
  
  if (currentState == DOOR_STATE_OPEN){
    Serial.println("MAIN: Door status changed to OPEN");
    sendUpdateForState(DOOR_STATE_OPEN, user.uid);
  }

  else{
    Serial.println("MAIN: Door status changed to CLOSED");
    sendUpdateForState(DOOR_STATE_CLOSED, user.uid);
  }
}

/*
 *                  Server Setup & Handling
 * 
 *    This section contains all code related to creating a server, 
 *    managing it, and responding to requests.
 *    
 *    The code for this was meant to be in a self contained class, 
 *    but that doesn't seem to respond to http GET req.
 * 
 */

void serverSetup(){

  // Attach listeners for all the endpoints
  server.on("/", handleRoot);
  server.on(actuateDoorEndpoint(), actuateDoor);
  server.on(healthCheckEndpoint(), respondToHealthCheck);
  server.on(forceDoorStatusCheckEndpoint(), sendDoorStatus);
  server.on(forceHealthCheckEndpoint(), respondToForcedHealthCheck);

  // Start the server
  server.begin();
}


/*
 *            * Handlers *
 * 
 */

void handleRoot() {
  Serial.println("SERVER: Received base API req");
  server.send(200, "text/plain", "Hello, Garage Door here. How can I help you paranoid human?");
}

void actuateDoor(){
  Serial.println("SERVER: Recieved req to actuate the door. On it.");
  server.send(200, "text/plain", "Hello, actuating door as per request.");

  // Actuate door
  doorIO.actuateDoor();
}

void respondToHealthCheck(){
  // Respond with connection time here (time stamp)
  Serial.println("SERVER: Received health inquery");
  server.send(200, "text/plain", "1522963577");
}

void sendDoorStatus(){
  Serial.println("SERVER: Queried about door status");
  const char* doorStateStr = user.currentDoorState == DOOR_STATE_OPEN ? "00" : "01";
  server.send(200, "text/plain", doorStateStr);
}

void respondToForcedHealthCheck(){
  // Nothing for now
  Serial.println("SERVER: Received http req to update health (forced check)");
  server.send(200, "text/plain", "1522963577");
}











/*
 *            * Utility methods *
 * 
 */

char*  actuateDoorEndpoint(){
  // All endpoints have the form: ~/localIP/XXXXXXXXXXXXXXXXXXXXXXXXXXXX/EndPointName 
  // Where XXX..XXX = user UID, and EndPoint name is as expected
  // Actuate Door end point
  char *actuateDoorStr = "/XXXXXXXXXXXXXXXXXXXXXXXXXXXX/ActuateDoor";
  for (int i = 0; i < strlen(actuateDoorStr); i++){
    if (i < 28){
      actuateDoorStr[i+1] = user.uid[i];
    }

    else{
      actuateDoorStr[i+1] = actuateDoorStr[i+1];
    }

    // Serial.println(i);
  }

  Serial.print("SERVER: Registering endpoint = ");
  Serial.println(actuateDoorStr);
  return actuateDoorStr;
}

char* healthCheckEndpoint(){
  // Health Check
  char* healthCheckStr = "/XXXXXXXXXXXXXXXXXXXXXXXXXXXX/HealthCheck";
  for (int i = 0; i<29; i++){
    if (i < 28){
      healthCheckStr[i+1] = user.uid[i];
    }

    else{
      healthCheckStr[i+1] = healthCheckStr[i+1];
    }
  }

  Serial.print("SERVER: Registering endpoint = ");
  Serial.println(healthCheckStr);
  return healthCheckStr;
}

char* forceDoorStatusCheckEndpoint(){
  
  char *forceDoorStatusCheckStr = "/XXXXXXXXXXXXXXXXXXXXXXXXXXXX/ForceDoorStatusCheck";
  for (int i = 0; i<29; i++){
    if (i < 28){
      forceDoorStatusCheckStr[i+1] = user.uid[i];
    }

    else{
      forceDoorStatusCheckStr[i+1] = forceDoorStatusCheckStr[i+1];
    }
  }

  Serial.print("SERVER: Registering endpoint = ");
  Serial.println(forceDoorStatusCheckStr);
  return forceDoorStatusCheckStr;
}

char* forceHealthCheckEndpoint(){
  // Health Check
  char *forceHealthCheckStr = "/XXXXXXXXXXXXXXXXXXXXXXXXXXXX/ForceHealthCheck";
  for (int i = 0; i< strlen(forceHealthCheckStr); i++){
    if (i < 28){
      forceHealthCheckStr[i+1] = user.uid[i];
    }

    else{
      forceHealthCheckStr[i+1] = forceHealthCheckStr[i+1];
    }
  }

  Serial.print("SERVER: Registering endpoint = ");
  Serial.println(forceHealthCheckStr);
  return forceHealthCheckStr;
}


/*      Testing Endpoints
 *  
 *  Remote test: {Remote IP}:6969/endpoint
 *  1. 192.143.147.232:6969/k4ozP54JHab6NrK8UU8HbskopPH3/ActuateDoor
 *  - 192.143.10.217:6969/k4ozP54JHab6NrK8UU8HbskopPH3/ActuateDoor
 *  
 *  1. 10.0.1.41/k4ozP54JHab6NrK8UU8HbskopPH3/ActuateDoor                 || 
 *  2. 10.0.1.41/2XsXatJ4DgdltTMcIcrC146aUNZ2/HealthCheck
 *  3. 10.0.1.41/2XsXatJ4DgdltTMcIcrC146aUNZ2/ForceDoorStatusCheck
 *  4. 10.0.1.41/2XsXatJ4DgdltTMcIcrC146aUNZ2/ForceHealthCheck
 *  
 *  Error: 10.0.1.41/XsXatJ4DgdltTMcIcrC146aUNZ2    http://hailmary.local
 *  
 */














