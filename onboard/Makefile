
all: read_memrange init_read_memrange
	#bash rowhammer_scan.sh
	python rowhammer.py 
read_memrange:
	gcc read_memrange.c -o read_memrange
init_read_memrange:
	gcc init_read_memrange.c -o init_read_memrange
test:
	./rowhammer.sh 0x30000000 0x30010000 
clean:
	rm -rf memimage* *.diff gmon.out
