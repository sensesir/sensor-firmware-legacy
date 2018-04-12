/*
	Garage Door interface class - to handle all interfaces with the 
	Web Server running on the ESP8266 on premises at people's houses
	
	Author: Josh Perry
	Copyright Josh Perry 2018

	// Testing:
	curl -X POST -H "Content-Type:application/json" -H "X-MyHeader: 123" https://us-central1-iot-za.cloudfunctions.net/doorStatusUpdate?foo=baz -d '{"statusUpdate":"open", "uid": "2XsXatJ4DgdltTMcIcrC146aUNZ2"}'
	curl -X POST -H "Content-Type:application/json" -H "X-MyHeader: 123" http://us-central1-iot-za.cloudfunctions.net/doorStatusUpdate -d '{"statusUpdate":"closed", "uid": "2XsXatJ4DgdltTMcIcrC146aUNZ2"}'
	curl -X POST -H "Content-Type:text/plain" -H "X-MyHeader: 123" http://us-central1-iot-za.cloudfunctions.net/doorStatusUpdate -d "{'statusUpdate':'closed', 'uid': '2XsXatJ4DgdltTMcIcrC146aUNZ2'}"
*/

const admin = require('firebase-admin');
const request = require('request');

// Door command constants
const actuateDoorCommand = "/ActuateDoor"
const healthCheckCommand = "/HealthCheck"
const forcedHealthCheckCommand = "/ForceHealthCheck"
const forceDoorStatusCheckCommand = "/ForceDoorStatusCheck"	 

class DoorInterface{

	constructor(){}

	/*	
	 *						Update Door Status
	 *
	 *	The req and res objects come from an http req from the 
	 *  ESP8266 Web Server. The req object should contain the UID 
	 *  of the sender, if it does not, we should record an error, 
	 *  otherwise a database write is performed to update the door 
	 *  status
	 *
	 */

	updateDoorStatus(req, res){
		
		console.log('DOOR INTERFACE: Attempting updating door status.');
		console.log(`Incoming req body: ${req.body}`);

		// Get vars from req object - comes as plain text
		const payload = JSON.parse(req.body);
		const statusCode = Number(payload.statusUpdate);
		const senderUID = payload.uid;

		// Check the credential of the requester
		if((typeof senderUID !== 'string') && !(senderUID.length === 28)){
			const credError = ERROR("DOOR INTERFACE: Invalid credentials on door status update request - req object:" + JSON.stringify(req));
			this.errorUpdatingDoorStatus(credError);
			
			return;
		}

		// Check the validity of the update
		else if (statusCode > 100 || statusCode < 0){
			const dataError = ERROR("DOOR INTERFACE: Invalid update data in req - req object:" + JSON.stringify(req));
			this.errorUpdatingDoorStatus(dataError);

			return;
		}

		// All things nominal invoke a DB write
		admin.database().ref(`/users/${senderUID}/doorState`).set(statusCode)
			.then(() => {
				// Successfully update door status
				console.log(`DOOR INTERFACE: Successfully updated door status to ${statusCode} for user ${senderUID}`);
				res.send({
							result:  "OK",
						  	message: "Updated door status successfully"
						});

				// Update the door's IP after this request
				this.updateSensorRemoteIP(senderUID, req);
				return;
			})

			.catch(error => {
				// Couldn't perform update 
				console.log(`DOOR INTERFACE: Couldn't update door status in DB for valid request from ${senderUID} for status = ${statusCode}`);
			});
	}

	/*
	 *				  	   Send door trigger API hit
	 *
	 *	  Method will user the information to construct the required http 
	 *    request to hit the ESP server, and trigger the door
	 *
	*/

	sendDoorTriggerReq(senderUID, remoteIP, port){
		
		return new Promise((resolve, reject) => {
			// Construct the URL 
			// Format: http://RemoteIP:port/UID/Command
			const httpURL = "http://" + remoteIP + ":" + port + "/" + senderUID + "/" + actuateDoorCommand;

			// Create http request and send
			request(httpURL, (error, response, body) => {
				// Completion handler
				if (response.statusCode === 200 && !error) {
					// Door actuated successfully
					console.log("DOOR INTERFACE: Received reply from door after actuate command");
					return resolve(true);
				} else {
					console.log("DOOR INTERFACE: Failed to send actuate command to door");
					return reject(error);
				}
			});
		});
	}

	/*
	 *				  	  Update IP Address of sender
	 *
	 *	  Method will update the database record for the sending user with 
	 *    the latest remote IP address of the garage door sensor
	 *
	*/

	updateSensorRemoteIP(senderUID, req){
		console.log(`DOOR INTERFACE: Updating remote IP address record for user ${senderUID}`);

		// Look for IP
		const ipData = req.headers['x-forwarded-for'];
		var currentIP;
		
		if (typeof(ipData) === 'string') {
			currentIP = ipData;
		}

		else if (Array.isArray(ipData)){
			console.log("WARNING: IP Address data came in as array **");
			currentIP = ipData[0];
		}

		else{
			console.log(`ERROR: Unknown IP Data type = ${ipData}`);
			return;
		}

		// Create update 
		const timeStamp = Date.now();
		const ipUpdate = {lastIPUpdate: timeStamp,
						   remoteIPAddress: currentIP};

		// Create database Ref & update the KV pairs
		const userDBNode = admin.database().ref(`/users/${senderUID}`).update(ipUpdate)
			.then(() => {
				console.log("DOOR INTERFACE: Successfully updated sensor's remote IP");
				return;
			})

			.catch(error => {
				// Could trigger forced health check here ?? 
				console.log("DOOR INTERFACE: ERROR!! Failed to updat sensor's remote IP");
			});
	}

	/*
	 *				  Get IP Address of sender
	 *
	 *	Method will query the request object and scan for the IP 
	 *  address of the sender.
	 *
	*/	

	getRemoteIPFromReq(req){
		const ipData = req.headers['x-forwarded-for'];
		console.log(`DOOR INTERFACE: Extracted IP address data = ${ipData}`);
		return ipData;
	}

	
	/*
	 *		Log error for requesr
	 *
	 *
	*/

	errorUpdatingDoorStatus(error){
		console.log(error.message);
		res.send({
							result:  "ERROR",
						  	message: error.message
						});
	}
}

module.exports = DoorInterface;