#include "PinChangeInterrupt.h"

volatile long temp, counter = 0;

int angle=0;
int count_rev=2*5736;


int pwmPin=9;
int dirPin1=8;
int dirPin2=7;
//DEFINED INTERRUPT PINS
#define rot_pin2 3

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(rot_pin1, INPUT_PULLUP); // internal pullup input pin 2 
  
  pinMode(rot_pin2, INPUT_PULLUP); // internal pullup input pin 3


  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(rot_pin1), ai0, RISING);
   
#define rot_pin1 4
  //B rising pulse from encodenren activated ai1(). AttachInterrupt 1 is DigitalPin nr 3 on moust Arduino.
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(rot_pin2), ai1, RISING);
}

void loop() {
  // put your main code here, to run repeatedly:
    digitalWrite(dirPin1,HIGH);
    digitalWrite(dirPin2,LOW);
    analogWrite(pwmPin,100);
    angle=360*counter/count_rev;
    angle=angle%360;
    Serial.println(angle);
}

void ai0() {
    // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
    // Check pin 3 to determine the direction
    if(digitalRead(rot_pin2)==LOW) {
    counter++;
    }else{
    counter--;
    }
  }
   
  void ai1() {
    // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
    // Check with pin 2 to determine the direction
    if(digitalRead(rot_pin1)==LOW) {
    counter--;
    }else{
    counter++;
    }
  }
