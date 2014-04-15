#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>       /* time */

const int width = 2000;
const int height = 2000;
const int depthstartmax = 10000;
const int depthstartmin = 3000;

int dirs[4];

int pos(int x, int y) {
  int pos = (width*y+x)*3;
  if (pos<0 || pos>=width*height*3-1) return -1;
  return pos;     
}

void heatmap(int depthstart, unsigned char* imageData, int pos, double d) {
  d*=5;
  if (d<1.0) {
    imageData[pos+0] = 1;  
    imageData[pos+1] = 0;  
    imageData[pos+2] = 255*d;  
  } else if (d<2.0) {
    imageData[pos+0] = 1+254*d;  
    imageData[pos+1] = 0;  
    imageData[pos+2] = 255;  
  } else if (d<3.0) {
    imageData[pos+0] = 255;  
    imageData[pos+1] = 0;  
    imageData[pos+2] = 255*(1-d);  
  } else if (d<4.0) {
    imageData[pos+0] = 255;  
    imageData[pos+1] = 255*d;  
    imageData[pos+2] = 0;  
  } else {
    imageData[pos+0] = 255;  
    imageData[pos+1] = 255;  
    imageData[pos+2] = 255*d;  
  }
}

int* createDirs() {
  int p = 0;
  while (p<4) {
    int d = rand()%4; 
    int exists = 0;    
    for (int p2=0; p2<p; p2++) {
      if (dirs[p2]==d) exists=1;
    }
    if (!exists) {
      dirs[p] = d;
      p++;
    }
  }
  return dirs;
}

void colorRec(int un, int ds, unsigned char* imageData, int x, int y, int depth, int* dirs) {
  if (depth<=0 || pos(x,y)==-1 ) return;
  heatmap(ds, imageData, pos(x,y), (double) depth/ds);
  
  if ( rand()%un == 1) dirs = createDirs();
  for (int crawl=0; crawl<4; crawl++) {
    int dir = dirs[crawl];
    if ( dir==0 && imageData[pos(x+1, y+0)+0]==0 ) colorRec(un, ds, imageData, x+1,y+0, depth-1, dirs);
    if ( dir==1 && imageData[pos(x+0, y-1)+0]==0 ) colorRec(un, ds, imageData, x+0,y-1, depth-1, dirs);
    if ( dir==2 && imageData[pos(x+0, y+1)+0]==0 ) colorRec(un, ds, imageData, x+0,y+1, depth-1, dirs);
    if ( dir==3 && imageData[pos(x-1, y+0)+0]==0 ) colorRec(un, ds, imageData, x-1,y+0, depth-1, dirs);
  }
}


int main(int argc, char** argv)
{   
  srand ( atoi(argv[2]) );
  unsigned char* imageData = malloc(width*height*3);
  int un;
  if (rand()%2==1) {  
    un = rand()%1000;
  } else {
    un = rand()%100;
  }
  int depth = depthstartmin+(rand()%depthstartmax+1000);
  printf( "depth=%d un=%d\n", depth, un); 
  colorRec(un, depth, imageData, width/2,height/2, depth, createDirs());
  FILE * fp;  
	fp = fopen(argv[1], "wb");   //mode = write binary    
  fprintf(fp, "P6\n%d\n%d\n%d\n", width, height, 255 ); //write pnm header
  fwrite(imageData, width*height*3, 1, fp);  //write array to file pointer
  fclose(fp);  
  free(imageData);
  return 0;
}

