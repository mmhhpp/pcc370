#! /usr/bin/make -f

ASMFILES = exa.out ex_addr.out ex_args.out ex.out ex_ca.out ex_cmds.out exd.out ex_file.out exg.out ex_get.out ex_glob.out ex_graphic.out exh.out ex_init.out ex_io.out exj.out ex_libS.out exm.out ex_note.out exo.out ex_pack.out ex_print.out ex_put.out exr.out ex_re.out ex_recover.out exs.out ex_set.out ex_shift.out ex_source.out ex_subr.out ex_tty.out exu.out ex_unix.out ex_vadjust.out exv.out ex_vclear.out ex_vcolumn.out ex_vcurs.out ex_vkey.out ex_vmain.out ex_voperate.out ex_vops.out ex_vset.out exw.out exx.out exz.out Version.out globals.out ttycap.out htmpg.out fake.out 

fixjcl:	jcl.txt
	mv jcl.txt jcl.tmp
	cat jcl.tmp | python fixjcl.py > jcl.txt
	rm jcl.tmp

include ../common.mak
