/*
Beware: Ugly C code that crashes depending on input
License: http://creativecommons.org/licenses/by/3.0/
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>       
#include <string.h>
#include "gradient.c"

const int width = 2000;
const int height = 1300;
const int depthstartmax = 50000;
const int depthstartmin = 1000;

int dirs[4];

int dpos(int x, int y) {
  int pos = (width*y+x);
  if (pos<0 || pos>=width*height) return -1;
  return pos;     
}

int pos(int x, int y) {
  int pos = (width*y+x)*3;
  if (pos<0 || pos>=width*height*3-1) return -1;
  return pos;     
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

void bwmap( unsigned char* imageData, int pos, int v) {
    imageData[pos+0] = v+1;  
    imageData[pos+1] = v+1;  
    imageData[pos+2] = v+1;  
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


void rec(int un, int ds, int* data, int x, int y, int depth) {
  if (depth<=0 || dpos(x,y)==-1 ) return;

  //printf("x=%d, y=%d, dpos=%d, data=%d \n", x,y,dpos(x,y), data[dpos(x+0, y+0)]);
  
  data[dpos(x+0, y+0)] = depth;

  if ( rand()%un == 0) {
    createDirs();
  }

  for (int crawl=0; crawl<4; crawl++) {
    int dir = dirs[crawl];
    int d = 1;
    if ( dir==0 && data[dpos(x+d, y+0)+0]==0 ) rec(un, ds, data, x+d,y+0, depth-1);
    if ( dir==1 && data[dpos(x+0, y-d)+0]==0 ) rec(un, ds, data, x+0,y-d, depth-1);
    if ( dir==2 && data[dpos(x+0, y+d)+0]==0 ) rec(un, ds, data, x+0,y+d, depth-1);
    if ( dir==3 && data[dpos(x-d, y+0)+0]==0 ) rec(un, ds, data, x-d,y+0, depth-1);
  }
}


int main(int argc, char** argv)
{   

  int* data = malloc(width*height*sizeof(int));

  srand ( atoi(argv[2]) );
  int un;
  if (rand()%2==1) {  
    un = rand()%1000;
  } else {
    un = rand()%100;
  }
  int depth = depthstartmin+(rand()%depthstartmax);
  //int sub = rand()%(rand()%10);
  
  //printf( "data=%lu sub=%d seed=%d, depth=%d un=%d gcount=%d \n", width*height*sizeof(short), sub, atoi(argv[2]), depth, un, gcount); 

  createDirs();


  if (rand()%3==0) {
    int de = rand()%depth;
    int asdf = rand()%50;
    int sc = rand() % (rand() % 20);
  for (int s=0; s<sc; s++) {
    
    int x = rand()%width;
    int y = rand()%height;
    int le = rand()%(rand()%100000);
    int vx = rand()%3-1;      
    int vy = rand()%3-1;      
    for (int l=0; l<le; l++) {
      if ( dpos(x,y) == -1 ) continue;  
      data[dpos(x,y)] = de;
      if ( rand()%asdf == 0) {
        vx = rand()%3-1;  
        vy = rand()%3-1;  
      }
      x+= vx;
      y+= vy;    
    }
  }
  }
 
  rec(un, depth, data, rand()%width,rand()%height, depth); 
 while (rand()%2) {
  rec(un, depth, data, rand()%width,rand()%height, depth);
 }

  if ( rand()%3==0 ) {
  int dx=rand()%200;
  int dy=rand()%200;
 for (int x=dx; x<width-dx; x++) {
  for (int y=dy; y<width-dy; y++) {
      data[dpos(x,y)] = (data[dpos(x+dx,y)]+data[dpos(x-dx,y)]+data[dpos(x,y+dy)]+data[dpos(x,y-dy)] ) / 4;
    }
  }

  }
  if ( rand()%3==0 ) {
  int q=rand()%6;
 for (int x=0; x<width; x++) {
  for (int y=0; y<height; y++) {
    if ( q == 0) {
        data[dpos(x,y)] += (int)((double)x/width*(double)depth);
      } else if ( q == 1) {
        data[dpos(x,y)] -= (int)((double)x/width*(double)depth);
      } else if ( q == 2) {
        data[dpos(x,y)] += (int)((double)y/height*(double)depth);
      } else if ( q == 3) {
        data[dpos(x,y)] -= (int)((double)y/height*(double)depth);
      } else if ( q == 4) {
        data[dpos(x,y)] += (int)( (0.5-abs(y-height/2)/(double)height)*(0.5-abs(x-width/2)/(double)width)*depth );
      }
       //data[dpos(x,y)] =  data[dpos(x,y)] % depth;        
      //data[dpos(x,y)] += (int)((double)y/height*(double)depth)*dy;
    }
  }
  }
  
  if ( rand()%10==0 ) {
  int noise = rand()%(rand()%(rand()%(rand()%depth)));
  for (int x=0; x<width; x++) {
  for (int y=0; y<height; y++) {

    data[dpos(x,y)] += rand()%noise;
  }
  }
}

float sinstrength=(float)(rand()%(rand()%10000))/10000;
int sinflags = rand()%8;
int repeat=(rand()%(rand()%20));
  if ( rand()%3==0 ) {
 for (int x=0; x<width; x++) {
  for (int y=0; y<height; y++) {
    data[dpos(x,y)] += (int)(sin((float)x/width*repeat)*depth * (sinflags&0b001?1:0) * sinstrength);
    data[dpos(x,y)] += (int)(sin((float)y/height*repeat)*depth * (sinflags&0b010?1:0) * sinstrength);
    data[dpos(x,y)] += (int)(tan((float)y/height*repeat)*depth * (sinflags&0b100?1:0) * sinstrength);
    }
  } 
}

if (rand()%4==0) {
int* temp = malloc(width*height*sizeof(int));
int blur=rand()%(rand()%(rand()%width/20)); 
 for (int x=0; x<width-blur; x++) {
   //printf("blurring %i\n", x);
  for (int y=0; y<height-blur; y++) {
     int blurval=0;
     double blurcount=0;
     for (int x2=0; x2<blur; x2++) {
      for (int y2=0; y2<blur; y2++) {
        double f = 1.0/(1+sqrt((x2-blur/2)*(x2-blur/2)+(y2-blur/2)*(y2-blur/2)));
        blurval+=data[dpos(x+x2,y+y2)]*f;
        blurcount+=f;
      }
    }
    blurval = blurval / blurcount;
    temp[dpos(x+blur/2,y+blur/2)] = blurval;
  } 
}
memcpy(data,temp,width*height*sizeof(int));
free(temp);
}

  createGrads();
  unsigned char* imageData = malloc(width*height*3);

 for (int x=0; x<width; x++) {
  for (int y=0; y<width; y++) { 
      gradMap( (double) (data[dpos(x,y)] % depth )/depth, imageData, pos(x,y));
    }
  }

  FILE * fp;  
  fp = fopen(argv[1], "wb");   //mode = write binary    
  fprintf(fp, "P6\n%d\n%d\n%d\n", width, height, 255 ); //write pnm header
  fwrite(imageData, width*height*3, 1, fp);  //write array to file pointer
  fclose(fp);  
  free(imageData);
  free(data);
  return 0;
}

