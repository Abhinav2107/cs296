#!/bin/bash
cd src
if [  $1 -eq 1 ]
then
	for file in $(ls *.cpp)
		do
			mv $file $file.bak
		done
fi
if [ $1 -eq 2 ]
then
	ls *.cpp | xargs -I{} mv {} {}.bak
fi
