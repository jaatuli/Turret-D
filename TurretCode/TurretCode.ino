#include <Servo.h>
#include "Coroutines.h"

#define trigPin 13
#define echoPin 12

const long stopDistance = 50; // Distance in cm when servo stops
const int servoWaitTime = 30; // Time in ms that coroutine wait for servo to move
const int servoStep = 1; // Angle that servo move per loop

Servo myservo;  // create servo object to control a servo

int pos = 0;    // variable to store the servo position
long duration, distance;

const int numReadings = 10;
long readings[numReadings];      // the readings from the input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
long average = 0;                // the average

Coroutines<1> coroutine;  // Create coroutine instance with 1 "thread"
Coroutine* coroutineRef;  // Pointer to suspend and resume coroutine in the main loop

void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);  
  myservo.attach(10);  // attaches the servo on pin to the servo object

  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }

  coroutineRef = &coroutine.start(moveTurret);
}

void loop() {   

    coroutine.update();
    measureDistance();
    Serial.print(average);
    Serial.print(" cm, ");
    Serial.print("angle ");
    Serial.println(pos);
    
    if(average < stopDistance){
      coroutineRef->suspend();
    } else {
      coroutineRef->resume();
    }
 
}

void moveTurret(COROUTINE_CONTEXT(coroutine)) {

  BEGIN_COROUTINE;
    
  for (pos = 0; pos <= 177; pos += servoStep) { // goes from 0 degrees to 180 degrees   
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    coroutine.wait(servoWaitTime);              // waits n ms for the servo to reach the position
    COROUTINE_YIELD;
  }
  
  for (pos = 177; pos >= 0; pos -= servoStep) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    coroutine.wait(servoWaitTime);              // waits n ms for the servo to reach the position
    COROUTINE_YIELD;
  }

  coroutine.loop();
  END_COROUTINE;
}

void measureDistance() {
   
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);

    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    duration = pulseIn(echoPin, HIGH);
    distance = (duration/2) / 29.1;
          
    // subtract the last reading:
    total = total - readings[readIndex];
    // read from the sensor:
    readings[readIndex] = distance;
    // add the reading to the total:
    total = total + readings[readIndex];
    // advance to the next position in the array:
    readIndex = readIndex + 1;

    // if we're at the end of the array...
    if (readIndex >= numReadings) {
      // ...wrap around to the beginning:
      readIndex = 0;
    }

    // calculate the average:
    average = total / numReadings; 

}
