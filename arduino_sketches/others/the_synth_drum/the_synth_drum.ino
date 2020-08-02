// Arduino synth library DRUM example
//
// Garvuino adaptation 
//
// Each of the four voices are assigned to MIDI channel 1,2,3 and 4
// For each channel:
// NOTE_ON with velocity over zero triggers voice. 
// Controller mWave controls waveform: SINE[0-21] TRIANGLE[22-42] SQUARE[43-63] SAW[64-84] RAMP[85-105] NOISE[106-127]
// Controller mEnv controls envelope: ENVELOPE0[0-32] ENVELOPE1[33-64] ENVELOPE2[65-96] ENVELOPE3[97-127]
// Controller mLength controls length: [0-127]
// Controller mMod  controls modulation: negative[0-63] neutral[64] positive[65-127]
//
// Digital 3: Audio out 
// Connect midi on the DIN5 of the Garvuino (it doesn't work with usb midi)
//
// On GARVUINO v2.10: connect PIN D3 to PIN AY>L or AY>R (under the L or R) to make the audio output!
// On GARVUINO v1.09: remove the arduino out of the socket
//      Then connect PIN D3 to PIN AY>L (under the L label) and GND to the GND in J1 on Garvuino (or on top of J4)
//        in order to get the audio output!
//      And connect the RX0 from the Arduino to the second pin hole from the top left on the empty
//        socket, to get the midi connection.
//
//      Use Switches sw1, sw2 and sw3 to trigger drum wavetables


// original code and idea: 
// https://github.com/dzlonline/the_synth/blob/master/examples/MIDI_synth/MIDI_synth.ino
// DZL 2014
// http://blog.dzl.dk/


// synth.h is in the arduino_sketches/libs/garvuino/ folder
#include <synth.h>
#include "ssd1306.h"  // oled display by Alexey Dynda
#include <garvuino.h>
#include "MIDI_parser.h"

int switchInPin1 = 6;  
int switchInPin2 = 10; 
int switchInPin3 = 7; 

const int KNOB_PIN_0 = 0; // set the input for the knob to analog pin 0
int knob_0 = 100; // initialisation

const int KNOB_PIN_1 = 1; // set the input for the knob to analog pin 1
int knob_1 = 100; // initialisation


#define OLED 

/* original version
#define mWave 13 
#define mEnv  12
#define mLength 10
#define mMod  7
*/

// for use with my Behringer UMA 25S controllers:
#define mWave 74 
#define mEnv  71
#define mLength 73
#define mMod  75

// try mWave(75), mEnv(55), mLength(68), mMod(64)


//#define mPWM  72
//int PWM = 125;


synth edgar;        //-Make a synth
midiParser parser;  //-Make a MIDI parser

void Kick()
{
  

  edgar.setWave(0,SQUARE); // try SQUARE for electro
  edgar.setEnvelope(0,58);
  edgar.setLength(0,45);  // 64 for electro kick
  edgar.setMod(0,21);
  
  edgar.mTrigger(0,35);
  //edgar.trigger(0);
  }


void Snare()
{
    edgar.setWave(1,NOISE); //
  edgar.setEnvelope(1,10);
  edgar.setLength(1,65);  // 
  edgar.setMod(1,45);
  
  edgar.mTrigger(1,35);
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


void testSwitches()
{
int sensorVal = digitalRead(switchInPin1); // push button
   if (sensorVal == HIGH) {
  // DO NOTHING
  } else {
  // DO
  //edgar.setUserWave(0,USER00);
  
  // KICK
  
  edgar.setWave(0,SQUARE); // try SQUARE for electro
  edgar.setEnvelope(0,58);
  edgar.setLength(0,24);  // 64 for electro kick
  edgar.setMod(0,22);
  
  edgar.mTrigger(0,35);
  }

   sensorVal = digitalRead(switchInPin2); // push button
   if (sensorVal == HIGH) {
  // DO NOTHING
  } else {
  // DO

  // SNARE
    edgar.setWave(1,NOISE); //
  edgar.setEnvelope(1,10);
  edgar.setLength(1,30);  // 
  edgar.setMod(1,45);
  
  edgar.mTrigger(1,35);

  }

 sensorVal = digitalRead(switchInPin3); // push button
   if (sensorVal == HIGH) {
  // DO NOTHING
  } else {
  // DO
  
// hi hat

// hit hat
  edgar.setWave(2,NOISE); // 
  edgar.setEnvelope(2,45);
  edgar.setLength(2,15);  //
  edgar.setMod(2,21);

  edgar.mTrigger(2,155);

  }
  
}

void setup() 
{
 // Serial.begin(31250);    //MIDI BAUD rate
  //Serial.begin(38400);    //MIDI BAUD rate
  edgar.begin(CHB);          //Init synth
  pinMode(8,OUTPUT);

  pinMode(switchInPin1, INPUT_PULLUP);
  pinMode(switchInPin2, INPUT_PULLUP);
  pinMode(switchInPin3, INPUT_PULLUP);

  /*
  edgar.setWave(0,TRIANGLE);
  edgar.setUserWave(0,USER00);
  edgar.setEnvelope(0,1);
  edgar.setLength(0,82);
  edgar.setMod(0,64);
*/
  edgar.setupVoice(0,SQUARE,60,ENVELOPE1,60,64);  //-Set up voice 0
  edgar.setupVoice(1,SINE,60,ENVELOPE1,60,64);      //-Set up voice 1
  edgar.setupVoice(2,SINE,60,ENVELOPE1,60,64);      //-Set up voice 1
  edgar.setupVoice(3,SINE,60,ENVELOPE1,60,64);      //-Set up voice 1


  // no sound if enabled:
  /*  ssd1306_128x64_i2c_init();

  ssd1306_fillScreen(0x00);
    ssd1306_drawBitmap(0, 0, 128, 64, garvuino_logo);
    delay(20);
    garvuino_welcome();
    */
}


void Seq01(int tempo)
{
  int wait = 60000 / tempo;
  Kick();HiHat();
  delay(wait);
  HiHat();
  delay(wait);
  Snare();HiHat();
  delay(wait);
  HiHat();
  delay(wait);
}


void Seq02(int tempo)
{
  int wait = 60000 / tempo;
  Kick();HiHat();
  delay(wait);
  HiHat();
  delay(wait);
  Snare();HiHat();
  delay(wait/2);
  Snare();
  delay(wait/2);
  HiHat();
  delay(wait/2);
  HiHat();
  delay(wait/2);
}

void Seq03(int tempo)
{
  int wait = 60000 / tempo;
  Kick();HiHat();
  delay(wait/2);
  Kick();HiHat();
  delay(wait/2);
  HiHat();
  delay(wait);
  Snare();HiHat();
  delay(wait);
  HiHat();
  delay(wait);
}

void loop()
{
 // unsigned char voice;
  //testSwitches();
 // edgar.setFrequency(0,440);
 // edgar.trigger(0);

  int knob_0 = analogRead(KNOB_PIN_0); // value is 0-1023
  int knob_0_remap = map(knob_0,0,1023, 0, 400);

  int knob_1 = analogRead(KNOB_PIN_1); // value is 0-1023
  int knob_1_remap = map(knob_1,0,1023, 0, 110);

  
  Seq01(knob_0_remap);
  Seq02(knob_0_remap);
  Seq01(knob_0_remap);
  Seq03(knob_0_remap);
  
}


