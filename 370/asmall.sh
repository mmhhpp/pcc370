#! /bin/bash

rm -f deck.txt

./asmdeck.sh allo.out
./asmdeck.sh cgram.out
./asmdeck.sh code.out
./asmdeck.sh comm1.out
./asmdeck.sh local.out
./asmdeck.sh local2.out
./asmdeck.sh match.out
./asmdeck.sh optim.out
./asmdeck.sh order.out
./asmdeck.sh pftn.out
./asmdeck.sh reader.out
./asmdeck.sh scan.out
./asmdeck.sh table.out
./asmdeck.sh trees.out
./asmdeck.sh xdefs.out
./asmdeck.sh ../lib/ctype.out
./asmdeck.sh ../lib/stdio.out
./asmdeck.sh ../lib/errno.out
./asmdeck.sh ../lib/signal.out
./asmdeck.sh ../lib/start.out
./asmdeck.sh ../lib/stdlib.out
./asmdeck.sh ../lib/string.out

cat asmjcl.txt ../lib/mvssupa.asm ss | sed "s/__MODULE__/MVSSUPA/" >> deck.txt
cat asmjcl.txt ../lib/mvsstart.asm ss | sed "s/__MODULE__/MVSSTART/" >> deck.txt
