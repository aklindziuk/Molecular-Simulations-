#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(){

  FILE *bfile;
  FILE *afile;

    		char buf[5000];
        int nB = 3168; /////////////////////////////
        int tSamples = 1000;
        int startafter = 100;
        int n = 1/.001;


        double an[nB], bn[nB] ;
        double ah[nB], bh[nB];
        double sum;

        char title1[20];
        sprintf(title1, "./tmp/h%d.txt", tSamples);
        afile = fopen(title1, "r");
        if (!afile)
        		return 1;
        int k = 0;
    		while (fgets(buf,1000, afile)!=NULL){
            sscanf(buf, "%lf %lf", &an[k], &ah[k]);
            k++;
          }
       fclose(afile);

        int i;
        for (i = (startafter +1); i < (tSamples+1); i ++){

        char title[20];
        sprintf(title, "./tmp/h%d.txt", i);
    		bfile =fopen(title,"r");
    		if (!bfile)
        		return 1;
        int k = 0;
    		while (fgets(buf,1000, bfile)!=NULL){
        		//printf("%s",buf);
            sscanf(buf, "%lf %lf", &bn[k], &bh[k]);
            k++;
          }
       fclose(bfile);

       sum =sum + (ah[n] - bh[n])*(ah[n] - bh[n]);
     }
     double error;
     error  = sqrt(sum/(tSamples-startafter-1));

 fprintf(stdout,"%lf\n",sum);
       fprintf(stdout,"%lf\n",error);
    }
