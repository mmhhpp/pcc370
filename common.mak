# common make rules - ASMFILES needs to be defined
# also BINFILES (for make clean)
CFLAGS=-O -g
INCLUDES=-I.
INCL_370=-I../include
DEFS_370=-Dibm -D__MVS__
COMP_DIR=../370
YACC=../yacc/yacc
EDITF=../370/editf.sh

.c.o:
	cc -c $(CFLAGS) $(INCLUDES) $<

.asm.out:
	$(EDITF) $* $(COMP_DIR)/asmjcl.txt > $@
	cat $< >> $@
	cat $(COMP_DIR)/ss >> $@

.c.out:
	$(EDITF) $* $(COMP_DIR)/asmjcl.txt > $@
	gcc $(DEFS_370) $(INCL_370) -Uunix -nostdinc -E $< | grep -v "^#" | $(COMP_DIR)/util | $(COMP_DIR)/comp -M$* | dd conv=ucase >> $@
	cat $(COMP_DIR)/ss >> $@

.SUFFIXES: .asm .out

jcl:	jcl.txt

jcl.txt:	$(ASMFILES) $(COMP_DIR)/comp $(COMP_DIR)/util
	cat $(ASMFILES) > $@

clean:
	rm -f *.o comp *.e *.out *.err jcl.txt $(ASMFILES) $(BINFILES)
