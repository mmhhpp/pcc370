#! /usr/bin/env python

# workaround for
#    IEW0254 ERROR - TABLE OVERFLOW -- TOO MANY EXTERNAL SYMBOLS IN ESD.

import sys

unref = open('unref').read().rstrip().split('\n')

while True:
	line = sys.stdin.readline()
	if not line:
		break
	line = line.rstrip()
	if ' PDPPRLG ' not in line:
		print line
		continue
	a = line.split()
	if a[0] in unref:
		#sys.stderr.write('replacing %s in %s\n' % (a[0], line))
		line = line.replace('ENTRY=YES', 'ENTRY=NO')
	print line
