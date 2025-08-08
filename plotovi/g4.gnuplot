set terminal pdfcairo size 12cm, 7cm font "Arial,10"
set output "putanje.pdf"
set key top left

plot "../putanje.txt" w l, "../moon_cm.txt" every 1000 pt 7 ps 0.1