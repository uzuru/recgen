/*
Beware: Ugly C code that crashes depending on input
License: http://creativecommons.org/licenses/by/3.0/
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>   

struct {
  double p;
  int r, g, b;
} typedef gradient;


int gcount;
gradient* gradients[100];


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
                    rgb[0]=255;
                    rgb[1]=0;
                    rgb[2]=255;
                    break;
            }
            //printf("h=%f, s=%f, v=%f, r=%d, g=%d, b=%d", h, s, v, rgb[0], rgb[1], rgb[2]);
}
void createGrads() {
  double po = 0;
  gcount = 0;

  int colors[3];
  
  double h = rand()%255;
  double s = rand()%1000/1000.0;
  double v = rand()%1000/1000.0;

  double vh = rand()%(rand()%100);
  double vs = rand()%1000/3000.0;
  double vv = rand()%1000/3000.0;

    getRgb(colors,h,s,v);

  while (po<1) { 
  //printf(" pos=%f\n", po);
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
  for (int g=1; g<gcount; g++) {

    if ( gradients[g]->p > d ) {

      int red = (double) gradients[g-1]->r * (1.0-d) + (double) gradients[g]->r * d;
      int gre = (double) gradients[g-1]->g * (1.0-d) + (double) gradients[g]->g * d;
      int blu = (double) gradients[g-1]->b * (1.0-d) + (double) gradients[g]->b * d;

      //printf("d= %lf\n", gradients[g-1]->r * (1.0-d) + (double) gradients[g]->r * d);

      imageData[pos+0] = red;
      imageData[pos+1] = gre;
      imageData[pos+2] = blu;

      return;    
    }
  }
}
