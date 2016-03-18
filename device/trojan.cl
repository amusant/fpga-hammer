#//--/* AUTHOR:SUMANTA CHAUHDURI */
#include "../host/inc/defines.h"
#define DDR_WINDOW_BASE 0x00000000
#define TIMER  0x20
#define ACP_BASE 0x80000000
#define PAGE_SIZE 0x1000
#define TOTAL_MEM 0x40000000
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


__kernel void trojan(global unsigned int * restrict timings,global volatile unsigned int * restrict test)
{
	int total,i,offset;
	volatile int readvalue;
	unsigned int adr=0;
	offset=0x40000000-(int)test;
	offset=0;
	printf("adr= 0x%x offset=%d\n",(int)test,offset);
	//switch_addr_space(0);
	//for(adr=DDR_WINDOW_BASE;adr<DDR_WINDOW_BASE+TOTAL_MEM;adr=adr+PAGE_SIZE) {
	#pragma unroll 1
	for(adr=0;adr<10;adr=adr+1) {
		//for(i=0;i<4000;i++) {
		////printf("adr= %d\n",end_time);
		//timings[i]=end_time;
		//}
		
		total=0;
		//wriitng to teh timer hansg teh program (below)
		//test[(offset+adr+0x20)/4]=0;
		//mem_fence(CLK_GLOBAL_MEM_FENCE);
		//wriitng to teh timer hansg teh program (end)
		printf("START adr= %d time=%d\n",adr,test[(offset+adr+0x20)/4]);
		timings[2*adr]=test[(offset+adr+0x20)/4];
		mem_fence(CLK_GLOBAL_MEM_FENCE);
		#pragma unroll 1
		for(i=0;i<4096;i++) {
		//int start_time=start_timer(test) ;
		//mem_fence(CLK_GLOBAL_MEM_FENCE);
		readvalue=test[adr]; //int is 4 bytes
		if(readvalue==0x12345678) break;
		//int end_time=stop_timer(test);
		//timings[adr/PAGE_SIZE]=timings[adr/PAGE_SIZE]+end_time-start_time; //int is 4 bytes
		//total=total+end_time-start_time;
		}
		readvalue=timings[2*adr+1]=test[(offset+adr+0x20)/4];
		//readvalue=test[(offset+adr+0x20)/4];
		//mem_fence(CLK_GLOBAL_MEM_FENCE);
		printf("STOP  adr= %d time=%d\n",adr,test[(offset+adr+0x20)/4]);
		if(readvalue==0x12345678) break;
		//timings[adr/PAGE_SIZE]=total; //int is 4 bytes
		//printf("adr=%x,\treadvalue=%x,\ttime=%d\n",adr,readvalue,timings[adr >> 12]);
	}
	//for(adr=DDR_WINDOW_BASE;adr<DDR_WINDOW_BASE+TOTAL_MEM;adr=adr+PAGE_SIZE) {
	//for(adr=(unsigned int *)0x00100000;adr<DDR_WINDOW_BASE+TOTAL_MEM;adr=adr+PAGE_SIZE) {
		//int start_time=start_timer() ;
		//for(i=0;i<1000;i++) readvalue=*(adr);
		//for(i=0;i<1000;i++) *(adr)=i;
		//int end_time=stop_timer();
		///timings[adr/PAGE_SIZE]=end_time-start_time;
		//timings[(int)adr >> 12]=*(adr);
		//printf("adr=%x,\ttime=%d\n",adr,end_time-start_time);
		//printf("readvalue=%d\n",*(adr));
		//printf("adr=%lx,\treadvalue=%x,\ttime=%d\n",adr,readvalue,end_time-start_time);
	//}
}
