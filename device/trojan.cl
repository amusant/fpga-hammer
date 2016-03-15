#//--/* AUTHOR:SUMANTA CHAUHDURI */
#include "../host/inc/defines.h"
#define DDR_WINDOW_BASE 0x00000000
//#define TIMER  (int *)0x40000020
#define ACP_BASE 0x180000000
#define PAGE_SIZE 0x1000
#define TOTAL_MEM 0x40000000
//#define ADDR_SPAN_EXT_CONTROL (int *)0x40000000


int start_timer(unsigned int *TIMER){
	//writing to the timer register resets it
	*(TIMER)=  0x0;
	return *(TIMER);
}


int stop_timer(unsigned int *TIMER){
	return *(TIMER);
}
void switch_addr_space(unsigned long int *ADDR_SPAN_EXT_CONTROL,int window){
	if(window==0)
		*(ADDR_SPAN_EXT_CONTROL)=0x0;
	else if(window==1)
		*(ADDR_SPAN_EXT_CONTROL)= ACP_BASE;
}


__kernel void trojan(global unsigned int * restrict timings, global  unsigned long TIMER, global unsigned long ADDR_SPAN_EXT_CONTROL)
{
	int i,readvalue;
	unsigned int adr;
	switch_addr_space((unsigned long *)ADDR_SPAN_EXT_CONTROL,0);
	for(adr=DDR_WINDOW_BASE;adr<DDR_WINDOW_BASE+TOTAL_MEM;adr=adr+PAGE_SIZE) {
		int start_time=start_timer((unsigned int *)TIMER) ;
		for(i=0;i<1000;i++) readvalue=*((int *)adr);
		int end_time=stop_timer((unsigned int *)TIMER);
		//printf("adr= %x\n",adr);
		timings[adr/PAGE_SIZE]=end_time-start_time;
	}
	switch_addr_space((unsigned long *)ADDR_SPAN_EXT_CONTROL,1);
	for(adr=DDR_WINDOW_BASE;adr<DDR_WINDOW_BASE+TOTAL_MEM;adr=adr+PAGE_SIZE) {
		int start_time=start_timer((unsigned int *)TIMER) ;
		for(i=0;i<1000;i++) readvalue=*((int *)adr);
		int end_time=stop_timer((unsigned int *)TIMER);
		timings[adr/PAGE_SIZE]=end_time-start_time;
		printf("adr=%x,readvalue=%x\ttime=%d\n",adr,readvalue,end_time-start_time);
	}
}
