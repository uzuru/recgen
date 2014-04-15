#include <math.h>
#include <stdio.h>
#include <stdlib.h>

const int width = 2000;
const int height = 2000;

int pos(int x, int y) {
  if (x<0) x+=width;  
  if (x>width) x-=width;
  
  if (y<0) y+=height;  
  if (y>width) y-=height;
  
  return (width*y+x)*3;     
}


void colorRec(unsigned char* imageData, int x, int y, int depth) {
  imageData[pos(x, y)+0] = 255+depth;  
  if (depth==0) return;
  
  for (int crawl=0; crawl<10; crawl++) {
    int dir = rand()%4;
    if ( dir==0 && imageData[pos(x+1,y+0)]==0 ) colorRec(imageData, x+1,y+0, depth-1);
    if ( dir==1 && imageData[pos(x-1,y+0)]==0 ) colorRec(imageData, x-1,y+0, depth-1);
    if ( dir==2 && imageData[pos(x+0,y-1)]==0 ) colorRec(imageData, x+0,y-1, depth-1);
    if ( dir==3 && imageData[pos(x+0,y+1)]==0 ) colorRec(imageData, x+0,y+1, depth-1);
  }
}


int main(int argc, char** argv)
{   
  unsigned char* imageData = malloc(width*height*3); 
  colorRec(imageData, width/2,height/2, 10000);
  FILE * fp;  
	fp = fopen(argv[1], "wb");   //mode = write binary    
  fprintf(fp, "P6\n%d\n%d\n%d\n", width, height, 255 ); //write pnm header
  fwrite(imageData, width*height*3, 1, fp);  //write array to file pointer
  fclose(fp);  
  return 0;
}

