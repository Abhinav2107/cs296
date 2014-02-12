#!/bin/bash
DATA_DIR=./../data
rm -f $DATA_DIR/g19_lab05data_random.csv

cat $DATA_DIR/g19_lab05data_02.csv | awk '
{
	if (NR%150==1) {
		
		for (i=0;i<150;++i) {
			array[i]=0;
		}
		
		srand(NR);
		NUM=15;
		MIN=0;
		MAX=149;
		Number=0;
		
		while (Number < NUM) {
			r=int(((rand() *(1+MAX-MIN))+MIN));
			if (array[r]==0) {
			    Number++;
			    array[r]++;
			}
		}
	}

	if (array[NR%150]) {
		print $0;
	}
}
' >> $DATA_DIR/g19_lab05data_random.csv
