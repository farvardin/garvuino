


## Create chiptunes for the Arduino

This project is based on the "Hardware Chiptune Project" by Kryo (Linus Åkesson) : https://www.linusakesson.net/hardware/chiptune.php

It was ported to Arduino by Stimmer: 
- https://forum.arduino.cc/index.php/topic,44907.0.html


And "Bearbone" made a converter so you can create your own tunes with Kryo's original Tracker (see below):

- http://bearborne.blogspot.com/2011/12/hi-everyone-today-im-back-again-with.html
- https://www.instructables.com/id/Arduino-Chiptunes/


You can run the "kryo_hcp_org.ino" sketch, which is roughly the original port by Stimmer (and should sound close to the original tune)




## Tracker:

you can compile the sources for the tracker (on Linux), in the src folder. The "tracker" binary is provided as well (originally found in Kryo's archive)
Instructions for using the tracker can be found in README-tracker.txt

You can use my makefile to help convert the data to arduino.

The original instructions for the converter are:


- Use the tracker (Linux) to compose it.
- Export it ("%" key in the tracker).
- Run the conversion script in the tracker folder (you'll need gcc and java for it to run!).
- Open the file "song.lst" and copy its contents to the clipboard.
- Open the player sketch and replace the byte sequence in "songdata" by the sequence from "song.lst"
- Update the defines of "MAXTRACK" and "SONGLEN" to the values found in "exported.h" in the tracker directory.
- Connect a speaker to port 3 and two LEDs to ports 8 and 13 and enjoy your personal disco with synchronized light show.


Except for the "MAXTRACK" and "SONGLEN", and for connecting the speaker, the makefile should handle everything else!


For French users, you'd better switch to qwerty (done in the makefile). Non-azerty users should remove the "setxkbmap fr" in the makefile.

You can change instruments with the two keys right to the "P" key.
You can change patterns with the two keys right to the "P" key (use SHIFT in addition).
You can save your work with Ctrl+w (the key left to the "E" key)
You can export your work with % (shift+5)

More info on http://bearborne.blogspot.com/2012/02/lft-tracker-howto.html


Change your working document in the makefile, and use "make edit" to open it with the tracker, "make convert" to generate the "hardware_chiptune_project.ino" (arduino requires the .ino file to have the same name as the containing folder). "make backup" is a conveniant way to save a snapshot of your current work, in the backups folder.

Important: you might need to optimise the song (press # in the editor) before exporting it (%), otherwise the song might not be correctly converted.


