#!/bin/bash
DATADIR=../data
rm -f $DATADIR/g19_lab05data_plot_random.csv
for i in {1..100}
	do
		stepsum=0
		for j in {1..15}
			do
				read line
				val=$(echo $line | awk -F, '{print $3}')
				stepsum=$(echo $stepsum + $val | bc -l)
			done
		stepavg=$(echo $stepsum / 15 | bc -l )
		echo $i,$stepavg >> $DATADIR/g19_lab05data_plot_random.csv
	done < $DATADIR/g19_lab05data_random.csv
