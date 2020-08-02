// Arduino Drone Sounds Generator
//
// for Garvuino + expansion card with 5 potentiometers
//
//
// There are four voices.
//
// Digital 3: Audio out 
//
// On GARVUINO v2.10: connect PIN D3 to PIN AY>L or AY>R (under the L or R) to make the audio output!
// On GARVUINO v1.09: remove the arduino out of the socket
//      Then connect PIN D3 to PIN AY>L (under the L label) and GND to the GND in J1 on Garvuino (or on top of J4)
//        in order to get the audio output!


/*

There are 4 modes, activated by switch sw1 (led blink for each change, hold switch if necessary):

== 1 ==

4 oscillators mode: sine, triangle, square and/or ramps.

You can change the pitch for each osc with knobs 1 to 4 (u1, u2, u3, u4).
You can change the oscillators with the 5th knob (u5) :

1: sine + sine + sine + sine
2: sine + sine + sine + tri
3: sine + sine + tri + tri
4: sine + tri + tri + tri
5: tri + tri + tri + tri
6: tri + tri + tri + sq
7: tri + tri + sq + sq
8: tri + sq + sq + sq
9: sq + sq + sq + sq
10: sq + sq + sq + ramp
11: sq + sq + ramp + ramp
12: sq + ramp + ramp + ramp
13: ramp + ramp + ramp + ramp
14: ramp + sq + tri + sin



== 2 ==

Same as above, but the 4th oscillator is now a noise sound.


== 3 ==

Beat mode.

Low kick generator. You can change the beat with the 4th switch, 
and the tonality and osc combinations with the others.


== 4 ==

Drum mode.

Simple drum generator. You can change the beat with the 4th switch, 
and the tonality and osc combinations with the others.




 
  */

 

// Made with "The Synth"
// http://blog.dzl.dk/


// synth.h is in the arduino_sketches/libs/garvuino/ folder
#include <synth.h>
#include "ssd1306.h"  // oled display by Alexey Dynda
#include <garvuino.h>


int switchInPin1 = 6;  

int switchInPin2 = 10; // not used
int switchInPin3 = 7;  // not used

#define LED 8          // 13 on previous shield 
byte ledStatus=0;
byte mode=0;
int modal=0;

const int KNOB_PIN_0 = 0; // set the input for the knob to analog pin 0
int knob_0 = 100; // initialisation


const int KNOB_PIN_1 = 1; // set the input for the knob to analog pin 1
int knob_1 = 100; // initialisation


const int KNOB_PIN_2 = 2; // set the input for the knob to analog pin 2
int knob_2 = 100; // initialisation


const int KNOB_PIN_3 = 3; // set the input for the knob to analog pin 3
int knob_3 = 100; // initialisation

const int KNOB_PIN_4 = 4; // set the input for the knob to analog pin 4
int knob_4 = 100; // initialisation


#define OLED 


synth edgar;        //-Make a synth


void BlinkLed(byte num)         // Basic blink function
{
    for (byte i=0;i<num;i++)
    {
        digitalWrite(LED,HIGH);
        delay(200);
        digitalWrite(LED,LOW);
        delay(200);
    }
}


void changeMode()
{
    mode++; 
    Serial.println("Mode: ");  Serial.println(mode);
      if (mode==0) {  
               BlinkLed(1); 
               }

             if (mode==1) { // rectangle
               BlinkLed(2);     
            //    edgar.setEnvelope(0,1);
            //    edgar.setEnvelope(1,1);
            //    edgar.setLength(1,82);
            //    edgar.setMod(1,64);
             //   edgar.setWave(1,SQUARE);
            //    edgar.mTrigger(1,75);    
               }
               
             if (mode==2) {  // saw
                    BlinkLed(3);           
               }
             if (mode==3) {  // saw
                     BlinkLed(4);             
               
               }
             if (mode==4) {
                 mode=-1;             
               
               }

}


void testSwitches()
{
int sensorVal = digitalRead(switchInPin1); // push button
   // push button
   if (sensorVal == HIGH) {
  // DO NOTHING
  } else {
// DO
  changeMode();
  }
}

