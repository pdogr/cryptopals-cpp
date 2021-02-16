#!/bin/bash
dir=$1

for i in `ls $dir/*.cpp`; do
	echo $i;
done
