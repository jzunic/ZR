set terminal pdfcairo size 12cm, 7cm font "Arial,10"
set output "putanje.pdf"
set key top left

plot "../putanje.txt" pt 7 ps 0.1, "../moon_cm.txt" w l 