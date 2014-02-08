#!/bin/bash
EXE=./../mybins/cs296_19_exe
DATA_DIR=../data/
for i in {1..100}
    do
		for j in {1..150}
			do
				./$EXE $i > $DATA_DIR/g19out-$i-$j.txt
			done
	done
  
