/*  Example of a sound being triggered by MIDI input.
 *   
 *   +  Example of filtering a wave,
    using Mozzi sonification library.
  
    Demonstrates LowPassFilter().
  
    Demonstrates playing notes with Mozzi in response to MIDI input,
    using  Arduino MIDI library v4.2 
    (https://github.com/FortySevenEffects/arduino_midi_library/releases/tag/4.2)
  
    Circuit: 
      MIDI input circuit as per http://arduino.cc/en/Tutorial/Midi
      Note: midi input on rx pin, not tx as in the illustration on the above page.
      Midi has to be disconnected from rx for sketch to upload.
      Audio output on digital pin 9 on a Uno or similar.
    
    Mozzi help/discussion/announcements:
    https://groups.google.com/forum/#!forum/mozzi-users
  
    Tim Barrass 2013-14, CC by-nc-sa.
*/


#include <MIDI.h>

#include <MozziGuts.h>
#include <Oscil.h> // oscillator template
#include <tables/chum9_int8.h> // recorded audio wavetable
#include <tables/cos2048_int8.h> // for filter modulation
#include <LowPassFilter.h>
#include <mozzi_rand.h>

#include <tables/sin2048_int8.h> // sine table for oscillator
#include <mozzi_midi.h>
#include <ADSR.h>


MIDI_CREATE_DEFAULT_INSTANCE();

// use #define for CONTROL_RATE, not a constant
#define CONTROL_RATE 128 // powers of 2 please

// audio sinewave oscillator
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);

// envelope generator
ADSR <CONTROL_RATE, AUDIO_RATE> envelope;

// low pass

Oscil<CHUM9_NUM_CELLS, AUDIO_RATE> aCrunchySound(CHUM9_DATA);
Oscil<COS2048_NUM_CELLS, CONTROL_RATE> kFilterMod(COS2048_DATA);

LowPassFilter lpf;



#define LED 8 // shows if MIDI is being recieved

void HandleNoteOn(byte channel, byte note, byte velocity) { 
  //lowpass
    aCrunchySound.setFreq(2.f);
  kFilterMod.setFreq((float)(note));
  lpf.setResonance(200);
  
 // aSin.setFreq(mtof(float(note)));
  if (rand(CONTROL_RATE/2) == 0){ // about once every half second
  kFilterMod.setFreq((float)(note));
  } 
  byte cutoff_freq = 100 + kFilterMod.next()/2;
  lpf.setCutoffFreq(cutoff_freq);
  envelope.noteOn();
  digitalWrite(LED,HIGH);
}

void HandleNoteOff(byte channel, byte note, byte velocity) { 
  envelope.noteOff();
  digitalWrite(LED,LOW);
}

void setup() {
  pinMode(LED, OUTPUT);  

  // Connect the HandleNoteOn function to the library, so it is called upon reception of a NoteOn.
  MIDI.setHandleNoteOn(HandleNoteOn);  // Put only the name of the function
  MIDI.setHandleNoteOff(HandleNoteOff);  // Put only the name of the function
  // Initiate MIDI communications, listen to all channels (not needed with Teensy usbMIDI)
  MIDI.begin(MIDI_CHANNEL_OMNI);  
    
  envelope.setADLevels(255,64);
  envelope.setTimes(50,200,10000,200); // 10000 is so the note will sustain 10 seconds unless a noteOff comes

  aSin.setFreq(440); // default frequency
  startMozzi(CONTROL_RATE); 

// lowpass
  aCrunchySound.setFreq(2.f);
  kFilterMod.setFreq(1.3f);
  lpf.setResonance(200);
}


void updateControl(){
/*   if (rand(CONTROL_RATE/2) == 0){ // about once every half second
    kFilterMod.setFreq((float)rand(255)/64);  // choose a new modulation frequency
  }
  // map the modulation into the filter range (0-255)
  byte cutoff_freq = 100 + kFilterMod.next()/2;
  lpf.setCutoffFreq(cutoff_freq);
  */
  MIDI.read();
  envelope.update();
  
}


int updateAudio(){
 // return (int) (envelope.next() * aSin.next())>>8;

//lowpass
  char asig = lpf.next(aCrunchySound.next());
  return (int) asig;
}


void loop() {
  audioHook(); // required here
} 



