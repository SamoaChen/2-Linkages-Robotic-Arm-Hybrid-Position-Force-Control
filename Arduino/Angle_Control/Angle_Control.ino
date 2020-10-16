#include "PinChangeInterrupt.h"


volatile long temp, counter = 0; //This variable will increase or decrease depending on the rotation of encoder
int angle=0;
int count_rev=2*5736;
//int count_rev=12136;
int pwmPin=9;
int dirPin1=8;
int dirPin2=7;
int stop_pin=2;
int num=0;
volatile int stop_limit=1;

//DEFINED INTERRUPT PINS
#define rot_pin1 3
#define rot_pin2 4

//CONSTANTS 
int ad=0;
int fd_pwm=0;

//controller
double error;
double kp=60;
double ki=0.03;
double C_kp;
double C_ki=0;
double C;
int C_pwm;
int C_pwm_old=0;

void setup() {
  Serial.begin (9600);

  pinMode(rot_pin1, INPUT_PULLUP); // internal pullup input pin 2 
  
  pinMode(rot_pin2, INPUT_PULLUP); // internal pullup input pin 3

  pinMode(stop_pin,INPUT_PULLUP);
  
   //Setting up interrupt
  //A rising pulse from encodenren activated ai0(). AttachInterrupt 0 is DigitalPin nr 2 on moust Arduino.
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(rot_pin1), ai0, RISING);
   
  //B rising pulse from encodenren activated ai1(). AttachInterrupt 1 is DigitalPin nr 3 on moust Arduino.
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(rot_pin2), ai1, RISING);
  
  //Attach interrupt to stop pin
  attachInterrupt(digitalPinToInterrupt(stop_pin),stop_func,FALLING);
  }
   
  void loop() {
  
    // Send the value of counter
    if( counter != temp ){
    angle=360*counter/count_rev;
    Serial.println (angle);
    //Serial.println (C_pwm);
    temp = counter;
    }
  
    //MOTOR CONTROL 
    error=ad-angle;
    C_kp=kp*error;
    
    if(num<1000){
    C_ki+=error;
    num+=1;
    }else{
      num=0;
      C_ki=0;
    }
    
    C=C_kp+ki*C_ki+fd_pwm;
    C_pwm=int(C);
  
    //write motor direction
    digitalWrite(dirPin1,LOW);
    digitalWrite(dirPin2,HIGH);
  
    if(C_pwm<0){
      digitalWrite(dirPin1,HIGH);
      digitalWrite(dirPin2,LOW);
      if(C_pwm<-255){
        C_pwm=255;
      }else{
        C_pwm*=-1;
      }
      
    }else if(C_pwm>255){
      digitalWrite(dirPin1,LOW);
      digitalWrite(dirPin2,HIGH);
      C_pwm=255;
    }

    if(stop_limit==0){
      C_pwm=0;
    }
    
    if (C_pwm!=C_pwm_old){
      analogWrite(pwmPin,C_pwm);
    }
    C_pwm_old=C_pwm;

    //ask for input angle
    long i=0;
     while(Serial.available()>0){
     ad=Serial.parseInt();
     i=i*10+ad;
     ad=i/10;
    }
   
  }
   
  void ai0() {
    // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
    // Check pin 3 to determine the direction
    if(digitalRead(5)==LOW) {
    counter++;
    }else{
    counter--;
    }
  }
   
  void ai1() {
    // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
    // Check with pin 2 to determine the direction
    if(digitalRead(4)==LOW) {
    counter--;
    }else{
    counter++;
    }
  }

  void stop_func(){
    stop_limit=0;
  
  }
