/*
Molecular Monte Carlo Simulation of Hard Sphere fluid
PHYS 610: Methods of Molecular Simulaitons

N = number of particles
rho = number density
d = particle diameter
binwidth = width of histogram bin
L = length of simulation box
tSamples = sampling freequency
nCycles = number of monte carlo steps
nB = number of bins
dr = scaling factor of monte carlo displacement

output: data files of xyz particle positions (#.txt) and
corresponding histograms (h#.txt) for each sampling event
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main ( int argc, char * argv[]) {

  //what we know <<-----our indep. variables (choose these)
  int N = 216;
  double rho=.85;
  int d = 1;
  //how many displacements
  int nCycles = 1000000;
  int nSamples = 1000;
  // disp length
  double dr = 0.05;
  double binwidth = 0.001;

 //what we dont know <<----- depend. vars. ( these vary based on N,rho)
  double L = pow((N/rho),0.3333333);
  double Vbox = L*L*L;
  double V = N/rho;
  double tSamples = nCycles/nSamples;
  double rc = L/2;   double hL = L/2;
  double rc2 = rc*rc;
  int nB = (int)(rc/binwidth) + 1;

   //output initial info
   fprintf(stdout,"# L = %.5lf; rho = %.5lf; N = %i; d = %i\n",
	  L,rho,N,d);

    //allocate array space - position arrays
    double * rx, * ry, * rz;
    double rxold, ryold, rzold;
    double  dx, dy, dz;
    double sx, sy, sz, sr2;
    rx = (double*)malloc(N*sizeof(double));
    ry = (double*)malloc(N*sizeof(double));
    rz = (double*)malloc(N*sizeof(double));

    //initialize rand numbers
    time_t t;
    srand((unsigned) time(&t));
    int p;

    //initialize files & buffers
    FILE * fp ;
    FILE *ptr_file;
    FILE *out;
    FILE *bfile, *afile;
    char buf[1000];
    char title[20];
    char htitle[20];

    // generate positions on simple cubic lattice
    init(rx, ry, rz, N, L);

   /* Monte Carlo steps: make displacements; each disp = 1 cycle */
    int c;
    for (c = 0; c < (nCycles+1); c ++){

      //pick particle randomly
      p = rand() % (N + 1);

    //calculate random displacement
     dx = dr*( 0.5 - ((double) rand() / RAND_MAX));
     dy = dr*( 0.5 - ((double) rand() / RAND_MAX));
     dz = dr*( 0.5 - ((double) rand() / RAND_MAX));

     //save old position
     rxold = rx[p];
     ryold = ry[p];
     rzold = rz[p];

     // displace particle c
     rx[p]+= dx;
     ry[p]+= dy;
     rz[p]+= dz;

     // check overlaps, undo disp if overlaps
     int j;
     for (j = 0; j < N; j ++){
       if (j != p){
         sx = rx[p] - rx[j];
         sy = ry[p] - ry[j];
         sz = rz[p] - rz[j];

         //periodic boundary conditions
         if (sx>hL)       sx-=L;
         else if (sx<-hL) sx+=L;
         if (sy>hL)       sy-=L;
         else if (sy<-hL) sy+=L;
         if (sz>hL)       sz-=L;
         else if (sz<-hL) sz+=L;

       sr2 = sx*sx + sy*sy + sz*sz;
       //place back if overlaps
       if (sqrt(sr2) < d) {
         rx[p] = rxold;
         ry[p] = ryold;
         rz[p] = rzold;
       }
       }
     }

     //save data to file every nSamples steps
     if (((c % nSamples) == 0)){
       char title[20];
       sprintf(title, "./tmp/%d.txt", c/nSamples);
       fp = fopen(title, "w");
       xyzout(fp, rx, ry, rz,  N);
       fclose(fp);
     }
    }
	  printf("Cycles completed: %d\n", c);

/* get the g(r) histograms for each sampling file */

    int samp;
    for (samp = 1; samp < (tSamples + 1); samp ++){

      //allocate space for the histogram
      int * H = (int*)calloc(nB,sizeof(int));
      double * hx, * hy, * hz;

      //arrays that will store xyz data fetched from files
      hx = (double*)malloc(N*sizeof(double));
      hy = (double*)malloc(N*sizeof(double));
      hz = (double*)malloc(N*sizeof(double));

      //fetch xyz coordinates from a sampling event
      sprintf(title, "./tmp/%d.txt", samp);
      ptr_file =fopen(title,"r");
      xyzin( ptr_file, hx, hy, hz, N );
      fclose(ptr_file);

      //make a histogram of particle positions
      histogram(hx, hy, hz, N, L, rc2, binwidth, H );

      //calculate g(r) and save data to file
      sprintf(htitle, "./tmp/h%d.txt", samp);
      out = fopen(htitle, "w");
      int c; double r;
      for (c=0;c<nB;c++) { /// c = n of lines
        r=binwidth*(c);
        fprintf(out,"%.4lf %.4lf\n", r,(double)(H[c])/(N*rho*4*M_PI*r*r*binwidth));
      }
      fclose(out);
    }
     printf("nB: %d\n", nB);
     printf("tSamples: %lf\n", tSamples);

     //check the ensamble after all the steps by viewing final configuration in 2d
     char * commandsForGnuplot[] = {"set title \"TITLEEEEE\"", "plot './tmp/90.txt'"};
     FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");
     int w;
     for (w=0; w < 2; w++) {
     fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[w]); //Send commands to gnuplot one by one.
     }
     fclose(gnuplotPipe);
     return 0;
    }
