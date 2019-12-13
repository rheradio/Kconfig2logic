#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

#include "mytime.hpp"
#include "cuddAdapter.hpp"
#include "SemanticsCudd.hpp"
#include "synExp.hpp"
#include "cuddAlgo.hpp"


int main(int argc, char** argv) {
    if (argc < 2 || argc > 5) {
        std::ostringstream ost;
        ost << "counter: Counts the number of solutions in a BDD" << std::endl;
        ost << "Usage: " << argv[0] << " [-v] [-fast] [ -t numthreads] <bdd file>" << std::endl;
        std::cerr << ost.str();
        exit(-1);
    }
    cuddAdapter *adapter = new cuddAdapter();
    int i = 1;
    bool verbose = 0;
    bool fast = false;
    int  nt = 0;
    while (i < argc-1) {
        if (std::string(argv[i]) == "-fast") {
            fast = true;
            i++;
            continue;
        }
        if (std::string(argv[i]) == "-v") {
            verbose = 1;
            i++;
            continue;
        }
        if (std::string(argv[i]) == "-t") {
            nt = atoi(argv[i+1]);
            i = i+2;
            continue;
        }
    }
    adapter->readBDD(argv[i], "");

    if (nt == 0) {
        std::cout << count(verbose, adapter) << std::endl;
    }
    else {
        std::cout << countMP(nt, adapter, verbose, fast) << std::endl;
    }
}
