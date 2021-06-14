# To execute this file and draw graph : gnuplot <filename>
set terminal png
set output 'solver_comp_diff.png'
set xlabel "Grid difficulty"
set ylabel "Time (milliseconds)"
set style line 1
set style line 2
set style line 3
set style line 4
plot 'solver_diff_0.dat' linestyle 1 title 'OPT\_BRUTEFORCE' with lines,\
'solver_diff_1.dat' linestyle 2 title 'BRUTEFORCE' with lines,\
'solver_diff_2.dat' linestyle 3 title 'BLOCKSIZE\_SORT' with lines,\
'solver_diff_3.dat' linestyle 4 title 'LOGIC' with lines
