// Arduino synth example song data adapted from Tandy Color Computer
//  https://www.youtube.com/watch?v=0_OW5CkqFvQ
//
//  THE WILLIAM TELLOVERTURE
//  BY GARRY AND LINDA HOWARD
//  8931 COMANCHE ROAD LONGMONT, COLORADO 80501
//  COPYRIGHT 1982
//
//  Adapted for Arduino by Rich Cook in March 2016
//  Hardware connections:
//
//                    +100µF
//PIN 3 ---[ 1k ]--+---||--->> Audio out
//                  |
//                 === 100nF
//                  |
//                 GND
//
// DZL 2014
// HTTP://dzlsevilgeniuslair.blogspot.dk
// HTTP://illutron.dk

#include <avr/pgmspace.h>
#include "synth.h"

synth edgar;    //-Make a synth

#include "SongDat3.h"
char note = 0, i = 0;
int ndel = 0, frame = 0;
void setup() {
 //edgar.begin();  //-Start it up (uses default PIN 11)
 edgar.begin(CHB);  //            (uses PIN 3)
 
  //Serial.begin(1200);  //for debug frames
  //Set up the voices:
  //setupVoice( voice[0-3] , waveform[SINE,TRIANGLE,SQUARE,SAW,RAMP,NOISE] , pitch[0-127], envelope[ENVELOPE0-ENVELOPE3], length[0-127], mod[0-127, 64=no mod]) 

  edgar.setupVoice(0, SINE, 60, ENVELOPE1, 120, 64);
  edgar.setupVoice(1, TRIANGLE, 60, ENVELOPE2, 120, 64);
  edgar.setupVoice(2, SINE, 60, ENVELOPE1, 120, 64);
  edgar.setupVoice(3, SINE, 60, ENVELOPE1, 120, 64);
}

void loop()
{
  for (unsigned int frame = 0; frame < 2148; frame++)
  { // loop thru the song data by frames
    // Serial.println(i,DEC); //debug for following frame playing.
    for ( i  = 0; i < 5; i++)
    {
      note = pgm_read_byte_near(mynotes + frame * 5 + i) ;
      if (~note) { // test = 255 no change this channel
        switch (i) {
          case 0:    // Read frame time
            ndel = note * 15;  // adjust the time to match original score tempo.
            break;
          default:
            edgar.mTrigger(i, note);
        }
      }
    }
    delay(ndel);  //let this frame run
  }
  exit(0) ;
}
