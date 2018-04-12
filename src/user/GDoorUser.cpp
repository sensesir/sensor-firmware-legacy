/*
*	User data struct for storage of user related data
*	Source file
*
*	Author: Josh Perry
*	Copyright 2018
*/

#include "GDoorUser.hpp"

GDoorUser::GDoorUser(){
	Serial.println("Instantiating user object");

	// The constructure should set the instance vars - in future this will load from memory **
	// uid = "k4ozP54JHab6NrK8UU8HbskopPH3";
	// ssid = "Pez_is_awesome";     // "FutureLab 2";  					// 
	// password = "50Million2021!"; // "CapeTownS001"; 				   // 

	currentDoorState = DOOR_STATE_CLOSED; 
}

/*
 *				  Startup State Flow
 *	
 *	Attempt to load data from memory. If successful, 
 *  will load class vars and return true, if no data 
 *  - will return false
 *
 */

bool GDoorUser::loadUserData(){
	// Initialize EEPROM
	Serial.println("GDOOR USER: Reading user data from disk");
	EEPROM.begin(512);

	bool dataExists = 0;	
	byte startCR = EEPROM.read(0);
	byte startLF = EEPROM.read(1);

	if (startCR == CR && startLF == LF) {
		// Data store exists
		Serial.println("GDOOR USER: Successfully found data in memory. Loading data.");

		// Read in new data
		readUserDataFromDisk();
 		dataExists = 1;
	}

	else{
		Serial.println("GDOOR USER: Persisted data does not exist.");
		Serial.print("GDOOR USER: Read memory address data => ");
		Serial.print(startCR);
		Serial.println(startLF);
		dataExists = 0;
	}

	EEPROM.end();
	return dataExists;
 }

 /*
 *				Read User Data from Disk
 *	
 *	Attempt to load user data from memory.
 *
 *	** DISK INTERFACE PROTOCOL **
 *	Started mem address: 0x00
 *	Element 1: CRLF (Signifies data has been written)
 *  Element 2: uid <len = 28>
 *  Element 3: ssid <len = variable>
 *	Element 4: password <len = variable>
 *	Data delimiter: CRLF
 *
 */

 void GDoorUser::readUserDataFromDisk(){
 	// EEPROM shiuld be pre-initialized
 	int memAddress = 2;	// Data starts at address 2

 	const char* tesingChar;

 	// Read UID
 	uid = readDataIntoCharPointer(&memAddress);
 	delay(500);
 	Serial.print("GDOOR USER: Read UID => ");
 	Serial.print(uid);
 	Serial.println("");

 	// Testing
 	for(int j = 0; j < strlen(uid); j++){
 		Serial.print("Index = ");
 		Serial.print(j);
 		Serial.print("    Value = ");
 		Serial.println(uid[j]);
 	}

 	const char* test = "k4ozP54JHab6NrK8UU8HbskopPH3";

	for(int j = 0; j < strlen(test); j++){
 		Serial.print("Index = ");
 		Serial.print(j);
 		Serial.print("   Value = ");
 		Serial.println(test[j]);
 	} 	
 	
 	ssid = readDataIntoCharPointer(&memAddress);
 	delay(500);
 	Serial.print("GDOOR USER: Reading SSID => ");
 	Serial.print(ssid);
 	Serial.println("");

 	password = readDataIntoCharPointer(&memAddress);
 	delay(500);
 	Serial.print("GDOOR USER: Reading Password => ");
 	Serial.print(password);
 	Serial.println("");

 	Serial.print("GDOOR USER: Read ");
	Serial.print(memAddress);
	Serial.println(" bytes from disk");
 }

 const char* GDoorUser::readDataIntoCharPointer(int* addrPointer){

 	int loopLen = 512 - *addrPointer;
	Serial.println("");
	Serial.print("GDOOR USER: Reading from address = ");
	Serial.println(*addrPointer);

 	char dataArray[40];					// Testing size

 	for (int i = 0; i < loopLen; ++i) {
 	
 		// Read byte from memory
 		byte data = EEPROM.read(*addrPointer);
 		
 		if (data == CR) {
 			// Check for LF at next address
 			Serial.println("GDOOR USER: Found CR delimiter. ");
 			
 			*addrPointer += 1;
 			char delimiter = EEPROM.read(*addrPointer);

 			if (delimiter == LF){
 				Serial.print("GDOOR USER: Found LF delimiter. Address = ");
 				Serial.println(*addrPointer);

 				// Add string terminator
 				dataArray[i] = 92;		// ASCII = '\'
 				dataArray[i+1] = 48;		// ASCII = '0'
 				break;
 			} else{
 				// Read error
 				Serial.println("GDOOR USER: Read error - isolated delimitor");
 			}
 		}

 		dataArray[i] = data;		// Implicit cast byte to char
 		*addrPointer += 1;
 	}

 	// Data array 
 	Serial.print("Full Array = ");
	Serial.println(dataArray);
 	return &dataArray[0];
 }

/*
 *					Write to memory 
 *	
 *	Method will use the currently set class props to 
 *  EEPROM mem. The writing process writes single bytes 
 *  at a time, so it's critical to observe the read and 
 *  write protocol outlined below.
 *
 *	** DISK INTERFACE PROTOCOL **
 *	Started mem address: 0x00
 *	Element 1: CRLF (Signifies data has been written)
 *  Element 2: uid <len = 28>
 *  Element 3: ssid <len = variable>
 *	Element 4: password <len = variable>
 *	Data delimiter: CRLF
 *  
 */

void GDoorUser::persistUserDataToDisk() {
	// Initialize the EEPROM
	Serial.println("GDOOR USER: Writing user data to disk");
	EEPROM.begin(512);

	// Add CRLF
	EEPROM.write(0, CR);
	EEPROM.write(1, LF);

	// Memory address pointer
	int memAddress = 2;
	int* memAddrPointer = &memAddress;

	// Write the uid
	Serial.print("GDOOR USER: Writing UID to disk = ");
	writeCharArrayToDisk(uid, memAddrPointer);
	Serial.print("GDOOR USER: Writing SSID to disk = ");
	writeCharArrayToDisk(ssid, memAddrPointer);
	Serial.print("GDOOR USER: Writing Password to disk = ");
	writeCharArrayToDisk(password, memAddrPointer);

	// Commit the data
	EEPROM.end();
	Serial.print("GDOOR USER: Wrote ");
	Serial.print(memAddress);
	Serial.println(" bytes to disk");
}

void GDoorUser::writeCharArrayToDisk(const char* data, int* addrPointer){

	for (int i = 0; i < strlen(data); ++i) {
		EEPROM.write(*addrPointer, data[i]);
		*addrPointer += 1;
		Serial.print(data[i]);
	}

	// Add CRLF
	EEPROM.write(*addrPointer, CR);
	*addrPointer += 1;
	EEPROM.write(*addrPointer, LF);
	*addrPointer += 1;

	Serial.println("");
	Serial.print("GDOOR USER: Current memory address pointer = ");
	Serial.println(*addrPointer);
}













