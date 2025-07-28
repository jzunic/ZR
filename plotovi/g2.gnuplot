set terminal pdfcairo size 12cm, 7cm font "Arial,10"
set output "profil_akceleracije.pdf"
set key top left

set xlabel "t [s]"
set ylabel "a [m/s^2]"

plot "../akce.txt" u 1:2 w l lc rgb "red" t "a_x(t)", "" u 1:3 w l lc rgb "purple" t "a_y(t)",\
 "" u 1:4 w l lc rgb "black" t "a_z(t)", "" u 1:5 w l lc rgb "pink" t "|a|"