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
	uid = "2XsXatJ4DgdltTMcIcrC146aUNZ2";
	ssid = "FutureLab 2";
	password = "CapeTownS001";

	currentDoorState = DOOR_STATE_CLOSED; 
}


