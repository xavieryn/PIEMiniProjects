#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
  
  
  Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
  Adafruit_DCMotor *leftMotor = AFMS.getMotor(2);
  Adafruit_DCMotor *rightMotor = AFMS.getMotor(1);
  
  // Defining Pins
  const int irSensorLeftPin = 2;
  const int irSensorLeftMiddlePin = 3;
  const int irSensorRightMiddlePin = 4;
  const int irSensorRightPin = 5;
  const int buttonPin = 6; // Starting the car

  String lastVal = "n";

  bool start = false; // set to true when button is pressed
  bool hitLeft = false;
  bool hitRight = false;

  const int slowSpeed = 30; // seems like 15 is the minimum because of friction
  const int turningSpeed = slowSpeed*1.3; // needs to turn a little faster because sometimes will stall out with just 30

void setup() {
  pinMode(2, INPUT); // left sensor
  pinMode(3, INPUT); // right sensor

  pinMode(buttonPin, INPUT); // Use internal pull-up resistor

  AFMS.begin(); // Start Arduino Shield
  Serial.begin(9600); // Initialize serial communication at 9600 baud

  // Intialize speed to 0
  leftMotor->setSpeed(0); 
  leftMotor->run(FORWARD); // these two are flipped, so one is forward and one is backward, but it results in it going forward
  rightMotor->setSpeed(0); 
  rightMotor->run(BACKWARD);
}

void loop() {
  // Read Sensor vals 
  int l = digitalRead(irSensorLeftPin);
  int ml = digitalRead(irSensorLeftMiddlePin);
  int mr = digitalRead(irSensorRightMiddlePin);
  int r = digitalRead(irSensorRightPin);
  // Serial.println("Left: " + String(l) + " - Middle:  " + String(ml) + " - Middle:  " + String(mr) + " - Right: " + String(r));
  // Serial.println(String(l) + "," + String(ml) + "," + String(mr) + "," + String(r));

  int buttonState = digitalRead(buttonPin); // read to see if car should be started up (permanently driving until reset button is pressed)
  if (buttonState == HIGH){
    start = true;
  }
  if (start == true) {
    drive();
  }
}

void drive(){
  int leftSensorVal = digitalRead(irSensorLeftPin);
  int middleLeftSensorVal = digitalRead(irSensorLeftMiddlePin);
  int middleRightSensorVal = digitalRead(irSensorRightMiddlePin);
  int rightSensorVal = digitalRead(irSensorRightPin);
  // hugging middle sensors to go straight
  if (middleLeftSensorVal == LOW && hitRight){ // now we need to start hugging 
      leftMotor->setSpeed(turningSpeed); // 0 - 255
      rightMotor->setSpeed(0);
      hitRight = false;
      // Serial.println("Going to middle right");
      lastVal = "MR->J"; 
  }
  else if (middleRightSensorVal == LOW && hitLeft){ // now we need to start hugging
      leftMotor->setSpeed(0); // 0 - 255
      rightMotor->setSpeed(turningSpeed);
      hitLeft = false;
      Serial.println("Going to middle left");
      lastVal = 2;
      lastVal = "ML->J"; 

  }
  else if (leftSensorVal == LOW){
      leftMotor->setSpeed(0); // normally is 0
      rightMotor->setSpeed(turningSpeed);
      hitLeft = true;
      //Serial.println("Going to middle right 2");
      lastVal = "L->MR"; 


   } else if (rightSensorVal == LOW){
      leftMotor->setSpeed(turningSpeed); // 0 - 255
      rightMotor->setSpeed(0); // normally is 0 
      //Serial.println("Going to middle left 2");
      hitRight = true;
      lastVal = "R->ML"; 

  } 
    // hugging middle sensors to go straight
  else if (middleLeftSensorVal == LOW && !hitLeft && !hitRight){ // now we need to start hugging (bouncing off middle two sensors until outer sensor gets hit)
      leftMotor->setSpeed(0); // 0 - 255
      rightMotor->setSpeed(turningSpeed);
      hitRight = false;
      //Serial.println("juggling");
      lastVal = "J"; 

  }
  else if (middleRightSensorVal == LOW && !hitLeft && !hitRight){ // now we need to start hugging (bounding off middle two sensors until outer sensor gets hit)
      leftMotor->setSpeed(turningSpeed); // 0 - 255
      rightMotor->setSpeed(0);
      hitLeft = false;
      //Serial.println("juggling");
      lastVal = "J"; 
  }
  Serial.println(String(leftSensorVal) + "," + String(middleLeftSensorVal) + "," + String(middleRightSensorVal) + "," + String(rightSensorVal) + "," + String(lastVal));



}
