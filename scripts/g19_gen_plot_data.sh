#!/bin/bash
DATADIR=../data
rm -f $DATADIR/g19_lab05data_plot.csv
for i in {1..100}
	do
		stepsum=0
		colsum=0
		velsum=0
		possum=0
		loopsum=0
		maxstep=0
		minstep=1000
		for j in {1..150}
			do
				read line
				val=$(echo $line | awk -F, '{print $3}')
				stepsum=$(echo $stepsum + $val | bc -l)
				cmp=$(echo $val \< $maxstep | bc -l)
				if [ $cmp -eq 0 ]
				then
					maxstep=$val
				fi
				cmp=$(echo $val \< $minstep | bc -l)
				if [ $cmp -eq 1 ]
				then
					minstep=$val
				fi
				val=$(echo $line | awk -F, '{print $4}')
				colsum=$(echo $colsum + $val | bc -l)
				val=$(echo $line | awk -F, '{print $5}')
				velsum=$(echo $velsum + $val | bc -l)
				val=$(echo $line | awk -F, '{print $6}')
				possum=$(echo $possum + $val | bc -l)
				val=$(echo $line | awk -F, '{print $7}')
				loopsum=$(echo $loopsum + $val | bc -l)
			done
		stepavg=$(echo $stepsum / 150 | bc -l )
		colavg=$(echo $colsum / 150 | bc -l )
		velavg=$(echo $velsum / 150 | bc -l )
		posavg=$(echo $possum / 150 | bc -l )
		loopavg=$(echo $loopsum / 150 | bc -l)
		echo $i,$stepavg,$colavg,$velavg,$posavg,$loopavg,$minstep,$maxstep >> $DATADIR/g19_lab05data_plot.csv
	done < $DATADIR/g19_lab05data_01.csv
