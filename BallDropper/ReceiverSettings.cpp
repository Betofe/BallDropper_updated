// 
// 
// 

#include "ReceiverSettings.h"
#include "ConfigurationSettings.h"
#include "Globals.h"

SetLinearActuatorAndLed actuatorLed;

void Receiver::run() {
	/*Serial.println(chan9_pwm);
	delay(300);*/
	if ((chan9_pwm > 1900 && chan9_pwm < 2100)) {
		trigger = true;
	}
	actuatorLed.changePosition();
}