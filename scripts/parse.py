import numpy

a=numpy.fromfile("timings.txt",dtype=numpy.int32,count=0x0037fd0)
for i in range(0x37fd0):
	print (a[2*i+2]-a[2*i])/4096	
