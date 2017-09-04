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


__kernel void trojan(	global volatile unsigned short * restrict timings, //timings will never go past 16 bits
			global volatile unsigned int * restrict test,
			const unsigned int range_low,
			const unsigned int range_high,
			const unsigned int pagesize,
			const unsigned int inner_iter, 	
			const unsigned int cache_user_mask)
{
	volatile int total,i,offset,index;
	int adr=0;
	offset=0x40000000-(int)test;
	//offset=0;
	//test[(offset+0x20)/4+2]=0x0; //initialize
	index=(range_low/(pagesize/2)); //timings is a short array i.e 2 bytes
		test[0]=(int)&test[0];
		test[1]=range_low-(int)&test[0]/4;
		test[2]=range_high-(int)&test[0]/4;
		//printf("test %x, low %d, high %d\n",(int)&test[0],(range_low-(int)&test[0]/4),(range_high-(int)&test[0]/4));
	#pragma unroll 1
	int low=range_low-(int)&test[0]/4;
	int high=range_high-(int)&test[0]/4;
	//test[(offset+ADDR_SPAN_EXT_CONTROL)/4]=0x00000000;  //lower word
	//test[(offset+ADDR_SPAN_EXT_CONTROL)/4+1]=0x00000000;// upeer word
	/////////test[(offset+0x10)/4]=0x3FF00000; //TEXT START
	/////////test[(offset+0x14)/4]=0x40000000; //TEXT END
	/////////test[(offset+0x4)/4]=0x1; //START
	//__local volatile int readvalue;
	__local int readvalue[2048];
	int shift[1024];
	adr=low;
	////////////////////////////////////////////////////////////////////for(adr=low;adr<high;adr=adr+pagesize/4) {
	//why the following lien doesn't work if range_high==&test[0]/4 ??
	//for(adr=(range_low-(int)&test[0]/4);adr< (range_high-(int)&test[0]/4);adr=adr+0x400) {
	volatile int start,stop;
		
		//rowhammer: turned off access through ACP
		//mem_fence(CLK_GLOBAL_MEM_FENCE);
		//#pragma unroll 
		//for(i=0;i<inner_iter;i++) {
		//#pragma unroll 8
		for(adr=range_low;adr<range_high;adr=adr+pagesize) {
			test[(offset+0x10)/4]=adr; //TEXT START
			test[(offset+0x14)/4]=0x1000000; //TEXT END (nombres de hammer)*4
			test[(offset+0x18)/4]=0x0; //timer
			test[(offset+0x4)/4]=0x1; //START
		for(i=0;i<0x1000000;i++) {
			//readvalue=readvalue+test[adr]; //int is 4 bytes
			//mem_fence(CLK_GLOBAL_MEM_FENCE);
			//readvalue=readvalue+test[adr-0x200]; //need to open another row in the same bank 
			//				     //(0x200 = 0x800/4) 0x800 2 rows back, div by 4 for integer
			//#pragma unroll
    			//for (int j = 1023; j > 0; --j) {
      			//	shift[j] = shift[j-1];
    			//}
			//shift[0]=test[adr];
			//if(readvalue[i-1]!=0xABCDEF12) readvalue[i]=test[adr+(4*i)]+test[adr-0x2000+4*i];
			mem_fence(CLK_LOCAL_MEM_FENCE);
			mem_fence(CLK_GLOBAL_MEM_FENCE);
			//each row 2K (A0-A9, 2 bytes) so neigbouring bank at 8*2K. 
			readvalue[3]=test[(offset+0x18)/4];
			mem_fence(CLK_LOCAL_MEM_FENCE);
			mem_fence(CLK_GLOBAL_MEM_FENCE);

			//readvalue[i]=test[adr+4*i]+test[adr-0x2000+4*i];
			//mem_fence(CLK_GLOBAL_MEM_FENCE);
		}
		}
		//mem_fence(CLK_GLOBAL_MEM_FENCE);
		
	//////////////////////////////////////////////////////////////////}
	timings[0]=readvalue[3];
		//for(i=0;i<1024;i++) {
		//	if(readvalue[i-1]!=0xABCDEF12) readvalue[i]=test[adr+(4*i)]+test[adr-0x2000+4*i];
		//}
	//writing 0x180000000 to address span extender should set the base address to ACP 
	//from FPGA2HPS. ACP is visible at 0x80000000 from FPGA2HPS
}
