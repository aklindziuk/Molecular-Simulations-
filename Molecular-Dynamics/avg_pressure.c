/*
Post-processing code to find the average pressure from data values
stored in the 'energy.dat' file

use: ./avg_pressure -ns <nSteps in MD simulation>
note: first 10 % is thrown out
outputs average pressure
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char * argv[]){

  FILE *fp;

    		char fn[20];
        int i;
        int nSteps;
        nSteps = 10000;

        int start;

        double * P;
        double P_avg;
        double dum;

        P =(double*)calloc(nSteps,sizeof(double));

        sprintf(fn, "./data/energy.dat");
        fp = fopen(fn, "r");

        for (i=1;i<argc;i++) {
          if (!strcmp(argv[i],"-ns")) nSteps = atoi(argv[++i]);
          else {
            fprintf(stderr,"Error: Command-line argument '%s' not recognized.\n",
      	      argv[i]);
            exit(-1);
          }
        }

        start = (int) nSteps*0.1;

    		for(i=0; i<nSteps; i++){
            fscanf(fp, "%lf %lf %lf %lf %lf %lf %lf %lf", &dum, &dum, &dum, &dum,&dum, &dum,&dum, &P[i]);
          }

       fclose(fp);

       P_avg = 0.0;
       for(i=start; i<nSteps; i++){
         P_avg += P[i];
         }
       P_avg /= (nSteps-start);

       fprintf(stdout,"avg pressure = %lf\n", P_avg);
}
