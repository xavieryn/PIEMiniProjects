#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
  
  
  Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
  Adafruit_DCMotor *leftMotor = AFMS.getMotor(2);
  Adafruit_DCMotor *rightMotor = AFMS.getMotor(1);

  const int buttonPin = 5;

  const int irSensorLeftPin = 2;
  const int irSensorMiddlePin = 3;
  const int irSensorRightPin = 4;

  bool start = false; 
  bool middle = false;

  const int slowSpeed = 25; // seems like 15 is the minimum because of friction
  const int fastSpeed = slowSpeed*2;
  const int turningSpeed = slowSpeed*1.7;
  int factor = 1;


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
  // int l = digitalRead(irSensorLeftPin);
  // int m = digitalRead(irSensorMiddlePin);
  // int r = digitalRead(irSensorRightPin);
  // Serial.println("Left: " + String(l) + " - Middle:  " + String(m) + " - Right: " + String(r));
  // Serial.println("____________________________________");

  int buttonState = digitalRead(buttonPin); // read to see if start button has been pressed
  if (buttonState == HIGH){
    start = true;
  }
  if (start == true) {
    delay(1000);
    drive();
  }
}

void drive(){
  int leftSensorVal = digitalRead(irSensorLeftPin);
  int middleSensorVal = digitalRead(irSensorMiddlePin);
  int rightSensorVal = digitalRead(irSensorRightPin);

  if (leftSensorVal == LOW){
      leftMotor->setSpeed(0); // 0 - 255
      rightMotor->setSpeed(turningSpeed*factor);
      // Serial.println("Turning left");
   } else if (rightSensorVal == LOW){
      leftMotor->setSpeed(turningSpeed); // 0 - 255
      rightMotor->setSpeed(0*factor);
      // Serial.println("Turning right");
  } else if (middleSensorVal == LOW){
      leftMotor->setSpeed(slowSpeed); // 0 - 255
      rightMotor->setSpeed(slowSpeed*factor);
      // Serial.println("Going straight");
  }

}
