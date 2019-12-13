//
//  deepOrder.cpp
//  myKconf
//
//  Created by david on 20/12/2016.
//  Copyright © 2016 david. All rights reserved.
//
#include <iostream>
#include <fstream>
#include "synExpDriver.hpp"

std::vector<synExp*> expList;

void readExp(std::string expFile) {
    // We read the variables in order
    std::ifstream ef(expFile.c_str());
    std::string temp;

    // We parse the expression file
    kconf::synExpDriver driver;
    driver.parse_file(expFile);
    expList  = driver.getConstraints();
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "extractVariables: Reads an expression file and writes the variables in .var format" << std::endl;
        std::cerr << argv[0] << ": <expression file>" << std::endl;
        exit(-1);
    }
    readExp(argv[1]);
    std::set<std::string> vars;
    for(synExp *s : expList) {
        for(auto x :  s->giveSymbols())
            vars.insert(x);
     }
    
    for(auto ss : vars)
        std::cout << ss << " ";
}
