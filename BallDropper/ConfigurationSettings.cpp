#include "ConfigurationSettings.h"
#include "ReceiverSettings.h"
#include "Globals.h"

Receiver receiver;

void SetLinearActuatorAndLed::init() {
   
    prestate = 0; //Initializing prestate
    ledcAttachPin(UbecPin, UbecChannel);
    ledcSetup(UbecChannel, FREQUENCY, RESOLUTION);
   
    pinMode(ENA_PIN, OUTPUT);
    pinMode(IN1_PIN, OUTPUT);
    pinMode(IN2_PIN, OUTPUT);

    pinMode(ENB_PIN, OUTPUT);
    pinMode(IN3_PIN, OUTPUT);
    pinMode(IN4_PIN, OUTPUT);

    pinMode(ENA_PIN2, OUTPUT);
    pinMode(IN1_PIN2, OUTPUT);
    pinMode(IN2_PIN2, OUTPUT);

    pinMode(ENB_PIN2, OUTPUT);
    pinMode(IN3_PIN2, OUTPUT);
    pinMode(IN4_PIN2, OUTPUT);


    //initializing state of EN
    digitalWrite(ENA_PIN, HIGH);
    digitalWrite(ENB_PIN, HIGH);
    digitalWrite(ENA_PIN2, HIGH);
    digitalWrite(ENB_PIN2, HIGH);
    ledcWrite(UbecChannel, MIN_PWM);
    delay(10000);
    ledcWrite(UbecChannel, MAX_PWM);
    delay(10000);
    ledcWrite(UbecChannel, MIN_PWM);
}

void SetLinearActuatorAndLed::changePosition() {
    
    if ((trigger == true)) {
        switch (prestate)
        {
        case 0:
            // retract the actuator 1
            digitalWrite(IN1_PIN, LOW);
            digitalWrite(IN2_PIN, HIGH);
            delay(3000);
            prestate = 1;
            Serial.print(prestate);
            delay(2000);
            digitalWrite(IN1_PIN, LOW);
            digitalWrite(IN2_PIN, LOW);
            break;
        case 1:
            // retract the actuator 2

            digitalWrite(IN1_PIN2, LOW);
            digitalWrite(IN2_PIN2, HIGH);
            Serial.print(prestate);
            delay(3000);
            digitalWrite(IN1_PIN2, LOW);
            digitalWrite(IN2_PIN2, LOW);

            // extend the actuator 1
            digitalWrite(IN1_PIN, HIGH);
            digitalWrite(IN2_PIN, LOW);
            prestate = 2;
            Serial.print(prestate);
            delay(2000);
            digitalWrite(IN1_PIN, LOW);
            digitalWrite(IN2_PIN, LOW);
            break;
        case 2:
            // retract the actuator 3

            digitalWrite(IN3_PIN, HIGH);
            digitalWrite(IN4_PIN, LOW);
            Serial.print(prestate);
            delay(3000);
            digitalWrite(IN3_PIN, LOW);
            digitalWrite(IN4_PIN, LOW);

            // extend the actuator 2
            digitalWrite(IN1_PIN2, HIGH);
            digitalWrite(IN2_PIN2, LOW);
            prestate = 3;
            Serial.print(prestate);
            delay(2000);
            digitalWrite(IN1_PIN2, LOW);
            digitalWrite(IN2_PIN2, LOW);
            break;
        case 3:
            // retract the actuator 4
            digitalWrite(IN3_PIN2, HIGH);
            digitalWrite(IN4_PIN2, LOW);
            Serial.print(prestate);
            delay(3000);
            digitalWrite(IN3_PIN2, LOW);
            digitalWrite(IN4_PIN2, LOW);

            // extend the actuator 3
            digitalWrite(IN3_PIN, LOW);
            digitalWrite(IN4_PIN, HIGH);
            prestate = 4;
            Serial.print(prestate);
            delay(2000);
            digitalWrite(IN3_PIN, LOW);
            digitalWrite(IN4_PIN, LOW);
            break;
        case 4:
            // retracts the actuator 4
            digitalWrite(IN3_PIN2, LOW);
            digitalWrite(IN4_PIN2, HIGH);
            prestate = 0;
            Serial.print(prestate);
            delay(2000);
            digitalWrite(IN3_PIN2, LOW);
            digitalWrite(IN4_PIN2, LOW);
            break;
        default:
            trigger = false;
            break;
        }

        trigger = false;

    }

}