# To execute this file and draw graph : gnuplot <filename>
set terminal png
set output 'courbe.png'
set xlabel "Grid size (n x n)"
set ylabel "Time (milliseconds)"
plot 'res.dat' with lines
