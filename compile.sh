#!/bin/bash

git clone https://github.com/ankursharma-iitd/clustering-data-mining.git
g++ dbscan.cpp std=c++11 -O3 -o dbscan
g++ kmeans.cpp std=c++11 -O3 -o kmeans

