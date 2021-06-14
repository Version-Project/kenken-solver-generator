#!/bin/bash

# Generate grids 10 grids of size 10 for each difficulties (from 1 to 10).

# sh generate_grids_difficulty.sh

echo "Script started"

cd ../../

for diff in `seq 1 10`
do
  for i in `seq 1 10`
  do
    FILE_NAME=$(echo $diff $i.ken)
    FILE_NAME=$(echo $FILE_NAME | tr "[:blank:]" "_")
    echo $FILE_NAME generated
    ./kenken -g10 -d$diff -n -o ./resources/test_grid/var_difficulty/$FILE_NAME
  done
done

echo "Script finished"
