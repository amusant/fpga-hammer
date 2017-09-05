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
static unsigned int read_mem(off_t target);
static void write_mem(off_t target,unsigned int writeval);

int main(int argc, char *argv[]){
int i;
off_t target_start=strtoul(argv[1],NULL,0);
int target_end=strtoul(argv[2],NULL,0);
printf("%x:%x\n",target_start, target_end);
unsigned int memsize=(target_end-target_start) >> 2;
unsigned int *memimage=malloc(sizeof(unsigned int)*memsize);
for (i=0; i< (target_end-target_start);i=i+4) {
	unsigned int oddrow=(i/0x4000) % 2;
	if (oddrow) write_mem(target_start+i,0xFFFFFFFF);
	else write_mem(target_start+i,0x00000000);
	memimage[i/4]=read_mem(target_start+i);
}
//memcpy(memimage,target_start,128*1024*1024);

FILE *f =fopen("memimage.txt", "w");
fwrite(memimage,sizeof(unsigned int),memsize,f);
fclose(f);
free(memimage);
	
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

