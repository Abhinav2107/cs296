#!/bin/bash
echo "Today is" $(date +"%A, %d %B, %Y").

abhinav="20140721"
anant="20140602"
stephan="20141212"


function remaining {
hours=$(echo \($(date +%s -d $1) - $(date +%s)\)/3600 | bc)
#echo $hours
if [ $(echo $hours \< 0 | bc) -eq 1 ]
then
hours=$(echo $hours+365*24 | bc)
fi
#echo $hours
weeks=$(echo $hours\/\(24*7\) | bc)
days=$(echo \($hours-\($weeks*24*7\)\)/24 | bc)
hours=$(echo \($hours-\($weeks*24*7\)-\($days*24\)\) |bc)

echo There are $weeks weeks $days days and $hours hours left for the birthday of $2
}

remaining $abhinav abhinav
remaining $anant anant
remaining $stephan stephan

echo Thank you for asking, $(id -u -n)

echo -n "Your system has been running for "
echo $(uptime) | awk '
{
	if ($3 ~ /:/) print $3;
	
	else
	print $3" "$4" "$5;
}' | sed 's/\(.*\),/\1 hours./'

location=$(df /home | awk FNR==2'{print "The current disk on which your home folder is located is "$1" and is "$3*100/($3+$4)"% full"}')
echo $location

echo "You are running $(lsb_release -i | awk '{print $3}') and $(lsb_release -r | awk '{print $2}') with Kernel $(uname -r)"

free | sed -n '2p' | awk '
{
	print "Your machine has "$2/1048576" GB RAM. Of which "($3/$2)*100"% is in use."
}'

words=$(wc -w scripts/myinfo.sh | awk '{print $1}')
lines=$(wc -l scripts/myinfo.sh | awk '{print $1}')
char=$(echo \($(wc -m scripts/myinfo.sh | awk '{print $1}') \- $(sed 's/[^	 ]//g' scripts/myinfo.sh | awk 'BEGIN{sum=0} {sum+=length} END{print sum}')\) | bc)
echo This script has $words words, $lines lines and $char characters \(without counting whitespace\)
