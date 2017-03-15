/*MEEM 4707 Lab5 extended version (spring break)*/
/*Complied by Deep*/

/*Task is to follow line on the floor and also detect obstacle, if any run away from it*/

/*3 line following modules
  if middle(pin4) is triggered --> forward
     left(pin 10) is triggered --> left
     right(pin 2) is triggered --> right
*/

#include <Servo.h>    //includes servo motor library

long int t1, tf;

int t0 = millis();

/*ultrasonic sensor pins*/
int enA = 5;
int enB = 11;
int in1 = 6;
int in2 = 7;
int in3 = 8;
int in4 = 9;
int trigPin = A5;
int echoPin = A4;

/*ultrasonic variables*/
int dis = 15;
int vel = 40;

Servo myServo;

void setup() {
  
  //updateMot(40, 40); //initalizes robot to move ahead and hunt for line to track
  
  Serial.begin(9600);
  myServo.attach(3);
  myServo.write(90); //I don't like my servo flopping around 

  
  //set up pins here
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(enA,OUTPUT);
  pinMode(enB,OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

/*function for runtime delay*/
long int mdelay(long int x) {
//  long int t1, tf;
  t1 = t0 + x;  // where x is desired time in microseconds
  tf = t1 % t0;

  if (tf <= 1) {
    Serial.println(tf);
  }
}

void updateMot(int left, int right) {
  //updateMot function. Handles two integer inputs that are the speeds of motor A and motor B between -100 and 100
  //returns nothing
  //controls the L298N module direction and scales the outputs to be full range

  //init pins
  int enA = 5;
  int enB = 11;
  int in1 = 6;
  int in2 = 7;
  int in3 = 8;
  int in4 = 9;
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  //set h-bridge directions
  if (left < 0) {
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  } else {
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }
  if (right > 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  } else {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }

  //scale the output to be from 0-255
  left = constrain(abs(left), 0, 100);
  right = constrain(abs(right), 0, 100);
  left = map(left, 0, 100, 0, 255);
  right = map(right, 0, 100, 0, 255);

  //PWM output
  analogWrite(enB, left);
  analogWrite(enA, right);
}

void loop() {

  int right = 2;      //defines right IR on pin 2
  int middle = 4;     //defines middle IR on pin 4
  int left = 10;      //defines left IR on pin 10

  pinMode(right, INPUT);    //keeping right side module as input
  pinMode(middle, INPUT);   //        middle module
  pinMode(left, INPUT);    //       left side module

  int mpin, rpin, lpin, mem, mem1, newmem;    //defines variable for all three IR and memory also

  /*setting negative logic for getting literal values corresponding to name, from IR sensor*/
  mpin = !digitalRead(middle); //reads middle pin in mpin
  rpin = !digitalRead(right);  //reads right pin in rpin
  lpin = !digitalRead(left);   //reads left pin in lpin

  delay(5);

  /*logic for line following starts from here*/

    //middle IR active then go straight
  if (mem = ((mpin == 1) && (rpin == 0) && (lpin == 0))) {
    updateMot(32, 32);
    delay(5);
    // delay(500);
    //mdelay(100);
  }
    //right IR active then take right turn
  else if (mem = ((mpin == 0) && (rpin == 1) && (lpin == 0))) {
    updateMot(50, -30);
//    delay(5);
//        if((mpin == 1) && (rpin == 0) && (lpin == 0)){
//      updateMot(0,0);
//    }
    // delay(500);
    //mdelay(100);
  }
    //left IR active then take left turn
  else if (mem = ((mpin == 0) && (rpin == 0) && (lpin == 1))) {
    updateMot(-30,50);
    delay(5);
    //delay(500);
    // mdelay(100);
  }

  /*For given two sensor detection*/

    //if center and right IR active then take gradual right turn
  else if (mem = ((mpin == 1) && (rpin == 1) && (lpin == 0))) {
    updateMot(70, -65);
    delay(5);
    // delay(500);
    //mdelay(100);
  }
    //if center and left IR active then take gradual left turn
  else if (mem = ((mpin == 1) && (rpin == 0) && (lpin == 1))) {
    updateMot(-65, 70);
    delay(5);
    //delay(500);
    // mdelay(100);
  }
  else if(mem=((mpin == 1) && (rpin == 1) && (lpin == 1))){
    //updateMot(50,0);
    //delay(200);
    //newmem=mem1;
    //mem1=newmem;
    updateMot(32,32);
    //delay(20);
    //newmem=mem1;
    //mem1=newmem;
    //updateMot(0,0);
  }
    //if no IR active then look for obstcale
  else if ((mpin == 0) && (rpin == 0) && (lpin == 0)) {
    //newmem = mem;
    //mem = newmem;
    //delay(200);

   // mdelay(1000);
    
      obstacle(); 
    
  }
//  else {
//    updateMot(0,0);    // if any loop crashes, then run straight
//    delay(5);
//  }
}

void obstacle(){
 float distance = readDistance();

//if any obstacle is within 20 cm periphery then turn right and move in that direction
if(distance <= 20.0){
  updateMot(-80,80);
  delay(500);
  updateMot(30,40);
  delay(20);
}

//if obstacle is detected but is outside of 20 cmm periphery then move right side gradually
else{
  updateMot(30,40);
}
}


float readDistance(void) {
  //readDistance function. Uses ultrasonic sensor and returns a floating point
  //distance in centimeters.

  //init pins
  int trigPin = A5;
  int echoPin = A4;
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //send a trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);

  //wait for a response and convert to cm
  return pulseIn(echoPin, HIGH)/29.0/2.0;
}
