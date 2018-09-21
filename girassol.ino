/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
int atual, anterior,esquerda;

int direita;
int ldr = 0;  // analog pin used to connect the potentiometer
int val = 90;    // variable to read the value from the analog pin
int valE;
int valD;


void setup() {
  Serial.begin(9600);
  pinMode(ldr, INPUT);

  anterior = analogRead(ldr);
  
  myservo.attach(9);   
myservo.write(val); 
  delay(50);
  atual = analogRead(ldr);
val = val + 20;
   myservo.write(val);
   delay(50);    
 esquerda = analogRead(ldr);
                 

}

void loop() {
  
 if(esquerda > atual){
  atual = esquerda;
  val+= 5;
   myservo.write(val);
   delay(50);  
  esquerda = analogRead(ldr);
  Serial.println("ENTREI NA ESQUERDA");
  }
else{
  val-= 5;
   myservo.write(val);
   delay(50);  
  direita = analogRead(ldr);
    Serial.println(" NAO ENTREI NA ESQUERDA");
}
  
  if(direita > atual) {
    atual = direita;
      val-= 5;
   myservo.write(val);
   delay(50);  
  direita = analogRead(ldr);
    Serial.println("ENTREI NA Direita");

    
  }
 /* else{
  val+= 5;
   myservo.write(val);
   delay(50);  
  esquerda = analogRead(ldr);
      Serial.println(" NAO ENTREI NA Direita");

  }*/
   




}
  
  

  

  

