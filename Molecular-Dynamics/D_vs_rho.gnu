
reset
set term post eps enh color "Times-Roman" 24 linewidth 2.0 solid
set out "D_vs_rho.eps"
set size square
set encoding iso_8859_1

set xr [.3:.9]
set yr [0:.6]
set xlabel "rho"
set ylabel "D"

p "./data/D_vs_rho.dat" using 1:2 notitle ps 2
