#!/bin/bash 
if ! [ -e /tmp/rowhammer_bkup ]
then cp -r /root/trojan_work/rowhammer_bkup /tmp
fi
cd /tmp/rowhammer_bkup
make all &> cluster.log


