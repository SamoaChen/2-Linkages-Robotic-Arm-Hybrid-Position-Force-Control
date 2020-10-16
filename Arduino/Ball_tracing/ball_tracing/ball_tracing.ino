int angle=0;
int angle2=0;

int fsr_value=0;

int count_rev=2*5736;
int count_rev2=2*11400;

int pwm=0;
int pwm2=0;

volatile long counter=0;
volatile long counter2=0;

//encoder pins
#define rot_pin1 3
#define rot_pin2 4

#define rot_pin3 5
#define rot_pin4 6

//direction pins
int dirpin1=8;
int dirpin2=7;

int dirpin3=12;
int dirpin4=11;

//calibration pin
int stop_pin=13;
int stop_pin2=1;

//pwm pin
int pwmpin=9;
int pwmpin2=10;

//force sensor pin
int fsr_pin=0;

//state determine constants
int state=0;
int err_state=0;

#include "PinChangeInterrupt.h"


void setup() {

  pinMode(rot_pin1, INPUT_PULLUP); 
  
  pinMode(rot_pin2, INPUT_PULLUP); 

  
  pinMode(rot_pin3,INPUT_PULLUP);

  pinMode(rot_pin4,INPUT_PULLUP);

  pinMode(stop_pin,INPUT_PULLUP);

  pinMode(stop_pin2,INPUT_PULLUP);

  initiate();

  
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(rot_pin1), ai0, RISING);
   
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(rot_pin2), ai1, RISING);


  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(rot_pin3), ai2, RISING);
   
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(rot_pin4), ai3, RISING);


  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(stop_pin),stop_function,FALLING);
 
  Serial.begin(9600);
}

void loop() {
  shrink_counter();
  if(err_state==0){
    force_measure();
    read_write();
  }
  
  if((state==0) || (err_state==1)){
    motor_initiation();
    
  }
  motor_drive();
  
}

void initiate(){
  while(digitalRead(stop_pin)==HIGH){
    digitalWrite(dirpin3,LOW);
    digitalWrite(dirpin4,HIGH);
    analogWrite(pwmpin2,80);
  } 
  long start=millis();
  while((millis()-start)<30){
    digitalWrite(dirpin3,HIGH);
    digitalWrite(dirpin4,LOW);
    analogWrite(pwmpin2,255);
  }
  analogWrite(pwmpin2,0);//set pwm to 0 before the first arm adjusted
  counter2=5193;
  

  delay(1000);//wait for the second arm to adjust

  while(digitalRead(stop_pin2)==HIGH){
    digitalWrite(dirpin1,LOW);
    digitalWrite(dirpin2,HIGH);
    analogWrite(pwmpin,100);
  } 
  start=millis();
  while((millis()-start)<0){
    digitalWrite(dirpin1,HIGH);
    digitalWrite(dirpin2,LOW);
    analogWrite(pwmpin,255);
  }

  counter=5273;

}


void read_write(){
  if(Serial.available()>0){
    if(Serial.peek()=='g'){
      Serial.read();
      //angle=360*counter/count_rev;
      //Serial.println(angle);  
      Serial.println(counter);
      state=1; //switch state
    }else if(Serial.peek()=='h'){
      Serial.read();
      //angle2=360*counter2/count_rev2;
      //Serial.println(angle2);
      Serial.println(counter2);
      state=1;
    }else if(Serial.peek()=='f'){
      Serial.read();
      Serial.println(fsr_value);
    }else if(Serial.peek()=='p'){
      Serial.read();
      pwm=Serial.parseInt();
      Serial.read(); //to read the stop sign
    }else if(Serial.peek()=='q'){
      Serial.read();
      pwm2=Serial.parseInt();
      Serial.read();
    }else if(Serial.peek()=='r'){
      Serial.read();
      pwm=Serial.parseInt();
      Serial.read();
      pwm2=Serial.parseInt();
      Serial.read();
    }
  }
}

void motor_drive(){
  if(pwm>=0){
    digitalWrite(dirpin1,LOW);
    digitalWrite(dirpin2,HIGH);
    analogWrite(pwmpin,pwm);
  }else if(pwm<0){
    digitalWrite(dirpin1,HIGH);
    digitalWrite(dirpin2,LOW);
    analogWrite(pwmpin,-1*pwm);
  }

  if(pwm2>=0){
    digitalWrite(dirpin3,LOW);
    digitalWrite(dirpin4,HIGH);
    analogWrite(pwmpin2,pwm2);
  }else if(pwm2<0){
    digitalWrite(dirpin3,HIGH);
    digitalWrite(dirpin4,LOW);
    analogWrite(pwmpin2,-1*pwm2);
  }
}

void motor_initiation(){

  int angle_err=45-int(360*counter/count_rev);
  int angle2_err=-45-int(360*counter2/count_rev2);
  pwm=60*angle_err;
  pwm2=60*angle2_err;
  if(pwm2>255){
    pwm2=255;
  }else if(pwm2<-255){
    pwm2=-255;
  }
  
  if(pwm>255){
    pwm=255;
  }else if(pwm<-255){
    pwm=-255;
  }

}

void force_measure(){
  for (int num=0;num<100;num++){
    fsr_value+= analogRead(fsr_pin);
  }
  fsr_value/=100;
 
}

void ai0() {
    if(digitalRead(rot_pin2)==LOW) {
    counter++;
    }else{
    counter--;
    }
  }
   
  void ai1() {
    if(digitalRead(rot_pin1)==LOW) {
    counter--;
    }else{
    counter++;
    }
  }


  void ai2() {
    if(digitalRead(rot_pin4)==LOW) {
    counter2++;
    }else{
    counter2--;
    }
  }
   
  void ai3() {
    if(digitalRead(rot_pin3)==LOW) {
    counter2--;
    }else{
    counter2++;
    }
  }


  void stop_function(){
    if((state==1)&&(err_state==0)){
        state=0;
        err_state=1;
    }
  }
  
  void shrink_counter(){
    if(counter>count_rev){
      counter%=count_rev;
    }else if(counter<-count_rev){
      counter=-((-1*counter)%count_rev);
    }

    if(counter2>count_rev2){
      counter2%=count_rev2;
    }else if(counter2<-count_rev2){
      counter2=-((-1*counter2)%count_rev2);
    }
  }
