
all: read_memrange init_read_memrange
	#bash rowhammer_scan.sh
	python rowhammer.py 
read_memrange:
	gcc read_memrange.c -o read_memrange
init_read_memrange:
	gcc init_read_memrange.c -o init_read_memrange
test: read_memrange init_read_memrange				
	echo 'launches a test from 0x2de00000 +1MB with RI 64ms'
			#start    RI=64ms           #logfile
	./rowhammer.sh 769654784 147639349 0x2de 0 test
clean:
	rm -rf memimage* *.diff gmon.out
