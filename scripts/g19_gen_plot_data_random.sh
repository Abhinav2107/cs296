#!/bin/bash
DATADIR=../data
rm -f $DATADIR/g19_lab05data_plot_random.csv
cat $DATADIR/g19_lab05data_random.csv | awk '
BEGIN {
	i=0
	stepsum=0
	FS=",";
}
{
	stepsum+=$3;
	
	if(NR%15==0) {
		i++;
		stepsum/=15;
		printf i","stepsum;
		print "";
		stepsum=0;
	}
}
END {}
' >> $DATADIR/g19_lab05data_plot_random.csv
