//
//  TravCount.cpp
//  myKconf
//
//  Created by david on 16/05/2018.
//  Copyright © 2018 david. All rights reserved.
//

#include "cuddAlgo.hpp"
int verbose = 0;

void getProducts(int plevel, int tlevel, int elevel, const mpz_class& tr, const mpz_class& er, mpz_class& thenPart, mpz_class& elsePart) {
    // thenPart = 2 ^ (tlevel-plevel-1) * tr
    mpz_ui_pow_ui(thenPart.get_mpz_t(), 2, tlevel - plevel -1);
    thenPart *= tr;
    
    // elsePart = 2 ^(elevel - plevel -1) * er
    mpz_ui_pow_ui(elsePart.get_mpz_t(), 2, elevel - plevel -1);
    elsePart *= er;
}

mpz_class _count(int plevel, int tlevel, int elevel, const mpz_class& tr, const mpz_class& er) {
    mpz_class thenPart, elsePart;
    getProducts(plevel, tlevel, elevel, tr, er, thenPart, elsePart);
    
    return (thenPart + elsePart);
}

mpz_class count(int xverbose, cuddAdapter* a) {
    return traverse(xverbose, a, mpz_class(0), mpz_class(1), &_count);
}

mpz_class countMP(int threads, cuddAdapter* a, int xverbose, bool fast) {
    return traverseMP(threads, xverbose, a, mpz_class(0), mpz_class(1), &_count, fast);
}



std::vector<int> levelNodes;

int _combLevelNodes(int plevel, int tlevel, int elevel,
                    const int& tr,
                    const int& er) {
    levelNodes[plevel]++;
    return 0;
}

std::vector<int> getLevelNodes(cuddAdapter* a) {
    levelNodes.resize(a->getNumVars());
    for(int i = 0; i < a->getNumVars(); i++)
        levelNodes[i] = 0;
    traverse(a, 0, 0, _combLevelNodes, false);
    return levelNodes;
}

std::set<int> numLooseVars;
std::map<int, bool> looseMap;
bool _looseVars(int plevel, int tlevel, int elevel, const bool& tr, const bool& er) {
    if (tr && tlevel > plevel+1)
        for(int x = plevel+1; x < tlevel; x++) {
            numLooseVars.insert(x);
        }
    if (er && elevel > plevel+1)
        for(int x = plevel+1; x < elevel; x++) {
            numLooseVars.insert(x);
        }
    if (tr == er) {
        numLooseVars.insert(plevel);
    }
    else {
        // If it's not in the map... we put it
        if (looseMap.find(plevel) == looseMap.end())
            looseMap[plevel] = tr;
        else {
            // If it is sometimes true and sometimes false... it is loose
            if (tr != looseMap[plevel])
                numLooseVars.insert(plevel);
        }
    }
    return true;
}

std::set<std::string>          computeOneSolution(cuddAdapter* a, int verbose) {
    traverse(verbose, a, false, true, _looseVars);
    std::set<std::string> looseVars;
    for(auto x : numLooseVars)
        looseVars.insert(a->getVarName(a->varAtPos(x)));
    return looseVars;
}



