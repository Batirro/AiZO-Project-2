#!/bin/bash
BUILD=~/aizo_project_2/build
cd $BUILD

ITERATIONS=50

SIZES_A="10 50 100 200 500"
DENSITY_A=50

SIZE_B=100
DENSITIES_B="25 50 75 99"

# Algorytmy MST: 1=prim, 2=kruskal
MST_ALGORITHMS="1 2"
# Algorytmy SP:  3=dijkstra, 4=bellmanFord
SP_ALGORITHMS="3 4"
# Struktury: 1=incidenceMatrix, 2=adjacencyList
STRUCTURES="1 2"


echo "MST - Badanie A (zmienny rozmiar, gestosc=${DENSITY_A}%)"
for alg in $MST_ALGORITHMS; do
  for struct in $STRUCTURES; do
    for n in $SIZES_A; do
      echo "MST alg=$alg struct=$struct n=$n d=$DENSITY_A"
      ./AizoProject2 --benchmark --problem 0 --algorithm $alg --structure $struct \
        --vertexCount $n --density $DENSITY_A --iterations $ITERATIONS \
        --resultsFile "mst_results_A.csv"
    done
  done
done

echo "SP - Badanie A (zmienny rozmiar, gestosc=${DENSITY_A}%)"
for alg in $SP_ALGORITHMS; do
  for struct in $STRUCTURES; do
    for n in $SIZES_A; do
      echo "SP alg=$alg struct=$struct n=$n d=$DENSITY_A"
      ./AizoProject2 --benchmark --problem 1 --algorithm $alg --structure $struct \
        --vertexCount $n --density $DENSITY_A --iterations $ITERATIONS \
        --resultsFile "sp_results_A.csv"
    done
  done
done

echo "MST - Badanie B (zmienna gestosc, n=${SIZE_B})"
for alg in $MST_ALGORITHMS; do
  for struct in $STRUCTURES; do
    for d in $DENSITIES_B; do
      echo "MST alg=$alg struct=$struct n=$SIZE_B d=$d"
      ./AizoProject2 --benchmark --problem 0 --algorithm $alg --structure $struct \
        --vertexCount $SIZE_B --density $d --iterations $ITERATIONS \
        --resultsFile "mst_results_B.csv"
    done
  done
done

echo "SP - Badanie B (zmienna gestosc, n=${SIZE_B})"
for alg in $SP_ALGORITHMS; do
  for struct in $STRUCTURES; do
    for d in $DENSITIES_B; do
      echo "SP alg=$alg struct=$struct n=$SIZE_B d=$d"
      ./AizoProject2 --benchmark --problem 1 --algorithm $alg --structure $struct \
        --vertexCount $SIZE_B --density $d --iterations $ITERATIONS \
        --resultsFile "sp_results_B.csv"
    done
  done
done

echo "Gotowe! Wyniki w ~/aizo_project_2/data/"
