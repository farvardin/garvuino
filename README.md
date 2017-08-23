

# Garvuino 

The Garvuino is a PCB board for creating musics, sounds and chiptunes, either through programmation, soundtracker or live recording (midi).

![](garvuino_pcb.png)

It works with an Arduino Nano and an Atmega8 chip.

## Features 

### AY-3-8910 emulator 

On Atmega8 chip, driven by the Arduino nano, from the [AVR-AY project](http://avray.ru/)

 * It can replay Sinclair ZX Spectrum, Atari ST, Amstrad chiptunes from the sd card.
 * It might be possible to use it as a (very simple and dirty) AY synth. We just need to code the synth in the future.

### Sid emulator 

On Arduino, from the [sid emulator lib](http://playground.arduino.cc/Main/SID-emulator)

 * It can replay some sid files, from the arduino memory, which is limited (so tunes will be cut). We'd like to find a way to stream the tune from the sd card, it's probably possible.
 * It can be used as a Sid synth, driven by midi.

### 1-bit music 

From [various 1-bit engines](http://randomflux.info/1bit/viewforum.php?id=5) (on arduino)

 * It can play music from the arduino memory. 1-bit music don't need much memory so a whole song, even a complex one, can fit in the memory. It might be possible to stream data from SD if the 1-bit music engine chosen is the same for all the songs.
 * It should be possible to create a 1-bit synth in the future. Probably not an easy task.

### Mozzi synth 

 * [Mozzy synth](https://sensorium.github.io/Mozzi), can output fun FM sounds and many other effects.

## How does it sound? 

### AVRAY 

 * http://picosong.com/HUGM/
 * http://battleofthebits.org/arena/Entry/Underground+Water/26509/
 * https://www.youtube.com/watch?v=gzpGbokxwZ4
 * *more to come*

### Mozzi 

 * *more to come*

### SID emulator 

 * https://soundcloud.com/memento-somniare/sidarduino-test
 * *more to come*

### Beeper 

 * http://garvalf.online.fr/index.php?page=blog_2017-02-12
 * *more to come*

## Bill of Materials 

    Name						nb 	(optionnal part)
    		
    Garvuino PCB				1	
    crystal 24 mhz				1	
    arduino nano				1	
    atmega8						1	
    ceramic capacitor 820 pF	2	
    electrolytic capac. 10 uF	2	
    ceramic capacitor 100 nF	1		x
    led 						1		x
    DIN5 connector (midi)		1		x
    1 kΩ Resistor				1		x
    10 kΩ Resistor				2	
    micro sd module				1	
    sd module					1		x
    momentary switch			1	
    audio jack 3.5 mm (TRS)		1	
    jumper						4	
    

## Atmega8 setup 

Get a "USBASP USB ISP Programmer & 10 Pin ISP interface Cable - AVR ATMEL ATMega".

Connect it according to this schematic for the atmega8:

![](atmega8/atmega8_burn_bb.png)

Basically it's:

| SD reader |Atmega|
|--------|-------|
|MISO |pin 18: PB4|
|MOSI |pin 17: PB3|
|RESET |pin 01: PC6|
|VCC |pin 20: AVCC||
|SCK |pin 19: PB5|
|GND |pin 22: GND|

Use a 16 Mhz crystal for example, on pin 09 (PB6) and pin 10 (PB7).

Once it's connected, you can program the atmega8 chip with this command-line:

    avrdude -p atmega8 -c USBasp -U flash:w:AY_Emul_244_2ch.hex -U eeprom:w:Conf_standard_24MHz_1_75Mhz.hex -U lfuse:w:0xCE:m -U hfuse:w:0xCF:m 

Please refer to [this folder](atmega8) for more info about setting up the atmega8 chip.

## PCB 

Please refer to [this folder](pcb) for the fritzing sources and gerber files.

## Breadboard 

It can also work without the PCB. Just use a breadboard and a few wires!

![](breadboard/garvuino_09g_breadboard_only_bb.jpg)

Please refer to [this folder](breadboard) for the fritzing sources and pdf version.

## Arduino sketches 

Get the sketches [in this folder](arduino_sketches/)

*(more infos to come later)*

You can find [some tunes for using with the AVR-AY setup in here](tunes/). The arduino sketch can read the RSF files from the SD (it can't read the YM or AY files directly). You can convert to RSF with the [AVR-AY Player](http://www.avray.ru/avr-ay-player/).

## Links 

 * http://avray.ru/
 * http://playground.arduino.cc/Main/SID-emulator
 * http://randomflux.info/1bit/viewforum.php?id=5
 * https://sensorium.github.io/Mozzi

## Credits 

 * Yoruk for his help on the PCB
 * Evgeniy for AVR-AY emulator and invaluable help (initial code by Ramiros)
 * Klaud for help with avr-ay
 * Shiru for the 1-bit engines on Arduino
 * Christoph Haberer and Mario Patino for the Sid-arduino lib
 * The Mozzi team

## Licence 

This BSD licence is only for the PCB sketch rendition, and code written from scratch. 
Code derived from other projects retains the original licence.
For example Avray is GPLv3.

    BSD 3-Clause License
    Copyright (c) 2015-2017, Garvalf
    All rights reserved.
    
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
          notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
          notice, this list of conditions and the following disclaimer in the
          documentation and/or other materials provided with the distribution.
        * Neither the name of the <organization> nor the
          names of its contributors may be used to endorse or promote products
          derived from this software without specific prior written permission.
    
    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

