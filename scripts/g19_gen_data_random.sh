#!/bin/bash
DATA_DIR=./../data
cmd="sed -n"
for i in {1..100}
	do
	for j in {1..15}
		do
			temp=" -e "
			cmd=$cmd$temp
			temp=$(( ($RANDOM % 150) + ( 150 * ($i - 1) ) + 1 ))
			cmd=$cmd$temp
			temp="p"
			cmd=$cmd$temp
		done
	done
$cmd $DATA_DIR/g19_lab05data_02.csv > $DATA_DIR/g19_lab05data_random.csv
		
