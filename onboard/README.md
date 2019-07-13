# after cloning 
	- unzip DE1SOC_OPENCL.zip which is the board package
	- set following env. variables

	export INSTALL_DIR=/.....<your altera installation path>...../altera/altera14.1/
	export SOCEDS_DEST_ROOT=$INSTALL_DIR/embedded
	export  QUARTUS_ROOTDIR=$INSTALL_DIR/quartus
	export  SOCEDS_DEST_ROOT=$INSTALL_DIR/embedded
	export  ALTERAOCLSDKROOT=$INSTALL_DIR/hld
	export  PATH=$INSTALL_DIR/quartus/bin:$PATH
	export  PATH=$INSTALL_DIR/quartus/sopc_builder/bin:$PATH
	export  PATH=$INSTALL_DIR/quartus/dsp_builder:$PATH
	export  PATH=$SOCEDS_DEST_ROOT/host_tools/altera/preloadergen/:$PATH
	export  PATH=$INSTALL_DIR/hld/bin:$PATH

	export AOCL_BOARD_PACKAGE_ROOT=`pwd`/DE1SOC_OPENCL

	- run make hw 
	- this will generate trojan.aocx
	

# I have only tested for altera 14.1, you can find the sdcard image here

	https://www.dropbox.com/s/xepcgjl1uqpf19f/de1soc.img.tar.gz?dl=0

	- boot with the sdcard in DE1SoC board.
	- MSEL setting should be 01010



# use with signaltap and single exp.
	- copy/clone the onboard directory in DE1SOC
	- check " #define SLEEPTIME_FOR_STP 30 " in lien 12 in host/src/main.cpp
		- it means the program will wait 30 seconds for you to setup signaltap
	- run  ./compile.sh to compile the host executable trojan.EXE
	- run make test to see if it finishes without err.
		-it will generate 
			memimage.txt.before : mem values 1MB before rowhammer
			memimage.txt.after  : mem values 1MB after rowhammer
			test.0x2de.diff     : diff of the above to files
			test.result         : count of errors
	- launch signaltap
		- quartus trojan/trojan.stp
		- assure the 5CSEMA device is chosen in the device pane.
	- launch make test, after it prints "start sleep" go to signaltap
	  window, start acquisition (autorun)
	- you should see signals VALID READY toggling.
	- the difference between each pulse of A4LMPORT_ARREADY  is the 
	  activation interval.
		

# copy/clone the onboard directory in DE1SOC
	- run  ./compile.sh to compile the host executable trojan.EXE
	- copy trojan.aocx to onboard directory
	- run make to start the experiments


# note 
	- make calls rowhammer.py 
	- inside rowhammer.py you can set Activation Interval, Refresh Interval and address range.
    - 512MB of memory is reserved away from the linux kernel on which we do rowhammer experiments.
		- this to avoid rowhammering a working linux memory which will result in freeze.
	- modify launch_a_test function in rowhammer.py
		ADDR: The start address. We will rowhammer a zone of 1MB starting from this address.
		AI:   Activation interval fixed. 45 ns
		RI:   Refresh interval in miliseconds, 
		Pattern: 

	The rowhammer.py calls rowhammer.sh which call the script run.


	The script rowhammer.sh does the following:
	
	1. initialize the memory zone with a pattern.
	2. read the memory zone and save a copy
	3. run rowhammer 
	4. read back the smae memory zone.
	5. show and log the differences.


# To Upgrade to a newer version:
	- First open the DE1SOC_OPENCL/de1soc_sharedonly/system.qsys with latest version( say 18.1)  
      of qsys-edit:
		$ qsys-edit DE1SOC_OPENCL/de1soc_sharedonly/system.qsys
	
	- generate the hdl from generate menu.

	- exit and open DE1SOC_OPENCL/de1soc_sharedonly/top.qpf  with quartus 18.1
		$ quartus DE1SOC_OPENCL/de1soc_sharedonly/top.qpf

	- It should automatically upgrade IPs and/or give warnings errors. 
	- Compile just to check the that it finishes without error.
    - Now it can be use to compile opencl programs by setting
		$ export AOCL_BOARD_PACKAGE_ROOT=`pwd`/DE1SOC_OPENCL

	- for DE1-SoC terasic provides linux sd card images with driver, runtime library 
      for versions 14.1 and 16.1

	- for later versions  you can  find  a busybox linux image here
		....altera18.1/hld/board/c5soc/linux_sd_card_image.tgz
	- this should work (not tested) but busybox is limited (no python , difficult to install packages)
	- you can also try copying the kernel (zImage) and drivers/librarires to the Terasic 
	  provided sd card (ubuntu).

