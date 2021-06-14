# To execute this file and draw graph : gnuplot <filename>
set terminal png
set output 'solver_comp_size.png'
set xlabel "Grid size (n x n)"
set ylabel "Time (milliseconds)"
set xrange[2:15]
# set yrange [0:800]
set style line 1
set style line 2
set style line 3
set style line 4
plot 'solver_0.dat' linestyle 1 title 'OPT\_BRUTEFORCE' with lines,\
'solver_1.dat' linestyle 2 title 'BRUTEFORCE' with lines,\
'solver_2.dat' linestyle 3 title 'BLOCKSIZE\_SORT' with lines,\
'solver_3.dat' linestyle 4 title 'LOGIC' with lines
