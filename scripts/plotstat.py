#!/bin/python
import re,os
from numpy import *
import matplotlib.pyplot as mpl
import numpy as np

def plot():
    a=np.loadtxt('stat.txt')
    mpl.hist(a[:,0], normed=False, histtype='stepfilled', bins=128,label='offset within a 4K page')
    mpl.xlim([0,4095])
    mpl.grid(b=True,which='major',color='gray',alpha=0.5,linestyle='-')
    mpl.savefig('plot_offset.pdf',format='pdf')
    mpl.show()
    x=log2(a[:,1]) 
    mpl.hist(x, normed=False, histtype='stepfilled', bins=8,label='offset within a 4K page')
    mpl.xlim([0,7])
    mpl.grid(b=True,which='major',color='gray',alpha=0.5,linestyle='-')
    mpl.savefig('plot_bit_position.pdf',format='pdf')
    mpl.show()
    a=np.loadtxt('fullstat.txt')
    wghts,bins=np.histogram(a,bins=4096,range=(0x30000000,0x40000000),density=False,normed=True)
    print wghts
    print bins
    mpl.matshow(np.reshape(wghts,(64,64)))
    mpl.show()



def write_stat():
    freq=np.ndarray(shape=(4096,8),dtype=float, order='F')
    stat=open('stat.txt','w')
    stat.close()
    fullstat=open('fullstat.txt','w')
    fullstat.close()
    for i in range(0x300,0x400,1):
        f='0_64.0x305.diff'
        f='0_64.'+str(hex(i))+'.diff'
        #f=str(j)+'_'+str(i)+'.result'
        if (not os.path.isfile(f)):
            continue
        #writes page offsets
        stat=open('stat.txt','a')
        #writes actual address
        fullstat=open('fullstat.txt','a')
        data=open(f)
        line=data.readline()
        while(line):
            frmt=re.search('< ([a-f,0-9]*):\s*([0-1]*)',line)
            if(frmt):
                adr=int('0x'+frmt.group(1),16)
                val=int('0b'+frmt.group(2),2)
                line=data.readline()
                line=data.readline()
                frmt1=re.search('> ([a-f,0-9]*):\s*([0-1]*)',line)
                adr1=int('0x'+frmt1.group(1),16)
                val1=int('0b'+frmt1.group(2),2)
                diff=abs(val1-val)
                bitposition=log2(diff)
                fulladdr=(adr1+(i<<20))
                #print hex(adr1+(i<<20))
                page_offset=mod(adr1,4096)
                if(adr==adr1):
                    freq[page_offset,bitposition]=freq[page_offset,bitposition]+1
                    stat.write( "%d\t%d\n" % (page_offset,diff))
                    fullstat.write( "0x%x\t%d\n" % (fulladdr,diff))
                else:
                    print "err"
                    return 1
        
            line=data.readline()
    stat.close()
    fullstat.close()
    plot()
    mpl.matshow(freq[1:128,:])
    mpl.show()
    freqf=open('freq.txt','w')
    for i in range(4096):
        freqf.write( "%d\t" % (i))
        for j in range(8):
            freqf.write( "%d\t" % (freq[i,j]))
        freqf.write("\n")
    freqf.close()

write_stat()    
    
