
* there is a IP (systemverilog) hammer.sv which does the rowhammering
  it is connected in 
	DE1SOC_OPENCL/de1soc_sharedonly/iface/acl_iface_system.qsys

* This ip is visible from opencl kernel from port kernel_mem1 at 
  address 0x40000000.
  for rowhammering I need to write the following registers:
	
	0x40000004  :  START HAMMERING
	0x40000010 :  HAMMER ADDR START
	0x40000014 :  HAMMER ADDR END
	0x40000018 :  TIMER

* please see the figure opencl.pdf in current directory
  you can open the design by qsys-edit DE1SOC_OPENCL/de1soc_sharedonly/iface/acl_iface_system.qsys
  - CDC_MEM0 corresponds to clock_cross_kernel_mem0 in acl_iface_system.qsys
  - CDC_MEM1 corresponds to clock_cross_kernel_mem1 in acl_iface_system.qsys
	


* this program uses a stange way of writing to registers to fool 
  the opencl compiler. otherwise the compiler will give errors 
  or optimize away the code.

* the idea is we take a dummy array, and then walk past the array 
  to write to our desired address.
  e.g to write to 0x40000004:
	- assume the dummy array is allocated at 0x28000000 (allocated by ocl compiler)
	- somehow we will give an index to this array so that the final write will be 
	  at address 0x40000004
	- offset=0x40000000-(int)test;    offset is  now 0x18000000
	- test[(offset+0x4)/4] now points to 0x40000004	
		- 0x28000000+0x18000000+0x4
		- divide by 4 is used since it is an int (4 byte) array  compiler will mulitply by 4 
		  to calculaet the address.

