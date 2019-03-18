/****************************************
        Arduino controls an
  Atmega8 MOS6581 SID Emulator
        by softare serial line
       
  (SID = Sound Interface Device on C64)
*****************************************

  date       authors             version       comment
  ======  ======================  =======   ==============================
  2007     (ch) Christoph Haberer  V1.0        First implementation
  2012      Mario Patino           V1.1        Adapted for Arduino SID Library
  2015-18   Eric Forgeot           V1.3        Adapted for midi input (monophonic + one channel only)

  Versions:

  V1.0
  - 2 channel random pitch

  V1.1
  - Adapted to work with the Arduino SID Library
    
   V1.3
    - Use your sid arduino as a midi synth!
    Some parts heavily borrowed from the cheapsynth projects and from the arduino midi lib

***************************************************************************/
/***************************************************************************
 *   
 *   (c) 2007 Christoph Haberer, christoph(at)roboterclub-freiburg.de
 *
 ***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation version 2 of the License,                *
 *   If you extend the program please maintain the list of authors.        *
 *   If you want to use this software for commercial purposes and you      *
 *   don't want to make it open source, please contact the authors for     *
 *   licensing.                                                            *
 ***************************************************************************/


// Define here if you're using USB MIDI (1) ou DIN5 MIDI (0)
#define USBMIDI 0

// if set to 1, will enable physical knobs and disable midi knobs on synth
#define ANALOG_KNOBS 1

#include <MIDI.h>
#include "noteList.h"
#include "pitches.h"
//#include <Tone.h>

#include <SID.h>

 
#define OFF 0
#define SETTRIANGLE_1 4,0x11,5,0xBB,6,0xAA,
#define SETRECTANGLE_1  4,0x41,5,0x24,6,0xAA, 
#define SETSAWTOOTH_1 4,0x21,5,0xBB,6,0xAA,  
#define C4_1  1,0x11,0,0x25,
#define CONTROLREG 4 // SID control register address

#define KNOB_PIN0 0

// waveforms
#define SETNOISE_1  4,0x81,5,0xBB,6,0xAD, // SID register setup to create noise

#define SETNOSOUND_1  4,0x00,5,0xBB,6,0xAD, // SID register setup to create no sound

byte inChannel = 1; // we can't change channel here at the moment
byte inChannel2 = 2; // we can't change channel here at the moment
byte inChannel3 = 3; // we can't change channel here at the moment

SID mySid;

// SID END


int maxmode=4;
byte mode=0;

int noteVolume=12;
int noteResonance=10;
int notePW=12;
int noteSustain=12;
int noteFilter=12;
int switchInPin = 6;   // 2 on previous shield  
#define LED 8          // 13 on previous shield 

int state = HIGH;      // the current state of the output pin
int reading;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin

// the follow variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long time = 0;         // the last time the output pin was toggled
long debounce = 400;   // the debounce time, increase if the output flickers

MIDI_CREATE_DEFAULT_INSTANCE();

// use #define for CONTROL_RATE, not a constant
#define CONTROL_RATE 128 // powers of 2 please

// This example shows how to make a simple synth out of an Arduino, using the
// tone() function. It also outputs a gate signal for controlling external
// analog synth components (like envelopes).

// <!> Use CHANNEL 1 !!

static const unsigned sGatePin     = 13;
static const unsigned sAudioOutPin = 9;
static const unsigned sMaxNumNotes = 30; //16
MidiNoteList<sMaxNumNotes> midiNotes;

float shifted=0.1f;


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


// -----------------------------------------------------------------------------

inline void handleGateChanged(bool inGateActive)
{
    digitalWrite(sGatePin, inGateActive ? HIGH : LOW);
}

inline void pulseGate()
{
    handleGateChanged(false);
    delay(1);
    handleGateChanged(true);
}

// -----------------------------------------------------------------------------

    #define CHANNEL1  0
    #define CHANNEL2  7
    #define CHANNEL3  14
    
