//HIGHSTATE E IL SETUP DELL'INTERVAL DEVONO RIMANERE IN QUESTO MODO O COMUNQUE ESSERE SINCRONIZZATI PER FAR FUNZIONARE IL PROGRAMMA
#define  hall 6
#define  bike 4
long interval=1000; //must be long, ints ARE ONLY 32K
long period=1000;
long duty= 50; //from 0 to 100
int acc=0;
int mapped_acc=0;
//50ms=2.5kmh
unsigned long ms=0;
bool highstate=true;
void setup() {
  // put your setup code here, to run once:
pinMode(hall,INPUT);
pinMode(bike,OUTPUT);
pinMode(LED_BUILTIN,OUTPUT);
Serial.begin(9600);
interval=(interval*(100-duty))/100;
 digitalWrite(LED_BUILTIN,LOW);
}
void asyncPower(int dutyCycle,int period){
 
 if(millis()-ms>=interval){ //overflow safe pattern (la sottrazione e' tra unsigned longs---> il risultato non e' mai negativo)
    if(highstate){ //questo if si occupa del periodo ALTO
      digitalWrite(bike,HIGH);
      // interval=period/(100-duty); //gives back the original interval //comment is obs WRONG CODE---it was interval/duty
      interval=(period*duty)/100; //cast to int
     
      highstate=false;
      Serial.print("interval high=");
    Serial.println(interval);
    }
    else{ 

      digitalWrite(bike,LOW);
      //interval=(period/duty)*100; WRONG CODE---it was interval/duty
      interval=(period*(100-duty))/100;
      highstate=true;
      Serial.print("interval low=");
      Serial.println(interval);
    }
    Serial.print("period=");
    Serial.println(period);
    ms=millis();
 
 }
}
//bool check=true;
bool throttle(){
  acc=analogRead(A0);
  //Serial.print("acc=");
  //Serial.println(acc);
  //da 163 a 757
  if(acc>180){ //circa 0.87v
      
    mapped_acc=map(acc, 180, 860, 0, 600);
     period=(long)(1000-mapped_acc); 

  }
  else{
    period=1000;
  }
       return true;
  /*if(Serial.available()){
    char c=Serial.read();
   // Serial.println(c);
    if(c=='f'){
      check=true;
      period-=50;
    }
    else if(c=='c'){
      period+=50;
      if(period>=1000){
        period=1000;
        check=false;
      }
      
    }
     
  }*/
//  return check;
}
void loop() {
  if(throttle()){
  asyncPower(duty,period);

  }

}
