#!/bin/bash -f 
aoc --profile -g --board de1soc_sharedonly -cl-opt-disable  device/trojan.cl&
pid=$!
while [ ! -e /tmp/TROJAN/trojan/system/synthesis/submodules/system_acl_iface.v ]; do 
echo waiting > /dev/null
done
kill -19  $pid
ehco 'stopped aocl'
cp system_acl_iface_cache_user.v /tmp/TROJAN/trojan/system/synthesis/submodules/system_acl_iface.v
ehco 'patched system_acl_iface.v'
kill -18 $pid
ehco 'restarted aocl'
