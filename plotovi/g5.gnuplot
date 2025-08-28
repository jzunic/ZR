set term pngcairo size 800,600
set output "inset_plot.png"

set multiplot

# --- Main plot ---
set size 1,1          # full canvas
set origin 0,0
set xlabel "x[m]"
set ylabel "y[m]"
plot "../putanje.txt" u 1:2 w l lc rgb "red" t "raketina putanja", "../moon_cm.txt" u 1:2 pt 7 ps 0.5 lc rgb "light-gray" t "centar Mjeseca"


# --- Inset plot ---
set size 0.4,0.4      # width=40%, height=40%
set origin 0.1,0.55 # position (x,y) of lower left corner (fraction of canvas)
set xrange [2.5e8:3e8]   # independent axis ranges
set yrange [2.3e8:2.6e8]
set xtics 2.5e8, 2.5e7, 3e8
set ytics 2.3e8, 1e7, 2.6e8
unset xlabel
unset ylabel
set xtics font ",10"
set ytics font ",10"
plot "../putanje.txt" u 1:2 w l lc rgb "red" t "", "../moon_cm.txt" u 1:2 pt 7 ps 1 lc rgb "light-gray" t ""

unset multiplot