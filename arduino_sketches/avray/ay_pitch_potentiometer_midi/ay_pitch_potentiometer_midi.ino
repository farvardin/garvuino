// Random notes generator for AY chip

// version for Garvuino https://github.com/farvardin/garvuino
// Works with Arduino IDE 1.8.3

// Connect a potentiometer on analog 0 of the arduino


#include <SPI.h>

//const int CS_Pin = 4; // check, if it is really pin 4 not 10 !!!!!!!!!

int switchInPin = 6;   
int LedPin = 8;
#define LED 8

const int KNOB_PIN = 0; // set the input for the knob to analog pin 0


// Define here if you're using USB MIDI (1) ou DIN5 MIDI (0) (din midi doesnt work here)
#define USBMIDI 1

#include <MIDI.h>
#include "noteList.h"
#include "pitches.h"
//#include <Tone.h>


MIDI_CREATE_DEFAULT_INSTANCE();

// needed here??
// use #define for CONTROL_RATE, not a constant
#define CONTROL_RATE 128 // powers of 2 please

static const unsigned sGatePin     = 13;
static const unsigned sAudioOutPin = 9;
static const unsigned sMaxNumNotes = 30; //16
MidiNoteList<sMaxNumNotes> midiNotes;

float shifted=0.1f;


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

int current_value = 256;

void handleNotesChanged(bool isFirstNote = false)
{
    if (midiNotes.empty())
    {
     
      //set_frequency(0,CHANNEL1); // change noise generator frequency
    }
    else
    {

        byte currentNote = 0;
        if (midiNotes.getLast(currentNote))
        {
           // tone(sAudioOutPin, sNotePitches[currentNote]);
                        
            //  set_frequency(sNotePitches[currentNote],CHANNEL1); // change noise generator frequency
       int current_value = sNotePitches[currentNote];
  
        playNote(current_value*10,current_value/256);
             

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

// ==============


void HandlePitchBend (byte channel, int bend)  // E17
{
//bend value from +/-8192, translate to 0.1-8 Hz?
shifted= float ((bend+8500) /2048.f ) +0.1f;  
    // filter
   // mySid.set_register(0,shifted);
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

void setup()
{

  pinMode(switchInPin, INPUT_PULLUP);
  pinMode(LedPin, OUTPUT);
  //pinMode(CS_Pin, OUTPUT);
  Serial.begin(38400);

  MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
 //   MIDI.setHandleProgramChange(HandleProgramChange); 
 //   MIDI.setHandleControlChange(HandleControlChange);
    MIDI.begin();

    if (USBMIDI == 1) {
    // uncomment the line below to enable ttymidi or Hailess midi serial (won't work with regular midi cable)
    Serial.begin(115200); // TTYMIDI will change baud rate of MIDI traffic from 31250 to 115200
    }
    
}


//#define BUF_MAX 16*15
//static byte buf[BUF_MAX];
static byte buf2[29];
//unsigned long t;



void loop()
{
      MIDI.read();
/* playNote(93,13);delay(20);
  playNote(231,11);delay(20);
   playNote(155,10);*/
  int knob_value = analogRead(KNOB_PIN); // value is 0-1023
  //int current_value = constrain(map(knob_value,0,1023, 1, 200),1, 200);

  // tone
//  int current_value = map(knob_value,0,1023, 0, 4096);
  
  // playNote(random(1,256),random(8,12));
  // playNote(random(1,256),random(current_value,current_value+2));
   
   //playEnveloppe(random(1,256),random(current_value,current_value+2));
  //playNote(current_value,current_value/256);
  // noise
  //int current_value = constrain(map(knob_value,0,1023, 1, 31),1, 31);
  // playNoise(random(current_value,current_value-2));
   
  /*  playNote(random(1,256),random(6,8));
    playNote(random(1,256),random(6,8));
   playNote(random(1,256),random(5,6));
   playNote(random(1,256),random(5,6));*/
 // delay(random(100,current_value*40));
// playNull();
 // delay(current_value*40);
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
 Serial.print(255,1);
 Serial.write(buf2,8);
   digitalWrite(LED,HIGH);
// delay(20);
digitalWrite(LED,LOW);
  }
}


void playEnveloppe(int r0, int r1)
{
  for (byte i=0;i<20;i++)
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
 Serial.print(255,1);
 Serial.write(buf2,14);
   digitalWrite(LED,HIGH);
// delay(20);
digitalWrite(LED,LOW);
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
 Serial.print(255,1);
 Serial.write(buf2,8);
   digitalWrite(LED,HIGH);
// delay(20);
digitalWrite(LED,LOW);
  }
}

void playNoise(int r0)
{
  for (byte i=0;i<20;i++)
  {
          buf2[0] = 8;  //Select register #8
          buf2[1] = 15; //Volume channel A full
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
