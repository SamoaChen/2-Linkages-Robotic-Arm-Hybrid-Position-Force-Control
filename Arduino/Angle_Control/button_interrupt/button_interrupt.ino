int stop_pin=2;
volatile int stop_limit=1;
void setup() {
  Serial.begin(9600);
  pinMode(stop_pin,INPUT_PULLUP);
  attachInterrupt(0,stop_func,FALLING);
}

void loop() {
  Serial.println(stop_limit);
  
}

void stop_func(){
  stop_limit=0;
}
