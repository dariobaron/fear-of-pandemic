#!/bin/bash

cd src/

make

cd ../

REACTIONTYPE=( 'soften' 'cut' )
FEARDISTRIBUTION=( 'fixed' 'uni' 'asyLow' 'asyHigh' 'bi' 'corr' 'anticorr' )
FEEDBACKTYPE=( 'none' 'short' 'long' 'shortlong' 'neighbours' 'all' )

if [[ $# -eq 1 && -e $1 ]]; then
	for reaction in "${REACTIONTYPE[@]}"; do
		for fear in "${FEARDISTRIBUTION[@]}"; do
			for feedback in "${FEEDBACKTYPE[@]}"; do
				src/simulation.x $1 $reaction $fear $feedback
			done
		done
	done
else 
	echo "Input file does not exist: pass a valid input file as FIRST and ONLY argument!"
fi 


