

# TUNE = 3ravens
# TUNE = gervbrg4
TUNE = garvalf_fugue01b

VOICES = 4

binaries:
	gcc  src/miditones.c -lm  -o miditones
	gcc  src/miditones.c -lm  -o miditones_scroll


tune:
	./miditones -v -i -pt -d  -t=${VOICES} $(TUNE)
	mv $(TUNE).c $(TUNE).h
	@printf "\n$(TUNE).h generated\n\n"
	


ino:
	make tune
	printf "#include <Playtune.h>\n" > $(TUNE).ino
	cat $(TUNE).h >> $(TUNE).ino 
	printf "Playtune pt; \n void setup() {\n  pt.tune_initchan (9); \n  pt.tune_initchan (5);\n  pt.tune_initchan (6);\n}\n\n void loop () {\n  pt.tune_playscore((byte *) score);  /* start playing */\n  while (pt.tune_playing) ;   /* wait here until playing stops */\n}\n" >> $(TUNE).ino
	@printf "\n$(TUNE).ino generated\n\n"


clean:
	rm -i *.c
	rm -i *.ino
