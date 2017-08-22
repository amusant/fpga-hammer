/*
 * devmem2.c: Simple program to read/write from/to any location in memory.
 *
 *  Copyright (C) 2000, Jan-Derk Bakker (jdb@lartmaker.nl)
 *
 *
 * This software has been developed for the LART computing board
 * (http://www.lart.tudelft.nl/). The development has been sponsored by
 * the Mobile MultiMedia Communications (http://www.mmc.tudelft.nl/)
 * and Ubiquitous Communications (http://www.ubicom.tudelft.nl/)
 * projects.
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>

#define FATAL do { fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", \
  __LINE__, __FILE__, errno, strerror(errno)); exit(1); } while (0)
 
#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)

#ifdef DEBUG
        #define debug(...) printf(__VA_ARGS__)
#else
        #define debug(...) do {} while(0)
#endif
#define  PROTRULERDWR  0xFFC2509C
#define  PROTRULEADDR  0xFFC25090
#define  PROTRULEID    0xFFC25094
#define  PROTRULEDATA  0xFFC25098
static unsigned int read_mem(off_t target);
static void write_mem(off_t target,unsigned int writeval);

int main(){
int i;
int addr_range,id_range,port_mask;
for (i=0; i< 20;i++) {
	
	write_mem(PROTRULERDWR,i);
	write_mem(PROTRULERDWR, 0x40|i);
	addr_range=read_mem(PROTRULEADDR);
	id_range=read_mem(PROTRULEID);
	port_mask=read_mem(PROTRULEDATA);
	if( ((port_mask & 0x4) >> 2) ==0x1) { 
	printf("----------------Valid Rule %d------------------------\n", i);
	if( ((port_mask & 0x2000) >> 13) ==0x1) printf("FAIL\n");
	else printf("ALLOW\n");
	printf("ADDR 0x%x -0x%x\n",((addr_range & 0xFFF000)>> 12),(addr_range & 0x000FFF));
	printf("ID 0x%x -0x%x\n",((id_range & 0xFFF000)>> 12),(id_range & 0x000FFF));
	printf("Port Mask 0x%x",port_mask);
	printf("Port Mask 0x%x,0x%x,0x%x,0x%x\n",((port_mask & 0x1000) >> 12),((port_mask & 0xF00) >> 8),((port_mask & 0xF0)>> 4),((port_mask & 0x8) >> 3));
	printf("----------------------------------------------------- \n");
	}
}

	
}

static unsigned int read_mem(off_t target){
        unsigned int read_result;
        int fd;
        void *map_base, *virt_addr;


        if ((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1)
                FATAL;
        debug("/dev/mem opened.\n");
        fflush(stdout);

        /* Map one page */
        map_base = mmap(0, MAP_SIZE, PROT_READ, MAP_SHARED, fd, target & ~MAP_MASK);
        if (map_base == (void *) -1)
                FATAL;
        debug("Memory mapped at address %p.\n", map_base);
        fflush(stdout);

        virt_addr = map_base + (target & MAP_MASK);
        read_result = *((unsigned int *) virt_addr);
        debug("%x\n", read_result);
        fflush(stdout);


        if (munmap(map_base, MAP_SIZE) == -1)
                FATAL;
        close(fd);
        return read_result;
}
static void write_mem(off_t target, unsigned int writeval){
        int fd;
        void *map_base, *virt_addr;
        if ((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1)
                FATAL;
        debug("/dev/mem opened.\n");
        fflush(stdout);

        /* Map one page */
        map_base = mmap(0, MAP_SIZE, PROT_WRITE, MAP_SHARED, fd, target & ~MAP_MASK);
        if (map_base == (void *) -1)
                FATAL;
        debug("Memory mapped at address %p.\n", map_base);
        fflush(stdout);

        virt_addr = map_base + (target & MAP_MASK);
        *((unsigned int *) virt_addr) = writeval;
        fflush(stdout);


        if (munmap(map_base, MAP_SIZE) == -1)
                FATAL;
        close(fd);
}

