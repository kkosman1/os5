#!/bin/bash

for PROGRAM in "focus" "scan" "sort"; do
	for ALGORITHM in "custom" "fifo" "rand"; do
		for FRAME in 25 50 75; do
			echo >> $PROGRAM.txt
			echo $FRAME $ALGORITHM $PROGRAM: >> $PROGRAM.txt
			./virtmem 100 $FRAME $ALGORITHM $PROGRAM >> $PROGRAM.txt 
		done
	done
done
