#//--/* AUTHOR:SUMANTA CHAUHDURI */
#include "../host/inc/defines.h"
#define DDR_WINDOW_BASE 0x00000000
#define TIMER  0x20
#define ACP_BASE 0x80000000
#define PAGE_SIZE 0x1000
#define TOTAL_MEM 0x7000000
#define ADDR_SPAN_EXT_CONTROL 0x00000000

inline int start_timer(global unsigned int *test){
	//writing to the timer register resets it
	int offset=0x40000000-(int)test;
	test[(offset+TIMER)/4]=  0x0;
	return test[(offset+TIMER)/4];
}


inline int stop_timer(global unsigned int *test){
	int offset=0x40000000-(int)test;
	return test[(offset+TIMER)/4];
}
void switch_addr_space(global unsigned int *test,int window){
	int offset=0x40000000-(int)test;
	if(window==0)
		test[(offset+ADDR_SPAN_EXT_CONTROL)/4]=0x0;
	else if(window==1)
		test[(offset+ADDR_SPAN_EXT_CONTROL)/4]= ACP_BASE;
}


__kernel void trojan(	global volatile unsigned int * restrict timings,
			global volatile unsigned int * restrict test,
			const unsigned int range_low,
			const unsigned int range_high,
			const unsigned int inner_iter, 	
			const unsigned int cache_user_mask)
{
	volatile int total,i,offset,index;
	int adr=0;
	offset=0x40000000-(int)test;
	//offset=0;
	index=(range_low/0x400);
		timings[131072]=(int)&test[0];
		timings[131073]=range_low-(int)&test[0]/4;
		timings[131074]=range_high-(int)&test[0]/4;
	#pragma unroll 1
	for(adr=(range_low-(int)&test[0]/4);adr< (range_high-(int)&test[0]/4);adr=adr+0x400) {
	volatile int readvalue;
		//printf("test %x,adr %x, low %x, high %x\n",(int)&test[0],adr,range_low-(int)&test[0],range_high+range_low-(int)&test[0]);
		
		//if(test[(offset+0x20)/4]==0x12345678) break;
		test[(offset+ADDR_SPAN_EXT_CONTROL)/4]=0x80000000;  //lower word
		test[(offset+ADDR_SPAN_EXT_CONTROL)/4+1]=0x00000001;// upeer word
		mem_fence(CLK_GLOBAL_MEM_FENCE);
		test[(offset+0x20)/4+2]= cache_user_mask; //cache and user settings reister address 0x28
		//printf("index %x\n",index);
		timings[2*index]=test[(offset+0x20)/4];
		//mem_fence(CLK_GLOBAL_MEM_FENCE);
		#pragma unroll 1
		for(i=0;i<inner_iter;i++) {
			mem_fence(CLK_GLOBAL_MEM_FENCE);
			readvalue=readvalue+test[adr]; //int is 4 bytes
			//if(readvalue==0x12345678) break;
			mem_fence(CLK_GLOBAL_MEM_FENCE);
		}
		mem_fence(CLK_GLOBAL_MEM_FENCE);
		timings[2*index+1]=test[(offset+0x20)/4];
		test[(offset+ADDR_SPAN_EXT_CONTROL)/4]=0x00000000;  //lower word
		test[(offset+ADDR_SPAN_EXT_CONTROL)/4+1]=0x00000000;// upeer word
		index=index+1;
		test[(offset+0x20)/4+2]=0x0; //cache and user settings
		//if(readvalue==0x12345678) break;
		mem_fence(CLK_GLOBAL_MEM_FENCE);
		//if  you take this line out readvalue related lines will be optimized out
		timings[131072]=readvalue;
		//timings[2*(adr >> 12)+1]=readvalue;
	}
	//writing 0x180000000 to address span extender should set the base address to ACP 
	//from FPGA2HPS. ACP is visible at 0x80000000 from FPGA2HPS
}