void testRotary()
{
 if ((modal > 0 ) && (modal <= 10)) {
      edgar.setWave(0,SINE);
      edgar.setWave(1,SINE);
      edgar.setWave(2,SINE);
      edgar.setWave(3,SINE);
 }
 if ((modal > 10 ) && (modal <= 20))   {
      edgar.setWave(0,SINE);
      edgar.setWave(1,SINE);
      edgar.setWave(2,SINE);
      edgar.setWave(3,TRIANGLE);
 }
  if ((modal > 20 ) && (modal <= 30))   {
      edgar.setWave(0,SINE);
      edgar.setWave(1,SINE);
      edgar.setWave(2,TRIANGLE);
      edgar.setWave(3,TRIANGLE);
 }
   if ((modal > 30 ) && (modal <= 40))   {
      edgar.setWave(0,SINE);
      edgar.setWave(1,TRIANGLE);
      edgar.setWave(2,TRIANGLE);
      edgar.setWave(3,TRIANGLE);
 }
   if ((modal > 40 ) && (modal <= 50))   {
      edgar.setWave(0,TRIANGLE);
      edgar.setWave(1,TRIANGLE);
      edgar.setWave(2,TRIANGLE);
      edgar.setWave(3,TRIANGLE);
 }
    if ((modal > 50 ) && (modal <= 60))   {
      edgar.setWave(0,TRIANGLE);
      edgar.setWave(1,TRIANGLE);
      edgar.setWave(2,TRIANGLE);
      edgar.setWave(3,SQUARE);
 }
     if ((modal > 60 ) && (modal <= 70))   {
      edgar.setWave(0,TRIANGLE);
      edgar.setWave(1,TRIANGLE);
      edgar.setWave(2,SQUARE);
      edgar.setWave(3,SQUARE);
 }
      if ((modal > 70 ) && (modal <= 80))   {
      edgar.setWave(0,TRIANGLE);
      edgar.setWave(1,SQUARE);
      edgar.setWave(2,SQUARE);
      edgar.setWave(3,SQUARE);
 }
       if ((modal > 80 ) && (modal <= 90))   {
      edgar.setWave(0,SQUARE);
      edgar.setWave(1,SQUARE);
      edgar.setWave(2,SQUARE);
      edgar.setWave(3,SQUARE);
 }
        if ((modal > 90 ) && (modal <= 100))   {
      edgar.setWave(0,SQUARE);
      edgar.setWave(1,SQUARE);
      edgar.setWave(2,SQUARE);
      edgar.setWave(3,RAMP);
 }
        if ((modal > 100 ) && (modal <= 110))   {
      edgar.setWave(0,SQUARE);
      edgar.setWave(1,SQUARE);
      edgar.setWave(2,SQUARE);
      edgar.setWave(3,RAMP);
 }
         if ((modal > 110 ) && (modal <= 120))   {
      edgar.setWave(0,SQUARE);
      edgar.setWave(1,SQUARE);
      edgar.setWave(2,RAMP);
      edgar.setWave(3,RAMP);
 }
         if ((modal > 120 ) && (modal <= 130))   {
      edgar.setWave(0,SQUARE);
      edgar.setWave(1,RAMP);
      edgar.setWave(2,RAMP);
      edgar.setWave(3,RAMP);
 }
        if ((modal > 130 ) && (modal <= 140))   {
      edgar.setWave(0,RAMP);
      edgar.setWave(1,RAMP);
      edgar.setWave(2,RAMP);
      edgar.setWave(3,RAMP);
 }
       if ((modal > 140 ) && (modal <= 150))   {
      edgar.setWave(0,RAMP);
      edgar.setWave(1,SQUARE);
      edgar.setWave(2,TRIANGLE);
      edgar.setWave(3,SINE);
 }
}


void Kick()
{
  
 
  edgar.setWave(0,TRIANGLE); // try SQUARE for electro
  edgar.setEnvelope(0,58);
  edgar.setLength(0,45);  // 64 for electro kick
  edgar.setMod(0,21);
  
  edgar.mTrigger(0,35);

  //digitalWrite(LED,HIGH);
  
  }


