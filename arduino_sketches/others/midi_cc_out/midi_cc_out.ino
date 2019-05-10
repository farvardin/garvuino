/* 
 *   Garvuino
 *   
 *   Use potentiometers to control CC values  
 *   over USB midi
 * 
 * * * * * * * * * * * * * * * * * * * * * */


// based on https://www.instructables.com/id/Easy-3-Pot-Potentiometer-Arduino-Uno-Effects-Midi-/
// Works with Arduino IDE 1.8.3

/*
 * Connect 5 potentiometers to A0, A1, A2, A3, A5 and A6 
 * Connect ssd1306 128x64 OLED display to A4 (SDA) and A5 (SCL)
 * Use hairless midi to establish the connection with the arduino
 * 
 */

#define OLED 

#include "ssd1306.h"  // oled display by Alexey Dynda
#include <garvuino.h>


// Our initial pot values. We need one for the first value and a second to test if 
//  there has been a change made. This needs to be done for all 3 pots.

int val = 0; 
int lastVal = 0;
int val2 = 0;
int lastVal2 = 0;
int val3 = 0;
int lastVal3 = 0;
int val4 = 0;
int lastVal4 = 0;
int val5 = 0;
int lastVal5 = 0;


  // you can define your prefered CC values here
#define CC1 91
#define CC2 92
#define CC3 93
#define CC4 94
#define CC5 95

void setup()
{ 
      // Set the speed of the midi port to the same as we will be using in the Hairless Midi software 
   Serial.begin(38400);  

#if defined (OLED)
   ssd1306_128x64_i2c_init();
   ssd1306_fillScreen(0x00);
   ssd1306_drawBitmap(0, 0, 128, 64, garvuino_logo);
   delay(20);
   garvuino_welcome();
#endif

}

void loop()
{
   val = map(analogRead(0),0,1023,0,127);      // midi range 0-127
   if (val != lastVal) // If the value does not = the last value the following command is made. 
                       // This is because the pot has been turned. Otherwise the pot remains 
                       // the same and no midi message is output.
   {
   MIDImessage(176,CC1,val);}         // 176 = CC command (channel 1 control change), CC1 = Which Control, val = value read from Potentionmeter 1
   lastVal = val;

   val2 = map(analogRead(1),0,1023,0,127);   
   if (val2 != lastVal2) 
   {
   MIDImessage(176,CC2,val2);}         // 176 = CC command, CC2 = Which Control, val2 = value read from Potentionmeter 2
   lastVal2 = val2;
   
   val3 = map(analogRead(2),0,1023,0,127);   // midi range 0-127
   if (val3 != lastVal3) 
   {
   MIDImessage(176,CC3,val3);}         // 176 = CC command, CC3 = Which Control, val3 = value read from Potentionmeter 3
   lastVal3 = val3;

   val4 = map(analogRead(3),0,1023,0,127);   // midi range 0-127
   if (val4 != lastVal4) 
   {
   MIDImessage(176,CC4,val4);}         // 176 = CC command, CC4 = Which Control, val4 = value read from Potentionmeter 4
   lastVal4 = val4;

   val5 = map(analogRead(6),0,1023,0,127);   // midi range 0-127 (analog 4 et 5 are used by oled display so we use 6 here)
   if (val5 != lastVal5) 
   {
   MIDImessage(176,CC5,val5);}         // 176 = CC command, CC5 = Which Control, val5 = value read from Potentionmeter 6
   lastVal5 = val5;

      // here we add a short delay to help prevent slight fluctuations, knocks on the pots etc. 
      // Adding this helped to prevent my pots from jumpin up or down a value when slightly touched or knocked.
    delay(10); 
  
  #if defined (OLED)
      // clean screen to make CC value less noticeable
    ssd1306_printFixed(0,  30, "          ", STYLE_NORMAL); // x, y position
  #endif
}

void MIDImessage(byte command, byte data1, byte data2) //pass values out through standard Midi Command
{
   Serial.write(command);
   Serial.write(data1);
   Serial.write(data2);
   
   #if defined (OLED)
      String str1;
      String str2;
      char noted[6];
      char noted2[6];
   
        // clean display before displaying new value
      ssd1306_printFixed(20,  30, "   ", STYLE_NORMAL); // x, y position
      ssd1306_printFixed2x(4,  4, "   ", STYLE_NORMAL); 
  
        // convert numeral data to string so it can be displayed by oled screen
      str1=String(data1);
      str1.toCharArray(noted,6); 
      str1=String(data2);
      str1.toCharArray(noted2,6); 
          
        // display values   
      ssd1306_printFixed(4,  30, "CC ", STYLE_NORMAL);  // x, y position
      ssd1306_printFixed(20,  30, noted, STYLE_NORMAL); // x, y position
      ssd1306_printFixed2x(4,  4, noted2, STYLE_NORMAL);  // x, y position
   #endif
}
