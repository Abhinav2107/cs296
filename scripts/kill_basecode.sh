#!/bin/bash

proc=$(ps aux | grep cs296_19_exe)
pid=$(echo $proc | awk '{print $2}')
user=$(echo $proc | awk '{print $1}')
curr=$(id -u -n)
running=$(ps -p $pid | grep $pid)
if [[ -n $running ]]
then
	
	echo "User: $user"
	echo "PID: $pid"
    if [[ "$curr" = "$user" ]]
    then
		echo "Do you want to kill the process y/n? "
		read -n 1 -s key
		if [[ $key = y || $key = Y ]]
		then
			kill $pid
		fi
	else
		echo "You don't have the permission to terminate this process"
	fi
else
	echo "Base Code not running"
fi
