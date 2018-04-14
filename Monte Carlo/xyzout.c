

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


void xyzout(FILE * fp, double * rx, double * ry, double * rz,  int n) {
  int i;
  for (i = 0; i < n; i++ ){
		fprintf(fp, "%.5lf %.5lf %.5lf\n", rx[i], ry[i], rz[i]);
	}
}
