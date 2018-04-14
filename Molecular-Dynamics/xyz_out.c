/*
outputs unfolded positions into xyz file
should be vmd compatible
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>


void *xyz_out (FILE * fp,
	      double * rx, double * ry, double * rz,
	      double * vx, double * vy, double * vz,
	      int * ix, int * iy, int * iz, double L,
	      int N) {
  int i;


  for (i=0;i<N;i++) {
    fprintf(fp,"%.8lf %.8lf %.8lf \n",
	    rx[i]+ix[i]*L,
	    ry[i]+iy[i]*L,
	    rz[i]+iz[i]*L);
}
}
