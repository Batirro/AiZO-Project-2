#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <limits>
#include <filesystem>


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

    std::cout << "Tryby dzialania:" << std::endl;
    std::cout << "  --singleFile   Wczytaj graf z pliku lub wygeneruj i rozwiaz problem" << std::endl;
    std::cout << "  --benchmark    Przeprowadz badania wydajnosci" << std::endl;
    std::cout << "  --help         Wyswietl te pomoc\n" << std::endl;

    std::cout << "Problemy (--problem <id>):" << std::endl;
    std::cout << "  0  mst   Minimalne drzewo rozpinajace" << std::endl;
    std::cout << "  1  sp    Najkrotsza sciezka\n" << std::endl;

    std::cout << "Algorytmy (--algorithm <id>):" << std::endl;
    std::cout << "  0  allAlgorithms  Wszystkie algorytmy dla danego problemu" << std::endl;
    std::cout << "  1  prim           Algorytm Prima (MST)" << std::endl;
    std::cout << "  2  kruskal        Algorytm Kruskala (MST)" << std::endl;
    std::cout << "  3  dijkstra       Algorytm Dijkstry (SP)" << std::endl;
    std::cout << "  4  bellmanFord    Algorytm Forda-Bellmana (SP)\n" << std::endl;

    std::cout << "Struktury (--structure <id>):" << std::endl;
    std::cout << "  0  allStructures    Obie struktury" << std::endl;
    std::cout << "  1  incidenceMatrix  Macierz incydencji" << std::endl;
    std::cout << "  2  adjacencyList    Lista sasiedztwa\n" << std::endl;

    std::cout << "Parametry trybu singleFile:" << std::endl;
    std::cout << "  --inputFile  <plik>    Wczytaj graf z pliku" << std::endl;
    std::cout << "  --outputFile <plik>    Zapisz wyniki do data/<plik>" << std::endl;
    std::cout << "  --vertexCount <n>      Generuj graf o n wierzcholkach" << std::endl;
    std::cout << "  --density <d>          Gestosc grafu w % (np. 50)" << std::endl;
    std::cout << "  --vertexStart <v>      Wierzcholek startowy (SP)" << std::endl;
    std::cout << "  --vertexEnd <v>        Wierzcholek koncowy (SP)\n" << std::endl;

    std::cout << "Parametry trybu benchmark:" << std::endl;
    std::cout << "  --vertexCount <n>      Liczba wierzcholkow" << std::endl;
    std::cout << "  --density <d>          Gestosc grafu w % (25/50/75/99)" << std::endl;
    std::cout << "  --iterations <i>       Liczba powtorzen (domyslnie 50)" << std::endl;
    std::cout << "  --resultsFile <plik>   Plik CSV z wynikami w data/<plik>\n" << std::endl;

    std::cout << "Przykladowe wywolania:" << std::endl;
    std::cout << "  ./AizoProject2 --singleFile --problem 0 --algorithm 1 --structure 2 --vertexCount 8 --density 50" << std::endl;
    std::cout << "  ./AizoProject2 --singleFile --problem 0 --algorithm 1 --structure 2 --inputFile graf.txt" << std::endl;
    std::cout << "  ./AizoProject2 --singleFile --problem 1 --algorithm 3 --structure 2 --vertexCount 8 --density 50 --vertexStart 0 --vertexEnd 7" << std::endl;
    std::cout << "  ./AizoProject2 --benchmark --problem 0 --algorithm 0 --structure 0 --vertexCount 100 --density 50 --iterations 50 --resultsFile wyniki.csv" << std::endl;
}

static std::string currentTimestamp() {
    auto now = std::time(nullptr);
    std::tm tm_buf{};
    localtime_r(&now, &tm_buf);
    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}



static void writeMSTToFile(std::ofstream& f, const MSTResult& r) {
    if (!r.success) {
        f << "MST nie znalezione lub graf nie jest spojny\n\n";
        return;
    }
    f << "Calkowity koszt: " << r.totalCost << "\n";
    f << "Krawedzie MST:\n";
    for (int i = 0; i < r.edges.getSize(); i++) {
        const Edge& e = r.edges[i];
        f << "  " << e.start << " -- " << e.end << " [waga: " << e.weight << "]\n";
    }
    f << "\n";
}

static void writeSPToFile(std::ofstream& f, const SPResult& r, int vs, int ve, int V) {
    if (!r.success) {
        f << "SP nie znalezione\n\n";
        return;
    }
    int cost = r.dist[ve];
    if (cost == std::numeric_limits<int>::max()) {
        f << "Brak sciezki z " << vs << " do " << ve << "\n\n";
        return;
    }
    int* path = new int[V];
    int pathLen = 0;
    int cur = ve;
    while (cur != -1) {
        path[pathLen++] = cur;
        cur = r.parent[cur];
    }
    f << "Koszt: " << cost << "\n";
    f << "Sciezka: ";
    for (int i = pathLen - 1; i >= 0; i--) {
        f << path[i];
        if (i > 0) f << " -> ";
    }
    f << "\n\n";
    delete[] path;
}

