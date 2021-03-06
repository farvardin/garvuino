

# Garvuino 

The Garvuino is a PCB board for creating music, sounds and chiptunes, either through programming, soundtracker or live recording (midi).

 ![](doc/garvuino_pcb.png)

It works with an Arduino Nano and an Atmega8 chip.

## Manual 

You can find the full manual [here](doc/garvuino_manual.md).

## Features 

 * AY-3-8910 emulator (using an atmega8 chip): it can replay Sinclair ZX Spectrum, Atari ST, Amstrad square wave chiptunes from the sd card.
 * Sid emulator: play some C64 musics, or use it as a midi synth with a fat sound.
 * 1-bit music: play beeper music. 
 * Mozzi synth: fun FM sounds and many other effects, driven by MIDI or some captors.

 ![](doc/garvuino01.jpg) 

## How does it sound? 

### AVRAY 

 * http://picosong.com/HUGM/ (Xenon / Spectrum)
 * http://picosong.com/wseMn/ (Cauldron / Amstrad)
 * http://battleofthebits.org/arena/Entry/Underground+Water/26509/
 * https://www.youtube.com/watch?v=gzpGbokxwZ4
 * *more to come*

### Mozzi 

 * http://picosong.com/wR2RN (polydrums)
 * http://picosong.com/wR2Rr (cheap FM)
 * *more to come*

### SID emulator 

 * https://soundcloud.com/memento-somniare/sidarduino-test
 * *more to come*

### Beeper 

 * http://garvalf.online.fr/index.php?page=blog_2017-02-12
 * *more to come*

## Atmega8 setup 

Please refer to the [manual](doc/garvuino_manual.md) and [this folder](atmega8) for more info about setting up the atmega8 chip.

## PCB 

Please refer to [this folder](pcb) for the fritzing sources and gerber files.

## Breadboard 

It can also work without the PCB. Just use a breadboard and a few wires!

![](breadboard/garvuino_09g_breadboard_only_bb.jpg)

Please refer to [this folder](breadboard) for the fritzing sources and pdf version.

## Arduino sketches 

Get the sketches [in this folder](arduino_sketches/)

You can find [some tunes for using with the AVR-AY setup in here](tunes/). The arduino sketch can read the RSF files from the SD (it can't read the YM or AY files directly). You can convert to RSF with the [AVR-AY Player](http://www.avray.ru/avr-ay-player/).

## Links 

 * http://avray.ru/
 * http://playground.arduino.cc/Main/SID-emulator
 * http://randomflux.info/1bit/viewforum.php?id=5
 * https://sensorium.github.io/Mozzi
 * http://risgk.github.io/
 * https://github.com/dzlonline/the_synth
 * https://github.com/ArminJo/Talkie (not included in this repo, but works)

## Credits 

 * Yoruk for his help on the PCB
 * Evgeniy for AVR-AY emulator and invaluable help (initial code by Ramiros)
 * Klaud for help with avr-ay
 * Shiru for the 1-bit engines on Arduino
 * Christoph Haberer and Mario Patino for the Sid-arduino lib
 * The Mozzi team
 * Protodome for the Micro Music Macro Language (μMML) (https://github.com/protodomemusic/mmml) 

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

