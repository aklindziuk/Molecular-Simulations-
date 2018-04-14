#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{

    FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");

    //Send commands to gnuplot one by one.
    fprintf(gnuplotPipe, "%s \n", "set terminal postscript eps size 3.5,2.62 enhanced color font 'Helvetica,20' lw 2");
    //fprintf(gnuplotPipe, "%s \n", "set output 'output3.eps'");
    //fprintf(gnuplotPipe, "%s \n", "set terminal x11 enhanced");

    fprintf(gnuplotPipe, "%s \n","set termoption enhanced");
    fprintf(gnuplotPipe, "%s \n","set key center top");
    fprintf(gnuplotPipe, "%s\n", "set xlabel \"{/Symbol h}\"");
    fprintf(gnuplotPipe, "%s\n", "set ylabel \"{/Symbol B}P/{/Symbol r}\" ");
    fprintf(gnuplotPipe, "%s\n", "set ylabel font \" Havetica, 30\"");
    fprintf(gnuplotPipe, "%s\n", "set xlabel font \" Havetica, 30\"");
    fprintf(gnuplotPipe, "%s \n", "set style line 1 linecolor rgb '#0060ad' linetype 1 linewidth 2  # blue");
    fprintf(gnuplotPipe, "%s \n", "plot 'pressure_data.txt' using 2:3 title \"Carlahan-Starling eqn.\" with lines ls 1, \
    'pressure_data.txt' using 2:1:4 with yerrorbars title \"simulation data\"  ");

    //fprintf(gnuplotPipe, "%s \n", "plot 'pressure_data.txt' using 2:1 ");

    fclose(gnuplotPipe);

}