void Snare()
{
    edgar.setWave(0,NOISE); //
  edgar.setEnvelope(0,10);
  edgar.setLength(0,70);  // 
  edgar.setMod(0,45);
  
  edgar.mTrigger(0,35);
  }


void HiHat()
{
// hi hat

  edgar.setWave(2,NOISE); // 
  edgar.setEnvelope(2,90);
  edgar.setLength(2,37);  //
  edgar.setMod(2,120);

  edgar.mTrigger(2,55);
  
}






void setup() 
{

  Serial.begin(9600); 
  
  edgar.begin(CHB);          //Init synth

 //   edgar.begin();                                   //-Start it up
  edgar.setupVoice(0,SINE,60,ENVELOPE1,60,64);  //-Set up voice 0
  edgar.setupVoice(1,SINE,60,ENVELOPE1,60,64);      //-Set up voice 1
  edgar.setupVoice(2,SINE,60,ENVELOPE1,60,64);      //-Set up voice 1
  edgar.setupVoice(3,SINE,60,ENVELOPE1,60,64);      //-Set up voice 1

  
  
  pinMode(8,OUTPUT);

  pinMode(switchInPin1, INPUT_PULLUP);
  pinMode(switchInPin2, INPUT_PULLUP);
  pinMode(switchInPin3, INPUT_PULLUP);

  
  edgar.setWave(0,TRIANGLE);
 // edgar.setUserWave(0,USER05);
  edgar.setEnvelope(0,1);
  edgar.setLength(0,82);
  edgar.setMod(0,64);

  
  //  ssd1306_128x64_i2c_init();

 // ssd1306_fillScreen(0x00);
 //   ssd1306_drawBitmap(0, 0, 128, 64, garvuino_logo);
 //   delay(20);
//    garvuino_welcome();
}

void loop()
{

  testSwitches();
  
  int knob_0 = analogRead(KNOB_PIN_0); // value is 0-1023
  int knob_0_remap = map(knob_0,0,1023, 0, 110);

  int knob_1 = analogRead(KNOB_PIN_1); // value is 0-1023
  int knob_1_remap = map(knob_1,0,1023, 0, 110);

  int knob_2 = analogRead(KNOB_PIN_2); // value is 0-1023
  int knob_2_remap = map(knob_2,0,1023, 0, 110);

  int knob_3 = analogRead(KNOB_PIN_3); // value is 0-1023
  int knob_3_remap = map(knob_3,0,1023, 0, 110);

  int knob_4 = analogRead(KNOB_PIN_4); // value is 0-1023
  int knob_4_remap = map(knob_4,0,1023, 0, 150);
  modal = knob_4_remap;

  testRotary();
  
  edgar.setFrequency(0,knob_0_remap);
  edgar.trigger(0);

  edgar.setFrequency(1,knob_1_remap);
  edgar.trigger(1);

  edgar.setFrequency(2,knob_2_remap);
  edgar.trigger(2);

  edgar.setFrequency(3,knob_3_remap);
  edgar.trigger(3);

 // only 4 voices, no more!

 if (mode == 1) {
  edgar.setWave(3,NOISE);
 }
 
 if (mode == 2) {
  Kick();
    digitalWrite(LED,HIGH);
  delay(knob_3_remap*5);
    digitalWrite(LED,LOW);
  delay(knob_3_remap*5);
    //if (ledStatus == 1) { digitalWrite(LED,HIGH);ledStatus = 0 ;} else {digitalWrite(LED,LOW); ledStatus++ ;}
  Serial.write(" Tempo:");
  Serial.print(knob_3_remap);
  // note: the displayed tempo won't be accurate because of arduino latency
 }

  if (mode == 3) {
  Kick();
  digitalWrite(LED,HIGH);
  HiHat();
  delay(knob_3_remap*10);
    HiHat();
    digitalWrite(LED,LOW);
  delay(knob_3_remap*10);
  Snare();
  digitalWrite(LED,HIGH);
  HiHat();
  delay(knob_3_remap*10);
    HiHat();
    digitalWrite(LED,LOW);
  delay(knob_3_remap*10);
  Serial.write(" Tempo:");
  Serial.print(knob_3_remap);
 }


 
}
