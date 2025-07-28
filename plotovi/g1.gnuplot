set terminal pdfcairo size 12cm, 7cm font "Arial,10"
set output "profil_brzine.pdf"
set key top left

set xlabel "t [s]"
set ylabel "v [m/s]"

plot "../rocket_v.txt" u 1:2 w l lc rgb "red" t "v_x(t)", "" u 1:3 w l lc rgb "purple" t "v_y(t)",\
 "" u 1:4 w l lc rgb "black" t "v_z(t)", "" u 1:5 w l lc rgb "pink" t "|v|"
