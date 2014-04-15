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
const int depthstartmax = 20000;
const int depthstartmin = 5000;

int dirs[4];
int gcount;
gradient* gradients[100];

int pos(int x, int y) {
  int pos = (width*y+x)*3;
  if (pos<0 || pos>=width*height*3-1) return -1;
  return pos;     
}

void getRgb( int* rgb, double h, double s, double v ) {
 int hi = floor(h / 60.0) ;
            double f = (h / 60.0) - floor(h / 60.0);

            double p = v * (1.0 - s);
            double q = v * (1.0 - (f * s));
            double t = v * (1.0 - ((1.0 - f) * s));

            switch (hi)
            {
                case 0:
                    rgb[0]=v*255;
                    rgb[1]=t*255;
                    rgb[2]=p*255;
                    break;
                case 1:
                    rgb[0]=q*255;
                    rgb[1]=v*255;
                    rgb[2]=p*255;
                    break;
                case 2:
                    rgb[0]=p*255;
                    rgb[1]=v*255;
                    rgb[2]=t*255;
                    break;
                case 3:
                    rgb[0]=p*255;
                    rgb[1]=q*255;
                    rgb[2]=v*255;
                    break;
                case 4:
                    rgb[0]=t*255;
                    rgb[1]=p*255;
                    rgb[2]=v*255;
                    break;
                case 5:
                    rgb[0]=v*255;
                    rgb[1]=p*255;
                    rgb[2]=q*255;
                    break;
                default:
                    rgb[0]=0;
                    rgb[1]=0;
                    rgb[2]=0;
                    break;
            }
            printf("h=%f, s=%f, v=%f, r=%d, g=%d, b=%d", h, s, v, rgb[0], rgb[1], rgb[2]);
}
void createGrads() {
  double po = 0;
  gcount = 0;

  int colors[3];
  
  double h = rand()%255;
  double s = rand()%1000/1000.0;
  double v = rand()%1000/1000.0;

  double vh = rand()%30;
  double vs = rand()%1000/5000.0;
  double vv = rand()%1000/5000.0;

    getRgb(colors,h,s,v);

  while (po<1) { 
  printf(" pos=%f\n", po);
    gradient* grad = malloc( sizeof(gradient) );
    grad->p = po;
    grad->r = colors[0];
    grad->g = colors[1];
    grad->b = colors[2];
    po+=(rand()%1000)/1000.0*0.1+0.1;
    gradients[gcount]=grad;
    gcount++;

    h += vh;
    s += vs;
    v += vv;

    if (h>255) h-=255;
    if (h<0) h+=255;

    if (s>1) {
      s-=vs;
      vs*=-1;
    }

    if (s<0) {
      s-=vs;
      vs*=-1;
    }

    if (v>1) {
      v-=vv;
      vv*=-1;
    }

    if (v<0) {
      v-=vv;
      vv*=-1;
    }
    getRgb(colors,h,s,v);
  }

    gradient* grad = malloc( sizeof(gradient) );
    grad->p = (double) 1.0;
    grad->r = colors[0];
    grad->g = colors[1];
    grad->b = colors[2];

    gradients[gcount]=grad;
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

void colorRec(int un, int ds, unsigned char* imageData, int x, int y, int depth) {
  if (depth<=0 || pos(x,y)==-1 ) return;
  //heatmap(imageData, pos(x,y), (double)depth/ds);
  gradMap( (double) depth/ds, imageData, pos(x,y));
  if ( rand()%un == 0) {
    createDirs();
  }
  for (int crawl=0; crawl<4; crawl++) {
    int dir = dirs[crawl];
    int d = 1;
    if ( dir==0 && imageData[pos(x+d, y+0)+0]==0 ) colorRec(un, ds, imageData, x+d,y+0, depth-1);
    if ( dir==1 && imageData[pos(x+0, y-d)+0]==0 ) colorRec(un, ds, imageData, x+0,y-d, depth-1);
    if ( dir==2 && imageData[pos(x+0, y+d)+0]==0 ) colorRec(un, ds, imageData, x+0,y+d, depth-1);
    if ( dir==3 && imageData[pos(x-d, y+0)+0]==0 ) colorRec(un, ds, imageData, x-d,y+0, depth-1);
  }
}


int main(int argc, char** argv)
{   
  srand ( atoi(argv[2]) );

  int un;
  if (rand()%2==1) {  
    un = rand()%1000;
  } else {
    un = rand()%100;
  }


  createDirs();
    createGrads();

  int sub = 2+rand()%10;
  unsigned char* imageData = malloc(sub*width*height*3);

  for (int r=1; r<sub; r++) {

    int depth = (depthstartmin+(rand()%depthstartmax+1000));
    printf( "sub=%d seed=%d, depth=%d un=%d gcount=%d \n", sub, atoi(argv[2]), depth, un, gcount); 
    colorRec(un, depth, imageData+width*height*3*r, width/2,height/2, depth);
  }
  
  for (int x=0; x<width; x++) {
    for (int y=0; y<width; y++) {
      imageData[pos(x, y)+0] = 128;
      imageData[pos(x, y)+1] = 128;
      imageData[pos(x, y)+2] = 128;
      for (int s=0; s<sub; s++) {

        int r = imageData[pos(x, y)+0] + (imageData[pos(x, y)+0+width*height*3*s] - 60)/2;
        int g = imageData[pos(x, y)+1] + (imageData[pos(x, y)+1+width*height*3*s] - 60)/2;
        int b = imageData[pos(x, y)+2] + (imageData[pos(x, y)+2+width*height*3*s] - 60)/2;
        if (r>255) r = 255;
        if (g>255) g = 255;
        if (b>255) b = 255;
        if (r<0) r = 0;
        if (g<0) g = 0;
        if (b<0) b = 0;

        imageData[pos(x, y)+0] = r;
        imageData[pos(x, y)+1] = g;
        imageData[pos(x, y)+2] = b;
      }
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

