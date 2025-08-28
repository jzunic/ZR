set terminal pngcairo size 600,600
# set xrange [2.2: 3e8]
# set yrange [2.2: 3e8]

# Get min and max of x
# stats "../moon_cm.txt" using 1 nooutput name "X"
# xmin = X_min
# xmax = X_max

# # Get min and max of y
# stats "../moon_cm.txt" using 2 nooutput name "Y"
# ymin = Y_min
# ymax = Y_max


r = 1737e3

do for [i=0:368397:1000] {
     # --- read i-th line ---
    set output sprintf("frameovi/frame_%03d.png", i)
     # get x from column 1
    stats '../moon_cm.txt' every ::i::i using 1 nooutput name "Xx"
    x0 = Xx_min
    
    # get y from column 2
    stats '../moon_cm.txt' every ::i::i using 2 nooutput name "Yy"
    y0 = Yy_min

    set xrange [2.655e8:2.705e8 + 1000]
    set yrange [2.73e8:2.8e8]

    set parametric
    plot r*cos(t)+x0, r*sin(t)+y0 with lines lw 2 lc rgb "red", "../putanje.txt" u 1:2 every ::i::(i) pt 7 ps 0.5 lt rgb "blue" lw 2 notitle
    unset parametric
    unset xrange
    unset yrange
}
unset output