
reset
set term post eps enh color "Times-Roman" 24 linewidth 2.0 solid
set out "msd2_rho4.eps"
set size square
set encoding iso_8859_1

set xlabel "Time (t)"
set ylabel "{/Symbol \341}r^2{/Symbol \361}/6t"
f2(x) = b
b=0.566
set key center bottom title "rho = 0.40 \n D = 0.566 "

p "./data/msd.dat" using ($1):($2/$1/6) notitle, f2(x) w l notitle
