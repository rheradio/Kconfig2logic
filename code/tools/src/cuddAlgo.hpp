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

extern int verbose;



std::set<std::string>             computeOneSolution(cuddAdapter* adapter, int verbose);
mpz_class                         count(int verbose, cuddAdapter* f);
mpz_class                         countMP(int threads, cuddAdapter* f, int verbose, bool fast);


std::vector<int>                  getLevelNodes(cuddAdapter* a);

#endif /* cuddAlgo_hpp */

