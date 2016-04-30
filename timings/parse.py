#!/usr/bin/python
import numpy,sys

a=numpy.fromfile(sys.argv[1],dtype=numpy.int16,count=2*int(sys.argv[2]))
for i in range(2,int(sys.argv[2])):
	print (a[2*i+1]-a[2*i])
