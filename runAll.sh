#!/bin/bash
if ! test -d out; then 
    mkdir out
else 
    rm out/*
fi
for filename in in/*; do
    echo $filename
    ./Pensje < $filename > "out/$(basename ${filename%.*}).out"
done
