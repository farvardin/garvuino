// notes generator for AY chip, controlled by potentiometers

// version for Garvuino https://github.com/farvardin/garvuino
// Works with Arduino IDE 1.8.3

// Connect potentiometers on analog 0, 1 and 2 of the arduino


#include <SPI.h>

int switchInPin = 6;   
int LedPin = 8;
#define LED 8

const int KNOB_PIN = 0; // set the input for the knob to analog pin 0
const int KNOB_PIN1 = 1; // set the input for the knob to analog pin 1
const int KNOB_PIN2 = 2; // set the input for the knob to analog pin 2
const int KNOB_PIN3 = 3; // set the input for the knob to analog pin 3

  int knob_value = 0;
  int knob_value1 = 0;
  int knob_value2 = 0;
  int knob_value3 = 0;

int current_value = 2000;  //tone
int tempo = 60;
int sweep = 20;

int volume = 32;



void setup()
{

  pinMode(switchInPin, INPUT_PULLUP);
  pinMode(LedPin, OUTPUT);
  Serial.begin(57600);
}



static byte buf2[29];




void loop()
{

  knob_value = analogRead(KNOB_PIN); // value is 0-1023
  knob_value1 = analogRead(KNOB_PIN1); // value is 0-1023
  knob_value2 = analogRead(KNOB_PIN2); // value is 0-1023
  knob_value3 = analogRead(KNOB_PIN3); // value is 0-1023
  

/*
  current_value = map(knob_value,0,1023, 0, 4096); //tone
  tempo = map(knob_value1,0,1023, 0, 120);  // speed
  sweep = map(knob_value2,0,1023, 5, 100);  // define how many time to refresh the AY (= duration of the note/enveloppe)
  */
  

// 
  
  //playRandom1(); 
  //playRandom2(current_value/8); 


  //playSeq2();


int variation1 = map(knob_value3,0,1023, 1, 8);

  playArp(2,variation1,5);
  
  //playNote(current_value,current_value/256);
  
  // noise
  
  /*current_value = constrain(map(knob_value,0,1023, 1, 31),1, 31);
  playNoise(random(current_value,current_value-2));
  */

  /*current_value = map(knob_value,0,1023, 0, 32); //freq
  tempo = map(knob_value1,0,1023, 0, 15);  // volume
  sweep = map(knob_value2,0,1023, 5, 100);  // define how many time to refresh the AY (= duration of the note/enveloppe)
  
   playNoise(current_value, tempo);*/
}




void playNote(int r0, int r1)
{
  for (byte i=0;i<20;i++)
  {
         //tone(sAudioOutPin, sNotePitches[currentNote]);
          buf2[0] = 8;  //Select register #8
          buf2[1] = 15; //Volume channel A full
          buf2[2] = 0; //Select register #0
          buf2[3] = r0; // Write #93 into register #0
          buf2[4] = 1;  //Select register #1
          buf2[5] = r1; // Write #13 into register #1
          buf2[6] = 7; // Select register #7
          buf2[7] = 62;  //Enable output Channel A (0011 1110)
 //Serial.print(255,1);
 Serial.write(buf2,8);
 //  digitalWrite(LED,HIGH);
// delay(20);
//digitalWrite(LED,LOW);
  }
}




void playEnveloppe(int r0, int r1)
{
  for (byte i=0;i<sweep;i++)
  {
          buf2[0] = 8;  //Select register #8
          buf2[1] = 15; //Volume channel A full
          buf2[2] = 0; //Select register #0
          buf2[3] = r0; // Write #93 into register #0
          buf2[4] = 1;  //Select register #1
          buf2[5] = r1; // Write #13 into register #1
          buf2[6] = 11; // Select register #B
          buf2[7] = r1; // Write # into register #B
          buf2[8] = 12; // Select register #C
          buf2[9] = r0; // Write # into register #C
          buf2[10] = 13; // Select register #D
          buf2[11] = 15; // Write # into register #D
          buf2[12] = 7; // Select register #7
          buf2[13] = 62;  //Enable output Channel A (0011 1110)
 //Serial.print(255,1);
 Serial.write(buf2,14);
   digitalWrite(LED,HIGH);
// delay(20);
//digitalWrite(LED,LOW);
  }
}


void playNull()
{
  for (byte i=0;i<20;i++)
  {
         //tone(sAudioOutPin, sNotePitches[currentNote]);
          buf2[0] = 8;  //Select register #8
          buf2[1] = 0; //Volume channel A null
          buf2[2] = 0; //Select register #0
          buf2[3] = 0; // Write #93 into register #0
          buf2[4] = 1;  //Select register #1
          buf2[5] = 0; // Write #13 into register #1
          buf2[6] = 7; // Select register #7
          buf2[7] = 62;  //Enable output Channel A (0011 1110)
 //Serial.print(255,1);
 Serial.write(buf2,8);
  // digitalWrite(LED,HIGH);
// delay(20);
digitalWrite(LED,LOW);
  }
}

void playNoise(int r0, int r1)
{
  for (byte i=0;i<sweep;i++)
  {
          buf2[0] = 8;  //Select register #8
          buf2[1] = r1; //Volume channel A 
          buf2[2] = 6; //Select register #6 (noise)
          buf2[3] = r0; // Write #n into register #6
          buf2[4] = 7; // Select register #7
          buf2[5] = 55;  //Enable output Channel A noise only (0011 0111)
 Serial.print(255,1);
 Serial.write(buf2,6);
   digitalWrite(LED,HIGH);
// delay(20);
digitalWrite(LED,LOW);
  }
}


void playSeq1()
{
  current_value = map(knob_value,0,1023, 0, 4096); //freq
  tempo = map(knob_value1,0,1023, 0, 80);  // tempo
  sweep = map(knob_value2,0,1023, 5, 100);  // define how many time to refresh the AY (= duration of the note/enveloppe)
  
  playEnveloppe(current_value,current_value/256/2);
  playNull();
  delay(tempo);
}

void playSeq2()
{
  current_value = map(knob_value,0,1023, 0, 4096); //freq
  tempo = map(knob_value1,0,1023, 0, 80);  // tempo
  sweep = map(knob_value2,0,1023, 5, 100);  // define how many time to refresh the AY (= duration of the note/enveloppe)
  
  playEnveloppe(current_value,current_value/256/2);
  playNull();
  delay(tempo);
  playEnveloppe(current_value*2,current_value/256);
    playNull();
  delay(tempo);
}

void playArp(int a, int b, int c) 
{
  current_value = map(knob_value,0,1023, 0, 4096); //freq
  tempo = map(knob_value1,0,1023, 0, 80);  // tempo
  sweep = map(knob_value2,0,1023, 5, 100);  // define how many time to refresh the AY (= duration of the note/enveloppe)
  
  playEnveloppe(current_value,current_value/256/2);
  playNull();
  delay(tempo);
  
  playEnveloppe(current_value*a/b,current_value/256/2*a/b);
  playNull();
  delay(tempo);

    
  playEnveloppe(current_value*b/c,current_value/256/2*b/c);
  playNull();
  delay(tempo);
}

void playRandom1()
{
playNote(random(1,256),random(8,12));
playNote(random(1,256),random(current_value,current_value+2));
}

void playRandom2(int rand1)
{
playNote(random(1,rand1),random(8,12));
playNote(random(1,256),random(current_value,current_value+2));
}
