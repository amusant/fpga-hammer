library( HilbertVisGUI )
x<-read.table("./del")
hilbertDisplay( (x[,2]-mean(x[,2]))/(sd(x[,2])))
