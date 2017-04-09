#include <Servo.h>

#define trigPin 13
#define echoPin 12

Servo myservo;  // create servo object to control a servo

int pos = 0;    // variable to store the servo position
long duration, distance;

const int numReadings = 10;
long readings[numReadings];      // the readings from the input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
long average = 0;                // the average
int range[180];

void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);  
  myservo.attach(10);  // attaches the servo on pin to the servo object

  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}

void loop() {   
  
  for (pos = 0; pos <= 177; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15ms for the servo to reach the position
    
    if(measureDistance() < 50){
      pos -= 1;
    }
    
  }
  for (pos = 177; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15ms for the servo to reach the position

    if(measureDistance() < 50){
      pos += 1;
    }
  }
 
}

long measureDistance() {
  
    digitalWrite(trigPin, LOW);  // Added this line
    delayMicroseconds(2); // Added this line
    digitalWrite(trigPin, HIGH);

    delayMicroseconds(10); // Added this line
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration/2) / 29.1;

    Serial.println(distance);
    
    /*
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
    */
    
    return distance;
    
}

