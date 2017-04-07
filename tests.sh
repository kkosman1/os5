#!/bin/bash

for FRAME in 25 50 75; do
	for PROGRAM in "focus" "scan" "sort"; do
		for ALGORITHM in "custom" "fifo" "rand"; do
			echo
			echo $FRAME $ALGORITHM $PROGRAM:
			./virtmem 100 $FRAME $ALGORITHM $PROGRAM
		done
	done
done
