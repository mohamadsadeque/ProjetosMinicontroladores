
int d = 261;
int r =  294;
int m = 329;
int f = 349; 
int S = 392; 
int l = 440; 
int s = 493; 
int D = 523; 
int piezo = 3;
char notes[] = "drmfslSD";
int beats[] = {1,1,1,1,1,1,1,1};
void setup() {
  // put your setup code here, to run once:
pinMode(piezo, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  for(int i = 0; i<8 ; i++){
    tone(piezo, notes[i], beats[i]*20 );
    }

}
