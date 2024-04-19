#!/usr/bin/env bash

BUILD_DIR=.
DOT_DIR=resultGauss/

SIZE_MIN=1000
SIZE_MAX=5000
SIZE_STEP=5000
NB_THREADS_MIN=8
NB_THREADS_MAX=16
NB_THREADS_STEP=2

cd $BUILD_DIR

if [ ! -f ./gauss/gauss ]; then
    echo "Error: gauss program not found."
fi

echo "result directory: " $DOT_DIR
for (( size=$SIZE_MIN; size<=$SIZE_MAX; size+=$SIZE_STEP )); do
    echo "size: " $size
    for (( nbThreads=$NB_THREADS_MIN; nbThreads<=$NB_THREADS_MAX; nbThreads*=$NB_THREADS_STEP )); do
        echo "- threads: " $nbThreads
        ./gauss/gauss -d $DOT_DIR -s $size -t $nbThreads
    done
done
