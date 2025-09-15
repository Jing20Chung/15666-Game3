#!/usr/bin/env python

# reference: export-scene.py


import sys,re
import struct

args = []
for i in range(0,len(sys.argv)):
	if sys.argv[i] == '--':
		args = sys.argv[i+1:]

if len(args) != 2:
	print("\n\nUsage:\nexport-level.py -- <infile.txt> <outfile.bin>\nExport level file into binary file.\n")
	exit(1)

infile = args[0]
outfile = args[1]
data = b''
# reference: https://www.geeksforgeeks.org/python/read-a-file-line-by-line-in-python/
with open(infile, 'r') as file:
	alllines = file.readlines()

assert(len(alllines) > 0)

for line in alllines:
	line = line.strip()
	# reference: https://www.geeksforgeeks.org/python/python-segregate-positive-and-negative-integers-from-mixed-string/
	values = re.findall(r'-?\d+', line)
	assert(len(values) > 0)
	col_size = len(values)
	for value in values:
		data += struct.pack('i', int(value))

with open(outfile, 'wb') as file:
	file.write(struct.pack('4s',b'cols')) #type
	file.write(struct.pack('I', 4)) #length
	file.write(struct.pack('I', col_size))
	file.write(struct.pack('4s',b'lvl1')) #type
	file.write(struct.pack('I', len(data))) #length
	file.write(data)
