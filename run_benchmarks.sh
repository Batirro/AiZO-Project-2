#!/bin/bash
BUILD=~/aizo_project_2/build
cd $BUILD

ITERATIONS=50
SIZES="10 50 100 200 500"
DENSITIES="25 50 75 99"

echo "=== MST ==="
for n in $SIZES; do
  for d in $DENSITIES; do
    echo "MST n=$n d=$d"
    ./AizoProject2 --benchmark --problem 0 --algorithm 0 --structure 0 \
      --vertexCount $n --density $d --iterations $ITERATIONS \
      --resultsFile mst_results.csv
  done
done

echo "=== SP ==="
for n in $SIZES; do
  for d in $DENSITIES; do
    echo "SP n=$n d=$d"
    ./AizoProject2 --benchmark --problem 1 --algorithm 0 --structure 0 \
      --vertexCount $n --density $d --iterations $ITERATIONS \
      --resultsFile sp_results.csv
  done
done

echo "Gotowe! Wyniki w ~/aizo_project_2/data/"
