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

	- boot with the sdcard in DE1SoC board.


# copy/clone the onboard directory in DE1SOC
	- run  ./compile.sh to compile the host executable trojan.EXE
	- copy trojan.aocx to onboard directory
	- run make to start teh experiments


# note 
	- calls rowhammer.py 
	- inside rowhammer.py you can set Activation Interval, Refresh Interval and address range.
    - 512MB of memory is reserved away from the linux kernel on which we do rowhammer experiments.
		- this to avoid rowhammering a working linux memory which will result in freeze.