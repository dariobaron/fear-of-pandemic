#!/bin/bash

cd src/

make

cd ../

REACTIONTYPE=( 'soften' 'cut' )
FEARDISTRIBUTION=( 'fixed' 'uni' 'asyLow' 'asyHigh' 'bi' 'corr' 'anticorr' )
FEEDBACKTYPE=( 'short' 'long' 'shortlong' 'neighbours' 'all' )

outputdir="${1/input/"output"}"
outputdir="${outputdir%.*}"
mkdir -p $outputdir

outputfile="$outputdir/execution_log.txt"

src/simulation.x $1 'none' 'none' 'none' 2>$outputfile

if [[ $# -eq 1 && -e $1 ]]; then
	for reaction in "${REACTIONTYPE[@]}"; do
		for fear in "${FEARDISTRIBUTION[@]}"; do
			for feedback in "${FEEDBACKTYPE[@]}"; do
					src/simulation.x $1 $reaction $fear $feedback 2>>$outputfile
			done
		done
	done
else 
	echo "Input file does not exist: pass a valid input file as FIRST and ONLY argument!"
fi 


