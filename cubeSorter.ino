int iterations=0;
float place=0;
float distance=0;
float coord=6;
bool one=false;
bool two=false;
bool three=false;

  
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include "Adafruit_TCS34725.h"

#include <SPI.h>
#include <Servo.h>

Servo myservo; 

#define GLOBAL_DELAY 50

int sensor1 = 13;
int sensor2 = 12;

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

Adafruit_StepperMotor *leftRight = AFMS.getStepper(200, 2);
Adafruit_StepperMotor *upDown = AFMS.getStepper(200, 1);
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
//MOVE FUNCTIONS
  void right(float distance) {
    leftRight->step(10*distance, FORWARD, INTERLEAVE);
    Serial.println("moved right:");
    Serial.println(distance);
  }
  void left(float distance) {
    leftRight->step(10*distance, BACKWARD, INTERLEAVE);
    Serial.println("moved left:");
    Serial.println(distance);}
  void up(float distance) {
    upDown->step(10*distance, FORWARD, INTERLEAVE);
    coord-=distance;
    Serial.println("coord:");
    Serial.println(coord);
    }
  void down(float distance) {
    upDown->step(10*distance, BACKWARD, INTERLEAVE);
    coord+=distance;
    Serial.println("coord:");
    Serial.println(coord);}

void sort(float place, float set, float iterations) { // moves to the y coordinate place, then pushes a block off
  distance=coord-place;
    if (distance > 0) {
      up(distance);}
    else if (distance<0) {
      down((-1)*distance);}   
    if (set!=1)
      {
      if (iterations==0)
      {left(6.5);}
      else
      {left(6);}
      }}
      
  //A block has been detected twice, put it in an empty place
void error()
{if (one==false) {
sort(17, 0, iterations);
  iterations++;
  one=true;
}
else if (two==false) {
sort(6, 0, iterations);
  iterations++;
  two=true;
}
else if (three==false) {
sort(-4, 0, iterations);
iterations++;
three=true;
}
}

void setup() {
  Serial.begin(9600);
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }
  AFMS.begin();  // create with the default frequency 1.6KHz

myservo.attach(9);

int iterations=0;
float place=0;
float distance=0;
float coord=6;
bool one=false;
bool two=false;
bool three=false;
pinMode(12, INPUT);
pinMode(13, INPUT);

}


void loop() {
  delay(2000);        // delay for stability
 leftRight->setSpeed(10);  // 10 rpm   
  upDown->setSpeed(10);
  tcs.setInterrupt(false);    
  delay(60); 
  tcs.setInterrupt(true);
  //CircuitPlayground.clearPixels();
  uint16_t clear, red, green, blue;
  tcs.getRawData(&red, &green, &blue, &clear);

// Check if optical switch is on
if (red<2300 or green < 2900 or blue < 2250) {
  myservo.write(180);
  if (iterations==4)
  {delay(5000);
    iterations=0;}
    if (iterations==5)
  {
    iterations=0;}
  Serial.println(red);
  Serial.println(green);
  Serial.println(blue);}
  else {
  if (iterations==0)
  {iterations=5;}
  myservo.write(90);
  Serial.println(red);
  Serial.println(green);
  Serial.println(blue);
  
  }
  if (iterations==0)
  {left(9);}

if (iterations < 3) {
  int sensorValue0 = digitalRead(sensor1);
  int sensorValue6 = digitalRead(sensor2);
  if (sensorValue0 ==false and sensorValue6 ==false) //Block 3 detected
  {if (three==false) {
    sort(-4, 0, iterations);//place in position 3
    iterations++;
    three=true;}
    else {//error: Block 3 already detected
error();
    }
    }
  else if (sensorValue0 ==true and sensorValue6 ==true) //Block 1 detected
  {if (one==false) {
    sort(17, 0, iterations); // Place in position 1
  iterations++;
  one=true;}
  else {//error: Block 1 already detected
  error();}
  }
  else //Block 2 detected
  {if (two==false) { 
  sort(6, 0, iterations); // Place in position 2
  iterations++;
  two=true;}
  else { //error: Block 2 already detected
  error();}
}}
Serial.println("iterations:");
Serial.println(iterations);
if (iterations==3)
{right(27.5);
  iterations=4;
  sort(6,1,4);
  one=false;
  two=false;
  three=false;}
Serial.println("coord:");
    Serial.println(coord);
    Serial.println("one");
    Serial.println(one);
    Serial.println("two");
    Serial.println(two);
    Serial.println("three");
    Serial.println(three);
 }
