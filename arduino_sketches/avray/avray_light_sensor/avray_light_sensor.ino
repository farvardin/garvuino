// Light sensor AY synth 
//

// version for Garvuino https://github.com/farvardin/garvuino
// Works with Arduino IDE 1.8.3
//


#include <SPI.h>
#include <MIDI.h>
#include "noteList.h"
#include "pitches.h"

//const int CS_Pin = 4; // check, if it is really pin 4 not 10 !!!!!!!!!

int switchInPin = 6;   
int LedPin = 8;

int lightPin = 0;  //define a pin for Photo resistor
//int ledPin=11;     //define a pin for LED

int mode = 0;

MIDI_CREATE_DEFAULT_INSTANCE();

#ifdef ARDUINO_SAM_DUE // Due has no tone function (yet), overriden to prevent build errors.
#define tone()
#define noTone()
#endif

// This example shows how to make a simple synth out of an Arduino, using the
// tone() function. It also outputs a gate signal for controlling external
// analog synth components (like envelopes).

static const unsigned sGatePin     = 13;
static const unsigned sAudioOutPin = 9;
static const unsigned sMaxNumNotes = 16;
MidiNoteList<sMaxNumNotes> midiNotes;
static byte buf2[29];
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

void handleNotesChanged(bool isFirstNote = false)
{
    if (midiNotes.empty())
    {
        handleGateChanged(false);
        noTone(sAudioOutPin); // Remove to keep oscillator running during envelope release.
         
    }
    else
    {
        // Possible playing modes:
        // Mono Low:  use midiNotes.getLow
        // Mono High: use midiNotes.getHigh
        // Mono Last: use midiNotes.getLast

        byte currentNote = 0;
        if (midiNotes.getLast(currentNote))
        {
            //tone(sAudioOutPin, sNotePitches[currentNote]);
// pb lecture midi si serial on ????? TODO

           buf2[0] = 8;  //Select register #8
 buf2[1] = 15; //Volume channel A full
 buf2[2] = 0; //Select register #0
 buf2[3] = 93; // Write #93 into register #0
 buf2[4] = 1;  //Select register #1
 buf2[5] = 13; // Write #13 into register #1
 buf2[6] = 7; // Select register #7
 buf2[7] = 62;  //Enable output Channel A (0011 1110)
 Serial.print(255,1);
 Serial.write(buf2,8);
delay(200); 

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

void setup()
{
    pinMode(sGatePin,     OUTPUT);
    pinMode(sAudioOutPin, OUTPUT);
    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.begin();
     
    pinMode(switchInPin, INPUT_PULLUP);
  pinMode(LedPin, OUTPUT);
  //pinMode(CS_Pin, OUTPUT);
  Serial.begin(57600); // bloque notes sur pin 9 !
   //Serial.begin(9600); 

  pinMode( LedPin, OUTPUT ); //light sensor
}

//#define BUF_MAX 16*15
//static byte buf[BUF_MAX];
//static byte buf2[29];
//unsigned long t;

int MyNote;
int reading;

void loop()
{ 
  reading = digitalRead(switchInPin);
   // MIDI.read();
    
    //DEBUG
    //Serial.println(analogRead(lightPin)); //Write the value of the photoresistor to the serial monitor.
    
    analogWrite(LedPin, analogRead(lightPin)/4);  //send the value to the LedPin. Depending on value of resistor 

// switch

 // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
 
  if (reading == HIGH ) {
   
    Serial.println(reading);
  }
    else
      { 
         changeMode();
      }

  //  time = millis();    
  

if (mode == 0) { toneA();}
if (mode == 1) {noise();}
if (mode == 2) {noise2();}

}

void toneA()
{
  MyNote = map(analogRead(lightPin), 1, 860, 1, 250); // tone on 8 bit
   buf2[0] = 8;  //Select register #8
 buf2[1] = 15; //Volume channel A full
 buf2[2] = 0; //Select register #0
 buf2[3] = MyNote; // Write #n into register #0
 buf2[4] = 1;  //Select register #1
 buf2[5] = MyNote/(MyNote/2); // Write #n into register #1
 buf2[6] = 7; // Select register #7
 buf2[7] = 62;  //Enable output Channel A (0011 1110)
 Serial.print(255,1);
 Serial.write(buf2,8);
 delay(10); 
}

void noise()
{
  MyNote = map(analogRead(lightPin), 1, 860, 1, 32); // noise on 5 bit
   buf2[0] = 8;  //Select register #8
 buf2[1] = 15; //Volume channel A full
 buf2[2] = 6; //Select register #0
 buf2[3] = MyNote; // Write #n into register #0
 buf2[4] = 1;  //Select register #1
 buf2[5] = MyNote/(MyNote/2); // Write #n into register #1
 buf2[6] = 7; // Select register #7
 buf2[7] = 55;  //Enable output Channel A + noise (0011 0111)
 Serial.print(255,1);
 Serial.write(buf2,8);
 delay(10); 
}

int pause;

void noise2()
{
  MyNote = map(analogRead(lightPin), 1, 860, 1, 32); // noise on 5 bit
   buf2[0] = 8;  //Select register #8
 buf2[1] = 15; //Volume channel A full
 buf2[2] = 6; //Select register #0
 buf2[3] = MyNote; // Write #n into register #0
 buf2[4] = 1;  //Select register #1
 buf2[5] = MyNote/(MyNote/2); // Write #n into register #1
 buf2[6] = 7; // Select register #7
 buf2[7] = 7;  //Enable output noise channels A + B + C (0000 0111)
 Serial.print(255,1);
 Serial.write(buf2,8);
 pause = analogRead(lightPin), 1, 860, 2, 30;
 delay(pause); 
   buf2[0] = 9;  //Select register #9
 buf2[1] = analogRead(lightPin), 1, 860, 8, 15; //Volume channel B 
 buf2[2] = 6; //Select register #0
 buf2[3] = MyNote/(MyNote/4); // Write #n into register #0
 buf2[4] = 1;  //Select register #1
 buf2[5] = MyNote/(MyNote/2); // Write #n into register #1
 buf2[6] = 7; // Select register #7
 buf2[7] = 7;  //Enable output noise channels A + B + C (0000 0111)
 Serial.print(255,1);
 Serial.write(buf2,8);
 pause = analogRead(lightPin), 1, 860, 2, 30;
 delay(pause); 
 buf2[0] = 8;  //Select register #8
 buf2[1] = analogRead(lightPin), 1, 860, 10, 15; //Volume channel A 
 buf2[2] = 0; //Select register #0
 buf2[3] = analogRead(lightPin), 1, 860, 150, 250; // Write #n into register #0
 buf2[4] = 1;  //Select register #1
 buf2[5] = MyNote/(MyNote/2); // Write #n into register #1
 buf2[6] = 7; // Select register #7
buf2[7] = 0;  //Enable output Channel A+noise (00 000 000)
 Serial.print(255,1);
 Serial.write(buf2,8);
 pause = analogRead(lightPin), 1, 860, 2, 30;
 delay(pause); 
}

void changeMode()
{
    mode++; 
    //Serial.println("Mode: ");  Serial.println(mode);
      if (mode==0) {  //tone
          
  
               }

      if (mode==1) { //noise  
             //  BlinkLed(2);        
               }
     if (mode>2) {mode = 0 ;}
}
