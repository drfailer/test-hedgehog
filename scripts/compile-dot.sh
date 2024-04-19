#!/usr/bin/env bash

DIR=.
if [ $# -eq 1 ]; then
    DIR=$1
fi

if [ ! -d $DIR/img ]; then
    mkdir -p $DIR/img
fi

for file in $DIR/*.dot; do
    dot -Tpng $file -o $(echo $file | sed 's/\.dot/\.png/')
done
mv $DIR/*.png $DIR/img
