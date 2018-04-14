#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void histogram( double * rx, double * ry, double * rz,
		   int N, double L, double rc2, double dr, int * H ) {

int i, j, bin;
double dx, dy, dz, r2;
double hL = L/2;


       for (i=0;i<(N-1);i++) {
         for (j=i+1;j<N;j++) {
					 if (j!=i){
           dx  = (rx[i]-rx[j]);
           dy  = (ry[i]-ry[j]);
           dz  = (rz[i]-rz[j]);
					 dx = fmod((fmod(dx,L)+L),L);
					 dy = fmod((fmod(dy,L)+L),L);
					 dz = fmod((fmod(dz,L)+L),L);
					 //dx = ((L % dx) + L) % L;
					 // dy = ((L % dy) + L) % L;
						 //dz = ((L % dz) + L) % L;
           if (dx>hL)       dx-=L;
           else if (dx<-hL) dx+=L;
           if (dy>hL)       dy-=L;
           else if (dy<-hL) dy+=L;
           if (dz>hL)       dz-=L;
           else if (dz<-hL) dz+=L;
           r2 = dx*dx + dy*dy + dz*dz;
           if (r2<(L/2)*(L/2)) { ////////////////////////////
      bin=(int)(sqrt(r2)/dr);////////////////////////////////
      H[bin]+=2;
		}
    }
 }
}
}
