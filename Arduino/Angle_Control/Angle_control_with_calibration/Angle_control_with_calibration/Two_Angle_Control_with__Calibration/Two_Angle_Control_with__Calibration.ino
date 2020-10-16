#include "PinChangeInterrupt.h"


//constants for the first motor 


volatile long temp, counter = 0; //This variable will increase or decrease depending on the rotation of encoder
int angle=-90;
int count_rev=2*5736; //for motor 1
//int count_rev=2*11400; //for motor 2

int pwmPin=9;
int dirPin1=8;
int dirPin2=7;
int stop_pin=0;
int num=0;
volatile int stop_limit=1;
volatile int precount=1;

//DEFINED INTERRUPT PINS
#define rot_pin1 3
#define rot_pin2 4

//CONSTANTS 
int ad=45;
int fd_pwm=0;

//controller
double error;
double kp=60;//for motor 1
//double kp=60;// for motor 2
double ki=0.02;//for motor 1
//double ki=0.03;//for motor 2
double C_kp;
double C_ki=0;
double C;
int C_pwm;
int C_pwm_old=0;

//constants for the second motor

volatile long temp2, counter2 = 0; //This variable will increase or decrease depending on the rotation of encoder
int angle2=-90;
int count_rev2=2*11400; //for motor 2

int pwmPin2=10;
int dirPin3=11;
int dirPin4=12;
int stop_pin2=13;
int num2=0; //for ki time laps number
volatile int stop_limit2=1;
volatile int precount2=1; //for determing whether the angle reached controlling zone

//DEFINED INTERRUPT PINS
#define rot_pin3 5
#define rot_pin4 6

//CONSTANTS 
int ad2=45;
int fd_pwm2=0;

//controller
double error2;
double kp2=60;// for motor 2
double ki2=0.03;//for motor 2
double C_kp2;
double C_ki2=0;
double C2;
int C_pwm2;
int C_pwm_old2=0;


void setup() {
 Serial.begin (9600);

//set up for the first motor


  pinMode(rot_pin1, INPUT_PULLUP); // internal pullup input pin 2 
  
  pinMode(rot_pin2, INPUT_PULLUP); // internal pullup input pin 3

  pinMode(stop_pin,INPUT_PULLUP);
  
  //pinMode(0,INPUT_PULLUP);

  //INITIATING THE SYSTEM
  while(digitalRead(stop_pin)==HIGH){
    digitalWrite(dirPin1,HIGH);
    digitalWrite(dirPin2,LOW);
    analogWrite(pwmPin,100);
    Serial.println("in set up");
  }

  counter=-2868;//calibrate angle for motor 1
  //counter=-5700;//calibrate angle for motor 2
 
  analogWrite(pwmPin,0);
 
   //Setting up interrupt
  //A rising pulse from encodenren activated ai0(). AttachInterrupt 0 is DigitalPin nr 2 on moust Arduino.
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(rot_pin1), ai0, RISING);
   
  //B rising pulse from encodenren activated ai1(). AttachInterrupt 1 is DigitalPin nr 3 on moust Arduino.
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(rot_pin2), ai1, RISING);
  
  //Attach interrupt to stop pin
  attachPinChangeInterrupt(digitalPinToInterrupt(stop_pin),stop_func,FALLING);

// set up for the second motor

  pinMode(rot_pin3, INPUT_PULLUP); // internal pullup input pin 2 
  
  pinMode(rot_pin4, INPUT_PULLUP); // internal pullup input pin 3

  pinMode(stop_pin2,INPUT_PULLUP);
  


  //INITIATING THE SYSTEM
  while(digitalRead(stop_pin2)==HIGH){
    digitalWrite(dirPin4,HIGH);
    digitalWrite(dirPin3,LOW);
    analogWrite(pwmPin2,100);
    Serial.println("motor2 in set up");
  }

  counter2=-5700;//calibrate angle for motor 2
 
  analogWrite(pwmPin2,0);
 
   //Setting up interrupt
  //A rising pulse from encodenren activated ai0(). AttachInterrupt 0 is DigitalPin nr 2 on moust Arduino.
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(rot_pin3), ai2, RISING);
   
  //B rising pulse from encodenren activated ai1(). AttachInterrupt 1 is DigitalPin nr 3 on moust Arduino.
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(rot_pin4), ai3, RISING);
  
  //Attach interrupt to stop pin
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(stop_pin2),stop_func2,FALLING);


