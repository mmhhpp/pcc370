#! /bin/sh

SUBDIRS="370 lib cpp yacc apl11 ex-1.1"

cd yacc
make yacc
cd ..

cd 370
make comp
make util
cd ..

for d in $SUBDIRS; do
	cd $d
	make jcl
	cd ..
done

for d in $SUBDIRS; do
	echo Assembler JCL needs to be submitted: $d/jcl.txt
done

echo after all assembler JCL submitted, link JCL needs to be submitted:
ls */linkjcl.txt
