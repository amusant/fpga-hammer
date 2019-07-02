#!/bin/bash 
#1:ADDR 2: RI 3: addr 4: pattern 5:logfilename(refresh)
end=`echo $1+1048576|bc` 
echo $end
./init_read_memrange $1 $end $4
mv memimage.txt memimage.txt.before
#increasing refresh interval
devmem2 0xFFC25008 w $2
#disable command reordering
devmem2 0xFFC25000 w 0xA1042
#./run $1 $end 0x0 2844444
./run $1 $end 0x0  5688888
./read_memrange $1 $end
mv memimage.txt memimage.txt.after
devmem2 0xFFC25008 w 0x8CCCC30
#diff memimage.txt.after memimage.txt.before
xxd -c 4 -b memimage.txt.before > del1
xxd -c 4 -b memimage.txt.after >  del2
echo "Errors: "
diff del1 del2 > $5.$3.diff
n=`diff del1 del2 | wc -l`
echo "$3: $n"  >> $5.result 
rm del1 del2 
