// another noise mixed with first one 
// to get maximum output-loudspeaker
// between that two pins give the maximum
// output power. Make paper trumpet to get
// directed sound, and you have 
// "noise from the hell" :D
 

int x=0;
int y=0;


void setup() {
  pinMode(9,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(6,OUTPUT);
 
}
 
void loop() {

//digitalWrite(7,random(map(analogRead(0),0, 1023, 2, 24))); 
//delay(10);
//digitalWrite(random(6,10),random(0,10)); // stereo
//analogWrite(9,random(255));
//digitalWrite(9,random(2));
//analogWrite(9,random(map(analogRead(0),0, 1023, 0, 255)));
//analogWrite(9,random(map(analogRead(0),0, 1023, 0, 255),map(analogRead(1),0, 1023, 0, 255)));
//generateSaw();
generateTriangle();
//generateSine();

//generateSquare();
//digitalWrite(9,1); 
//generateNoise();
//generateSinNoise();

//int x=0;
 
}

void generateSinNoise() {
  for (x=0;x<180;x++) { 
  digitalWrite(9,random(x,sin(x)));
  digitalWrite(9,random(x/2));
  break;
  }
}

void generateSaw() {
  for (x=0;x<map(analogRead(0),0, 1023, 1, 254);x++) { 
  //analogWrite(9,1/sin(x));
  analogWrite(9,x);
  delay(1);
  //digitalWrite(9,random(x/2));
  break;
  }
  for (y=map(analogRead(0),0, 1023, 1, 254);y>0;y--) { 
  //analogWrite(9,1/sin(x));
  analogWrite(9,y);
  delay(1);
  //digitalWrite(9,random(x/2));
  break;
  }
}


void generateTriangle() {
  for (x=0;x<map(analogRead(0),0, 1023, 1, 254);x=x+1) { 
  //analogWrite(9,1/sin(x));
  analogWrite(9,x);
  delay(1);
  //digitalWrite(9,random(x/2));
  //break;
  }
  for (y=map(analogRead(0),0, 1023, 1, 254);y>0;y=y-1) { 
  //analogWrite(9,1/sin(x));
  analogWrite(9,y);
  delay(1);
  //digitalWrite(9,random(x/2));
  //break;
  }
}

void generateSine() {
             analogWrite(9, 10); delay(10);
            analogWrite(9, 19); delay(10);
            analogWrite(9, 45); delay(10);
            analogWrite(9, 84); delay(10);
            analogWrite(9, 130); delay(10);
            analogWrite(9, 176); delay(10);
            analogWrite(9, 215); delay(10);
            analogWrite(9, 241); delay(10);
            analogWrite(9, 250); delay(10);
            analogWrite(9, 241); delay(10);
            analogWrite(9, 215); delay(10);
            analogWrite(9, 176); delay(10);
            analogWrite(9, 130); delay(10);
            analogWrite(9, 84); delay(10);
            analogWrite(9, 45); delay(10);
            analogWrite(9, 19); delay(10);
            analogWrite(9, 0); delay(10);
}



void generateSquare() {
          //  digitalWrite(9, 250); delay(200);
          //  digitalWrite(9, 0); delay(200);
          analogWrite(6,map(analogRead(0),0, 1023, 1, 254)); 

}


//https://arduino.stackexchange.com/questions/6715/audio-frequency-white-noise-generation-using-arduino-mini-pro
 void generateNoise(){
   unsigned long int newr;
    int reg = 0;
   unsigned char lobit;
   unsigned char b31, b29, b25, b24;
   b31 = (reg & (1L << 31)) >> 31;
   b29 = (reg & (1L << 29)) >> 29;
   b25 = (reg & (1L << 25)) >> 25;
   b24 = (reg & (1L << 24)) >> 24;
   lobit = b31 ^ b29 ^ b25 ^ b24;
   newr = (reg << 1) | lobit;
   reg = newr;
   digitalWrite (9, reg & 1);
   delayMicroseconds (20);} // Changing this value changes the frequency.
