DE1SOC_BOARD_PACKAGE=./DE1SOC_OPENCL/
KERNEL=trojan
#CROSS_COMPILE=arm-linux-gnueabihf-
CROSS_COMPILE=
GCC=${CROSS_COMPILE}g++
CL=aoc
INCLUDE_FLAGS=$(shell aocl compile-config) -I${DE1SOC_BOARD_PACKAGE}/examples/common/inc
LDFLAGS=$(shell aocl link-config) -L./${KERNEL} -lkernel -lpthread
DBGFLAGS=-Wall -std=c++11 -g -pg -fopenmp 
SRCS=${DE1SOC_BOARD_PACKAGE}/examples/common/src/AOCL_Utils.cpp host/src/myopencl.hpp host/src/main.cpp
cl_compile:
	${CL} -march=emulator -cl-opt-disable  --profile device/${KERNEL}.cl
compile:cl_compile
	${GCC} ${DBGFLAGS} ${INCLUDE_FLAGS} ${SRCS} ${LDFLAGS} -o ${KERNEL}.EXE
sim:
	#module load s5_ref altera/15.1
	LD_LIBRARY_PATH=/opt/altera/14.1/hld/board/s5_ref/linux64/lib:/opt/altera/14.1/hld/host/linux64/lib:./trojan \
	CL_CONTEXT_EMULATOR_DEVICE_ALTERA=1 ./${KERNEL}.EXE  0 268435456 0
	#module unload s5_ref altera/15.1


hw:
	#module unload s5_ref altera/15.1
	#module load de1soc altera/14.1
	#aoc --profile -g --board de1soc_sharedonly -cl-opt-disable  device/trojan.cl
	./hwcompile.sh
	cp ${KERNEL}.EXE ${KERNEL}.aocx bkup/lastbuild
	
	
show:
	avplay -f rawvideo -pixel_format yuyv422 -video_size 544x288 -framerate 30  video4444.out


clean:
	rm -rf *.aocx *.aoco *.EXE *.out ${KERNEL}
copy:
	scp ssh2.enst.fr:/tmp/fdetectbg.EXE  ./
	scp ssh2.enst.fr:/tmp/libkernel.so ./fdetectbg/
