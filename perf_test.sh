#!/bin/bash

NUM_TESTS=10

cd ./src
make clean
make

cd ../test
make clean
make

rm perf_report.txt
touch perf_report.txt

for (( i=1; i<=$NUM_TESTS; i++ ))
do
  echo "Test no. $i"
  make run

  cat qrs_log.txt >> perf_report.txt
done

cd ..
./perf_test_report.py -b ./test/perf_report.txt -t QRS_report -n ${NUM_TESTS}
