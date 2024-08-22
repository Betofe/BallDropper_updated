
#pragma once
#include<Arduino.h>

#define ENA_PIN  17
#define IN1_PIN	 5
#define IN2_PIN	 21
#define ENB_PIN	 17
#define IN3_PIN	 23
#define IN4_PIN	 22

#define ENA_PIN2 27
#define IN1_PIN2 26
#define IN2_PIN2 25
#define ENB_PIN2 27
#define IN3_PIN2 33
#define IN4_PIN2 32
#define UbecPin 13   
#define FREQUENCY 400
#define RESOLUTION 8
#define UbecChannel 0
#define MAX_PWM 253
#define MIN_PWM 10

class SetLinearActuatorAndLed {
private:
	
	int prestate;

public:

	void init();

	void changePosition();
};