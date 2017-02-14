#! /bin/bash

OPTS="-a -o -e -r -Xiiiii -Xeeeee -Xdddddddd"
unset OPTS

files="cgram.c xdefs.c scan.c pftn.c trees.c optim.c code.c local.c reader.c local2.c order.c match.c allo.c comm1.c table.c code.c"

for f in $files; do
	bname=`basename -s.c $f`
	echo compiling $f
	gcc -Dibm -DTARGET_370 -D__MVS__ -I ~/pcc/include -nostdinc -E ${bname}.c | grep -v "^#" > ${bname}.e
	cat ${bname}.e | ./comp ${OPTS} > ${bname}.out 2> ${bname}.err
done

