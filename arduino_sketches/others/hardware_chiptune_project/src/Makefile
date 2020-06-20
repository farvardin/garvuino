# makefile for The hardware chiptune project
# by Kryo (Linus Åkesson)
# https://www.linusakesson.net/hardware/chiptune.php
# licence: public domain

LDFLAGS=-lSDL -lncurses
CFLAGS=-Wall
CC=gcc

all:		tracker

tracker:	main.o chip.o gui.o
		gcc -o $@ $^ ${LDFLAGS}

%.o:		%.c stuff.h Makefile
