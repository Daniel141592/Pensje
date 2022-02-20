#!/bin/bash

for filename in out/*; do
    diff -s $filename "out-wzor/$(basename $filename)"
done
