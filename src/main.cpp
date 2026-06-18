#include <iostream>
#include "Parameters.h"
#include "Parameters.h"
#include "AdjacencyList.hpp"
#include "IncidenceMatrix.hpp"
#include "GraphGenerator.hpp"
#include "GraphLoader.hpp"
#include "PrimMST.hpp"
#include "KruskalMST.hpp"
#include "DijkstraSP.hpp"
#include "BellmanFordSP.hpp"

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
    std::cout << "  -n <liczba>      - Generuj graf o podanej liczbie wierzcholkow" << std::endl;
    std::cout << "  -d <gestosc>     - Gestosc grafu w % (np. 50)" << std::endl;
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

static void runSingleFile() {
    bool directed = (Parameters::problem == Parameters::Problems::sp);

    bool doList   = (Parameters::structure == Parameters::Structures::adjacencyList
                     || Parameters::structure == Parameters::Structures::allStructures
                     || Parameters::structure == Parameters::Structures::undefined);
    bool doMatrix = (Parameters::structure == Parameters::Structures::incidenceMatrix
                     || Parameters::structure == Parameters::Structures::allStructures
                     || Parameters::structure == Parameters::Structures::undefined);

    auto runOnGraph = [&](bool isList) {
        Graph* graph = nullptr;

        if (!Parameters::inputFile.empty()) {
            // Wczytaj z pliku
            graph = GraphLoader::loadFromFile(
                Parameters::inputFile.c_str(), isList, directed);
            if (!graph) return;
        } else if (Parameters::vertexCount > 0) {
            // Generuj losowy graf
            float density = Parameters::density > 0
                            ? Parameters::density / 100.0f : 0.5f;
            GraphGenerator gen(Parameters::vertexCount, density, directed);
            graph = gen.generate(isList);
            std::cout << "\nWygenerowano graf: "
                      << graph->getVerticesCount() << " wierzcholkow, "
                      << graph->getEdgesCount() << " krawedzi, "
                      << "gestosc=" << (Parameters::density > 0 ? Parameters::density : 50)
                      << "%, " << (directed ? "skierowany" : "nieskierowany") << std::endl;
            // Zapis do pliku
            if (!Parameters::outputFile.empty()) {
                std::string dataPath = std::string(PROJECT_DATA_DIR) + "/" + Parameters::outputFile;
                gen.saveToFile(graph, dataPath.c_str());
                std::cout << "Graf zapisany do: " << dataPath << std::endl;
            }
        } else {
            std::cerr << "Blad: Podaj plik (-f) lub liczbe wierzcholkow (-n)" << std::endl;
            return;
        }

        std::cout << "Reprezentacja: "
                  << (isList ? "Lista sasiedztwa" : "Macierz incydencji") << std::endl;
        graph->print();

        // MST
        if (Parameters::problem == Parameters::Problems::mst
            || Parameters::problem == Parameters::Problems::undefined) {

            auto algo = Parameters::algorithm;
            if (algo == Parameters::Algorithms::prim
                || algo == Parameters::Algorithms::allAlgorithms
                || algo == Parameters::Algorithms::undefined) {
                std::cout << "\n--- Algorytm Prima ---" << std::endl;
                MSTResult r = PrimMST::findMST(graph);
                PrimMST::printMST(r);
            }
            if (algo == Parameters::Algorithms::kruskal
                || algo == Parameters::Algorithms::allAlgorithms
                || algo == Parameters::Algorithms::undefined) {
                std::cout << "\n--- Algorytm Kruskala ---" << std::endl;
                MSTResult r = KruskalMST::findMST(graph);
                KruskalMST::printMST(r);
            }
        }

        // SP
        if (Parameters::problem == Parameters::Problems::sp) {
            int vs = Parameters::vertexStart;
            int ve = Parameters::vertexEnd;
            int V  = graph->getVerticesCount();

            if (vs < 0 || vs >= V || ve < 0 || ve >= V) {
                std::cerr << "Blad: Podaj poprawne wierzcholki (--vs i --ve) w zakresie 0-"
                          << V - 1 << std::endl;
                delete graph;
                return;
            }

            auto algo = Parameters::algorithm;
            if (algo == Parameters::Algorithms::dijkstra
                || algo == Parameters::Algorithms::allAlgorithms
                || algo == Parameters::Algorithms::undefined) {
                std::cout << "\n--- Algorytm Dijkstry ---" << std::endl;
                DijkstraSP dijkstra;
                SPResult r = dijkstra.findSP(graph, vs);
                dijkstra.printSP(r, vs, ve, V);
            }
            if (algo == Parameters::Algorithms::bellmanFord
                || algo == Parameters::Algorithms::allAlgorithms
                || algo == Parameters::Algorithms::undefined) {
                std::cout << "\n--- Algorytm Forda-Bellmana ---" << std::endl;
                BellmanFordSP bellman;
                SPResult r = bellman.findSP(graph, vs);
                bellman.printSP(r, vs, ve, V);
            }
        }

        delete graph;
    };

    if (doList)   runOnGraph(true);
    if (doMatrix) runOnGraph(false);
}

int main(int argc, char* argv[]) {
    Parameters::readParameters(argc - 1, argv + 1);
    std::cout << "DATA DIR: " << PROJECT_DATA_DIR << std::endl;

    switch (Parameters::runMode) {
        case Parameters::RunModes::help:
            printHelp();
            break;
        case Parameters::RunModes::singleFile:
            runSingleFile();
            break;
        default:
            std::cout << "Nie podano trybu. Uzyj -m help aby zobaczyc pomoc." << std::endl;
            printHelp();
            break;
    }
    return 0;
}
