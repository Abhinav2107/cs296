#!/bin/bash
DATADIR=../data
rm -f $DATADIR/g19_lab05data_plot_02.csv
cat $DATADIR/g19_lab05data_02.csv | awk '
BEGIN {
	i=0
	stepsum=0
	FS=",";
}
{
	stepsum+=$3;
	
	if(NR%150==0) {
		i++;
		stepsum/=150;
		printf i","stepsum;
		print "";
		stepsum=0;
	}
}
END {}
' >> $DATADIR/g19_lab05data_plot_02.csv
