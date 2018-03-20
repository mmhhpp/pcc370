CC= cc
CFLAGS= -O
# LD=/bin/ld
# LFLAGS=
LIBS= -lm
YACC= yacc

OS = a0.o a1.o a2.o a3.o a4.o a5.o a6.o a7.o a8.o a9.o \
	 aa.o ab.o ac.o ad.o ae.o af.o ag.o ah.o ai.o aj.o \
	 ak.o al.o am.o ao.o aq.o at.o aw.o ax.o az.o \
	 y.tab.o gamma.o

.c.o:
	$(CC) $(CFLAGS) -c $<

apl:	$(OS)
	$(CC) -static -o apl $(OS) $(LIBS)

y.tab.o: y.tab.c lex.c tab.c apl.h
	$(CC) $(CFLAGS) -c y.tab.c

y.tab.c: apl.y
	$(YACC) apl.y

rebuild:
	$(YACC) apl.y
	$(CC) $(CFLAGS) -c a?.c y.tab.c gamma.c
	$(LD) $(LFLAGS) /lib/crt0.o -o apl *.o $(LIBS)
	: APL rebuild complete

apl2:
	rm -f *.o y.tab.c
	$(YACC) apl.y
	$(CC) $(CFLAGS) -DAPL2 -c a?.c y.tab.c gamma.c
	$(LD) $(LFLAGS) /lib/crt0.o -o apl2 *.o $(LIBS)
	rm -f *.o y.tab.c
	: APL2 build complete

backup.tar:
	rm -f backup.tar
	tar cvf backup.tar *.h a*.c cvt.c gamma.c lex.c tab.c apl.y

count:
	wc apl.h a*.c cvt.c gamma.c lex.c tab.c apl.y

clean:
	rm -f apl *.o core y.tab.c

a0.o: apl.h a0.c
a1.o: apl.h a1.c
a2.o: apl.h a2.c
a3.o: apl.h a3.c
a4.o: apl.h a4.c
a5.o: apl.h a5.c
a6.o: apl.h a6.c
a7.o: apl.h a7.c
a8.o: apl.h a8.c
a9.o: apl.h a9.c
aa.o: apl.h aa.c
ab.o: apl.h ab.c
ac.o: apl.h ac.c
ad.o: apl.h ad.c
ae.o: apl.h ae.c
af.o: apl.h af.c
ag.o: apl.h ag.c
ah.o: apl.h ah.c
ai.o: apl.h ai.c
aj.o: apl.h aj.c
ak.o: apl.h ak.c
al.o: apl.h al.c
am.o: apl.h am.c
an.o: apl.h an.c
ao.o: apl.h ao.c
aq.o: apl.h aq.c
at.o: apl.h at.c
aw.o: apl.h aw.c
ax.o: apl.h ax.c
az.o: apl.h az.c
cvt.o: apl.h cvt.c
gamma.o: apl.h gamma.c
