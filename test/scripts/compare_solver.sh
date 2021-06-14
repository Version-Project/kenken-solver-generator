#!/bin/bash

# Compares solver algorithms on a list of grids with a difficulty passed as
# parameter. The folder containing the grids has to have already been created.
# First parameter corresponds to algorithm you want to use :
# 0,OPT_BRUTEFORCE  optimized backtracking bruteforce algorithm
# 1,BRUTEFORCE      classic backtracking bruteforce algorithm
# 2,BLOCKSIZE_SORT  backtracking algorithm sorting blocks by size
# 3,LOGIC           backtracking algorithm using logic
# Second parameter corresponds to the number of directory that will be used
# as : /resources/test_grid_diff<number>
# Warning : file list is retrieved and sorted by the date of creation

# sh compare_solver.sh <algorithm_number> <direcotry_number>

MOY=0
IT=1
NB_GRID=10
echo "First parameter corresponds to the number of the algorithm"
echo "Second parameter corresponds to the difficulty of the grid"

echo "Script started"

cd ../../resources/test_grid_diff$2

FILE_LIST=`ls -t -r`

cd ../../

NUM=$1

# It is not recommended to uncomment this line. There's no security that an
# algorithm will not exceed 1 hour calculation.
#for NUM in 0 1 2 3
#do
  echo 0 0 >> ./test/scripts_results/solver_$NUM.dat
  for file in $FILE_LIST
  do
    echo "Solver $NUM : Collecting data for $file"
    START=$(echo $(($(date +%s%3N)))) #milliseconds
    ./kenken -n -A$NUM ./resources/test_grid_diff$2/$file
    END=$(echo $(($(date +%s%3N))))
    RES=$(($END-$START))
    MOY=$(($MOY+$RES))
    IT=$(($IT+1))
    if [ $IT -gt $NB_GRID ]
    then
      MOY=$(($MOY/$NB_GRID))
      SIZE=`echo $file | cut -d'_' -f1`
      echo $SIZE $MOY >> ./test/scripts_results/solver_$NUM.dat
      IT=1
      MOY=0
    fi
  done
#done

#cd test

#echo "Drawing graph"

#gnuplot draw_solver_graph.gnu

echo "Script finished"
