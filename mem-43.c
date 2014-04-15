#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>       /* time */

struct {
  double p;
  int r, g, b;
} typedef gradient;


const int width = 2000;
const int height = 2000;
const int depthstartmax = 10000;
const int depthstartmin = 3000;

int dirs[4];
int gcount;
gradient* gradients[100];


int pos(int x, int y) {
  int pos = (width*y+x)*3;
  if (pos<0 || pos>=width*height*3-1) return -1;
  return pos;     
}

void createGrads() {
  double po = 0;
  gcount = 0;

  int r = rand()%255;
  int g = rand()%255;
  int b = rand()%255;

  int vr = rand()%50-25;
  int vg = rand()%50-25;
  int vb = rand()%50-25;


  while (po<1) {
    gradient* grad = malloc( sizeof(gradient) );
    grad->p = po;
    grad->r = r;
    grad->g = g;
    grad->b = b;
    po+=(rand()%1000)/1000.0*0.1+0.1;
    gradients[gcount]=grad;
    gcount++;

    r+=vr;
    g+=vg;
    b+=vb;
/*
    if (r>255) r=255;
    if (r<0) r=0;

    if (g>255) g=255;
    if (g<0) g=0;

    if (b>255) b=255;
    if (b<0) b=0;
*/
    vr += rand()%50-25;
    vg += rand()%50-25;
    vb += rand()%50-25;

  }

    gradient* grad2 = malloc( sizeof(gradient) );
    grad2->p = (double) 1.0;
    grad2->r = r;
    grad2->g = g;
    grad2->b = b;

    gradients[gcount]=grad2;
    gcount++;
}

void gradMap(double d, unsigned char* imageData, int pos) {
  for (int g=0; g<gcount; g++) {

    if ( gradients[g]->p > d ) {

      int red = 1+gradients[g-1]->r * (1-d) + gradients[g]->r * d;
      int gre = gradients[g-1]->g * (1-d) + gradients[g]->g * d;
      int blu = gradients[g-1]->b * (1-d) + gradients[g]->b * d;

      imageData[pos+0] = red;
      imageData[pos+1] = gre;
      imageData[pos+2] = blu;

      return;    
    }
  }
}

void heatmap( unsigned char* imageData, int pos, double d) {
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

void colorRec(int un, int ds, unsigned char* imageData, int x, int y, int depth) {
  if (depth<=0 || pos(x,y)==-1 ) return;
  //heatmap(imageData, pos(x,y), (double) depth/ds);
  gradMap( (double) depth/ds, imageData, pos(x,y));
  if ( rand()%un == 1) createDirs();
  for (int crawl=0; crawl<4; crawl++) {
    int dir = dirs[crawl];
    if ( dir==0 && imageData[pos(x+1, y+0)+0]==0 ) colorRec(un, ds, imageData, x+1,y+0, depth-1);
    if ( dir==1 && imageData[pos(x+0, y-1)+0]==0 ) colorRec(un, ds, imageData, x+0,y-1, depth-1);
    if ( dir==2 && imageData[pos(x+0, y+1)+0]==0 ) colorRec(un, ds, imageData, x+0,y+1, depth-1);
    if ( dir==3 && imageData[pos(x-1, y+0)+0]==0 ) colorRec(un, ds, imageData, x-1,y+0, depth-1);
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

  createDirs();
  createGrads();

  printf( "depth=%d un=%d gcount=%d \n ", depth, un, gcount); 


 colorRec(un, depth, imageData, width/2,height/2, depth);
  for (int x=0; x<width; x++) {
    for (int y=0; y<width; y++) {
      if (imageData[pos(x, y)+0]==0) gradMap( 0, imageData, pos(x,y));
    }
  } 

  FILE * fp;  
	fp = fopen(argv[1], "wb");   //mode = write binary    
  fprintf(fp, "P6\n%d\n%d\n%d\n", width, height, 255 ); //write pnm header
  fwrite(imageData, width*height*3, 1, fp);  //write array to file pointer
  fclose(fp);  
  free(imageData);
  return 0;
}

