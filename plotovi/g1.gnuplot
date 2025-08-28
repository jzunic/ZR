set terminal pdfcairo size 12cm, 7cm font "Arial,10"
set output "profil_brzine.pdf"
set key top left

set xlabel "t [s]"
set ylabel "v [m/s]"

set yrange [0:8000]

plot "../rocket_cm.txt" u 1:2 w l lc rgb "red" t "v_{rad}(t)", "" u 1:3 w l lc rgb "black" t "v_{hor}(t)"
