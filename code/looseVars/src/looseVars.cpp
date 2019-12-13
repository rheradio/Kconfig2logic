//
//  commonality.cpp
//  myKconf
//
//  Created by David Fernandez Amoros on 19/06/2018.
//  Copyright © 2018 david. All rights reserved.
//

#include <stdio.h>
#include "cuddAlgo.hpp"
#include "cuddAdapter.hpp"

int main(int argc, char** argv) {
    if (argc < 2 || argc > 5) {
        std::ostringstream ost;
        ost << "looseVars: Computes the loose variables in a BDD, i.e. variables that are sometimes true and sometimes false" << std::endl;
        ost << "Usage: oneSolution [-v] <bdd file>" << std::endl;
        std::cerr << ost.str();
        exit(-1);
    }
    cuddAdapter *adapter = new cuddAdapter();
    bool verbose = 0;
    int i = 1;
    while (i < argc-1) {
        if (std::string(argv[i]) == "-v") {
            verbose = 1;
            i++;
        }
    }
    
    adapter->readBDD(argv[i], "");
    std::set<std::string> looseVars = computeOneSolution(adapter, verbose);
    int max = 0;
    for(auto s : looseVars)
        if (s.length() > max)
            max = s.length();
    for(auto s : looseVars)
       std::cout << "Variable " << std::setw(max+1) << s << " can be true or false " << std::endl;
}
    
