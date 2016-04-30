#!/bin/bash
unzip timings.zip
for i in `ls *.txt`; do ./parse.py $i 16777216 > $i.r; rm -rf $i;done
paste -d, *.r > result
rm -rf *.r

