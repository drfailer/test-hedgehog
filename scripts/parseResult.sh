#!/usr/bin/env bash

DIR=.
if [ $# -eq 1 ]; then
    DIR=$1
fi

# duration times
for file in $DIR/*.dot; do
    printf "%s: " $file
    grep "Execution duration:" $file | awk -F'\\\\n' '{ print $2 }'
done
