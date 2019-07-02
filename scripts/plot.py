#!/bin/python
import sys
import matplotlib as mpl
mpl.use('Agg')
import pylab as plt
import numpy as np 
import os
err=np.ndarray(shape=(13,2),dtype=float, order='F')
for j in range(0,3,1):
	count=0
	for i in range(32,129,8):
		f=str(j)+'_'+str(i)+'.result'
		if (not os.path.isfile(f)):
			continue
		a=np.loadtxt(f,dtype='str')
		print a 
		err[count,0]=i
		err[count,1]=(np.sum(a[:,1].astype(int))/4)
		err[count,1]=err[count,1]*(1e9/(16*1024*1024*8.0))
		print err[count,1]
		print err[count,0]
		count=count+1
	if (j==0):
		plt.plot(err[:,0],err[:,1],  marker='o', label='RowStripe')
	elif(j==1):
		plt.plot(err[:,0],err[:,1],  marker='o', label='Checkered')
	elif(j==2):
		plt.plot(err[:,0],err[:,1],  marker='o', label='Random')
plt.xlabel("Refresh Interval ( in ms)")
plt.xlim([32,128])
major_ticks = np.arange(32, 129, 8) 
plt.xticks(major_ticks)
plt.ylabel("Errors/ 1e9 bits")
plt.yscale('log')
plt.ylim([0,1e6])
plt.legend(loc='upper left',prop={'size':10})
plt.grid(b=True,which='major',color='gray',alpha=0.5,linestyle='-')

plt.savefig('plot.pdf',format='pdf')
	
