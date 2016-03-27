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
using namespace std;
unsigned int *input = NULL;
unsigned int *timings = NULL;
volatile unsigned int *timer = NULL;
int main(int argc, char *argv[]){
myopencl trojan;
int i,j=0;
	//allocate memory
  	timings = (unsigned int *)alignedMalloc(sizeof(unsigned int) * 2*(TOTAL_MEM/PAGE_SIZE));
  	timer =   (unsigned int *)alignedMalloc(sizeof(unsigned int) * 10);
	timer[8]=0xABABABAB;
	cout << "start sleep" << endl;
	std::this_thread::sleep_for (std::chrono::seconds(30));
	cout << "end sleep" << endl;
	if (argc < 3) { cout << "needs two arguments:range_low rang_high" << endl;exit(0); }
	else trojan.set_range(atoi(argv[1]),atoi(argv[2]));
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
	} else{
	printf("I'm thread %d, acessing time\n",ID);
	for (i=0;i<10;i++) 
		for(j=0;j<1000000;j++)
		//printf("I'm thread %d, acessing time\n",ID);
		if(timer[i]==0x12345678)  printf("strange to fidn this number here %d",timer[i]);
	}
	}
	if(of) of.write((char *)(timings),2*4*(TOTAL_MEM/PAGE_SIZE));
	//for(i=0;i<TOTAL_MEM/(2*PAGE_SIZE);i++) printf("PAGE=0x%x, time=%d\n",i,(timings[2*i+2]-timings[2*i])/10);
	//std::this_thread::sleep_for (std::chrono::seconds(1));
	of.close();
	//if (input) alignedFree(input);
	if (timings) alignedFree(timings);
	//fdetect.profile();
	return 0;
}
