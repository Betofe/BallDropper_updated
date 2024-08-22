/*
 Name:		Sketch1.ino
 Created:	10/18/2022 10:27:01 AM
 Author:	Imami Joel Betofe
*/

#include "MavlinkSettings.h"
#include "ReceiverSettings.h"
#include "ConfigurationSettings.h"

SetLinearActuatorAndLed actuatorLed_main;
Receiver receiver_main;
MavlinkConnection mavlink;

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);
	actuatorLed_main.init();
	mavlink.init();
}

// the loop function runs over and over again until power down or reset
void loop() {
	mavlink.run();
	receiver_main.run();
}

