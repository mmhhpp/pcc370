#! /bin/bash

f=$1
srcf=$2
result=`echo $f | sed 's/\..*$//' | sed 's/_/@/g' | dd conv=ucase 2> /dev/null`
truncf=${result:0:8}

cat $srcf | sed "s/__MODULE__/$truncf/"
