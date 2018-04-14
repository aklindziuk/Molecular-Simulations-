reset
set term post eps enh color "Times-Roman" 24 linewidth 2.0 solid
set out "energy_rho4.eps"
set size square
set encoding iso_8859_1

set xlabel "r"
set ylabel "g(r) font 26

set yr [-6:6]
set ylabel "U/N"
set xlabel "time (t)"
set key center title "rho = 0.40"

set style data line
plot "./data/energy.dat" using 1:($5/216) t"total", "" u 1:($3/216) t"potential", "" u 1:($4/216) t"kinetic"
