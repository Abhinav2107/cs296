#!/bin/sh
cat ~/.bash_history | cut -d' ' -f1 | sort | uniq -c | sort -nr | head
