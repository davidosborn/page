#!/usr/bin/env python

import array
import sys # argv, exit

if len(sys.argv) < 2:
	print 'error: no input file'
	sys.exit(1)

inPath = sys.argv[1]
outPath = len(sys.argv) >= 3 and sys.argv[2] or sys.argv[1] + '.xor'

fin = open(inPath, 'rb')
fout = open(outPath, 'wb')

# write header
fout.write('x100') # used to be 'frob'

# write data
while True:
	data = array.array('B', fin.read(1048576))
	if not data: break
	for i in range(len(data)): data[i] ^= 100 # used to be 42
	fout.write(data.tostring())
