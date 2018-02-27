# Molecular-Simulations- Monte Carlo



Molecular Monte Carlo Simulation of Hard Sphere Fluid:

Problem description and results are found in the .pdf file.

1) Implement Metropolis Monte Carlo to simulate hard sphere fluid 
2) Calculate radial distribution function at various stages
3) Find average radial distribution of the ensemble
4) Find standard error of the values in the histogram

- main.c does Monte Carlo, finds g(r) histograms for each sampling instance, and saves them as text file in folder tmp
- histogram.c , xyzout.c, xyzin.c, init.c are subroutines of main.c
- avg_hist.c takes the histograms from the folder and averages them saving them in tmp
- error.c computes the standard error of g(d+) 

If you want to run this code, you will need :
- gnuplot, which I called with C commands to plot the data and make the graphs
- folder tmp

To run on linux
1) make main
2) ./ main
this will generate files of xyz positions of ensemble after each sampling called #.txt, where # is the number of sampling event
and it will generate files with histogram of each sampling event called h#.txt
3) make sure nB and tSamples in the avg_hist.c is the same as that in main.c
4) gcc -o avg_hist avg_hist.c -lm 
5) ./avg_hist
6) similar for error.c as for avg_hist.c
