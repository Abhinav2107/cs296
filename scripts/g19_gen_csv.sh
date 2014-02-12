#!/bin/bash
DATA_DIR=../data/
rm -f $DATA_DIR/g19_lab05data_01.csv
for i in {1..100}
	do
		for j in {1..150}
			do
				cmd=$i,$j,
				#for num in {0..5}
					#do
						#read line
						#if [ $num -eq 1 ]
						#then
							#val=$(echo $line | awk '{print $6}')
							#cmd=$cmd$val,
						#elif [ $num -eq 2 ]
						#then
							#val=$(echo $line | awk '{print $6}')
							#cmd=$cmd$val,
						#elif [ $num -eq 3 ]
						#then
							#val=$(echo $line | awk '{print $7}')
							#cmd=$cmd$val,
						#elif [ $num -eq 4 ]
						#then
							#val=$(echo $line | awk '{print $7}')
							#cmd=$cmd$val,
						#elif [ $num -eq 5 ]
						#then
							#val=$(echo $line | awk '{print $5}')
							#cmd=$cmd$val
						#fi
					#done <$DATA_DIR/g19out-$i-$j.txt
				OLD_Delim=$IFS
				IFS=$'\n'
				var=($(cat $DATA_DIR/g19out-$i-$j.txt))
				IFS=$OLD_Delim
				#var1=$(sed -n 1,1p $DATA_DIR/g19out-$i-$j.txt)
				#var2=$(sed -n 2,2p $DATA_DIR/g19out-$i-$j.txt)
				#var3=$(sed -n 3,3p $DATA_DIR/g19out-$i-$j.txt)
				#var4=$(sed -n 4,4p $DATA_DIR/g19out-$i-$j.txt)
				#var5=$(sed -n 5,5p $DATA_DIR/g19out-$i-$j.txt)
				#var6=$(sed -n 6,6p $DATA_DIR/g19out-$i-$j.txt)
				#echo "${var[1]}"
				var[0]=${var[0]/"Numer of Iterations: "/}
				var[1]=${var[1]/"Average time per step is "/}
				var[2]=${var[2]/"Average time for collisions is "/}
				var[3]=${var[3]/"Average time for velocity updates is "/}
				var[4]=${var[4]/"Average time for position updates is "/}
				var[5]=${var[5]/"Total loop time is "/}
				temp="$i,$j,${var[1]},${var[2]},${var[3]},${var[4]},${var[5]}"
				temp=${temp//" ms"/}
				echo $temp >> $DATA_DIR/g19_lab05data_01.csv
			done
	done
