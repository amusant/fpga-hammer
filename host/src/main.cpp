#include "myopencl.hpp"
//#include "webcam.hpp"
#include <iostream>     
#include <stdio.h>     
#include <stdlib.h>     
#include <fstream>
#include <time.h>
//#include <thread>
//#include <chrono>
#define EXCEP_VECTOR_TABLE 0xFFFF0000
using namespace std;
unsigned int *input = NULL;
unsigned int *timings = NULL;
unsigned int *timer = NULL;
int main(int argc, char *argv[]){
myopencl trojan;
int i=0;
	//allocate memory
  	//input = (unsigned int *)alignedMalloc(sizeof(unsigned int) * ROWS * COLS/2);
  	timings = (unsigned int *)alignedMalloc(sizeof(unsigned int) * (TOTAL_MEM/PAGE_SIZE));
  	timer =   (unsigned int *)alignedMalloc(sizeof(unsigned int) * 4);
	//timer=(unsigned int *)0xc000000;
	//load Image
	std::ofstream of ("timings.txt", std::ofstream::binary|std::ofstream::app);
	////opencl enqueue and launch
	//cout << "B1" << i << endl;
	//trojan.enqueue(timer);
	//cout << "B2" << i << endl;
	trojan.launch();
	//cout << "B3" << i << endl;
	//std::this_thread::sleep_for (std::chrono::seconds(1));
	trojan.dequeue(timings);
	//cout << "B4" << i << endl;
	//trojan.profile();
	//cout << "B5" << i << endl;
	////write Image
	if(of) of.write((char *)(timings),TOTAL_MEM/PAGE_SIZE);
	//std::this_thread::sleep_for (std::chrono::seconds(1));
	of.close();
	//if (input) alignedFree(input);
	if (timings) alignedFree(timings);
	//fdetect.profile();
	return 0;
}
