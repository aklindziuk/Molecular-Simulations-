#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void *init (double *rx, double * ry, double *rz, int n, double L){

//index
int i,ix,iy,iz;
ix = iy = iz = 0;

//side length of cube of our particles in terms of 'units'
int n3 = 2;
while ((n3*n3*n3) < n) n3++;

// define 'unit'
double u;
u = L/n3; //1.259

//distribute particles on simple cubic lattice in a cube
for (i = 0; i < n; i ++) {
  rx[i] = ((double)ix + 0.5)*u;
  ry[i] = ((double)iy + 0.5)*u;
  rz[i] = ((double)iz + 0.5)*u;
  ix ++;
  if (ix == n3) {
    ix = 0;
    iy ++;
    if (iy == n3) {
      iy = 0;
      iz ++;
    }
  }
}
}
