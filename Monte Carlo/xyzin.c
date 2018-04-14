
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void xyzin (FILE * fp, double * rx, double * ry, double * rz,
	     int N) {

       int i;

for (i=0;i<N;i++) {
  fscanf(fp,"%lf %lf %lf ",&rx[i],&ry[i],&rz[i]);
}
}
