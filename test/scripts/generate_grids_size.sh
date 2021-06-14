#!/bin/bash

# Generate 10 grids from minimal to maximum size (must be specified) with a
# given difficulty (must be specified).
# First parameter corresponds to the difficulty of the grids
# Second parameter corresponds to the minimal size of the grids
# Third parameter corresponds to the maximal size of the grids

# sh generate_grids_size.sh <difficulty> <min_grid_size> <max_grid_size>

echo "Script started"

cd ../../
make > /dev/null
mkdir ./resources/test_grid_diff$1
for size in `seq $2 $3`
do
  for i in `seq 1 10`
  do
    FILE_NAME=$(echo $size $size $i.ken)
    FILE_NAME=$(echo $FILE_NAME | tr "[:blank:]" "_")
    echo $FILE_NAME generated
    ./kenken -g$size -d$1 -n -o ./resources/test_grid_diff$1/$FILE_NAME
  done
done

echo "Script finished"
