
# makefile for use with https://github.com/protodomemusic/mmml


SONG=deux_etoiles
#SONG=torche
#SONG=fly-me-to-the-moon


	
wav:
	./mmml-compiler -f ${SONG}.mmml -t data
	./mmml-desktop-synthesizer -f output.mmmldata -s 105
	
	
avr:
	./mmml-compiler -f ${SONG}.mmml -t avr
	avr-gcc -g -Os -mmcu=atmega8 -DF_CPU=8000000 -o mmml.bin mmml-avr-player.c 
	avr-objcopy -j .text -j .data -O ihex mmml.bin mmml.hex
	avr-size --format=avr --mcu=atmega8 mmml.bin
	
flash:
	avrdude -p atmega8 -c USBasp -U flash:w:mmml.hex:i -U lfuse:w:0xe4:m -U hfuse:w:0xd9:m 
	
	
tools:
	gcc mmml-compiler.c -o mmml-compiler
	gcc mmml-desktop-synthesizer.c -o mmml-desktop-synthesizer
	
	
clean:
	rm mmml.hex
	rm mmml.bin
	rm -i output.wav
	rm avr-mmml-data.h
	
	