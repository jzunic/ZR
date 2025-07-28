# Set terminal
set terminal pngcairo size 800,600




# Loop over frames
do for [frame=1:80000:100] {
    set output sprintf("frameovi/frame_%03d.png", frame)
    splot \
        "../rocket_cm.txt" u 1:2:3 every ::0::(frame-1) with lines lt rgb "blue" lw 2 notitle, \
        "../rocket_cm.txt" u 1:2:3 every ::(frame-1)::(frame-1) with points pt 7 ps 0.2 lt rgb "red" notitle, \
        "../earth_first.txt" u 1:2:3 every ::0::183 pt 7 ps 0.2 notitle
}
