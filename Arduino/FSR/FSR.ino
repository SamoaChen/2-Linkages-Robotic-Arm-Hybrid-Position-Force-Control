/* FSR testing sketch. 
 
Connect one end of FSR to 5V, the other end to Analog 0.
Then connect one end of a 10K resistor from Analog 0 to ground
Connect LED from pin 11 through a resistor to ground 
 
For more information see www.ladyada.net/learn/sensors/fsr.html */


int fsrAnalogPin = 0; // FSR is connected to analog 0
int LEDpin = 11;      // connect Red LED to pin 11 (PWM pin)
int fsrReading_old=0; // the analog reading from the FSR resistor divider
int fsrReading_new;
int LEDbrightness;
 
void setup(void) {
  Serial.begin(9600);   // We'll send debugging information via the Serial monitor
  pinMode(LEDpin, OUTPUT);
}
 
void loop(void) {
  for (int num=0;num<100;num++){
    fsrReading_new += analogRead(fsrAnalogPin);
  }
  fsrReading_new/=100;
  if(fsrReading_new!=fsrReading_old){
    Serial.print("Analog reading = ");
    Serial.println(fsrReading_new);
  }
  fsrReading_old=fsrReading_new;
  
  // we'll need to change the range from the analog reading (0-1023) down to the range
  // used by analogWrite (0-255) with map!
//  LEDbrightness = map(fsrReading_new, 0, 1023, 0, 255);
  // LED gets brighter the harder you press
//  analogWrite(LEDpin, LEDbrightness);
 
}
