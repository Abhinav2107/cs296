#!/bin/bash
DATA_DIR=./../data/
EXE=../mybins/cs296_19_exe
rm -f $DATA_DIR/g19_lab05data_02.csv
for i in {1..100}
	do
		for j in {1..150}
			do
				out=$(./$EXE $i)
				cmd=$i,$j,$(echo $out | awk '{print $10}'),$(echo $out | awk '{print $17}'),$(echo $out | awk '{print $25}'),$(echo $out | awk '{print $33}'),$(echo $out | awk '{print $39}')
				echo $cmd >> $DATA_DIR/g19_lab05data_02.csv
			done
	done
