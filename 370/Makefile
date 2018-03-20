#! /usr/bin/make -f

all:	comp util

OFILES = cgram.o xdefs.o scan.o pftn.o trees.o optim.o code.o local.o reader.o local2.o order.o match.o allo.o comm1.o table.o
ASMFILES = cgram.out xdefs.out scan.out pftn.out trees.out optim.out code.out local.out reader.out local2.out order.out match.out allo.out comm1.out table.out util.out
BINFILES = comp util

comp:	$(OFILES)
	cc $(CFLAGS) -o comp $(OFILES)

util:	util.c
	gcc -g -o $@ $?

include ../common.mak

cgram.c:	cgram.y
	$(YACC) $(YFLAGS) $<
	mv y.tab.c $@
