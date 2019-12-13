//
//  cuddAlgo.hpp
//  myKconf
//
//  Created by david on 16/05/2018.
//  Copyright © 2018 david. All rights reserved.
//

#ifndef cuddAlgo_hpp
#define cuddAlgo_hpp

#include <stdio.h>
#include <sstream>
#include <gmpxx.h>
#include <gmp.h>
#include <set>
#include "cuddInt.h"
#include "cuddAdapter.hpp"
#include "Traverser.hpp"
#include "Objective.hpp"
#include "Distribution.hpp"

extern int verbose;
typedef std::vector<char> partialProduct;
typedef std::pair<std::vector<mpq_class>, std::vector<std::vector<mpq_class>>> coupleCommRes;

extern partialProduct emptyProduct;

std::set<std::string>             positiveRandom(cuddAdapter* adapter);
std::set<std::string>             computeOneSolution(cuddAdapter* adapter, int verbose);
mpz_class                         count(int verbose, cuddAdapter* f);
mpz_class                         countMP(int threads, cuddAdapter* f, int verbose, bool fast);

void                              compProbabilities(int verbose, cuddAdapter* a);
void                              compProbabilitiesMP(int verbose, int threads, cuddAdapter* a, bool fast);

std::vector<bool>                 genRandom(cuddAdapter* a);
std::string                       nameRandom(cuddAdapter *a);
std::string 			  nameStratifiedRandom(cuddAdapter *a, int xones);

mpf_class                         getCommonality(int i);
std::vector<mpf_class>            computeCommonality(int verbose, cuddAdapter *a);
std::vector<mpf_class>            computeCommonalityMP(int num, cuddAdapter *a, int xverbose, bool fast);

coupleCommRes                     computeCoupleCommonality(int verbose, cuddAdapter *a);
coupleCommRes                     computeCoupleCommonalityMP(int num, cuddAdapter *a, int verbose, bool fast);

std::vector<mpz_class>            computeDistribution(int verbose, cuddAdapter* a);
std::vector<mpz_class>            computeDistributionMP(int threads, cuddAdapter* a, int verbose, bool fast);
std::vector<mpz_class>            crappyDistribution(cuddAdapter* a);

std::vector<std::vector<bool>>    generateProducts(int verbose, cuddAdapter *a);
std::string                       nameProduct(cuddAdapter* a, std::vector<bool> v);

std::vector<int>                  getLevelNodes(cuddAdapter* a);
void                              computeImpact(int verbose, cuddAdapter* a);

void computeStratificationProbabilities(int verbose, cuddAdapter* a);
void computeStratificationProbabilitiesMP(int threads, cuddAdapter* a, int verbose,  bool fast);

Distribution                      computePerformanceDistribution(int verbose, int ncomps, Objective obj, std::map<int, Objective*> filteredObjectives,  cuddAdapter* a);
//Distribution                      computePerformanceDistribution(int verbose, Objective obj, std::map<int, Objective*> filteredObjectives,  cuddAdapter* a);
Distribution                      computePerformanceDistributionMP(int threads, bool fast, int verbose, int ncomps, Objective obj, std::map<int, Objective*> filteredObjectives,  cuddAdapter* a);
//Distribution                      computePerformanceDistributionMP(int threads, bool fast, int verbose, Objective obj, std::map<int, Objective*> filteredObjectives,  cuddAdapter* a);

class ucr {
public:
    // How many solutions to the BDD rooted in this node
    mpz_class               counter;
    // For each variable from this level to the bottom, how many
    // solutions with the variable set to true
    std::vector<mpz_class>  products;
    friend  std::ostream&          operator<<(std::ostream& os, const ucr& u);

    
};

class udc {
    public :
    ucr myUcr;
    std::vector<std::vector<mpz_class>> doubleProducts;
    friend  std::ostream&          operator<<(std::ostream& os, const udc& u);

};
#endif /* cuddAlgo_hpp */

