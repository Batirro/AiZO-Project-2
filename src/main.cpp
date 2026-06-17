#include <iostream>
#include "Parameters.h"

// Help
static void printHelp() {
    std::cout << "Uzycie: ./AizoProject2 [parametry]\n" << std::endl;
    std::cout << "Pomoc (-h / --help):" << std::endl;
    std::cout << "Tryby dzialania (-m / --mode):" << std::endl;
    std::cout << "  singleFile  - Wczytaj graf z pliku i rozwiaz problem" << std::endl;
    std::cout << "  benchmark   - Przeprowadz badania wydajnosci" << std::endl;
    std::cout << "Problemy (-p / --problem):" << std::endl;
    std::cout << "  mst         - Minimalne drzewo rozpinajace" << std::endl;
    std::cout << "  sp          - Najkrotsza sciezka" << std::endl;
    std::cout << "  mf          - Maksymalny przeplyw (niezaimplementowany)\n" << std::endl;
    std::cout << "Algorytmy (-a / --algorithm):" << std::endl;
    std::cout << "  allAlgorithms - Wszystkie algorytmy dla danego problemu" << std::endl;
    std::cout << "  prim          - Algorytm Prima (MST)" << std::endl;
    std::cout << "  kruskal       - Algorytm Kruskala (MST)" << std::endl;
    std::cout << "  dijkstra      - Algorytm Dijkstry (SP)" << std::endl;
    std::cout << "  bellmanFord   - Algorytm Forda-Bellmana (SP)\n" << std::endl;
    std::cout << "Struktury (-s / --structure):" << std::endl;
    std::cout << "  allStructures   - Obie struktury" << std::endl;
    std::cout << "  incidenceMatrix - Macierz incydencji" << std::endl;
    std::cout << "  adjacencyList   - Lista sasiedztwa\n" << std::endl;
    std::cout << "Parametry trybu singleFile:" << std::endl;
    std::cout << "  -f <plik>        - Plik wejsciowy z grafem" << std::endl;
    std::cout << "  --vs <wierzch>   - Wierzcholek startowy (SP)" << std::endl;
    std::cout << "  --ve <wierzch>   - Wierzcholek koncowy (SP)\n" << std::endl;
    std::cout << "Parametry trybu benchmark:" << std::endl;
    std::cout << "  -n <liczba>      - Liczba wierzcholkow" << std::endl;
    std::cout << "  -d <gestosc>     - Gestosc grafu w % (25/50/75/99)" << std::endl;
    std::cout << "  -i <iteracje>    - Liczba powtorzen (domyslnie 50)" << std::endl;
    std::cout << "  -o <plik>        - Plik wyjsciowy CSV z wynikami\n" << std::endl;
    std::cout << "Przykladowe wywolania:" << std::endl;
    std::cout << "  ./AizoProject2 -m singleFile -p mst -a prim -s adjacencyList -f graf.txt" << std::endl;
    std::cout << "  ./AizoProject2 -m benchmark -p sp -a allAlgorithms -s allStructures -n 100 -d 50 -i 50 -o wyniki.csv" << std::endl;
    std::cout << "  ./AizoProject2 -m help" << std::endl;
}

int main(int argc, char* argv[]) {
    Parameters::readParameters(argc - 1, argv + 1);

    switch (Parameters::runMode) {
        case Parameters::RunModes::help:
            printHelp();
            break;
        default:
            std::cout << "Nie podano trybu. Uzyj -m help aby zobaczyc pomoc." << std::endl;
            printHelp();
            break;
    }
    return 0;
}
