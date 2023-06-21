#!/bin/sh

HW=02b
PROGRAM=./b3b36prg-hw$HW-genref

mkdir -p files
for i in `seq 1 10`
do
   PROBLEM=files/hw$HW-$i
   echo "Generate random input '$PROBLEM.in'"
   #echo "$PROGRAM -generate > $PROBLEM.in 2>/dev/null"
   #$PROGRAM -generate > $PROBLEM.in 2>/dev/null
   $PROGRAM -generate > $PROBLEM.in
   echo "Solve '$PROBLEM.in' and store the reference solution to '$PROBLEM.out'"
   $PROGRAM < $PROBLEM.in > $PROBLEM.out 2>$PROBLEM.err
done

return 0