// clean up serial buffer
//serial_flush();

}



void loop() {
  motor1_control();
  motor2_control();
  
  long i=0;
     if(Serial.available()>0){
      if (Serial.read()=='a'){
        Serial.println(Serial.available());
         while(Serial.available()>0){
           ad=Serial.parseInt();
           i=i*10+ad;
           ad=i/10;
         }
      }else if(Serial.read()=='b'){
        Serial.println(Serial.available());
         while(Serial.available()>0){
           ad2=Serial.parseInt();
           i=i*10+ad2;
           ad2=i/10;
         }
      }
    }

}


void serial_flush(){
  while(Serial.available()>0){
      char t=Serial.read();
  }
}


void serial_input(){
  
      //ask for input angle
    long i=0;
    
      char input=Serial.read();
      if (input=='a'){
         Serial.println("angle1");
         Serial.println(Serial.available());
         while(Serial.available()>0){
           ad=Serial.parseInt();
           i=i*10+ad;
           ad=i/10;
           Serial.println(ad);
         }
      }else if(input=='b'){
         Serial.println("angle2");
         Serial.println(Serial.available());
         while(Serial.available()>0){
           ad2=Serial.parseInt();
           i=i*10+ad2;
           ad2=i/10;
           Serial.println(ad2);
         }
      }
//      while(Serial.available()>0){
//        char t=Serial.read();
//      }
      
   
    
}

void motor1_control(){
  //CLEAR PREINTERRUPT GIVE IT A NO INTERRUP RANGE
    if(precount==1 && angle>=0){
      precount=0;
    }
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
      ad=45;
      stop_limit=1;
    }
    
    if (C_pwm!=C_pwm_old){
      analogWrite(pwmPin,C_pwm);
    }
    C_pwm_old=C_pwm;

}

//motor2 control

void motor2_control(){
  //CLEAR PREINTERRUPT GIVE IT A NO INTERRUP RANGE
    if(precount2==1 && angle2>=0){
      precount2=0;
    }
    // Send the value of counter
    if( counter2 != temp2 ){
    angle2=360*counter2/count_rev2;
    Serial.println (angle2);
    //Serial.println (C_pwm2);
    temp2 = counter2;
    }
  
    //MOTOR CONTROL 
    error2=ad2-angle2;
    C_kp2=kp2*error2;

   if(num2<1000){
    C_ki2+=error2;
    num2+=1;
    }else{
      num2=0;
      C_ki2=0;
    }
    
    C2=C_kp2+ki2*C_ki2+fd_pwm2;
    C_pwm2=int(C2);
  
    //write motor direction
    digitalWrite(dirPin4,LOW);
    digitalWrite(dirPin3,HIGH);
  
    if(C_pwm2<0){
      digitalWrite(dirPin4,HIGH);
      digitalWrite(dirPin3,LOW);
      if(C_pwm2<-255){
        C_pwm2=255;
      }else{
        C_pwm2*=-1;
      }
      
    }else if(C_pwm2>255){
      digitalWrite(dirPin4,LOW);
      digitalWrite(dirPin3,HIGH);
      C_pwm2=255;
    }

    if(stop_limit2==0){
      ad2=45;
      stop_limit2=1;
    }
    
    if (C_pwm2!=C_pwm_old2){
      analogWrite(pwmPin2,C_pwm2);
    }
    C_pwm_old2=C_pwm2;

}

// interrupts for motor1 

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

  void stop_func(){
    if(precount==0){
      stop_limit=0;
    }
    Serial.println("called");
  }

//interrupts for motor2

void ai2() {
    // ai2 is activated if DigitalPin nr 2 is going from LOW to HIGH
    
    if(digitalRead(rot_pin4)==LOW) {
    counter2++;
    }else{
    counter2--;
    }
  }
   
  void ai3() {
    // ai3 is activated if DigitalPin nr 3 is going from LOW to HIGH
    
    if(digitalRead(rot_pin3)==LOW) {
    counter2--;
    }else{
    counter2++;
    }
  }

  void stop_func2(){
    if(precount2==0){
      stop_limit2=0;
    }
    Serial.println("called2");
  }
