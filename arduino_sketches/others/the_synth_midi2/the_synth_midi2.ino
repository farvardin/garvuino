// Arduino synth library absolute frequency example

//Hardware connections:

//                    +10µF 
//PIN 11 ---[ 1k ]--+---||--->> Audio out
//                  |
//                 === 10nF
//                  |
//                 GND


#include <synth.h>
#include <MIDI.h> // Requires Arduino MIDI Library v4.2 or later
#include <noteList.h>  // in garvuino lib
#include <pitches.h>   // in garvuino lib

// Define here if you're using USB MIDI (1) ou DIN5 MIDI (0)
#define USBMIDI 1

// *** MIDI Parameters ***********************************************************
#define BENDRANGE 12 // Change to set Number of Semitones for Pitch Bend (1 to 24)
// *******************************************************************************
#define MIDICHANNEL 1 // Can be a value from 1 to 16

MIDI_CREATE_DEFAULT_INSTANCE();


#define KNOB_PIN0 0
#define KNOB_PIN1 1
#define KNOB_PIN2 2
#define KNOB_PIN3 3
#define KNOB_PIN6 6


//TODO : faire simple déclaration
int knob1_value= analogRead(KNOB_PIN0); // value is 0-1023
int knob2_value= analogRead(KNOB_PIN1); // value is 0-1023
int knob3_value= analogRead(KNOB_PIN2); // value is 0-1023
int knob4_value= analogRead(KNOB_PIN3); // value is 0-1023
int knob5_value= analogRead(KNOB_PIN6); // value is 0-1023

int outputValue1 = 0;        // value output to the PWM (analog out)
int outputValue2 = 0;        // value output to the PWM (analog out)
int outputValue3 = 0;        // value output to the PWM (analog out)
int outputValue4 = 0;        // value output to the PWM (analog out)
int outputValue5 = 0;        // value output to the PWM (analog out)



byte mode=0;


//midi
static const unsigned sGatePin     = 13;
static const unsigned sAudioOutPin = 9;
static const unsigned sMaxNumNotes = 30; //16
MidiNoteList<sMaxNumNotes> midiNotes;



synth edgar;    //-Make a synth

void setup() {

  edgar.begin(CHB);                                   //-Start it up
 // edgar.setupVoice(0,TRIANGLE,60,ENVELOPE1,127,64);  //-Set up voice 0

  MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.begin();

    if (USBMIDI == 1) {
    // uncomment the line below to enable ttymidi or Hailess midi serial (won't work with regular midi cable)
    Serial.begin(115200); // TTYMIDI will change baud rate of MIDI traffic from 31250 to 115200
    }
}

void loop()
{
   MIDI.read();


}


void handleNotesChanged(bool isFirstNote = false)
{
    if (midiNotes.empty())
    {
        handleGateChanged(false);
      //  edgar.setupVoice(0,SINE,65,ENVELOPE1,0,64);
    }
    else
    {

        byte currentNote = 0;
        if (midiNotes.getLast(currentNote))
        {
      
              // DURING NOTE CHANGE (not mode change)
               if (mode==0) {
  //edgar.setFrequency(0,map(analogRead(KNOB_PIN0),0,1023, 5, 440));

  //length
   knob1_value= analogRead(KNOB_PIN0); // value is 0-1023
   outputValue1 = map(knob1_value, 0, 1023, 0, 120); 

// sine, square...
      knob2_value= analogRead(KNOB_PIN1); // value is 0-1023
   outputValue2 = map(knob2_value, 0, 1023, 0, 5); 

// enveloppe
         knob3_value= analogRead(KNOB_PIN2); // value is 0-1023
   outputValue3 = map(knob3_value, 0, 1023, 0, 3); 
      
  edgar.setupVoice(0,outputValue2,65,outputValue3,80,64);
  edgar.setFrequency(0,sNotePitches[currentNote]);
  edgar.trigger(0);
  //delay(100);
               }

             if (mode==1) {


               }
               
             if (mode==2) {

               }

             if (mode==3) {

               }

             if (mode==4) {
               mode=0;
               }
               
       

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




  

