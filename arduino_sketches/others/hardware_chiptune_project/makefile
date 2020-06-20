


#SONG=bearborne
#SONG=kryo_hcp
SONG=garvalf01

convert:
	gcc -c exported.s
	objdump -s exported.o | tail -n+5 | java SongConverter > ${SONG}.lst
	make generate_arduino
	
	
edit:
	#setxkbmap us
	./tracker ${SONG}.song
	#setxkbmap fr
	

backup:
	cp ${SONG}.song backups/${SONG}_`date +%Y-%m-%d_%Hh%M`.song
	

generate_arduino:
	cp ${SONG}.lst temp.lst
	perl  -pe "s/--replace--/`cat temp.lst`/g" template.ino > ${SONG}.ino
	#perl  -pe "s/--include--/`cat exported.h`/g" template.ino > ${SONG}.ino
	cp ${SONG}.ino hardware_chiptune_project/hardware_chiptune_project.ino

