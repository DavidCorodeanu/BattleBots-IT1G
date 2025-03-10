#include "other.h"



void setup()
{
  Serial.begin(9600);
  pinMode(MA1, OUTPUT);
  pinMode(MA2, OUTPUT);

  pinMode(MB1, OUTPUT);
  pinMode(MB2, OUTPUT);

  pinMode(SR1, INPUT);
  pinMode(SR2, INPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(13, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(SR1), countRotationsSr1, FALLING);
  attachInterrupt(digitalPinToInterrupt(SR2), countRotationsSr2, FALLING);
  allS();
  callibrate();
  Serial.println(black);
}
int speed = 250;
void optimus_avoiding_object(){
  if (!moreDistance(20)){
    allS();
    delay(30);
    leftF(250);
    stopAfterLeft(0.75);
    forward(250);
    delay(500);
    allS();
    rightF(250);
    stopAfterRight(0.75);
    forward(250);
    delay(1000);
    allS();
    rightF(250);
    stopAfterRight(0.75);
    forward(250);
    delay(500);
    allS();
    leftF(250);
    stopAfterLeft(0.75);

  } else if(moreDistance(40)){
    setBothMotor(speed);
  }
}
void otimus_basic_move(){
  forward(250);
  stopAfterRight(5);
  allS();
  back(250);
  stopAfterRight(5);
  allS();
  delay(1000);
  rightF(250);
  stopAfterRight(0.75);
  delay(500);
  leftF(250);
  stopAfterLeft(0.75);
  delay(10000);
}


void loop()
{
   
  optimus_followLine();
}



