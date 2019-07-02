HOSTS=$(shell cat hosts.txt)
N_HOSTS=$(shell cat hosts.txt | wc -l )
JOBS=$(patsubst %,%.job,${HOSTS})
CLEANS=$(patsubst %,%.clean,${HOSTS})
PLOTS=$(patsubst %,%.plot,${HOSTS})

launch:
	$(MAKE) -j ${N_HOSTS} ${JOBS}
#copy_scr:
#	for i in `cat hosts.txt` ; do sshpass -f ./passwd scp ./cluster.sh root@$$i:/tmp; done
%.job:
	sshpass -f ./passwd scp ./cluster.sh root@$*:/tmp
	sshpass -f ./passwd ssh root@$* "nohup bash /tmp/cluster.sh "

clean:
	$(MAKE) -j ${N_HOSTS} ${CLEANS}
%.clean:
	sshpass -f ./passwd scp ./cluster_clean.sh root@$*:/tmp
	sshpass -f ./passwd ssh root@$* "nohup bash /tmp/cluster_clean.sh "

plot:
	$(MAKE) -j ${N_HOSTS} ${PLOTS}
%.plot:
	mkdir -p $*
	sshpass -f ./passwd scp  root@$*:/tmp/rowhammer_bkup/*.result $*/ 
	sshpass -f ./passwd scp  root@$*:/tmp/rowhammer_bkup/*.diff $*/ 
	#sshpass -f ./passwd scp  root@$*:/tmp/rowhammer_bkup/*.diff $*/ 
	echo "all:\n\tpython ../plot.py" >  $*/Makefile
	make -C $*
