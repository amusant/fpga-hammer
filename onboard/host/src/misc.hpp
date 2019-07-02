#include <iostream>     
#include <stdio.h>     
#include <stdlib.h>     
#include <fstream>
#include <thread>
#include <chrono>

static void create_template(char *filename, int x_top_right, int y_top_right,int x_bottom_left,int y_bottom_left){
std::ifstream is (filename, std::ifstream::binary);
std::ofstream of ("video4444.out", std::ofstream::binary|std::ofstream::app);
unsigned int y_offset=(y_top_right-1);
unsigned int x_offset=(x_top_right-1);
unsigned int height=y_bottom_left-y_top_right;
unsigned int width=x_bottom_left-x_top_right;
unsigned int *line=(unsigned int *)malloc(sizeof(unsigned int)*width/2);
unsigned int *lineout=(unsigned int *)malloc(sizeof(unsigned int)*width/2);
unsigned char templatey[height][width];
unsigned char templateu[height][width];
unsigned char  templatev[height][width];
unsigned int pixel;
unsigned int i,j;
//for (i=0;i<32;i++){
for (i=0;i<height;i++){
		if(is) { is.seekg((y_offset+i)*COLS*2+(x_offset)*2);
		is.read ((char *)(line),width*2);
		}
		//for (j=0;j<17;j++){
		for (j=0;j<width/2;j++){
			templatey[i][2*j]=line[j] & 0xff;
			templateu[i][2*j]=(line[j] >> 8) & 0xff;
			templatey[i][2*j+1]=(line[j] >> 16) & 0xff;
			templatev[i][2*j]=(line[j] >> 24) & 0xff;
		}
}
for (i=0;i<height;i++){
		for (j=0;j<width;j++)
		printf("%c ",templatey[i][j]);
		//cout << templatey[i][j] << " ";
		cout << endl;
}
for (i=0;i<height;i++){
		if(is) { is.seekg((y_offset+i)*COLS*2+(x_offset)*2);
		is.read ((char *)(line),width*2);
		}
		for (j=0;j<width/2;j++){
			pixel=templatev[i][2*j];
			pixel=(pixel << 8) |templatey[i][2*j+1];
			pixel=(pixel << 8) |templateu[i][2*j];
			pixel=(pixel << 8) |templatey[i][2*j];
			line[j]=pixel;
		}
		if(of) of.write((char *)(line),width*2);
}
free(line);
}	
