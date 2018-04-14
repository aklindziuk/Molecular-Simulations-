
reset
set term post eps enh color "Times-Roman" 24 linewidth 2.0 solid
set out "msd1_rho4.eps"
set size square
set encoding iso_8859_1

set xlabel "Time (t)"
set ylabel "{/Symbol \341}r^2{/Symbol \361}"
f2(x) = b*x
b=3.4
set key left top title "rho = 0.4 \n {/Symbol \341}r^2{/Symbol \361} = 3.4 t"

p "./data/msd.dat" u 1:2 w lp pt 4 notitle, f2(x) w l notitle
