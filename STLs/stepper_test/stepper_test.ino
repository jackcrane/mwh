#include <Wire.h>
#include <Adafruit_MotorShield.h>
//#include "utility/Adafruit_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);

Adafruit_StepperMotor *myMotor1 = AFMS.getStepper(200, 1);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Stepper Test");

  AFMS.begin();

  myMotor->setSpeed(40);

  myMotor1->setSpeed(40);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.write("forward");
  myMotor->step(100, FORWARD, SINGLE);
  myMotor1->step(100, FORWARD, SINGLE);
  Serial.write("backward");
  myMotor->step(100, BACKWARD, SINGLE);
  myMotor1->step(100, BACKWARD, SINGLE);
}
