#!/usr/bin/Rscript
z<-matrix(0,262144,33)
for (i in 1:16){
 x<-read.table(paste("./del_addr",i,sep=""))
 y<-read.table(paste("./del_no_addr",i,sep=""))
 z[,2*i]<-x[,2]
 z[,2*i+1]<-y[,2]
}
z[,1]<-x[,1]

up<-matrix(0,262144,16)
for (i in 1:262144){
 for (j in 1:16){
  if((z[i,2*j+1]> 2*z[i,2*j])) {
	up[i,j]<-1
  }
 }
}
down<-matrix(0,262144,15)
for (i in 1:262144){
 for (j in 1:15){
  if((z[i,2*j+1]<0.5*z[i,2*j+2])) {
	down[i,j]<-1
  }
 }
}
result<-matrix(0,262144,1)
for (i in 1:262144){
 s<-0
 for (j in 2:16){
  s=s+up[i,j]
 }
 for (j in 2:15){
  s=s+down[i,j]
 }
 result[i,1]<-s
}
#prints the address, max max-1 max -2
for (i in x[row(result)[result==max(result)],1]) print(i)
for (i in x[row(result)[result==max(result)-1],1]) print(i)
for (i in x[row(result)[result==max(result)-2],1]) print(i)
