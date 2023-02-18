#!/bin/bash

#$ -S /bin/bash
#$ -N fop_l5
#$ -q rostam.q
#$ -cwd
#$ -o /data.nst/dbarone/fop_log/
#$ -e /data.nst/dbarone/fop_log/

source ~/.bashrc

REACTIONTYPE=( 'soften' 'cut' )
n_r=${#REACTIONTYPE[@]}
FEARDISTRIBUTION=( 'fixed' 'uni' 'asyLow' 'asyHigh' 'bi' 'corr' 'anticorr' )
n_d=${#FEARDISTRIBUTION[@]}
FEEDBACKTYPE=( 'short' 'long' 'shortlong' 'neighbours' 'all' )
n_f=${#FEEDBACKTYPE[@]}

cd /home/dbarone/fear-of-pandemic/

if [ -z ${SGE_TASK_ID+x} ]; then 
	echo "do not run simulation because SGE_TASK_ID not set";
	echo "$(( n_r*n_d*n_f )) jobs are needed to parse the arguments"
else 
	echo "start simulation on cluster with SGE_TASK_ID=${SGE_TASK_ID}"; 
	index_reaction=$(( (${SGE_TASK_ID}-1)/n_r/n_d ))
	index_distribution=$(( (${SGE_TASK_ID}-1)/n_r%n_d ))
	index_feedback=$(( (${SGE_TASK_ID}-1)%n_r ))
	src/simulation.x 'input/bianconi-barabasi_10000_5/' ${REACTIONTYPE[index_reaction]} ${FEARDISTRIBUTION[index_distribution]} ${FEEDBACKTYPE[index_feedback]}
fi
