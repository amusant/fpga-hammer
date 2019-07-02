import subprocess
def launch_a_test(ADDR,AI,RI,pattern):
	logfile=str(pattern)+'_'+str(RI)
	reftime_for_row=(RI*1e-3)/8192 #8192 rows
	N_RI=(0x8CCC000|int(reftime_for_row/(1/400e6)))
	print('./rowhammer.sh '+str(ADDR)+' '+str(N_RI)+' '+str(hex(ADDR >> 20))+' '+str(pattern)+ ' '+ logfile)
	subprocess.call('./rowhammer.sh '+str(ADDR)+' '+str(N_RI)+' '+str(hex(ADDR >> 20))+' '+str(pattern)+ ' '+ logfile,shell=True)
	print N_RI


def campaign():
	for k in range(0,1,1):
		for j in range(64,72,8):
			for i in range(0x2de00000,0x40000000,0x100000):
				print("0x%x: " % i)
				launch_a_test(i,45,j,k)
campaign()
#launch_a_test(0x30e00000,45,128,1)
