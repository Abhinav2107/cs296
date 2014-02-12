
#!/bin/bash
DATADIR=../data
rm -f $DATADIR/g19_lab05data_plot.csv
cat $DATADIR/g19_lab05data_01.csv | awk '
BEGIN {
	stepsum=0
	colsum=0
	velsum=0
	possum=0
	loopsum=0
	maxstep=0
	minstep=1000
	FS=",";
}
{
	stepsum+=$3;
	colsum+=$4;
	velsum+=$5;
	possum+=$6;
	loopsum+=$7;
	
	if (maxstep < $3)
	maxstep=$3;
	if (minstep > $3)
	minstep=$3;

	if(NR%150==0) {
		stepsum/=150;
		colsum/=150;
		velsum/=150;
		possum/=150;
		loopsum/=150;
		printf NR","stepsum","colsum","velsum","possum","loopsum","minstep","maxstep;
		print "";
		stepsum=0;
		colsum=0;
		velsum=0;
		possum=0;
		loopsum=0;
		maxstep=0;
		minstep=1000;
	}
}
END {}
' >> $DATADIR/g19_lab05data_plot.csv
