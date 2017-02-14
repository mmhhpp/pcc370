CFLAGS=-O -g
all: yacc
	:

cmp: all
	cmp yacc /bin/yacc
	rm yacc *.o

cp:	all
	cp yacc /bin/yacc
	rm yacc *.o

yacc: y1.o y2.o y3.o y4.o
	gcc -g -o yacc y?.o

y1.o y2.o y3.o y4.o: dextern files

clean:
	rm -f y?.o yacc
