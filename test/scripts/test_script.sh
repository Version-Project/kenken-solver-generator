#!/bin/bash

# Generator mode : retrieve time taken by the generator for different grid size.
# Solver mode (you should use other scripts) : retrieve time taken by the
# specified solver to solve grids in resources/test_grid.

# If you want to test generator : sh test_script.sh -g
# If you want to test solver : sh test_script.sh -s [algorithm number] :
# 0,OPT_BRUTEFORCE  optimized backtracking bruteforce algorithm
# 1,BRUTEFORCE      classic backtracking bruteforce algorithm
# 2,BLOCKSIZE_SORT  backtracking algorithm sorting blocks by size
# 3,LOGIC           backtracking algorithm using logic

# Before the script end, a graph is drawn with gnuplot.

# Warning : file list is retrieved and sorted by the date of creation

ERR_MODE=false
GEN_MODE=false
SOLV_MODE=false

if [ $# -gt 3 ] || [ $# -lt 1 ]
then
  ERR_MODE=true
  echo "Script error : wrong number of arguments"
fi

case $1 in
    "-g")
      GEN_MODE=true
      ;;
    "-s")
      SOLV_MODE=true
      ;;
    *)
      ERR_MODE=true
      echo "Script error : argument 1 : wrong argument"
      ;;
esac

if [ "$GEN_MODE" = true ]
then
  if [ $# -gt 1 ]
  then
    ERR_MODE=true
    echo "Script error : wrong number of arguments"
  fi
fi

if [ "$SOLV_MODE" = true ]
then
  if [ $# -lt 2 ]
  then
    ERR_MODE=true
    echo "Script error : no algorithm specified"
  fi
  if [ $2 -lt 0 ] || [ $2 -gt 3 ]
  then
    ERR_MODE=true
  	echo "Script error : wrong algorithm number"
  fi
fi

if [ "$ERR_MODE" = true ]
then
  echo "If you want to test generator : sh test.sh -g"
  echo "If you want to test solver : sh test.sh -s [algorithm number]"
  echo "Algorithm number : "
  echo "0,OPT_BRUTEFORCE  optimized backtracking bruteforce algorithm"
  echo "1,BRUTEFORCE      classic backtracking bruteforce algorithm"
  echo "2,BLOCKSIZE_SORT  backtracking algorithm sorting blocks by size"
  echo "3,LOGIC           backtracking algorithm using logic"
  exit
fi

echo "Script started"

touch res.dat

cd ..

if [ "$GEN_MODE" = true ]
then
  for size in 10 50 100 250 300 450 500
  do
  	MOY=0
    	echo "Collecting data for" $size "x" $size "grid"
  	for i in `seq 1 100`
  	do
  		START=$(echo $(($(date +%s%3N)))) #milliseconds
  		./kenken -g$size -n
  		END=$(echo $(($(date +%s%3N))))
  		RES=$(($END-$START))
  		MOY=$(($MOY+$RES))
  	done
  	MOY=$(($MOY/100))
  	echo $size $MOY >> ./test/res.dat
  done
fi

if [ "$SOLV_MODE" = true ]
then

  cd resources/test_grid

  FILE_LIST=`ls -t -r`

  cd ../..

  for file in $FILE_LIST
  do
    echo "Collecting data for $file"
    START=$(echo $(($(date +%s%3N)))) #milliseconds
    ./kenken -n -A$2 ./resources/test_grid/$file
    END=$(echo $(($(date +%s%3N))))
    RES=$(($END-$START))
    SIZE=`echo $file | cut -d'x' -f1`
    echo $SIZE $RES >> ./test/res.dat
  done
fi

cd test

echo "Drawing graph"

gnuplot draw.gnu

echo "Script finished"

rm res.dat