static void runSingleFile() {
    (void)std::filesystem::create_directories(PROJECT_DATA_DIR);

    bool directed = (Parameters::problem == Parameters::Problems::sp);

    bool doList   = (Parameters::structure == Parameters::Structures::adjacencyList
                     || Parameters::structure == Parameters::Structures::allStructures
                     || Parameters::structure == Parameters::Structures::undefined);
    bool doMatrix = (Parameters::structure == Parameters::Structures::incidenceMatrix
                     || Parameters::structure == Parameters::Structures::allStructures
                     || Parameters::structure == Parameters::Structures::undefined);

    if (!Parameters::outputFile.empty()) {
        std::string outPath = std::string(PROJECT_DATA_DIR) + "/" + Parameters::outputFile;
        std::ofstream outInit(outPath, std::ios::out);
        if (!outInit) {
            std::cerr << "Blad: Nie mozna utworzyc pliku wynikowego: " << outPath << std::endl;
        }
        outInit.close();
    }

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
        } else {
            std::cerr << "Blad: Podaj plik (-f) lub liczbe wierzcholkow (-n)" << std::endl;
            return;
        }

        std::ofstream outFile;
        if (!Parameters::outputFile.empty()) {
            std::string outPath = std::string(PROJECT_DATA_DIR) + "/" + Parameters::outputFile;
            outFile.open(outPath, std::ios::app);
            if (!outFile) {
                std::cerr << "Blad: Nie mozna otworzyc pliku wynikowego: " << outPath << std::endl;
            }
        }

        std::cout << "Reprezentacja: "
                  << (isList ? "Lista sasiedztwa" : "Macierz incydencji") << std::endl;

        if (outFile.is_open()) {
            outFile << "Reprezentacja: "
                    << (isList ? "Lista sasiedztwa" : "Macierz incydencji") << " ===\n";
            std::ostringstream graphOss;
            std::streambuf* oldBuf = std::cout.rdbuf(graphOss.rdbuf());
            graph->print();
            std::cout.rdbuf(oldBuf);
            std::string graphStr = graphOss.str();
            std::cout << graphStr;
            outFile << graphStr << "\n";
        } else {
            graph->print();
        }

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
                if (outFile.is_open()) {
                    outFile << "--- Algorytm Prima ---\n";
                    writeMSTToFile(outFile, r);
                }
            }
            if (algo == Parameters::Algorithms::kruskal
                || algo == Parameters::Algorithms::allAlgorithms
                || algo == Parameters::Algorithms::undefined) {
                std::cout << "\n--- Algorytm Kruskala ---" << std::endl;
                MSTResult r = KruskalMST::findMST(graph);
                KruskalMST::printMST(r);
                if (outFile.is_open()) {
                    outFile << "--- Algorytm Kruskala ---\n";
                    writeMSTToFile(outFile, r);
                }
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
                if (outFile.is_open()) {
                    outFile << "--- Algorytm Dijkstry ---\n";
                    writeSPToFile(outFile, r, vs, ve, V);
                }
            }
            if (algo == Parameters::Algorithms::bellmanFord
                || algo == Parameters::Algorithms::allAlgorithms
                || algo == Parameters::Algorithms::undefined) {
                std::cout << "\n--- Algorytm Forda-Bellmana ---" << std::endl;
                BellmanFordSP bellman;
                SPResult r = bellman.findSP(graph, vs);
                bellman.printSP(r, vs, ve, V);
                if (outFile.is_open()) {
                    outFile << "--- Algorytm Forda-Bellmana ---\n";
                    writeSPToFile(outFile, r, vs, ve, V);
                }
            }
        }

        delete graph;
    };

    if (doList)   runOnGraph(true);
    if (doMatrix) runOnGraph(false);
}

static void runBenchmark() {
    (void)std::filesystem::create_directories(PROJECT_DATA_DIR);

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

        std::ifstream checkCsv(outFile);
        bool needsHeader = !checkCsv.good();
        checkCsv.close();

        std::ofstream csv(outFile, std::ios::app);
        if (!csv) {
            std::cerr << "Blad: Nie mozna otworzyc pliku: " << outFile << std::endl;
            return;
        }
        if (needsHeader) {
            csv << "timestamp,problem,algorithm,structure,vertices,density_pct,iteration,time_us\n";
        }

        std::string ts = currentTimestamp();

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

            csv << ts << "," << probName << "," << algoName << ","
                << structName << "," << vertices << "," << densityPct << ","
                << i << "," << us << "\n";

            delete g;
        }

        long long avgUs = total / iters;

        std::cout << "[" << ts << "] " << algoName << " / " << structName
                  << " | avg=" << avgUs << "us  min=" << minUs << "us  max=" << maxUs << "us" << std::endl;
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
