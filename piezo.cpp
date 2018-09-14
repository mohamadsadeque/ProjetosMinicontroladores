
int d = 261;
int r = 294;
int m = 329;
int f = 349; 
int S = 392; 
int l = 440; 
int s = 493; 
int D = 523; 

int piezo = 3;
int pot = A0;
int botao = 7;
int tom;
char notes[] = "drmfff drdrrr dSfmmm drmfff";


//char notes[] = {'d','S','p','f','D','S','l','f'};
int beats[] = {1, 1, 1, 2, 1, 1,1,1, 1, 1, 2, 1, 1,1,1,1,1,2,1,1,1,1,1,1,2,1,1,4};
void setup() {
  // put your setup code here, to run once:
pinMode(piezo, OUTPUT);
pinMode(botao, INPUT_PULLUP);
pinMode(pot, INPUT);
Serial.begin(9600);
}

void loop() {

if(!digitalRead(botao)){
  tom = map(analogRead(pot),0,1023,261,523); 
 
  tone(piezo, tom);

}

else
noTone(piezo);

}


/*
  for(int i = 0; i<sizeof(beats)/2 ; i++){
switch (notes[i]){
case 'd': tom =  261;
break;
case 'r': tom =  294;
break;
case 'm': tom =  329;
break;
case 'f': tom =  349;
break;
case 'S': tom =  392;
break;
case 'l': tom =  440;
break;
case 's': tom =  493;
break;
case 'D': tom =  523;
break;
case ' ': tom =  0;
break;
}
    if(tom == 0){
    noTone(piezo); 
    }
    else{
    tone(piezo, tom);
    
    }
    delay(beats[i]*200); 
    noTone(piezo);
  delay(40);
  }
*/

 
