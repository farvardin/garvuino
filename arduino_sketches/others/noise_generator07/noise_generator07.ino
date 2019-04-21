/*  Example playing an enveloped noise source
    using Mozzi sonification library.
  
    Demonstrates Ead (exponential attack decay).
  
    Circuit: Audio output on digital pin 9 on a Uno or similar, or
    DAC/A14 on Teensy 3.0/3.1, or 
    check the README or http://sensorium.github.com/Mozzi/
  
    Mozzi help/discussion/announcements:
    https://groups.google.com/forum/#!forum/mozzi-users
  
    Tim Barrass 2012, CC by-nc-sa
 */

//#include <ADC.h>  // Teensy 3.0/3.1 uncomment this line and install http://github.com/pedvide/ADC
#include <MozziGuts.h>
#include <Oscil.h> // oscillator template
#include <tables/brownnoise8192_int8.h> // recorded audio wavetable
#include <tables/sin2048_int8.h> // sine table for oscillator
#include <Ead.h> // exponential attack decay
#include <EventDelay.h>
#include <mozzi_rand.h>

#define CONTROL_RATE 256 // powers of 2 please

Oscil<BROWNNOISE8192_NUM_CELLS, AUDIO_RATE> aNoise(BROWNNOISE8192_DATA);
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);

EventDelay kDelay; // for triggering envelope start
Ead kEnvelope(CONTROL_RATE); // resolution will be CONTROL_RATE

int gain;


void setup(){
  // use float to set freq because it will be small and fractional
  aNoise.setFreq((float)AUDIO_RATE/BROWNNOISE8192_SAMPLERATE);
  //aSin.setFreq(0.05f);
  randSeed(); // fresh random, MUST be called before startMozzi - wierd bug
  startMozzi(CONTROL_RATE);
  kDelay.start(1000);
}


void updateControl(){
  // jump around in audio noise table to disrupt obvious looping
  aNoise.setPhase(rand((unsigned int)BROWNNOISE8192_NUM_CELLS));
  
  int value1 = map(analogRead(2),0, 1023, 1, 100);
  aSin.setFreq(20.0f/value1); // set the frequency
  
  if(kDelay.ready()){
    // set random parameters
   // unsigned int duration = rand(500u)+200;
    unsigned int duration =  rand(10u)*map(analogRead(0),0, 1023, 0, 15)+map(analogRead(0),0, 1023, 1, 1023);
    //unsigned int attack = rand(75)+5; // +5 so the internal step size is more likely to be >0
    unsigned int attack = rand(75)+map(analogRead(1),0, 1023, 5, 40);; // +5 so the internal step size is more likely to be >0
    unsigned int decay = duration - attack;
    kEnvelope.start(attack,decay);
    kDelay.start(duration+500);
  }
  gain = (int) kEnvelope.next();
}


int updateAudio(){
  return (gain*aNoise.next())>>8;
}


void loop(){
  audioHook(); // required here
}
