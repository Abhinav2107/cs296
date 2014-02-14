#!/bin/bash
echo $(ls -R $1 | grep -v : | grep -e '^$' -v | wc -l) files and directories
echo
echo Executable Files
echo
find $1 -type f -executable
