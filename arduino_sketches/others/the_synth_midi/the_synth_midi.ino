// Arduino synth library MIDI example
//
// Garvuino adaptation 
//
// The arduino receives MIDI over Serial at 31250 BAUD
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
// Connect midi on the DIN5 of the Garvuino
//
// On GARVUINO v2.10: connect PIN3 to PIN AY>L or AY>R (under the L or R) to make the audio output!
// On GARVUINO v1.09: remove the arduino out of the socket
//      Then connect PIN3 to PIN AY>L (under the L label) and GND to the GND in J1 on Garvuino
//      in order to get the audio output!


// original code and idea: 
// https://github.com/dzlonline/the_synth/blob/master/examples/MIDI_synth/MIDI_synth.ino
// DZL 2014
// http://blog.dzl.dk/


#include <synth.h>
#include "MIDI_parser.h"



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


synth edgar;        //-Make a synth
midiParser parser;  //-Make a MIDI parser

void setup() 
{
  Serial.begin(31250);    //MIDI BAUD rate
  edgar.begin(CHB);          //Init synth
  pinMode(13,OUTPUT);
}

void loop()
{
  unsigned char voice;
  while(Serial.available())
  {
    if(parser.update(Serial.read()))  //-Feed MIDI stream to parser and execute commands
    {
      switch(parser.midi_cmd)
      {
        //*********************************************
        // Handle MIDI notes
        //*********************************************
      case 0x90: //-Channel 1 (voice 0)
      case 0x91: //-Channel 2 (voice 1)
      case 0x92: //-Channel 3 (voice 2)
      case 0x93: //-Channel 4 (voice 3)

        voice = parser.midi_cmd-0x90;
        if(parser.midi_2nd)  //-Velocity not zero (could implement NOTE_OFF here)
          edgar.mTrigger(voice,parser.midi_1st);
        break;

        //*********************************************
        // Handle MIDI controllers
        //*********************************************
      case 0xb0:  //-Channel 1 (voice 0)
      case 0xb1:  //-Channel 2 (voice 1)
      case 0xb2:  //-Channel 3 (voice 2)
      case 0xb3:  //-Channel 4 (voice 3)
        voice=parser.midi_cmd-0xb0;
        switch(parser.midi_1st)  //-Controller number
        {
        case mWave:  //-Controller 13 
          edgar.setWave(voice,parser.midi_2nd/21);
          break;
        case mEnv:  //-Controller 12
          edgar.setEnvelope(voice,parser.midi_2nd/32);
          break;   
        case mLength:  //-Controller 10
          edgar.setLength(voice,parser.midi_2nd);
          break;  
        case mMod:   //-Controller 7
          edgar.setMod(voice,parser.midi_2nd);
          break;
        }
        break;
      }
    }
  }
}




