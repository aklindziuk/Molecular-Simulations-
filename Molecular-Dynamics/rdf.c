/*
Post processing code to find the radial distribution function

use: ./rdf -for <start>,<stop>,<fSamp> - rho <rho>
where <start> = index of first trajectory file
      <stop> = index of last trajectory FILE
      <fSamp> = sampling frequency in the MD simulation, index Increment
example: ./rdf -for 0,4900,100 - rho 0.5
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int xyz_in (FILE * fp, double * rx, double * ry, double * rz,
	     int * N) {
  int i;
  for (i=0;i<(*N);i++) {
    fscanf(fp,"%lf %lf %lf ",&rx[i],&ry[i],&rz[i]);
  }
}

void histogram ( double * rx, double * ry, double * rz,
		   int N, double L, double rc2, double dr, int * H ) {

   int i,j;
   double dx, dy, dz, r2;
   int bin;
   double hL = L/2;

   for (i=0;i<(N-1);i++) {
     for (j=i+1;j<N;j++) {
       dx  = (rx[i]-rx[j]);
       dy  = (ry[i]-ry[j]);
       dz  = (rz[i]-rz[j]);
       if (dx>hL)       dx-=L;
       else if (dx<-hL) dx+=L;
       if (dy>hL)       dy-=L;
       else if (dy<-hL) dy+=L;
       if (dz>hL)       dz-=L;
       else if (dz<-hL) dz+=L;
       r2 = dx*dx + dy*dy + dz*dz;
       if (r2<rc2) {
	 bin=(int)(sqrt(r2)/dr);
	 H[bin]+=2;
       }
     }
   }
}

int main ( int argc, char * argv[] ) {

  double * rx, * ry, * rz;
  int N=216, nB=0; // number of partilces and bins
  int * H; //histogram
  double L=0.0, V=0.0;
  double dr=0.1, r;
  double rc2 = 10.0, rho=0.5;
  int start=0,stop=0,step=1,nTraj=0;
  int i;

  char * fname;
  fname = "./data/%i.xyz";
  FILE * fp;
  char fn[35];

  // set up command line input arguments
  for (i=1;i<argc;i++) {
    if (!strcmp(argv[i],"-dr")) dr=atof(argv[++i]); // bin width
    else if (!strcmp(argv[i],"-for")) sscanf(argv[++i],"%i,%i,%i",&start,&stop,&step);
    else if (!strcmp(argv[i],"-rho")) rho=atof(argv[++i]);
    }

  //find the number of bins
  nB = (int)(rc2/dr) + 1;
  H = (int*)calloc(nB,sizeof(int));
  rc2*=rc2;

 //position arrays
  rx = (double*)malloc(N*sizeof(double));
  ry = (double*)malloc(N*sizeof(double));
  rz = (double*)malloc(N*sizeof(double));

  nTraj=0; //number of files

	//if old rdf.dat file is there, remove it
	if(out=fopen("./data/energy.dat", "r" )){
		remove("./data/rdf.dat");
		fprintf(stdout, "file removed");
	}

	//for each trajectory file
  int s;
  for (s=start;s<stop+1;s+=step) {
    sprintf(fn,fname,s);
    fp=fopen(fn,"r");
    if (fp) {
      nTraj++;
      xyz_in(fp,rx,ry,rz,&N);
      fclose(fp);
      if (i==start) {
	       L = pow((N/rho),0.3333333);
	       if (sqrt(rc2)>L/2){
					 //if cutoff is too large, reassign
	          rc2 = L/2;
	          rc2 *= rc2;
	          }
      }

			//update histogram
      histogram(rx,ry,rz,N,L,rc2,dr,H);
    }
  }

  for (i=0;i<nB;i++) {
    r=dr*(i+0.5);
    sprintf(fn, "./data/rdf.dat");
    fp = fopen(fn, "a");
    fprintf(fp,"%.4lf %.4lf\n",i*dr,(double)(H[i])/(4*N*nTraj*r*r*rho*dr));
    fclose(fp);
  }
}
