//  Random notes generator for SID chip


// version for Garvuino https://github.com/farvardin/garvuino
// Works with Arduino IDE 1.8.1

/*
Use a 10k potentiometer 

connect:
Potent.        Arduino
-------        -------
VCC            5V on bottom right pin in the group of 6 pins on the top
GND            GND on bottom left pin in the group of 6 pins on the top
middle pin     Analog input 0

note: there is no VCC or GND pin on the pot, just use the 2 extremities


*/


#include <SID.h>
#include "noteList.h"
#include "pitches.h"

#define OFF 0
#define SETTRIANGLE_1  4,0x11,5,0xBB,6,0xAA,
#define SETRECTANGLE_1  4,0x41,5,0x24,6,0xAA, 
#define SETSAWTOOTH_1 4,0x21,5,0xBB,6,0xAA, 
#define C4_1  1,0x11,0,0x25,
#define CONTROLREG 4 // SID control register address

// waveforms
#define SETNOISE_1  4,0x81,5,0xBB,6,0xAD, // SID register setup to create noise

#define SETNOSOUND_1  4,0x00,5,0xBB,6,0xAD, // SID register setup to create no sound

byte inChannel = 1; // we can't change channel here at the moment
byte inChannel2 = 2; // we can't change channel here at the moment
byte inChannel3 = 3; // we can't change channel here at the moment

SID mySid;

int noteVolume=12;

int switchInPin = 6;   
int reading;           // the current reading from the input pin

// the follow variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long time = 0;         // the last time the output pin was toggled
long debounce = 800;   // the debounce time, increase if the output flickers



const int KNOB_PIN = 0; // set the input for the knob to analog pin 0

int ny=0;

#define dt 20                       // time difference in milli seconds
unsigned long t=0; // Time Variables

byte mode=1; // push button change mode



// Main Code
void setup(){
  
  pinMode(switchInPin, INPUT_PULLUP); // uses internal arduino resistor
  Serial.begin(38400);
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



  /************************************************************************
  
    void set_frequency(uint16_t pitch,uint8_t channel)
    
    Set sid frequncy registers.
    
    The pitch has to be:
    
    pitch=16.77*frequency
    
    The SID has 3 channels, which should be accesed according to this
    definition:
    
    #define CHANNEL1  0
    #define CHANNEL2  7
    #define CHANNEL3  14
    
************************************************************************/
// pitch=16.77*frequency
void set_frequency(uint16_t pitch,uint8_t channel)
{
    mySid.set_register(channel, pitch&0xFF); // low register adress
    mySid.set_register(channel+1, pitch>>8); // high register adress
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
    #define CHANNEL1  0
    #define CHANNEL2  7
    #define CHANNEL3  14
    

#define LED 8          

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
    
 void changeMode()
{
    mode++; 
    Serial.println("Mode: ");  Serial.println(mode);
   BlinkLed(1);

  if (mode==3) { 
      mode = 1 ;
     BlinkLed(3);
     }
}

void loop(){


  reading = digitalRead(switchInPin);

  if (reading == HIGH  ) { Serial.println(reading); }
  else {    changeMode();  // mettre la suite dans un autre programme pour que ça fonctionne ??
     }
  

 if (mode==1) {
  //setwaveform_triangle(CHANNEL1);
  //setwaveform_triangle(CHANNEL2);
  setwaveform_triangle(CHANNEL3);


  //setwaveform_rectangle(CHANNEL1);
  setwaveform_rectangle(CHANNEL2);
  //setwaveform_rectangle(CHANNEL3);


  setwaveform_sawtooth(CHANNEL1);
  //setwaveform_sawtooth(CHANNEL2);
  //setwaveform_sawtooth(CHANNEL3);
  BlinkLed(1);
}

if (mode==2) {  // noise
  setwaveform_noise(CHANNEL1);
  setwaveform_noise(CHANNEL2);
  setwaveform_noise(CHANNEL3);
  BlinkLed(2);
}
    
 while(1)
  {
    uint16_t currentNote = 0;

    int knob_value = analogRead(KNOB_PIN); // value is 0-1023
    int current_value = constrain(map(knob_value,0,1023, 40, 60),40, 60);
    
  //  ny=current_value;

    // set_frequency(ny*17,CHANNEL1); // change noise generator frequency
   // ny=abs(ny);
    //currentNote = map(ny, -720, 720, 40, 60);
 //   currentNote = constrain(currentNote, 40, 60);
    //currentNote = 50 ;
    //set_frequency(sNotePitches[currentNote],CHANNEL1); 
    set_frequency(sNotePitches[current_value],CHANNEL1); 
    

    
   // nz=angle_z*8;
    //set_frequency(nz*17,CHANNEL2); // change noise generator frequency
   // nz=abs(nz);
   // currentNote = map(nz, -720, 720, 60, 80);
   // currentNote = constrain(currentNote, 60, 80);
    set_frequency(sNotePitches[current_value*2],CHANNEL2); 
    
    
   // nx=angle_x*8;
    //set_frequency(nx*17,CHANNEL3); // change noise generator frequency
   // nx=abs(nx);
   // currentNote = map(nx, -720, 720, 60, 90);
   // currentNote = constrain(currentNote, 60, 90);
    set_frequency(sNotePitches[current_value/2],CHANNEL3); 


// filter

int noteFilter = map(current_value, 0, 1023, 0, 15);
mySid.set_register(22,noteFilter);

// pw
     int notePW = map(current_value, 0, 1023, 0, 15);
      mySid.set_register(2,notePW);
      notePW = map(current_value, 0, 1023, 0, 15);
     mySid.set_register(3,notePW);
     
//sustain
    int  noteSustain = map(current_value, 0, 1023, 0, 15);
      
     mySid.set_register(6,noteSustain);
     
    delay(100);
    t=millis(); 

 

  
  while((millis()-t) < dt){ // Making sure the cycle time is equal to dt
  // Do nothing
  }
  
  
  }



}




  
