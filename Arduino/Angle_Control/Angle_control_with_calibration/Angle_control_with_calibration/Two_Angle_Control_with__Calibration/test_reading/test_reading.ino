int ad=0;
int ad2=0;


void setup() {
  Serial.begin(9600);


  
}

void loop() {
    long i=0;
    
     if(Serial.available()>0){
      if (Serial.read()=='a'){
         
         while(Serial.available()>0){
           ad=Serial.parseInt();
//           i=i*10+ad;
//           ad=i/10;
          
         }
      }else if(Serial.peek()=='b'){
         Serial.read();
         while(Serial.available()>0){
           ad2=Serial.parseInt();
           i=i*10+ad2;
           ad2=i/10;
           
         }
      }
    }

 Serial.println(ad);
 Serial.println(ad2);
}
