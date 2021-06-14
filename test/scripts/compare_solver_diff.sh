#!/bin/bash

# Compares solver algorithms on a list of grids of the same size but various
# difficulties (from 1 to 10).
# First parameter corresponds to algorithm you want to use :
# 0,OPT_BRUTEFORCE  optimized backtracking bruteforce algorithm
# 1,BRUTEFORCE      classic backtracking bruteforce algorithm
# 2,BLOCKSIZE_SORT  backtracking algorithm sorting blocks by size
# 3,LOGIC           backtracking algorithm using logic
# Warning : file list is retrieved and sorted by the date of creation

# sh compare_solver_diff.sh <algorithm_number>

MOY=0
IT=1
NB_GRID=10

echo "Script started"

cd ../../resources/test_grid/var_difficulty

FILE_LIST=`ls -t -r`

cd ../../../..

NUM=$1

# It is not recommended to uncomment this line. There's no security that an
# algorithm will not exceed 1 hour calculation.
#for NUM in 0 1 2 3
#do
  echo 0 0 >> ./test/solver_diff_$NUM.dat
  for file in $FILE_LIST
  do
    echo "Solver $NUM : Collecting data for $file"
    START=$(echo $(($(date +%s%3N)))) #milliseconds
    ./kenken -n -A$NUM ./resources/test_grid/var_difficulty/$file
    END=$(echo $(($(date +%s%3N))))
    RES=$(($END-$START))
    MOY=$(($MOY+$RES))
    IT=$(($IT+1))
    if [ $IT -gt $NB_GRID ]
    then
      MOY=$(($MOY/$NB_GRID))
      SIZE=`echo $file | cut -d'_' -f1`
      echo $SIZE $MOY >> ./test/solver_diff_$NUM.dat
      IT=1
      MOY=0
    fi
  done
#done

#cd test

#echo "Drawing graph"

#gnuplot draw_solver_graph_diff.gnu

echo "Script finished"

#don't forget to rm
