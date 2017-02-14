#! /bin/sh

file=$1

bname=`basename -s.c $file`

#OPTS="-a -o -e -r"
#OPTS="-Xe -Xd -Xt Xx"

gcc -D__MVS__ -I ~/pcc/include -nostdinc -E ${bname}.c | grep -v "^#" > ${bname}.e
cat ${bname}.e | ../370/comp ${OPTS} > ${bname}.out 2> ${bname}.err
