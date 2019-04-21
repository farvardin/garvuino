
#include <MozziGuts.h>
#include <mozzi_rand.h>
#include <Oscil.h> // oscillator template
#include <tables/sin2048_int8.h> // sine table for oscillator

// use: Oscil <table_size, update_rate> oscilName (wavetable), look in .h file of table #included above
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);

// use #define for CONTROL_RATE, not a constant
#define CONTROL_RATE 64 // powers of 2 please

#define FILTER_SHIFT 6 // 5 or 6 work well - the spectrum of 6 looks a bit more linear, like the generated brown noise in audacity

//int value1 = 1;

void setup()
{
  startMozzi(CONTROL_RATE); // set a control rate of 64 (powers of 2 please)
 // aSin.setFreq(0.05f); // set the frequency
 // aSin.setFreq(0.5f); // set the frequency

}


void updateControl()
{
  // put changing controls in here
    int value1 = map(analogRead(1),0, 1023, 1, 100);
 // float value1 = 0.2;
  aSin.setFreq(20.0f/value1); // set the frequency
}


int updateAudio()
{
  static int filtered;

  //char whitenoise = rand((byte)255) - 128;
  char whitenoise = rand((byte)255) - map(analogRead(0),0, 1023, 0, 128);
  
  filtered = filtered - (filtered>>FILTER_SHIFT) + whitenoise;

  //int asig = filtered>>3; // shift to 8 bit range (trial and error)
  int asig = filtered>>map(analogRead(2),0, 1023, 1, 8); // shift to 8 bit range (trial and error)
  return ((int)asig * aSin.next())>>8;
}


void loop(){
  audioHook(); // required here
}




