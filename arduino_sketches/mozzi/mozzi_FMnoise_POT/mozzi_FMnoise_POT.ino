/*
 * 
 * https://forum.arduino.cc/index.php?topic=453966.0
 * by user fanchovitch
 * 
 * 
  Sketch based on the Mozzi exemple Knob_lightLevel_x2_FMsynth using 5 potentiometres to change FM synthesis parameters, and a knob for fundamental frequency,
  using Mozzi sonification library.

  Demonstrates analog input, audio and control oscillators, phase modulation
  and smoothing a control signal at audio rate to avoid clicks.
  Also demonstrates AutoMap, which maps unpredictable inputs to a set range.
  
  Mozzi exemple Knob_lightLevel_x2_FMsynth goes with a tutorial on the Mozzi site:
  http://sensorium.github.io/Mozzi/learn/Mozzi_Introductory_Tutorial.pdf
  
  The circuit:
     Audio output on digital pin 9 (on a Uno or similar), or
     check the README or http://sensorium.github.com/Mozzi/

     Potentiometer connected to analog pin 0.
       Center pin of the potentiometer goes to the analog pin.
       Side pins of the potentiometer go to +5V and ground
      
     Potentiometer connected to analog pin 1.
       Center pin of the potentiometer goes to the analog pin.
       Side pins of the potentiometer go to +5V and ground
      
     Potentiometer connected to analog pin 2.
       Center pin of the potentiometer goes to the analog pin.
       Side pins of the potentiometer go to +5V and ground
  
    Potentiometer connected to analog pin 2.
       Center pin of the potentiometer goes to the analog pin.
       Side pins of the potentiometer go to +5V and ground
    
    Potentiometer connected to analog pin 2.
       Center pin of the potentiometer goes to the analog pin.
       Side pins of the potentiometer go to +5V and ground
      
    Digital PWM Pin 11 has an LED connected to 220 ohm resitor on Arduino Nano board
    Digital PWM Pin 10 has an LED connected to 220 ohm resitor on Arduino Nano board
    Digital PWM Pin 6 has an LED connected to 220 ohm resitor on Arduino Nano board
    Digital PWM Pin 5 has an LED connected to 220 ohm resitor on Arduino Nano board
    Digital PWM Pin 3 has an LED connected to 220 ohm resitor on Arduino Nano board
  
  Mozzi help/discussion/announcements:
  https://groups.google.com/forum/#!forum/mozzi-users

  Tim Barrass 2013.
  This example code is in the public domain.
*/

#include <MozziGuts.h>
#include <Oscil.h> // oscillator
#include <tables/cos2048_int8.h> // table for Oscils to play
#include <Smooth.h>
#include <AutoMap.h> // maps unpredictable inputs to a range
 
// desired carrier frequency max and min, for AutoMap
const int MIN_CARRIER_FREQ = 22;
const int MAX_CARRIER_FREQ = 440;

const int MIN = 1;
const int MAX = 10;
const int MIN_2 = 1;
const int MAX_2 = 15;

// desired intensity max and min, for AutoMap, note they're inverted for reverse dynamics
const int MIN_INTENSITY = 700;
const int MAX_INTENSITY = 10;

// desired mod speed max and min, for AutoMap, note they're inverted for reverse dynamics
const int MIN_MOD_SPEED = 10000;
const int MAX_MOD_SPEED = 1;

AutoMap kMapCarrierFreq(0,1023,MIN_CARRIER_FREQ,MAX_CARRIER_FREQ);
AutoMap kMapIntensity(0,1023,MIN_INTENSITY,MAX_INTENSITY);
AutoMap kMapModSpeed(0,1023,MIN_MOD_SPEED,MAX_MOD_SPEED);
AutoMap mapThis(0,1023,MIN,MAX);
AutoMap mapThisToo(0,1023,MIN_2,MAX_2);

const int KNOB_PIN1 = 0; // set the input for the knob1 to analog pin 0
const int KNOB_PIN2 = 1; // set the input for the knob1 to analog pin 1
const int KNOB_PIN3 = 2; // set the input for the knob1 to analog pin 2
const int KNOB_PIN4 = 3; // set the input for the knob1 to analog pin 3
const int KNOB_PIN5 = 4; // set the input for the knob1 to analog pin 4

const int analogOutPin1 = 11;
const int analogOutPin2 = 6;
const int analogOutPin3 = 10;
const int analogOutPin4 = 5;
const int analogOutPin5 = 3;