void handleNotesChanged(bool isFirstNote = false)
{
    if (midiNotes.empty())
    {
        handleGateChanged(false);
        //noTone(sAudioOutPin); //Stops the generation of a square wave triggered by tone(). Has no effect if no tone is being generated. 
      setwaveform_nosound(CHANNEL1);
      setwaveform_nosound(CHANNEL2);
      setwaveform_nosound(CHANNEL3);
      //set_frequency(0,CHANNEL1); // change noise generator frequency
    }
    else
    {
        // Possible playing modes:
        // Mono Low:  use midiNotes.getLow
        // Mono High: use midiNotes.getHigh
        // Mono Last: use midiNotes.getLast
        setwaveform_nosound(CHANNEL1);
        setwaveform_nosound(CHANNEL2);
        setwaveform_nosound(CHANNEL3);
        byte currentNote = 0;
        if (midiNotes.getLast(currentNote))
        {
           // tone(sAudioOutPin, sNotePitches[currentNote]);
           
              // n=zufall()*8;
  
  /*            
  setwaveform_triangle(CHANNEL1);
  setwaveform_rectangle(CHANNEL2);
  setwaveform_sawtooth(CHANNEL3);
  
              set_frequency(sNotePitches[currentNote],CHANNEL1); // change noise generator frequency
              set_frequency(sNotePitches[currentNote],CHANNEL2); // change noise generator frequency
              set_frequency(sNotePitches[currentNote],CHANNEL3); // change noise generator frequency
              
              */  
              // DURING NOTE CHANGE (not mode change)
               if (mode==0) {
               setwaveform_triangle(CHANNEL1);
              // setwaveform_rectangle(CHANNEL1);
              // setwaveform_sawtooth(CHANNEL1);
               }

             if (mode==1) {
               setwaveform_rectangle(CHANNEL1);
               }
               
             if (mode==2) {
               setwaveform_sawtooth(CHANNEL1);
               setwaveform_triangle(CHANNEL2);
               }

             if (mode==3) {
               setwaveform_noise(CHANNEL1);
               }

             if (mode==4) {
               mode=0;
               }
               
              set_frequency(sNotePitches[currentNote],CHANNEL1); // change noise generator frequency
              set_frequency(sNotePitches[currentNote],CHANNEL2);       
              set_frequency(sNotePitches[currentNote],CHANNEL3); 
             
                 // n=zufall()*2;
                 // set_frequency(n*17,CHANNEL2); // change noise generator frequency

            if (isFirstNote)
            {
                handleGateChanged(true);
            }
            else
            {
                pulseGate(); // Retrigger envelopes. Remove for legato effect.
            }
        }
    }
}

// -----------------------------------------------------------------------------

void HandleProgramChange (byte channel, byte number)
{
 // modOffset = modOffsets[number%15];
 // UpdateFreqs();
 // !! we use this because I don't know how to use HandleContinue
 
   // READ HF,LF,DR
   // play(25,177,250);
   changeMode();
}


// ================================

void HandleControlChange (byte channel, byte number, byte value)
{
// http://www.indiana.edu/~emusic/cntrlnumb.html

/*
Serial.end(); // sample debug code to check values through (noisy) terminal interface 
Serial.begin(38400);
Serial.print("  number:");   
Serial.print(number);
Serial.print(" value:");   
Serial.println( value );
Serial.println();
Serial.end();
Serial.begin(31250);
*/

  switch(number) {
//  case 3: // could trap different controller IDs here

  case 1: // modulation wheel E18
   //   BlinkLed(value);   
    // setenvelope(value, CHANNEL1);
    // filter
    mySid.set_register(1,value);
  break;

  case 07: // volume wheel
    // volume
    
    noteVolume = map(value, 0, 127, 0, 15);
      mySid.set_register(24,noteVolume); // SET VOLUME
    mySid.set_register(24,value);   
    //Serial.print(value);
  break;
  
  
  case 71: // E10
      // resonance
      noteResonance = map(value, 0, 127, 0, 15);
      
     mySid.set_register(23,noteResonance);

  break;

  case 72: // E13
      // filter 1
      noteFilter = map(value, 0, 127, 0, 15);
      mySid.set_register(23,1);
     mySid.set_register(21,noteFilter);
     mySid.set_register(22,noteFilter/8);

  break;


  case 73: // E11
      // pw low
      notePW = map(value, 0, 127, 0, 15);
      mySid.set_register(2,notePW);

  break;
  

  case 74: // E9
      // SUSTAIN/RELEASE
      noteSustain = map(value, 0, 127, 0, 256);
      
     mySid.set_register(6,noteSustain);
    // mySid.set_register(5,value);
   //   mySid.set_register(24,value); 
                //mySid.set_register(6,89);
     //           noteVolume = map(value, 0, 127, 0, 15);
    //  mySid.set_register(24,noteVolume); // SET VOLUME
  break;
    
  
  case 75: // E12
      // pw high
      notePW = map(value, 0, 127, 0, 15);
     mySid.set_register(3,notePW);

  break;
  
  
    case 91: // E14
      // filter 1
      noteFilter = map(value, 0, 127, 0, 15);
      
     mySid.set_register(22,noteFilter);

  break;

  
  case 21: // E1
  setwaveform_nosound(CHANNEL1);
    changeMode();
  break;
 // Serial.println("Mode: ");  Serial.println(mode);
 {if (mode>maxmode) mode = 0;} //currently has a silent mode at the end so you know where you are : )

}

}


