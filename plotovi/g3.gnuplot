set terminal pdfcairo size 10cm, 10cm font "Arial,10"
set output "putanja_rakete.pdf"

set xlabel "x [m]"
set ylabel "y [m]"
set view equal

#plot "../udaljenosti.txt" u 1:2 w l 
plot "../yaaa.txt" u 1:2 w l 