int outputValue1 = 0;        // value output to the PWM (analog out)
int outputValue2 = 0;        // value output to the PWM (analog out)
int outputValue3 = 0;        // value output to the PWM (analog out)
int outputValue4 = 0;        // value output to the PWM (analog out)
int outputValue5 = 0;        // value output to the PWM (analog out)

Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aCarrier(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aModulator(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, CONTROL_RATE> kIntensityMod(COS2048_DATA);

int mod_ratio = 5; // brightness (harmonics)
long fm_intensity; // carries control info from updateControl to updateAudio

// smoothing for intensity to remove clicks on transitions
float smoothness = 0.95f;
Smooth <long> aSmoothIntensity(smoothness);


void setup(){
  Serial.begin(115200); // set up the Serial output so we can look at the light level
  startMozzi(); // :))
}


void updateControl(){
  

  // read the knob4
  int freqVal = mozziAnalogRead(KNOB_PIN4); // value is 0-1023
  int FRQ = mapThis(freqVal);
   /*
   outputValue4 = map(freqVal, 0, 1023, 0, 255);  
   // change the analog out value:
   analogWrite(analogOutPin4, outputValue4);
   */
  // read the knob5
  int knob5 = mozziAnalogRead(KNOB_PIN5); // value is 0-1023
  int knob5Val = mapThis(knob5);
   outputValue5 = map(knob5, 0, 1023, 0, 255);  
   // change the analog out value:
   analogWrite(analogOutPin5, outputValue5);
  
  // read the knob1
  int knob1_value = mozziAnalogRead(KNOB_PIN1); // value is 0-1023
   // map it to the range of the analog out:
   outputValue1 = map(knob1_value, 0, 1023, 0, 255);  
   // change the analog out value:
   analogWrite(analogOutPin1, outputValue1);
    // map the knob to carrier frequency
  int carrier_freq = kMapCarrierFreq(knob1_value);
  //calculate the modulation frequency to stay in ratio
  int mod_freq = carrier_freq * mod_ratio * FRQ;
    // set the FM oscillator frequencies
    aCarrier.setFreq(carrier_freq);
    aModulator.setFreq(mod_freq);
      // print the results to the serial monitor:
      Serial.print("knob1 = " );                      
      Serial.print(knob1_value);      
      Serial.print("\t output1 = ");      
      Serial.println(outputValue1);
  
  // read the knob2
  int knob2_value= mozziAnalogRead(KNOB_PIN2); // value is 0-1023
    /*
    // map it to the range of the analog out:
    outputValue2 = map(knob2_value, 0, 1023, 0, 255);  
    // change the analog out value:
    analogWrite(analogOutPin2, outputValue2);
    */
      // print the value to the Serial monitor for debugging
      Serial.print("knob2 = ");
      Serial.print(knob2_value);
      Serial.print("\t"); // prints a tab
  int knob2_calibrated = kMapIntensity(knob2_value);
      Serial.print("knob2_calibrated = ");
      Serial.print(knob2_calibrated);
      Serial.print("\t"); // prints a tab
    // calculate the fm_intensity
    fm_intensity = ((long)knob2_calibrated * knob5Val * (kIntensityMod.next()+128))>>8; // shift back to range after 8 bit multiply  
      Serial.print("fm_intensity = ");
      Serial.print(fm_intensity);
      Serial.print("\t"); // prints a tab
  
  // read the knob dependent resistor on the speed Analog input pin
  int knob3_value= mozziAnalogRead(KNOB_PIN3); // value is 0-1023
    
    outputValue3 = map(knob3_value, 0, 1023, 0, 255);  
    // change the analog out value:
    analogWrite(analogOutPin3, outputValue3);
      Serial.print("knob3 = ");
      Serial.print(knob3_value);
      Serial.print("\t"); // prints a tab
  // use a float here for low frequencies
  float mod_speed = (float)kMapModSpeed(knob3_value)/1000;
      Serial.print("   mod_speed = ");
      Serial.print(mod_speed);
    kIntensityMod.setFreq(mod_speed);
  
      Serial.println(); // finally, print a carraige return for the next line of debugging info
}


int updateAudio(){
  long modulation = aSmoothIntensity.next(fm_intensity) * aModulator.next();
  return aCarrier.phMod(modulation);
}


void loop(){
  audioHook();

}




