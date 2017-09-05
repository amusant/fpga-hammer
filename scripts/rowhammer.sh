#!/bin/bash 
end=`echo $1+1048576|bc` 
echo $end
./init_read_memrange $1 $end
mv memimage.txt memimage.txt.before
#increasing refresh interval
#devmem2 0xFFC25008 w 0x8CCDFFF
#disable command reordering
devmem2 0xFFC25000 w 0xA1042
./run $1 $end 0x0 2844444
./read_memrange $1 $end
mv memimage.txt memimage.txt.after
devmem2 0xFFC25008 w 0x8CCCC30
#diff memimage.txt.after memimage.txt.before
xxd -c 1 -b memimage.txt.before > del1
xxd -c 1 -b memimage.txt.after >  del2
diff del1 del2 | wc -l 
rm del1 del2 
