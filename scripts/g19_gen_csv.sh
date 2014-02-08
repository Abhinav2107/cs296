#!/bin/bash
DATA_DIR=../data/
rm -f $DATA_DIR/g19_lab05data_01.csv
for i in {1..100}
	do
		for j in {1..150}
			do
				cmd=$i,$j,
				for num in {0..5}
					do
						read line
						if [ $num -eq 1 ]
						then
							val=$(echo $line | awk '{print $6}')
							cmd=$cmd$val,
						elif [ $num -eq 2 ]
						then
							val=$(echo $line | awk '{print $6}')
							cmd=$cmd$val,
						elif [ $num -eq 3 ]
						then
							val=$(echo $line | awk '{print $7}')
							cmd=$cmd$val,
						elif [ $num -eq 4 ]
						then
							val=$(echo $line | awk '{print $7}')
							cmd=$cmd$val,
						elif [ $num -eq 5 ]
						then
							val=$(echo $line | awk '{print $5}')
							cmd=$cmd$val
						fi
					done <$DATA_DIR/g19out-$i-$j.txt
				echo $cmd >> $DATA_DIR/g19_lab05data_01.csv
			done
	done
