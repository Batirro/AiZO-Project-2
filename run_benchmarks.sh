#!/bin/bash
BUILD=~/aizo_project_2/build
cd $BUILD

ITERATIONS=50
SIZES="10 50 100 200 500"
DENSITIES="25 50 75 99"
MST_RESULTS="mst_results.csv"
SP_RESULTS="sp_results.csv"

# Algorytmy MST: 1=prim, 2=kruskal
MST_ALGORITHMS="1 2"
# Algorytmy SP:  3=dijkstra, 4=bellmanFord
SP_ALGORITHMS="3 4"
# Struktury: 1=incidenceMatrix, 2=adjacencyList
STRUCTURES="1 2"

echo "=== MST ==="
for alg in $MST_ALGORITHMS; do
  for struct in $STRUCTURES; do
    for n in $SIZES; do
      for d in $DENSITIES; do
        echo "MST alg=$alg struct=$struct n=$n d=$d"
        ./AizoProject2 --benchmark --problem 0 --algorithm $alg --structure $struct \
          --vertexCount $n --density $d --iterations $ITERATIONS \
          --resultsFile "$MST_RESULTS"
      done
    done
  done
done

echo "=== SP ==="
for alg in $SP_ALGORITHMS; do
  for struct in $STRUCTURES; do
    for n in $SIZES; do
      for d in $DENSITIES; do
        echo "SP alg=$alg struct=$struct n=$n d=$d"
        ./AizoProject2 --benchmark --problem 1 --algorithm $alg --structure $struct \
          --vertexCount $n --density $d --iterations $ITERATIONS \
          --resultsFile "$SP_RESULTS"
      done
    done
  done
done

echo "Gotowe! Wyniki w ~/aizo_project_2/data/"
