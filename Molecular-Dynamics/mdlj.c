/*
molecular dynamics simulation for particles in a Lennard-Jones potetntial
finds trajectory of the particles...outputs to #.xyz
finds kinetic  and potential energy of the ensemble..outputs to energy.dat
finds pressure of the ensemble..outputs to energy.dat
implements potential cutoff
to compile: make mdlj
to run: ./mdlj -T0 2.0 -rc 2.5 -ns 5000 -fs 100 -rho 0.4
command line options:
-N <num of particles>
-rc <potential cutoff dist>
-ns <number of MD steps>
-T0 <initial temp>
-rho <density>
-T <temperature>
-dt <MD time step sizeof>
-fs <sampling frequency>
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>


double force ( double * rx, double * ry, double * rz,
		 double * fx, double * fy, double * fz,
		 int N, double L, double rc2, double E_cor, double E_cut, double * vir ) {

   int i,j;
   double dx, dy, dz, r2, r6i;
   double e = 0.0, hL=L/2.0,f;
	 *vir=0.0;

	 //zero all forces
   for (i=0;i<N;i++) {
     fx[i]=fy[i]=fz[i]=0.0;
   }

	 //for all nearest neighbors...
   for (i=0;i<(N-1);i++) {
     for (j=i+1;j<N;j++) {
  //find interparticle separations
	dx  = (rx[i]-rx[j]);
	dy  = (ry[i]-ry[j]);
	dz  = (rz[i]-rz[j]);

	// apply boundary conditions
	if (dx>hL) dx-=L;
	else if (dx<-hL) dx+=L;
	if (dy>hL) dy-=L;
	else if (dy<-hL) dy+=L;
	if (dz>hL) dz-=L;
	else if (dz<-hL) dz+=L;

	// get radial separation
	r2 = dx*dx + dy*dy + dz*dz;
	if (r2<rc2) {
		// get potetntial enegy for each pair and force
	  r6i   = 1.0/(r2*r2*r2);
	  e    += 4*(r6i*r6i - r6i) - E_cut;
	  f     = 48*(r6i*r6i-0.5*r6i);
	  fx[i] += dx*f/r2;
	  fx[j] -= dx*f/r2;
	  fy[i] += dy*f/r2;
	  fy[j] -= dy*f/r2;
	  fz[i] += dz*f/r2;
	  fz[j] -= dz*f/r2;
	  *vir += f;
	}
     }
   }
	 //return total PE
   return e+N*E_cor;
}


int main ( int argc, char * argv[] ) {

  double * rx, * ry, * rz; // positions
  double * vx, * vy, * vz; // velocities
  double * fx, * fy, * fz; // forces
  int * ix, * iy, * iz; // boundary crossings counter
  int N=216,c,a;
  double L=0.0;
  double rho=0.5, T=0.0, rc2 = 2.5;
	double vir, virial_old, P_cor, V;
  double PE, KE, TE, E_cor, E_cut, T0=1.0, TE0;
  double rr3,dt=0.001, dt2;
  int i,j,s;
  int nSteps = 10, fSamp=100;

  char fn[20];
  FILE * out;

  gsl_rng * r = gsl_rng_alloc(gsl_rng_mt19937);
  unsigned long int Seed = 23410981;

  for (i=1;i<argc;i++) {
    if (!strcmp(argv[i],"-N")) N=atoi(argv[++i]); // num of particles
		else if (!strcmp(argv[i],"-rc")) rc2=atof(argv[++i]); // potential cutoff dist
		else if (!strcmp(argv[i],"-ns")) nSteps = atoi(argv[++i]); // number of MD steps
		else if (!strcmp(argv[i],"-T0")) T0=atof(argv[++i]); //initial temp
    else if (!strcmp(argv[i],"-rho")) rho=atof(argv[++i]); // density
    else if (!strcmp(argv[i],"-T")) T=atof(argv[++i]); // temperature
    else if (!strcmp(argv[i],"-dt")) dt=atof(argv[++i]); // MD time step sizeof
    else if (!strcmp(argv[i],"-fs")) fSamp=atoi(argv[++i]); // sampling frequency
    else {
      fprintf(stderr,"Error: unknown command line argument '%s'.\n",  argv[i]);
      exit(-1);
    }
  }

// compute tail corrections
  rr3 = 1.0/(rc2*rc2*rc2);
  E_cor = 8*M_PI*rho*(rr3*rr3*rr3/9.0-rr3/3.0); // energy correction
  P_cor = 16.0/3.0*M_PI*rho*rho*(2./3.*rr3*rr3*rr3-rr3); //pressure corection
  E_cut = 4*(rr3*rr3*rr3*rr3-rr3*rr3); //cutoff energy

  rc2*=rc2;
  dt2=dt*dt;
	L = pow((V=N/rho),0.3333333);

  //seed random generator
  gsl_rng_set(r,Seed);

  // allocate position, counter, velocity and force arrays
  rx = (double*)malloc(N*sizeof(double));
  ry = (double*)malloc(N*sizeof(double));
  rz = (double*)malloc(N*sizeof(double));

  ix = (int*)malloc(N*sizeof(int));
  iy = (int*)malloc(N*sizeof(int));
  iz = (int*)malloc(N*sizeof(int));

  vx = (double*)malloc(N*sizeof(double));
  vy = (double*)malloc(N*sizeof(double));
  vz = (double*)malloc(N*sizeof(double));

  fx = (double*)malloc(N*sizeof(double));
  fy = (double*)malloc(N*sizeof(double));
  fz = (double*)malloc(N*sizeof(double));

	memset(ix,0,N*sizeof(int));
	memset(iy,0,N*sizeof(int));
	memset(iz,0,N*sizeof(int));

	// initialize on a lattice and assign velocities
  init(rx,ry,rz,vx,vy,vz,N,L,r,T0,&KE);
  sprintf(fn,"./data/%i.xyz",0);
  out=fopen(fn,"w");
  xyz_out(out,rx,ry,rz,vx,vy,vz,ix,iy,iz,L,N);
  fclose(out);
	//compute forces and potentail energy
  PE = force(rx,ry,rz,fx,fy,fz,N,L,rc2,E_cor,E_cut,&virial_old);
  TE0=PE+KE;

  //if old energy.dat file is there, remove it
	if(out=fopen("./data/energy.dat", "r" )){
 	 remove("./data/energy.dat");
  }
	
  for (s=0;s<nSteps;s++) {
    //Velocity verlet
		//fist half step
    for (i=0;i<N;i++) {
      rx[i]+=vx[i]*dt+0.5*dt2*fx[i];
      ry[i]+=vy[i]*dt+0.5*dt2*fy[i];
      rz[i]+=vz[i]*dt+0.5*dt2*fz[i];
      vx[i]+=0.5*dt*fx[i];
      vy[i]+=0.5*dt*fy[i];
      vz[i]+=0.5*dt*fz[i];
			//boundary conditions with a boundary crossing counter ix, iy, iz
      if (rx[i]<0.0) {
				rx[i]+=L; ix[i]--;
			}
      if (rx[i]>L) { rx[i]-=L; ix[i]++;
		  }
      if (ry[i]<0.0) {
				ry[i]+=L; iy[i]--;
		  }
      if (ry[i]>L)   {
			  ry[i]-=L; iy[i]++;
			}
      if (rz[i]<0.0) {
				 rz[i]+=L; iz[i]--;
			}
      if (rz[i]>L)   {
				 rz[i]-=L; iz[i]++;
			 }
    }
    //calculate force and potetial energy
    PE = force(rx,ry,rz,fx,fy,fz,N,L,rc2,E_cor,E_cut,&vir);
    // second half step
    KE = 0.0;
    for (i=0;i<N;i++) {
      vx[i]+=0.5*dt*fx[i];
      vy[i]+=0.5*dt*fy[i];
      vz[i]+=0.5*dt*fz[i];
      KE+=vx[i]*vx[i]+vy[i]*vy[i]+vz[i]*vz[i];
    }
    KE*=0.5;
    TE=PE+KE;
		//output data

      sprintf(fn, "./data/energy.dat");
      out = fopen(fn, "a");
      fprintf(out,"\n%i %.5lf %.5lf %.5lf %.5lf %.5le %.5lf",
            s,s*dt,PE,KE,TE,KE*2/3./N,rho*KE*2./3./N+vir/3.0/V);
     fclose(out);
    if (!(s%fSamp)) {
      sprintf(fn,"./data/%i.xyz",s);
      out=fopen(fn,"w");
      xyz_out(out,rx,ry,rz,vx,vy,vz,ix,iy,iz,L,N);
      fclose(out);
    }
  }
}
