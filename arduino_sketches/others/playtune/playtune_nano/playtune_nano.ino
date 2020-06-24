//**************************************************
//
// Test of playtune library for Arduino NANO
//
// L. Shustek, 31 Jan 2011, initial release.
// L. Shustek, 27 Feb 2011, new score data
// L. Shustek, 6 April 2015; change for compatibility with Arduino IDE v1.6.x
// T. Wasiluk, 28 May 2016; added debug & delay code, moved to examples subdirectory
//
// 2014-09-25, Adapted code for Arduino 1.0.5
// see http://forum.arduino.cc/index.php?topic=206353
//
// 2018-2020, modified for Garvuino: uses PIN 5, 6 and 9
// 

//
//*****************************************************************************


#include "Playtune.h"

//------------------------------------------------
//   scores are put in program space memory
//------------------------------------------------

//#include "bach01.h"
//#include "3ravens.h"
//#include "gervaise.h"
//#include "garvalf_sanctuaire_hibou.h"
//#include "garvalf_musicbox01.h"
//#include "temp.h"
#include "garvalf_fugue01.h"


Playtune pt;

void setup() {

  // Enable all 3 NANO timers for notes that are output on digital pins 10, 11, and 12 (originally) 
  //                                                                     or on pins 9, 5 and 6 (Garvuino)
  // Those pins should be wired to 500-ohm resistors, the other ends of which should be connected together
  // to one terminal of an 8-ohm speaker. The other terminal of the speaker should be connected to ground.
  // No other hardware is needed!

  pt.tune_initchan (9); 
  pt.tune_initchan (5);
  pt.tune_initchan (6);
 // pt.tune_initchan (7);  // define as many initchan as there are tone generators! (displayed in the tune###".h file)


  #define DBUG 0
  #if DBUG
  Serial.begin(9600);
  Serial.println("Debug");
  #endif
  
}

void loop() {

  pt.tune_playscore (score); /* start playing */
  while (pt.tune_playing) ; /* wait here until playing stops */
  pt.tune_delay(1000); /* wait a second */
  
}
