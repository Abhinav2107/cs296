#!/bin/bash
if [  $1 -eq 1 ]
then
	tar pczf src.tar.gz src
fi
if [ $1 -eq 2 ]
then
	mkdir srcgzip
	for file in $(ls src)
		do
			gzip -cf src/$file > srcgzip/$file.gz
		done
	tar pcf src.tar srcgzip
	rm -rf srcgzip
fi
