#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <limits>

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

static std::string currentTimestamp() {
    auto now = std::time(nullptr);
    std::tm tm_buf{};
    localtime_r(&now, &tm_buf);
    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

static void writeCsvRow(const std::string& file, const std::string& timestamp,
                        const std::string& problem, const std::string& algorithm,
                        const std::string& structure, int vertices, int density,
                        long long avgUs, long long minUs, long long maxUs) {
    std::ifstream check(file);
    bool needsHeader = !check.good();
    check.close();

    std::ofstream ofs(file, std::ios::app);
    if (!ofs) {
        std::cerr << "Blad: Nie mozna otworzyc pliku: " << file << std::endl;
        return;
    }
    if (needsHeader) {
        ofs << "timestamp,problem,algorithm,structure,vertices,density_pct,"
               "avg_us,min_us,max_us\n";
    }
    ofs << timestamp << "," << problem << "," << algorithm << ","
        << structure << "," << vertices << "," << density << ","
        << avgUs << "," << minUs << "," << maxUs << "\n";
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

static void runBenchmark() {
    int   vertices   = (Parameters::vertexCount > 0) ? Parameters::vertexCount : 100;
    int   densityPct = (Parameters::density     > 0) ? Parameters::density     : 50;
    int   iters      = (Parameters::iterations  > 0) ? Parameters::iterations  : 50;
    float density    = densityPct / 100.0f;
    bool  directed   = (Parameters::problem == Parameters::Problems::sp
                        || Parameters::problem == Parameters::Problems::mf);

    std::string outFile = Parameters::resultsFile.empty()
                          ? std::string(PROJECT_DATA_DIR) + "/results.csv"
                          : std::string(PROJECT_DATA_DIR) + "/" + Parameters::resultsFile;

    bool doList   = (Parameters::structure == Parameters::Structures::adjacencyList
                     || Parameters::structure == Parameters::Structures::allStructures
                     || Parameters::structure == Parameters::Structures::undefined);
    bool doMatrix = (Parameters::structure == Parameters::Structures::incidenceMatrix
                     || Parameters::structure == Parameters::Structures::allStructures
                     || Parameters::structure == Parameters::Structures::undefined);

    std::cout << "Badanie: n=" << vertices << ", gestosc=" << densityPct
              << "%, iteracje=" << iters << ", plik=" << outFile << std::endl;

    auto measure = [&](const std::string& probName, const std::string& algoName,
                       const std::string& structName, bool useList) {

        long long total = 0;
        long long minUs = std::numeric_limits<long long>::max();
        long long maxUs = 0;

        for (int i = 0; i < iters; i++) {
            GraphGenerator gen(vertices, density, directed);
            Graph* g = gen.generate(useList);
            // Zapis do pliku
            if (i == 0) {
                std::string graphFile = std::string(PROJECT_DATA_DIR) + "/"
                                      + probName + "_"
                                      + algoName + "_"
                                      + structName + "_"
                                      + "n" + std::to_string(vertices)
                                      + "_d" + std::to_string(densityPct)
                                      + ".txt";
                gen.saveToFile(g, graphFile.c_str());
            }

            auto t0 = std::chrono::high_resolution_clock::now();

            if      (algoName == "prim")        { MSTResult r = PrimMST::findMST(g);              (void)r; }
            else if (algoName == "kruskal")     { MSTResult r = KruskalMST::findMST(g);           (void)r; }
            else if (algoName == "dijkstra")    { DijkstraSP d;    SPResult r = d.findSP(g, 0);   (void)r; }
            else if (algoName == "bellmanFord") { BellmanFordSP b; SPResult r = b.findSP(g, 0);   (void)r; }

            auto t1 = std::chrono::high_resolution_clock::now();
            long long us = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();

            total += us;
            if (us < minUs) minUs = us;
            if (us > maxUs) maxUs = us;

            delete g;
        }

        long long avgUs = total / iters;
        std::string ts  = currentTimestamp();

        std::cout << "[" << ts << "] " << algoName << " / " << structName
                  << " | avg=" << avgUs << "us  min=" << minUs << "us  max=" << maxUs << "us" << std::endl;

        writeCsvRow(outFile, ts, probName, algoName, structName,
                    vertices, densityPct, avgUs, minUs, maxUs);
    };

    auto algo = Parameters::algorithm;
    auto prob = Parameters::problem;

    bool runPrim        = (prob == Parameters::Problems::mst || prob == Parameters::Problems::undefined)
                       && (algo == Parameters::Algorithms::prim
                           || algo == Parameters::Algorithms::allAlgorithms
                           || algo == Parameters::Algorithms::undefined);
    bool runKruskal     = (prob == Parameters::Problems::mst || prob == Parameters::Problems::undefined)
                       && (algo == Parameters::Algorithms::kruskal
                           || algo == Parameters::Algorithms::allAlgorithms
                           || algo == Parameters::Algorithms::undefined);
    bool runDijkstra    = (prob == Parameters::Problems::sp)
                       && (algo == Parameters::Algorithms::dijkstra
                           || algo == Parameters::Algorithms::allAlgorithms
                           || algo == Parameters::Algorithms::undefined);
    bool runBellmanFord = (prob == Parameters::Problems::sp)
                       && (algo == Parameters::Algorithms::bellmanFord
                           || algo == Parameters::Algorithms::allAlgorithms
                           || algo == Parameters::Algorithms::undefined);

    if (doList) {
        if (runPrim)        measure("mst", "prim",        "adjacencyList",   true);
        if (runKruskal)     measure("mst", "kruskal",     "adjacencyList",   true);
        if (runDijkstra)    measure("sp",  "dijkstra",    "adjacencyList",   true);
        if (runBellmanFord) measure("sp",  "bellmanFord", "adjacencyList",   true);
    }
    if (doMatrix) {
        if (runPrim)        measure("mst", "prim",        "incidenceMatrix", false);
        if (runKruskal)     measure("mst", "kruskal",     "incidenceMatrix", false);
        if (runDijkstra)    measure("sp",  "dijkstra",    "incidenceMatrix", false);
        if (runBellmanFord) measure("sp",  "bellmanFord", "incidenceMatrix", false);
    }

    std::cout << "Wyniki zapisane do: " << outFile << std::endl;
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
        case Parameters::RunModes::benchmark:
            runBenchmark();
            break;
        default:
            std::cout << "Nie podano trybu. Uzyj -m help aby zobaczyc pomoc." << std::endl;
            printHelp();
            break;
    }
    return 0;
}
