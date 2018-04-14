
	 #include <stdio.h>
	 #include <stdlib.h>
	 #include <math.h>
	 #include <gsl/gsl_rng.h>
	 #include <gsl/gsl_randist.h>
	 #include <string.h>

void *init ( double * rx, double * ry, double * rz,
	    double * vx, double * vy, double * vz,
	    int n, double L, gsl_rng * r, double T0,
	    double * KE) {

  int i,iix,iiy,iiz;
  double cmvx=0.0,cmvy=0.0,cmvz=0.0;
  double T, factor;
  int n3=2;

    while ((n3*n3*n3)<n) n3++;

    iix=iiy=iiz=0;

		//assign particles on a grid
    for (i=0;i<n;i++) {
      rx[i] = ((double)iix+0.5)*L/n3;
      ry[i] = ((double)iiy+0.5)*L/n3;
      rz[i] = ((double)iiz+0.5)*L/n3;
      iix++;
      if (iix==n3) {
	iix=0;
	iiy++;
	if (iiy==n3) {
	  iiy=0;
	  iiz++;
	}
      }
    }

    for (i=0;i<n;i++) {
			vx[i]=0.5-gsl_rng_uniform(r);
			vy[i]=0.5-gsl_rng_uniform(r);
			vz[i]=0.5-gsl_rng_uniform(r);
    }

 //get rid of center of mass drift
  for (i=0;i<n;i++) {
    cmvx+=vx[i];
    cmvy+=vy[i];
    cmvz+=vz[i];
  }
  (*KE)=0;
  for (i=0;i<n;i++) {
    vx[i]-=cmvx/n;
    vy[i]-=cmvy/n;
    vz[i]-=cmvz/n;
    (*KE)+=vx[i]*vx[i]+vy[i]*vy[i]+vz[i]*vz[i];
  }
  (*KE)*=0.5;
  // scale velocities according to given temperature
  if (T0>0.0) {
    T=(*KE)/n*2./3.;
    factor=sqrt(T0/T);
    (*KE)=0;
    for (i=0;i<n;i++) {
      vx[i]*=factor;
      vy[i]*=factor;
      vz[i]*=factor;
      (*KE)+=vx[i]*vx[i]+vy[i]*vy[i]+vz[i]*vz[i];
    }
    (*KE)*=0.5;
  }

}
