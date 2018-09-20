#!/bin/bash

if [ "$2" = "-kmeans" ]; then
	./kmeans $3 $4
elif [ "$2" = "-dbscan" ]; then
 	./dbscan $3 $4 $5
elif [ "$2" = "-optics" ]; then
	python3 optics.py $5 $4 $3
else
	echo "Incorrect usage"
fi
#sh <RollNo>.sh <inputfile> -plot
