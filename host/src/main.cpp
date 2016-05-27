#include "myopencl.hpp"
//#include "webcam.hpp"
#include <iostream>     
#include <stdio.h>     
#include <stdlib.h>     
#include <fstream>
#include <time.h>
#include <omp.h>
#include <thread>
#include <chrono>
#define EXCEP_VECTOR_TABLE 0xFFFF0000
#define SLEEPTIME_FOR_STP 30
using namespace std;
unsigned int *input = NULL;
unsigned short *timings = NULL;
volatile unsigned int *timer = NULL;
int main(int argc, char *argv[]){
myopencl trojan;
int i,j=0;
printf("system call pointer %p\n",(void *)system);
	//allocate memory
  	timings = (unsigned short *)alignedMalloc(sizeof(unsigned short) * 2*(TOTAL_MEM/PAGE_SIZE));
  	timer =   (unsigned int *)alignedMalloc(sizeof(unsigned int) * 10);
	timer[8]=0xABABABAB;
	cout << "start sleep" << endl;
	cout << (unsigned long int)&timings[0] << endl;
	cout << (unsigned long int)&timer[0] << endl;
	cout << 0-(unsigned long int)&timer[0] << endl;
	std::this_thread::sleep_for (std::chrono::seconds(SLEEPTIME_FOR_STP));
	cout << "end sleep" << endl;
	if (argc < 4) { cout << "needs three arguments:range_low rang_high cache_user_mask" << endl;exit(0); }
	else { 
		trojan.set_range(strtoul(argv[1],NULL,0),strtoul(argv[2],NULL,0));
		trojan.set_cache_user_mask(strtoul(argv[3],NULL,0));
	}
	// USER [4:1]
	//b0000 = Strongly Ordered
	//b0001 = Device
	//b0011 = Normal Memory Non-Cacheable
	//b0110 = Write-Through
	//b0111 = Write Back no Write Allocate
	//b1111 = Write Back Write Allocate.
	//[0]	Shared bit	
	//
	//b0 = Non-shared
	//
	//b1 = Shared
	//CACHE[3:0]
	//0	0	0	0	Noncacheable, nonbufferable 	Strongly ordered   (0)
	//0	0	0	1	Bufferable only	Device (1)
	//0	0	1	0	Cacheable but do not allocate	Outer noncacheable (2) 
	//0	0	1	1	Cacheable and bufferable, do not allocate	Outer noncacheable (3)
	//0	1	1	0	Cacheable write-through, allocate on read	Outer write-through, no allocate on write (6)
	//0	1	1	1	Cacheable write-back, allocate on read	Outer write-back, no allocate on write (7)
	//1	0	1	0	Cacheable write-through, allocate on write	-(A)
	//1	0	1	1	Cacheable write-back, allocate on write	-(B)
	//1	1	1	0	Cacheable write-through, allocate on both read and write (E)	-
	//1	1	1	1	Cacheable write-back, allocate on both read and write	Outer write-back, write allocate (F)
	std::ofstream of ("timings.txt", std::ofstream::binary|std::ofstream::app);
	////opencl enqueue and launch
	trojan.enqueue((unsigned int *)timer);
	#pragma omp parallel num_threads(2)
	{
	int ID = omp_get_thread_num();
	if(ID==0) {
	printf("I'm thread %d, I'm launching opencl kernel\n",ID);
	trojan.launch();
	std::this_thread::sleep_for (std::chrono::seconds(1));
	trojan.dequeue(timings);
	//cout << "B4" << i << endl;
	//trojan.profile();
	//cout << "B5" << i << endl;
	////write Image
	printf("I'm thread %d, exit\n",ID);
	} else{
	printf("I'm thread %d, acessing 0x%lx\n",ID,strtoul(argv[4],NULL,0));
	unsigned long int ex_table=strtoul(argv[4],NULL,0);//0xFFFF0000;
	for (i=0;i<200;i++) 
		for(j=0;j<1000000;j++)
		//printf("I'm thread %d, acessing time\n",ID);
		//if(timer[i]==0x12345678)  printf("strange to fidn this number here %d",timer[i]);
		if(ex_table!=0) {
		if(*((unsigned long int *)ex_table)==0x12345678)  printf("strange to fidn this number here %d",timer[i]);
		}
	printf("I'm thread %d, exit\n",ID);
	}
	}
	if(of) of.write((char *)(timings),2*2*(TOTAL_MEM/PAGE_SIZE));
	printf("test: %x, low %d, high %d\n",(int)timer[0],(int)timer[1],timer[2]);
	
	//for(i=0;i<TOTAL_MEM/(2*PAGE_SIZE);i++) printf("PAGE=0x%x, time=%d\n",i,(timings[2*i+2]-timings[2*i])/10);
	//std::this_thread::sleep_for (std::chrono::seconds(1));
	of.close();
	//if (input) alignedFree(input);
	if (timings) alignedFree(timings);
	printf("system call pointer %p\n",(void *)system);
	//fdetect.profile();
	return 0;
}
