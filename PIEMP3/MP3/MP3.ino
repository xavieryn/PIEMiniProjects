#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
  
  
  Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
  Adafruit_DCMotor *leftMotor = AFMS.getMotor(2);
  Adafruit_DCMotor *rightMotor = AFMS.getMotor(1);

  const int irSensorLeftPin = 2;
  const int irSensorLeftMiddlePin = 3;
  const int irSensorRightMiddlePin = 4;
  const int irSensorRightPin = 5;

  const int buttonPin = 6;


  bool start = false; 

  bool hitLeft = false;
  bool hitRight = false;

  const int slowSpeed = 30; // seems like 15 is the minimum because of friction
  const int turningSpeed = slowSpeed*1.3;
  const int testSpeed = 20;
  const int delayVar = 100;

void setup() {
  pinMode(2, INPUT); // left sensor
  pinMode(3, INPUT); // right sensor

  pinMode(buttonPin, INPUT); // Use internal pull-up resistor

  AFMS.begin();

  Serial.begin(9600); // Initialize serial communication at 9600 baud
  leftMotor->setSpeed(0); // 0 - 255
  leftMotor->run(FORWARD); // these two are flipped, so one is forward and one is backward, but it results in it going forward

  rightMotor->setSpeed(0); // 0 - 255
  rightMotor->run(BACKWARD);
}

void loop() {
  int l = digitalRead(irSensorLeftPin);
  int ml = digitalRead(irSensorLeftMiddlePin);
  int mr = digitalRead(irSensorRightMiddlePin);
  int r = digitalRead(irSensorRightPin);
  Serial.println("Left: " + String(l) + " - Middle:  " + String(ml) + " - Middle:  " + String(mr) + " - Right: " + String(r));
  // Serial.println("____________________________________");
  int buttonState = digitalRead(buttonPin); // read to see if start button has been pressed
  if (buttonState == HIGH){
    start = true;
  }
  if (start == true) {
    //delay(1000);
    drive();
  }
}

void drive(){
  int leftSensorVal = digitalRead(irSensorLeftPin);
  int middleLeftSensorVal = digitalRead(irSensorLeftMiddlePin);
  int middleRightSensorVal = digitalRead(irSensorRightMiddlePin);
  int rightSensorVal = digitalRead(irSensorRightPin);
  Serial.println(hitLeft);
  // hugging middle sensors to go straight
  if (middleLeftSensorVal == LOW && hitRight){ // now we need to start hugging 
      leftMotor->setSpeed(turningSpeed); // 0 - 255
      rightMotor->setSpeed(0);
      hitRight = false;
      Serial.println("Going to middle right");
  }
  else if (middleRightSensorVal == LOW && hitLeft){ // now we need to start hugging
      leftMotor->setSpeed(0); // 0 - 255
      rightMotor->setSpeed(turningSpeed);
      hitLeft = false;
      Serial.println("Going to middle left");
  }
  // delay(delayVar);
  else if (leftSensorVal == LOW){
      leftMotor->setSpeed(0); // normally is 0
      rightMotor->setSpeed(turningSpeed);
      // Serial.println("Turning left");
      hitLeft = true;
      Serial.println("Going to middle right 2");

   } else if (rightSensorVal == LOW){
      leftMotor->setSpeed(turningSpeed); // 0 - 255
      rightMotor->setSpeed(0); // normally is 0 
        Serial.println("Going to middle left 2");

      hitRight = true;
  } 

    // hugging middle sensors to go straight
  else if (middleLeftSensorVal == LOW && !hitLeft && !hitRight){ // now we need to start hugging 
      leftMotor->setSpeed(0); // 0 - 255
      rightMotor->setSpeed(turningSpeed);
      hitRight = false;
      Serial.println("juggling");
  }
  else if (middleRightSensorVal == LOW && !hitLeft && !hitRight){ // now we need to start hugging
      leftMotor->setSpeed(turningSpeed); // 0 - 255
      rightMotor->setSpeed(0);
      hitLeft = false;
      Serial.println("juggling");
  }



}
