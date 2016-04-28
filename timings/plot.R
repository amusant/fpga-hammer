#!/usr/bin/Rscript
args = commandArgs(trailingOnly=TRUE)
pdf(file="result.pdf",height=6,width=8)
x<-read.table(args[1]);
#l=line, p=point s=stair
plot(x[,1],type="p")
to_plot<-density(x[,1],bw=1)
plot(to_plot,main="",xlim=c(0,400),ylim=c(0,0.1),col="red")

