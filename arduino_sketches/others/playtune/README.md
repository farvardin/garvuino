

## Playtune and Miditones for Arduino 

This library is an adapted version of 

 * https://github.com/LenShustek/arduino-playtune
 * https://github.com/lenshustek/miditones

They were copied and adapted for the Garvuino device:

 * https://github.com/farvardin/garvuino/

## Compile and use Miditones 

To compile miditones, just enter the "miditones" folder and type:

    make binaries

Then you can put your midifiles into this folder, edit the makefile 
(don't happend .mid at the end of the file declaration), and can type 
`make tone` to produce a .c file which you can use with playtune. 

## Playtune 

Import Playtune as a new library from your Arduino IDE.

You can use the .c file from above, or type `make ino` and it will 
provide a file you can paste or load in the Arduino IDE 
(it works great in my Arduino Uno).

## Wiring to your Arduino 

To hear the music, you have to wire a headphone or speaker to the pins 5, 6 and 9 
of your arduino (link the 3 pins together, on a breadboard for example), 
and the other end of your speaker to a ground pin. 

In addition you can add a resistor (300 or 600 ohm for ex.) to reduce some high frequencies.

