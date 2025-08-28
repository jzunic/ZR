set terminal pdfcairo size 11cm, 9cm font "Arial,10"
set output "putanja_rakete.pdf"
set key top left offset -2,0

set xlabel "x [m]"
set ylabel "y [m]"
set parametric
set size ratio -1
set xrange [-2.5e6:2.5e6]
set yrange [-2.5e6:2.5e6]
R = 1737e3  # Earth radius in meters
r=1
# set yrange [-5e7:3e8]
# set xrange [-5e7:3e8]
plot \
  "../putanje.txt" u 1:2 w l lc rgb "red" t "raketina putanja", R*cos(t), R*sin(t) w l lc rgb "light-gray" t "Mjesečeva površina", "../bla.txt" pt 7 ps 0.2 lc rgb "red" t "raketa"
