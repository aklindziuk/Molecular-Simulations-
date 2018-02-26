/*
Function that averages g(r) histograms stored in files
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(){

  /*get xyz data from file */

  FILE *bfile;
  FILE *afile;

    		char buf[5000];
        int nB = 3168;  //<<---make sure this matches the MC simulation
        int tSamples = 1000; //<<--make sure this matches simulation
        int startafter = 100; //<<--choose how much of the run to throw

        double an[nB], bn[nB] ;
        double ah[nB], bh[nB];

        //get hist A from file
        char title1[20];
        sprintf(title1, "./tmp/h%d.txt", startafter);
        afile = fopen(title1, "r");
        if (!afile)
        		return 1;
        int k = 0;
    		while (fgets(buf,1000, afile)!=NULL){
            sscanf(buf, "%lf %lf", &an[k], &ah[k]);
            k++;
          }
       fclose(afile);

       //for the rest ofthe files, get hist B
        int i;
        for (i = (startafter +1); i < (tSamples+1); i ++){
        char title[20];
        sprintf(title, "./tmp/h%d.txt", i);
    		bfile =fopen(title,"r");
    		if (!bfile)
        		return 1;
        int k = 0;
    		while (fgets(buf,1000, bfile)!=NULL){
            sscanf(buf, "%lf %lf", &bn[k], &bh[k]);
            k++;
          }
       fclose(bfile);

      //find the average of each bin in A and B
      int j;
      for (j = 0; j< (nB + 1); j ++){
       ah[j] =((i-1)*ah[j] + bh[j])/i;
     }

     //print each one of the averages: last avg is the overall avg
     FILE *out;
     char avgtitle[20];
     sprintf(avgtitle, "./tmp/avg%d.txt", i);
     out = fopen(avgtitle, "w");
      int c;
      for (c=0;c<(nB+1);c++) {
       fprintf(out,"%.4lf %.4lf\n", an[c], ah[c]);
     }
     fclose(out);
}

//use gnuplot to plot the overall average
FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");
//Send commands to gnuplot one by one
//fprintf(gnuplotPipe, "%s \n", "set terminal postscript eps size 5,4 enhanced color font 'Helvetica,20' lw 2");
//fprintf(gnuplotPipe, "%s \n", "set output 'rho95dr01.eps'");
fprintf(gnuplotPipe, "%s \n","set key center top title \"eta = 0.50\"");
fprintf(gnuplotPipe, "%s\n", "set key font \" Havetica, 50\"");
fprintf(gnuplotPipe, "%s\n", "set xlabel \"r\"");
fprintf(gnuplotPipe, "%s\n", "set xlabel font \" Havetica, 50\"");
fprintf(gnuplotPipe, "%s\n", "set ylabel \"g(r)\" font \"26\"");
fprintf(gnuplotPipe, "%s\n", "set ylabel font \" Havetica, 50\"");
fprintf(gnuplotPipe, "%s \n", "plot [1:3.5]'./tmp/avg1000.txt'  notitle ");
//fprintf(gnuplotPipe, "%s\n", "set xrange [1:4]");
//fprintf(gnuplotPipe, "%s\n", "set xrange [0:3]");
//fprintf(gnuplotPipe, "%s \n", "set tics scale 1.75");
pclose(gnuplotPipe);
return 0;
}
