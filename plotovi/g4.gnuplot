set terminal pdfcairo size 10cm, 9cm font "Arial,10"
set output "putanje.pdf"
set key top right
set view equal xyz
set ticslevel 0 
set view 0, 0

set xlabel "x[m]"
set ylabel "y[m]"
set zlabel "z[m]"

# set xtics 2.81e8, 1.5e6, 2.89e8
# set ytics 2.55e8, 1.5e6, 2.63e8
# set ztics -2e6, 750e3, 2e6

# set xtics offset -2,-1
# set ytics offset 3,0

# set xlabel offset -3, -1
# set ylabel offset 2, 0
# set zlabel offset -3, 0

# set xtics -8e6, 2500e3, 8e6
# set ytics -8e6, 2500e3, 8e6

set parametric


set print "moon_points.dat"  # save points to file
R = 1737e3
n_long = 20   # number of longitude points
n_lat  = 10   # number of latitude points

do for [i=0:n_lat] {
    theta = pi * i / n_lat         # polar angle (0 = north pole)
    do for [j=0:n_long-1] {
        phi = 2*pi * j / n_long   # azimuth angle
        x = R * cos(phi) * sin(theta)# + 201.043
        y = R * sin(phi) * sin(theta)#+ 0.620061
        z = R * cos(theta)
        print x, y, z
    }
}
unset print

splot "../putanje.txt" w l t "raketina putanja" lc rgb "red", "moon_points.dat" pt 7 ps 0.1 lc rgb "light-gray" t "Mjesec", "../bla.txt" pt 7 ps 0.1 lc rgb "red" t "Raketa"