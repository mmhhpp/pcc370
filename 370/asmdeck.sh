#! /bin/sh

f=$1
f_uc=${f}.uppercase
bname_uc=`basename -s.out $f | dd conv=ucase`

echo "processing $bname_uc"

grep -v '^*' $f | dd of=$f_uc conv=ucase

cat asmjcl.txt $f_uc ss | sed "s/__MODULE__/$bname_uc/" >> deck.txt
