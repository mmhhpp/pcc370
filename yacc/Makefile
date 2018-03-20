#! /usr/bin/make -f

all: yacc

yacc: y1.o y2.o y3.o y4.o
	gcc -g -o yacc y?.o

y1.o y2.o y3.o y4.o: dextern files

ASMFILES = y1.out y2.out y3.out y4.out
BINFILES = yacc

include ../common.mak
