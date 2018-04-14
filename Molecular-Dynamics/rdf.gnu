set term post eps enh color "Times-Roman" 24 linewidth 2.0 solid
set size square
set out "rdf_rho1.eps"
reset

set xr [0:3.0]
set xlabel "r"
set ylabel "g(r)"
set key center top title "rho = 0.10"

p "./data/rdf.dat"
