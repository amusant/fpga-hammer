#from http://www.delorie.com/gnu/docs/textutils/coreutils_156.html

for i in `(echo obase=10; seq -f %1.f 0x20000000 0x100000 0x3FF00000) | bc` ; 
do 
	./rowhammer.sh  $i > $i.log 
done