// ==============


void HandlePitchBend (byte channel, int bend)  // E17
{
//bend value from +/-8192, translate to 0.1-8 Hz?
shifted= float ((bend+8500) /2048.f ) +0.1f;  
    // filter
    mySid.set_register(0,shifted);
   // Serial.print(shifted);

}





//=====================

void handleNoteOn(byte inChannel, byte inNote, byte inVelocity)
{
    const bool firstNote = midiNotes.empty();
    midiNotes.add(MidiNote(inNote, inVelocity));
    handleNotesChanged(firstNote);
}

void handleNoteOff(byte inChannel, byte inNote, byte inVelocity)
{
    midiNotes.remove(inNote);
    handleNotesChanged();
}

// -----------------------------------------------------------------------------


//<!> play is used for direct playing notes from arduino, not from MIDI
void play(byte hf, byte lf, byte dr){
  
  mySid.set_register(1,hf);
  mySid.set_register(0,lf);
  // CONTROL REGISTER = 4
  
 // Serial.print(F("HF/LF:"));
 // Serial.print(hf*256+lf);
//  Serial.println("");
  mySid.set_register(4,33);  // wave form
  delay(dr*2);
  mySid.set_register(4,32);
  mySid.set_register(24,noteVolume); // SET VOLUME
}

void play(byte hf, byte lf, byte dr,byte hf1, byte lf1, byte dr1){
  play(hf,lf,dr);
  play(hf1,lf1,dr1);
}


void setup()
{
//    pinMode(switchInPin, INPUT);
    pinMode(switchInPin, INPUT_PULLUP); // uses internal arduino resistor
     //  Set MIDI baud rate:
    //Serial.begin(31250);
    Serial.begin(38400);
    pinMode(sGatePin,     OUTPUT);
    pinMode(sAudioOutPin, OUTPUT);
    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.setHandleProgramChange(HandleProgramChange); 
    MIDI.setHandleControlChange(HandleControlChange);
    MIDI.begin();

    if (USBMIDI == 1) {
    // uncomment the line below to enable ttymidi or Hailess midi serial (won't work with regular midi cable)
    Serial.begin(115200); // TTYMIDI will change baud rate of MIDI traffic from 31250 to 115200
    }
    
    mySid.begin();
}

void setwaveform_triangle(uint8_t channel)
{
  uint8_t dataset[]={ SETTRIANGLE_1 C4_1 0xFF };
  //  uint8_t dataset[]={SETNOISE_1 C4_1 0xFF};
  uint8_t n=0; 
  
  while(dataset[n]!=0xFF) 
  {
     mySid.set_register(channel+dataset[n], dataset[n+1]); 
     // register address, register content
     n+=2;
     mySid.set_register(24,noteVolume); // SET VOLUME
  }
}

void setwaveform_rectangle(uint8_t channel)
{
  uint8_t dataset[]={ SETRECTANGLE_1 C4_1 0xFF };
//uint8_t dataset[]={ SETTRIANGLE_1 C4_1 0xFF };
  //  uint8_t dataset[]={SETNOISE_1 C4_1 0xFF};
  uint8_t n=0; 
  
  while(dataset[n]!=0xFF) 
  {
     mySid.set_register(channel+dataset[n], dataset[n+1]); 
     // register address, register content
     n+=2;
     mySid.set_register(24,noteVolume); // SET VOLUME
  }
}

