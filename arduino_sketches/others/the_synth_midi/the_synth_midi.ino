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
// Connect midi on the DIN5 of the Garvuino (it doesn't work with usb midi)
//
// On GARVUINO v2.10: connect PIN D3 to PIN AY>L or AY>R (under the L or R) to make the audio output!
// On GARVUINO v1.09: remove the arduino out of the socket
//      Then connect PIN D3 to PIN AY>L (under the L label) and GND to the GND in J1 on Garvuino (or on top of J4)
//        in order to get the audio output!
//      And connect the RX0 from the Arduino to the second pin hole from the top left on the empty
//        socket, to get the midi connection.


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


void testSwitches()
{
int sensorVal = digitalRead(switchInPin1); // push button
   // push button
   if (sensorVal == HIGH) {
  // DO NOTHING
  } else {
// DO
  //edgar.setUserWave(0,USER00);
  edgar.setEnvelope(0,1);
  edgar.setLength(0,82);
  edgar.setMod(0,64);
  edgar.setWave(0,TRIANGLE);
  edgar.mTrigger(0,75);
  }
}


void Kick()
{
  

  edgar.setWave(0,TRIANGLE); // try SQUARE for electro
  edgar.setEnvelope(0,58);
  edgar.setLength(0,45);  // 64 for electro kick
  edgar.setMod(0,21);
  
  edgar.mTrigger(0,35);
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
  Serial.begin(31250);    //MIDI BAUD rate
  //Serial.begin(38400);    //MIDI BAUD rate
  edgar.begin(CHB);          //Init synth
  pinMode(8,OUTPUT);

  pinMode(switchInPin1, INPUT_PULLUP);
  pinMode(switchInPin2, INPUT_PULLUP);
  pinMode(switchInPin3, INPUT_PULLUP);

  
  edgar.setWave(0,TRIANGLE);
  //edgar.setUserWave(0,USER00);
  edgar.setEnvelope(0,1);
  edgar.setLength(0,82);
  edgar.setMod(0,64);

  
    ssd1306_128x64_i2c_init();

  ssd1306_fillScreen(0x00);
    ssd1306_drawBitmap(0, 0, 128, 64, garvuino_logo);
    delay(20);
    garvuino_welcome();
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
#if defined (OLED)
        digitalWrite(LedPin,HIGH);
        delay(1);
        digitalWrite(LedPin,LOW);
        delay(1);
   //      garvuino_redrawLogo(); ssd1306_printFixed2x(0,  2, "       ", STYLE_NORMAL);
    //    char noted[6];
     // String str1 ;
#endif
        switch(parser.midi_1st)  //-Controller number
        {
        case mWave:  //-Controller 13 
          edgar.setWave(voice,parser.midi_2nd/21);
		    //edgar.setUserWave(voice,parser.midi_2nd/21);
#if defined (OLED)
        //  garvuino_redrawLogo(); 
          ssd1306_printFixed2x(0,  2, "Wave  ", STYLE_NORMAL);    // x, y position
          ssd1306_printFixed2x(0,  2, "      ", STYLE_NORMAL);    // x, y position
          /*str1=String(parser.midi_2nd/21); // add too much latency
          str1.toCharArray(noted,6); 
          ssd1306_printFixed(0,  32, noted, STYLE_NORMAL);
          ssd1306_printFixed(0,  32, "     ", STYLE_NORMAL);*/
#endif
          break;
        case mEnv:  //-Controller 12
          edgar.setEnvelope(voice,parser.midi_2nd/32);
#if defined (OLED)
          //garvuino_redrawLogo(); 
          ssd1306_printFixed2x(0,  2, "Env.", STYLE_NORMAL);
          ssd1306_printFixed2x(0,  2, "      ", STYLE_NORMAL);
          /*str1=String(parser.midi_2nd/32); 
          str1.toCharArray(noted,6); 
          ssd1306_printFixed(0,  32, noted, STYLE_NORMAL);
          ssd1306_printFixed(0,  32, "     ", STYLE_NORMAL);*/
#endif
          break;   
        case mLength:  //-Controller 10
          edgar.setLength(voice,parser.midi_2nd);
#if defined (OLED)
  // garvuino_redrawLogo(); 
          ssd1306_printFixed2x(0,  2, "Length", STYLE_NORMAL);
          ssd1306_printFixed2x(0,  2, "      ", STYLE_NORMAL);
          /*str1=String(parser.midi_2nd); 
          str1.toCharArray(noted,6); 
          ssd1306_printFixed(0,  32, noted, STYLE_NORMAL);
          ssd1306_printFixed(0,  32, "     ", STYLE_NORMAL);*/
#endif
          break;  
        case mMod:   //-Controller 7
          edgar.setMod(voice,parser.midi_2nd);
#if defined (OLED)
          //garvuino_redrawLogo();
          ssd1306_printFixed2x(0,  2, "Mod   ", STYLE_NORMAL);
          ssd1306_printFixed2x(0,  2, "      ", STYLE_NORMAL);
          /*str1=String(parser.midi_2nd); 
          str1.toCharArray(noted,6); 
          ssd1306_printFixed(0,  32, noted, STYLE_NORMAL);
          ssd1306_printFixed(0,  32, "     ", STYLE_NORMAL);*/
#endif
          break;
        //case mPWM:   // 
        //  PWM=parser.midi_2nd;
        //  break;
        }
        break;
      }
    }
  }
}