void setwaveform_sawtooth(uint8_t channel)
{
  uint8_t dataset[]={ SETSAWTOOTH_1 C4_1 0xFF };
  //  uint8_t dataset[]={SETNOISE_1 C4_1 0xFF};
  uint8_t n=0; 
  
  while(dataset[n]!=0xFF) 
  {
     mySid.set_register(channel+dataset[n], dataset[n+1]); 
     // register address, register content
     n+=2;
     mySid.set_register(24,noteVolume); // SET VOLUME
  }
}

void setwaveform_noise(uint8_t channel)
{
  uint8_t dataset[]={ SETNOISE_1 C4_1 0xFF };
  //  uint8_t dataset[]={SETNOISE_1 C4_1 0xFF};
  uint8_t n=0; 
  
  while(dataset[n]!=0xFF) 
  {
     mySid.set_register(channel+dataset[n], dataset[n+1]); 
     // register address, register content
     n+=2;
     mySid.set_register(24,noteVolume); // SET VOLUME
  }
}


void setwaveform_nosound(uint8_t channel)
{
  uint8_t dataset[]={ SETNOSOUND_1 C4_1 0xFF };
  //  uint8_t dataset[]={SETNOISE_1 C4_1 0xFF};
  uint8_t n=0; 
  
  while(dataset[n]!=0xFF) 
  {
     mySid.set_register(channel+dataset[n], dataset[n+1]); 
     // register address, register content
     n+=2;
  }
}



void set_frequency(uint16_t pitch,uint8_t channel)
{
    mySid.set_register(channel, pitch&0xFF); // low register adress
    mySid.set_register(channel+1, pitch>>8); // high register adress
    }


void changeMode()
{
    mode++; 
    Serial.println("Mode: ");  Serial.println(mode);
      if (mode==0) {  // triangle
          setwaveform_triangle(CHANNEL1); 
               BlinkLed(1); 
                 
                 // Attack/decay
              //  mySid.set_register(5,9);
                //mySid.set_register(5,190);
                // SUSTAIN/RELEASE
              //  mySid.set_register(6,9);
                
                
                // Violino (per dire)
                //mySid.set_register(5,88);
                //mySid.set_register(6,89);
               
                // Piano D=9, il resto zero
                //mySid.set_register(5,9);
                //mySid.set_register(6,0);
                
                  mySid.set_register(24,noteVolume); // SET  VOLUME

  
               }

             if (mode==1) { // rectangle
                setwaveform_rectangle(CHANNEL1); 
               mySid.set_register(2,13);  // notePW low
               mySid.set_register(3,13);  // notePW high
               BlinkLed(2);     
                     
               }
               
             if (mode==2) {  // saw
                 setwaveform_sawtooth(CHANNEL1);
                    BlinkLed(3);           
               }

             if (mode==3) {  // noise
setwaveform_noise(CHANNEL1);
                     BlinkLed(4);             
               
               }
             if (mode==4) {
                 mode=-1;             
               
               }
               setwaveform_nosound(CHANNEL1);
}

uint8_t zufall()
{
  static uint16_t startwert=0x0AA;

  uint16_t temp;
  uint8_t n;
    
  for(n=1;n<8;n++)
  {
    temp = startwert;
    startwert=startwert << 1;
  
    temp ^= startwert;
    if ( ( temp & 0x4000 ) == 0x4000 ) 
    { 
      startwert |= 1;
    }
  }
  
  return (startwert);
}


/************************************************************************
  
    main loop

    create space ship noise
    
************************************************************************/   


void loop()
{
     reading = digitalRead(switchInPin);
    MIDI.read();
  uint16_t n;
  uint8_t flag,k;
  uint16_t soundindex=0;
  mySid.set_register(24,noteVolume); // SET VOLUME
  

//  setwaveform_triangle(CHANNEL1);
//  setwaveform_triangle(CHANNEL2);
//  setwaveform_triangle(CHANNEL3);

// switch

 // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  if (reading == HIGH  ) {
   
    Serial.println(reading);
  }
    else
      { setwaveform_nosound(CHANNEL1);
         changeMode();
      }

  //  time = millis(); 

      if (ANALOG_KNOBS==1)
      // will enable physical knobs on garvuino, and disable MIDI knobs
      {
       // read analog port 
       // E12
       // pw high 
     notePW = map(analogRead(KNOB_PIN0), 0, 1023, 0, 15);  //value is 0-1023
     mySid.set_register(3,notePW);
      }
  }

   // previous = reading;




