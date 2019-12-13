//
//  TravCount.cpp
//  myKconf
//
//  Created by david on 16/05/2018.
//  Copyright © 2018 david. All rights reserved.
//

#include "cuddAlgo.hpp"
//#include <random>
int verbose = 0;
partialProduct emptyProduct;
// For each node, the probability to reach the true node choosing the then child
std::map<DdNode*, mpf_class>                                        probabilities;
// For each node, the vector of probabilities[i] of reaching the true node
// choosing the then child in a product of i components set to one.
std::map<DdNode*, std::vector<mpf_class>>                           mapVecProb;
std::map<DdNode*, int>                                              mapMin;
std::list<DdNode*>                                                  buffer;
std::vector<std::list<std::pair<short int, short int>>::iterator>   implierUp, implierDown, excluder;
std::list<std::pair<short int, short int>>                          implicationsUp, implicationsDown, exclusions;
std::vector<bool> existProducts;

class Impact {
public:
    // For every level in the subtree, we store if
    // the variable of that level is always true or
    // always false in valid products
    Impact() {};
    std::vector<bool> alwaysFalse, alwaysTrue;
};
int impactcounter = 0;

std::list<std::pair<short int, short int>>::iterator deleteExclusion(
                                                                     std::list<std::pair<short int, short int>>::iterator it) {
    int i = it->first;
    while (i >= 0)
        if (excluder[i] == it)
            excluder[i--]++;
        else
            break;
    return exclusions.erase(it);
}

std::list<std::pair<short int, short int>>::iterator deleteUp(
                                                              std::list<std::pair<short int, short int>>::iterator it) {
    //std::cerr << "In deleteUp " << first << "," << second << std::endl;
    int i = it->first;
    while (i >= 0)
        if (implierUp[i] == it)
            implierUp[i--]++;
        else
            break;
    
    
    return implicationsUp.erase(it);
}

std::list<std::pair<short int, short int>>::iterator deleteDown(
                                                                std::list<std::pair<short int, short int>>::iterator it) {
    int i = it->first;
    while (i >= 0)
        if (implierDown[i] == it)
            implierDown[i--]++;
        else
            break;
    
    return implicationsDown.erase(it);;
}
void processExclusions(int plevel, bool branch, int childlevel, const Impact& childres) {
    std::list<std::pair<short int, short int>>::iterator it = excluder[plevel];
    // Phase 1: plevel & deleted node
    while (it != exclusions.end() && it->first == plevel && it->second < childlevel)
        if (branch)
            it = deleteExclusion(it);
        else
            it++;
    // Phase 2: plevel & not deleted
    while (it != exclusions.end() && it->first == plevel && it->second >= childlevel)
        if (branch && !childres.alwaysFalse[it->second - childlevel])
            it = deleteExclusion(it);
        else
            it++;
    // Phase 3: deleted & deleted
    while (it != exclusions.end() && it->first < childlevel && it->second < childlevel)
        it = deleteExclusion(it);
    // Phase 4: deleted & not deleted
    while (it != exclusions.end() && it->first < childlevel && it->second >= childlevel)
        if (!childres.alwaysFalse[it->second - childlevel])
            it = deleteExclusion(it);
        else
            it++;
}
void processLevelUp(int plevel, bool branch, int childlevel, const Impact& childres) {
    std::list<std::pair<short int, short int>>::iterator it = implierUp[plevel];
    // We only need to check until the childlevel
    // Phase 1: plevel & deleted node
    while (it != implicationsUp.end() && it->first == plevel && it->second < childlevel) {
        if (branch)
            it = deleteUp(it);
        else
            it++;
    }
    // Phase 2: plevel & not deleted
    while (it != implicationsUp.end() && it->first == plevel && it->second >= childlevel)
        // branch && !alwaysTrue -> A->B is false
        if (branch && !childres.alwaysTrue[it->second - childlevel])
            it = deleteUp(it);
        else
            it++;
    
    // Phase 3: deleted & deleted
    while (it != implicationsUp.end() && it->first < childlevel && it->second < childlevel)
        it = deleteUp(it);
    
    // Phase 4: deleted & not deleted
    while (it != implicationsUp.end() && it->first < childlevel && it->second >= childlevel)
        if (!childres.alwaysTrue[it->second - childlevel])
            it = deleteUp(it);
        else
            it++;
}
void processLevelDown(int plevel, bool branch, int childlevel, const Impact& childres) {
    std::list<std::pair<short int, short int>>::iterator it = implierDown[plevel];
    // We only need to check until the childlevel
    // Phase 1: plevel & deleted node
    while (it != implicationsDown.end() && it->first == plevel && it->second < childlevel) {
        if (!branch)
            it = deleteDown(it);
        else
            it++;
    }
    // Phase 2: plevel & not deleted
    while (it != implicationsDown.end() && it->first == plevel && it->second >= childlevel) {
        // branch -> evidence for B->A
        // !branch && alwaysFalse -> evidence for B->A
        // !branch && !alwaysFalse -> B->A is false
        if (!branch && !childres.alwaysFalse[it->second - childlevel])
            it = deleteDown(it);
        else
            it++;
    }
    // Phase 3: deleted & deleted
    while (it != implicationsDown.end() && it->first < childlevel && it->second < childlevel)
        it = deleteDown(it);
    
    // Phase 4: deleted & not deleted
    while (it != implicationsDown.end() && it->first < childlevel && it->second > childlevel)
        if (!childres.alwaysFalse[it->second - childlevel])
            it = deleteDown(it);
        else
            it++;
    
}

Impact _getImpact(int plevel, int tlevel, int elevel,
                  const Impact& tr,
                  const Impact& er) {
    Impact res;
    
    // If else child is zero, the node is always true (in all valid products)
    if (er.alwaysFalse.size() == 1 && er.alwaysFalse[0] == true) {
        res.alwaysTrue.push_back(true);
    }
    else
        res.alwaysTrue.push_back(false);
    
    // If then child is zero, the node is always false (in all valid products)
    if (tr.alwaysFalse.size() == 1 && tr.alwaysFalse[0] == true) {
        res.alwaysFalse.push_back(true);
    }
    else
        res.alwaysFalse.push_back(false);
    
    // We complete res to the rest of the levels
    int max;
    if (res.alwaysTrue[0])
        max = tlevel;
    else
        if (res.alwaysFalse[0])
            max = elevel;
        else {
            if (tlevel > elevel)
                max = tlevel;
            else
                max = elevel;
        }
    // Deleted nodes are neither always false nor always true
    for(int i = plevel+1; i < max ; i++) {
        res.alwaysTrue.push_back(false);
        res.alwaysFalse.push_back(false);
    }
    if (res.alwaysTrue[0])
        for(int i = 0; i < tr.alwaysTrue.size(); i++) {
            res.alwaysTrue.push_back(tr.alwaysTrue[i]);
            res.alwaysFalse.push_back(tr.alwaysFalse[i]);
        }
    else
        if (res.alwaysFalse[0])
            for(int i = 0; i < er.alwaysTrue.size(); i++) {
                res.alwaysTrue.push_back(er.alwaysTrue[i]);
                res.alwaysFalse.push_back(er.alwaysFalse[i]);
            }
        else {
            int itT, itE;
            for(itT = max - tlevel,
                itE = max - elevel;
                itT < tr.alwaysTrue.size() && itE < er.alwaysTrue.size(); itT++,itE++) {
                res.alwaysTrue.push_back(tr.alwaysTrue[itT] && er.alwaysTrue[itE]);
                res.alwaysFalse.push_back(tr.alwaysFalse[itT] && er.alwaysFalse[itE]);
            }
        }
    
    // Does A->B?
    // Does B -> A? I.E Does not A -> not B?
    processLevelUp  (plevel,  true ,  tlevel, tr);
    processLevelDown(plevel,  true ,  tlevel, tr);
    processLevelUp  (plevel,  false,  elevel, er);
    processLevelDown(plevel,  false,  elevel, er);
    processExclusions(plevel, true,   tlevel, tr);
    processExclusions(plevel, false,  elevel, er);
    
    return res;
}

void computeImpactInit(cuddAdapter* a) {
    implierUp.resize(a->getNumVars());
    implierDown.resize(a->getNumVars());
    excluder.resize(a->getNumVars());
    
    for(int i = 0; i < a->getNumVars();i++) {
        for(int j = i+1; j < a->getNumVars();j++) {
            implicationsUp.  push_back(std::pair<short int, short int>(i,j)); // A -> B
            implicationsDown.push_back(std::pair<short int, short int>(i,j)); // A -> B
            exclusions.push_back(std::pair<short int, short int>(i,j));
            
            if (j == i+1) {
                implierUp[i] = implicationsUp.end();
                implierUp[i]--;
                implierDown[i] = implicationsDown.end();
                implierDown[i]--;
                excluder[i] = exclusions.end();
                excluder[i]--;
            }
        }
        implierUp  [a->getNumVars()-1] = implicationsUp  .end();
        implierDown[a->getNumVars()-1] = implicationsDown.end();
        excluder[a->getNumVars()-1] = exclusions.end();
    }
    
}

void showImpact(cuddAdapter* a) {
    int max = 0;
    for(int i = 0; i < a->getNumVars(); i++) {
        if (a->getVarName(i).length() > max)
            max = a->getVarName(i).length();
    }
    
    for(std::pair<short int, short int> p : implicationsUp)
        std::cout << std::setw(max) << a->getVarName(a->varAtPos(p.first)) << " implies "
        << std::setw(max) << a->getVarName(a->varAtPos(p.second)) << std::endl;
    
    for(std::pair<short int, short int> p : implicationsDown)
        std::cout << std::setw(max) << a->getVarName(a->varAtPos(p.second)) << " implies "
        << std::setw(max) << a->getVarName(a->varAtPos(p.first)) << std::endl;
    
    for(std::pair<short int, short int> p : exclusions)
        std::cout << std::setw(max) << a->getVarName(a->varAtPos(p.first)) << " excludes "
        << std::setw(max) << a->getVarName(a->varAtPos(p.second)) << std::endl;
    
    if (implicationsUp.empty() && implicationsDown.empty() && exclusions.empty())
        std::cerr << "Nothing implies or excludes anything" << std::endl;
}

void computeImpact(int verbose, cuddAdapter* a) {
    computeImpactInit(a);
    Impact izero, ione;
    izero.alwaysTrue.push_back(false);
    izero.alwaysFalse.push_back(true);
    ione.alwaysTrue.push_back(true);
    ione.alwaysFalse.push_back(false);
    traverse(verbose, a, izero , ione, _getImpact);
    
    showImpact(a);
    
}

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

int _listNodes(int plevel, int tlevel, int elevel, const int& tr, const int& er, DdNode* node) {
    buffer.push_back(node);
    return 0;
}
mpz_class countMP(int threads, cuddAdapter* a, int xverbose, bool fast) {
    return traverseMP(threads, xverbose, a, mpz_class(0), mpz_class(1), &_count, fast);
}
mpz_class _probabilities(int plevel, int tlevel, int elevel, const mpz_class& tr, const mpz_class& er, DdNode* node) {
    mpz_class thenPart, elsePart;
    getProducts(plevel, tlevel, elevel, tr, er, thenPart, elsePart);
    mpf_class fthen = thenPart;
    mpf_class felse = elsePart;
    probabilities[node] = fthen / (fthen + felse);
    return (thenPart + elsePart);
}

void compProbabilitiesMP(int verbose, int threads, cuddAdapter* a, bool fast) {
    unsigned int temp = (unsigned int) get_cpu_time() % 1000000;
    //cout << "Initializing random seed " << temp << endl;
    srandom(temp);
    traverseMP(threads, verbose, a, mpz_class(0), mpz_class(1), &_probabilities, fast);
}

void compProbabilities(int verbose, cuddAdapter* a) {
    unsigned int temp = (unsigned int) get_cpu_time() % 1000000;
    //cout << "Initializing random seed " << temp << endl;
    srandom(temp);
    traverse(verbose, a, mpz_class(0), mpz_class(1), &_probabilities);
}

bool fiftyfifty() {
    float randNum = (float)random()/(float)RAND_MAX;
    return (randNum < 0.5);
}

std::string nameProduct(cuddAdapter*a, std::vector<bool> v) {
    std::ostringstream ost;
    int var = 0;
    for(bool b : v) {
        std::string nameVar = a->getVarName(var++);
        if (b)
            ost <<  "    " << nameVar << " ";
        else
            ost << "not " << nameVar << " ";
    }
    return ost.str();
}

std::set<std::string> positiveRandom(cuddAdapter* adapter) {
    std::set<std::string> result;
    int var = 0;
    for(bool b : genRandom(adapter)) {
        if (b)
            result.insert(adapter->getVarName(var));
        var++;
    }
    
    return result;
}
    
std::string nameRandom(cuddAdapter *a) {
    std::ostringstream ost;
    return nameProduct(a, genRandom(a));;
}
std::vector<bool> genRandom(cuddAdapter* a) {
    //unsigned int temp = (unsigned int) get_cpu_time() % 1000000;
    //cout << "Initializing random seed " << temp << endl;
    //srandom(temp);
    // We assume the probabilities have been computed already
    DdNode *g1, *g0;
    int index;
    std::vector<bool> exemplar(a->getNumVars());
    DdNode *trav = a->getBDD();
    DdNode *one  = a->getOne();
    DdNode *zero = a->getZero();
    int    pos   = 0;
    if (trav == zero)
        return exemplar;
    
    index = a->getLevel(trav);
    for(int i = 0; i< index; i++)
        exemplar[a->varAtPos(pos++)] = fiftyfifty();
    
    while (trav != one) {
        cuddGetBranches(trav, &g1, &g0);
        float randNum = (float)random()/(float)RAND_MAX;

        if (randNum <= probabilities[trav]) {
            trav = g1;
            exemplar[a->varAtPos(pos++)] = true;
        }
        else {
            exemplar[a->varAtPos(pos++)] = false;
            trav = g0;
        }
        for(int i = index+1; i < a->getLevel(trav); i++)
            exemplar[a->varAtPos(pos++)] = fiftyfifty();
        
        index = a->getLevel(trav);
    }
    return exemplar;
}

std::vector<int> randChoose(int amount, int min, int max) {
    std::vector<int> res;
    std::set<int>    fast;
    // Choose at most amount numbers between min and max
    //std::random_device rd; // obtain a random number from hardware
    //std::mt19937 eng(rd()); // seed the generator
    //std::uniform_int_distribution<> distr(min, max); // define the range
    int counter = 0;
    if (amount > max - min + 1)
        amount = max - min + 1;
    int howmany = std::rand() % (amount+1);
    
    while (counter < howmany) {
        int num = min + std::rand() % (max-min+1);
        if (fast.find(num) == fast.end()) {
            fast.insert(num);
            res.push_back(num);
            counter++;
        }
    }
    return res;
}

std::vector<bool> genStratifiedRandom(cuddAdapter* a,int xones) {
    if (xones < 0 || xones > existProducts.size() || !existProducts.at(xones)) {
        std::ostringstream ost;
        ost << "There are no products with exactly " << xones
        << " components set to true" << std::endl;
        throw std::logic_error(ost.str());
    }
    unsigned int temp = (unsigned int) get_cpu_time() % 1000000;
    //cout << "Initializing random seed " << temp << endl;
    srandom(temp);
    // We assume the probabilities have been computed already
    DdNode *g1, *g0;
    int index;
    std::vector<bool> exemplar(a->getNumVars());
    DdNode *trav = a->getBDD();
    DdNode *one  = a->getOne();
    DdNode *zero = a->getZero();
    
    if (trav == zero)
        return exemplar;
    
    // Looking for the root of the BDD
    // The root has a range of weights for the products
    // i.e., all products have between min and max components
    // set to true. We have to decide how many then paths we
    // are going to take and then randomly decide them.
    index = a->getLevel(trav);
    std::vector<int> theOnes = randChoose(xones-mapMin[trav], 0, index-1);
    xones -= theOnes.size();
    for(int pos : theOnes)
        exemplar[pos] = true;
    
    while (trav != one) {
        cuddGetBranches(trav, &g1, &g0);
        float randNum = (float)random()/(float)RAND_MAX;
        if (randNum < mapVecProb[trav][xones]) {
            trav = g1;
            exemplar[a->getLevel(trav)] = true;
            xones--;
        }
        else {
            exemplar[a->getLevel(trav)] = false;
            trav = g0;
        }
        std::vector<int> theOnes = randChoose(xones-mapMin[trav], a->getLevel(trav)-1, index+1);
        xones -= theOnes.size();
        
        for(int pos : theOnes)
            exemplar[pos] = true;
        
        index = a->getLevel(trav);
    }
    return exemplar;
}

std::string nameStratifiedRandom(cuddAdapter *a, int xones) {
    std::ostringstream ost;
    return nameProduct(a, genStratifiedRandom(a, xones));
}


int distcounter = 1;
int totalNodes  = 0;
int divider     = 0;
std::vector<std::vector<mpz_class>> combinations;
std::vector<mpz_class> powersof2;
std::set<int> levelJump;

mpz_class& getPowerof2(int n) {
    return powersof2[n];
}

void makePowersof2(int n ) {
    powersof2.resize(n+1);
    levelJump.insert(0);
    for(int i : levelJump)
        mpz_ui_pow_ui(powersof2[i].get_mpz_t(), 2, i);
}

mpz_class& getCombinations(int a, int b) {
    //std::cerr << "getCombinations(" << a << ", " << b << ")=";
    if (b < a/2+1) {
        //std::cerr << combinations.at(a).at(b) << std::endl;
        return combinations.at(a).at(b);
    }
    else {
        //std::cerr << combinations.at(a).at(a-b) << std::endl;
        return combinations.at(a).at(a-b);
    }
}
void makeCombinations(int n) {
    std::vector<mpz_class> lastRow, current;
    mpz_class lastNum;
    // 0 over 0
    lastRow.push_back(1);
    combinations.push_back(lastRow);
    lastRow.clear();
    // 1 over 0
    lastRow.push_back(1);
    // 1 over 1
    lastRow.push_back(1);
    combinations.push_back(lastRow);
    
    for(int x = 2; x <= n; x++) {
        if (x % 1000 == 0) std::cerr << "combinations: " << x << std::endl;
        current.clear();
        lastNum = 0;
        for(int k = 0; k < x/2+1; k++) {
            mpz_class trav = getCombinations(x-1, k);
            current.push_back(lastNum+trav);
            //std::cerr << "Computed combinations " << x << "," << k << std::endl;
            lastNum = trav;
        }
        combinations.push_back(current);
        lastRow = current;
        //std::cerr << x << ":" ;
        //for(mpz_class& zz : current)
        //    std::cerr << zz << " ";
        //std::cerr << std::endl;
    }
}
//void makeCombinations(int n) {
//    mpz_class factor   = 1;
//    int multiply       = n;
//    int divide         = 1;
//    int half           = n/2;
//    for(int x = 0; x <= half; x++){
//        combMap[n].push_back(factor);
//        factor = (factor*multiply--)/divide++;
//    }
//    if (n % 2 == 0) half--;
//    for(int x = half;  x >= 0; x--) {
//        combMap[n].push_back(combMap[n][x]);
//    }
//}

// We return the biggest jump.
int  _compLevelJump(int plevel, int tlevel, int elevel,
                    const int& tr,
                    const int& er) {
    int max = -1;
    if (tr > max) max = tr;
    if (er > max) max = er;
    
    if (tr != -1) {
        levelJump.insert(tlevel - plevel - 1);
        if (tlevel - plevel - 1 > max) {
            max = tlevel - plevel - 1;
            //std::cerr << "plevel " << plevel << " tlevel " << tlevel << " tr " << tr << std::endl;
        }
    }
    if (er != -1) {
        levelJump.insert(elevel - plevel - 1);
        if (elevel - plevel - 1 > max) {
            max = elevel - plevel - 1;
            //std::cerr << "plevel " << plevel << " elevel " << elevel << " er " << er << std::endl;
        }
    }
    if (max == -1) max = 0;
    return max;
}


void  distCombineHelper(int plevel, int tlevel, int elevel,
                        const std::vector<mpz_class>& tr,
                        const std::vector<mpz_class>& er,
                        std::vector<mpz_class>& thenBit,
                        std::vector<mpz_class>& elseBit) {
    // Now we compute the distribution of the "else" (low) path.
    
    // If the else node is zero, then we don't do anything
    if (er.size() != 1 || er[0] != 0)
        for(int a = 0; a < elseBit.size(); a++) {
            // We fill theBit one component at a time
            for(int b = 0;  b < elevel - plevel ; b++) {
                // How many different paths (along omitted nodes) are there from
                // the node to the "else" descendant passing through b omitted
                //  "then" arcs? Answer ((elevel - plevel -1) over b)
                // factor is (elevel - plevel -1) over b
                if (a-b >= 0 && a-b < er.size()) {
                    //std::cerr << "1 a " << a << " b " << b << " elseBit[" << a << "] += " << getCombinations(elevel-plevel-1, b)
                    //<< " * " << er[a-b] << " = " << elseBit[a] << std::endl;
                    elseBit[a] += getCombinations(elevel-plevel-1, b) * er[a-b];
                }
            }
        }
    
    // Now we compute the distribution of the "then" (high) path. (almost symmetrical)
    // a starts at 1 because thenBit[0] has to be 0.
    // If the then node is zero, then we don't do anything
    if (tr.size() != 1 || tr[0] != 0)
        for(int a = 1; a < thenBit.size(); a++) {
            // We fill highdif one component at a time
            for(int b = 0; b < tlevel - plevel; b++) {
                // How many different paths (along omitted nodes) are there from
                // the node to the "then" descendant passing through b omitted
                //  "then" arcs? Answer (tlevel - plevel - 1) over b
                // factor is (tlevel - plevel - 1) over b
                if (a-b >= 0 && a-b-1 < tr.size()) {
                    //std::cerr << "2 a " << a << " b " << b << " thenBit[" << a << "] += "
                    //<< getCombinations(tlevel-plevel-1, b) << " * " << tr[a-b-1] << " = " << thenBit[a] << std::endl;
                    thenBit[a] += getCombinations(tlevel-plevel-1, b) * tr[a-b-1];
                }
            }
        }
}

std::vector<mpz_class>  _distCombine(int plevel, int tlevel, int elevel,
                                     const std::vector<mpz_class>& tr,
                                     const std::vector<mpz_class>& er) {
    std::vector<mpz_class> thenBit(tlevel - plevel + tr.size());
    std::vector<mpz_class> elseBit(elevel - plevel + er.size());
    //std::cerr << "plevel " << plevel << " tlevel " << tlevel << " elevel " << elevel << std::endl;
    // Size of the distribution is any of the lowdif or highdif
    // it's the same number
    std::vector<mpz_class> infoDist(elseBit.size());
    distCombineHelper(plevel, tlevel, elevel, tr, er, thenBit, elseBit);
    // Now we combine both;
    for(int l = 0; l < infoDist.size(); l++) {
        // We add the branches
        infoDist[l] = thenBit[l] + elseBit[l];
    }
    return infoDist;
}

std::vector<mpz_class> _probStratification(int plevel, int tlevel, int elevel,
                                           const std::vector<mpz_class>& tr,
                                           const std::vector<mpz_class>& er, DdNode* node) {
    // First, we mimic the distribution function
    std::vector<mpz_class> thenBit(tlevel - plevel + tr.size());
    std::vector<mpz_class> elseBit(elevel - plevel + er.size());
    // Size of the distribution is any of the lowdif or highdif
    // it's the same number
    std::vector<mpz_class> result(elseBit.size());
    distCombineHelper(plevel, tlevel, elevel, tr, er, thenBit, elseBit);
    // Now we combine both;
    int min = -1;
    for(int l = 0; l < result.size(); l++) {
        // We add the branches
        result[l] = thenBit[l] + elseBit[l];
        if (min == -1 && result[l] > 0)
            min = l;
    }
    std::vector<mpf_class> vectProb(result.size());
    // Now, we traverse the distribution...
    for(int i = 0; i < result.size(); i++)
        if (result[i] != 0) {
            mpz_class total = thenBit[i] + elseBit[i];
            vectProb[i] = thenBit[i] / total;
        }
        else vectProb[i] = 0;
    mapVecProb[node] = vectProb;
    mapMin[node]     = min;
    return result;
}


std::vector<mpz_class>  computeDistributionMP(int threads, cuddAdapter* a, int xverbose, bool fast) {
    std::vector<mpz_class> dZero, dOne;
    dZero.resize(1);
    dOne.resize(1);
    dZero[0] = 0;
    dOne[0]  = 1;
    // Jumps to node zero are ignored. But jumps to one are counted.
    int maxJump = traverse(a, -1, 0, _compLevelJump);
    totalNodes = a->nodecount();
    divider    = totalNodes/100;
    //std::cerr << "Coverage " << 100*levelJump.size()/a->getNumVars() << "%" <<  std::endl;
    //std::cerr << "Numvars " << a->getNumVars() << " max jump " << maxJump << std::endl;
    makeCombinations(maxJump);
    return traverseMP(threads, xverbose, a, dZero, dOne, _distCombine, fast);
}

void  computeStratificationProbabilitiesMP(int threads, cuddAdapter* a, int xverbose,  bool fast) {
    std::vector<mpz_class> dZero, dOne;
    dZero.resize(1);
    dOne.resize(1);
    dZero[0] = 0;
    dOne[0]  = 1;
    // Jumps to node zero are ignored. But jumps to one are counted.
    int maxJump = traverse(a, -1, 0, _compLevelJump);
    totalNodes = a->nodecount();
    divider    = totalNodes/100;
    //std::cerr << "Coverage " << 100*levelJump.size()/a->getNumVars() << "%" <<  std::endl;
    //std::cerr << "Numvars " << a->getNumVars() << " max jump " << maxJump << std::endl;
    makeCombinations(maxJump);
    std::vector<mpz_class> result = traverseMP(threads, xverbose, a, dZero, dOne, _probStratification, fast);
    for(mpz_class num : result)
        existProducts.push_back(num > 0);
}

void computeStratificationProbabilities(int xverbose, cuddAdapter* a) {
    std::vector<mpz_class> dZero, dOne;
    dZero.resize(1);
    dOne.resize(1);
    dZero[0] = 0;
    dOne[0]  = 1;
    int maxJump = traverse(a, -1, 0, _compLevelJump);
    totalNodes = a->nodecount();
    divider    = totalNodes/100;
    makeCombinations(maxJump);
    std::vector<mpz_class> result = traverse(xverbose, a, dZero, dOne, _probStratification);
    for(mpz_class num : result)
        existProducts.push_back(num > 0);
}
std::vector<mpz_class> computeDistribution(int xverbose, cuddAdapter* a) {
    std::vector<mpz_class> dZero, dOne;
    dZero.resize(1);
    dOne.resize(1);
    dZero[0] = 0;
    dOne[0]  = 1;
    int maxJump = traverse(a, -1, 0, _compLevelJump);
    totalNodes = a->nodecount();
    divider    = totalNodes/100;
    //std::cerr << "Coverage " << 100*levelJump.size()/a->getNumVars() << "%" <<  std::endl;
    //std::cerr << "Numvars " << a->getNumVars() << " max jump " << maxJump << std::endl;
    makeCombinations(maxJump);
    return traverse(xverbose, a, dZero, dOne, _distCombine);
}

// This is to generate ALL the products in a not very efficient way
std::vector<std::vector<bool>> _genProducts(int plevel, int tlevel, int elevel,
                                            const std::vector<std::vector<bool>>&  tr,
                                            const std::vector<std::vector<bool>>&  er) {
    std::vector<std::vector<bool>>  res, tproducts, eproducts, temp;
    std::vector<bool> oneProduct;
    oneProduct.push_back(true);
    tproducts.push_back(oneProduct);
    for(int i = plevel+1; i < tlevel; i++) {
        for(auto trav : tproducts) {
            std::vector<bool> copyTrue, copyFalse;
            copyTrue  = trav;
            copyFalse = trav;
            copyTrue.push_back(true);
            copyFalse.push_back(false);
            temp.push_back(copyTrue);
            temp.push_back(copyFalse);
        }
        tproducts.clear();
        tproducts = temp;
        temp.clear();
    }
    for(auto x : tproducts)
        for(auto y : tr) {
            std::vector<bool> cp = x;
            for(auto z : y)
                cp.push_back(z);
            res.push_back(cp);
        }
    oneProduct.clear();
    oneProduct.push_back(false);
    eproducts.push_back(oneProduct);
    for(int i = plevel+1; i < elevel; i++) {
        for(auto trav : eproducts) {
            std::vector<bool> copyTrue, copyFalse;
            copyTrue  = trav;
            copyFalse = trav;
            copyTrue.push_back(true);
            copyFalse.push_back(false);
            temp.push_back(copyTrue);
            temp.push_back(copyFalse);
        }
        eproducts.clear();
        eproducts = temp;
        temp.clear();
    }
    for(auto x : eproducts)
        for(auto y : er) {
            std::vector<bool> cp = x;
            for(auto z : y)
                cp.push_back(z);
            res.push_back(cp);
        }
    return res;
}

std::vector<std::vector<bool>> generateProducts(int xverbose, cuddAdapter *a) {
    std::vector<std::vector<bool>> dZero, dOne, res;
    std::vector<bool> single;
    dOne.push_back(single);
    std::vector<std::vector<bool>> disordered =  traverse(xverbose, a, dZero, dOne, _genProducts);
    for(auto old : disordered) {
        std::vector<bool> piece(old.size());
        for(int x = 0; x < old.size(); x++) {
            piece[a->varAtPos(x)] = old[x];
        }
        
        res.push_back(piece);
    }
    return res;
}

std::vector<mpz_class>  crappyDistribution(cuddAdapter* a) {
    std::vector<mpz_class> dist(a->getNumVars()+1);
    std::vector<std::vector<bool>> dZero, dOne, res;
    std::vector<bool> single;
    dOne.push_back(single);
    std::vector<std::vector<bool>> disordered =  traverse(a, dZero, dOne, _genProducts);
    for(std::vector<bool> vb : disordered) {
        int counter = 0;
        for(bool b : vb)
            if (b) counter++;
        dist[counter]++;
    }
    return dist;
}

ucr _upProducts(int plevel, int tlevel, int elevel, const ucr& tr, const ucr& er) {
    ucr res, resT, resE;
    mpz_class thenPart, elsePart;

    if (tr.counter != 0) {
        mpz_class& tempThen = getPowerof2(tlevel - plevel - 1);
        thenPart = tr.counter*tempThen;
        mpz_class halfThen = thenPart/2;
        resT.counter = thenPart;
        resT.products.push_back(thenPart);
        // For reduced nodes to the then child
        for(int i = plevel+1; i < tlevel; i++)
            resT.products.push_back(halfThen);
        
        // temp = 2 ^ (tlevel - plevel -1)
        for(const mpz_class& t : tr.products) {
            resT.products.push_back(t*tempThen);
        }
    }
    
    if (er.counter != 0) {
        mpz_class& tempElse = getPowerof2(elevel - plevel - 1);
        elsePart = er.counter*tempElse;
        mpz_class halfElse = elsePart/2;
        resE.counter = elsePart;
        resE.products.push_back(0);
        for(int i = plevel+1; i < elevel; i++)
            resE.products.push_back(halfElse);
        
        for(const mpz_class& e : er.products) {
            resE.products.push_back(e*tempElse);
        }
    }
    if (thenPart != 0 && elsePart != 0) {
        res.counter = thenPart + elsePart;
        for(int i = 0; i < resT.products.size(); i++)
            res.products.push_back(resT.products[i] + resE.products[i]);
        
        return res;
    }
    if (thenPart == 0)
        return resE;
    
    return resT;
}
void _upDoubleCommon(int plevel, int childLevel, const udc& cr, udc& res) {
    //Now the intersection of the vars in the jump with others
    for(int i = plevel+1; i < childLevel; i++) {
        std::vector<mpz_class> intersection;
        // First the intersection of the vars in the jump with the other vars in the jump
        for(int j = i+1; j < childLevel; j++)
            intersection.push_back(cr.myUcr.counter*getPowerof2(childLevel - j - 1));
        // Now the intersection of the vars in the jump with the rest
        for(auto prod : cr.myUcr.products)
            intersection.push_back(prod*getPowerof2(childLevel - plevel - 2));

        res.doubleProducts.push_back(intersection);
    }
    //std::cerr << "So far, doubleProducts size is " << res.doubleProducts.size() << std::endl;
    // Now the old intersections multiplied by the jump
    //std::cerr << "child DoubleProducts " << cr.doubleProducts.size() << std::endl;
    for(auto doubleprod : cr.doubleProducts) {
        std::vector<mpz_class> temp;
        for(auto num : doubleprod)
            temp.push_back(num*getPowerof2(childLevel - plevel - 1));
        res.doubleProducts.push_back(temp);
    }
}

udc _upDoubleProducts(int plevel, int tlevel, int elevel, const udc& tr, const udc& er) {
    udc result, resT, resE;
    result.myUcr = _upProducts(plevel, tlevel, elevel, tr.myUcr, er.myUcr);

    if (tr.myUcr.counter != 0) {
        // The intersection of variable at plevel and the others is the same as the
        // the number of products

        std::vector<mpz_class> firstIntersection;
        // For reduced nodes to the then child
        for(int i = plevel+1; i < tlevel; i++)
            firstIntersection.push_back(tr.myUcr.counter*getPowerof2(tlevel - plevel - 2));
        
        for(const mpz_class& t : tr.myUcr.products)
              firstIntersection.push_back(t*getPowerof2(tlevel - plevel - 1));
          
        resT.doubleProducts.push_back(firstIntersection);
        _upDoubleCommon(plevel, tlevel, tr, resT);
    }
    
    if (er.myUcr.counter != 0) {
        // First vector filled with a lot of zeroes
        resE.doubleProducts.push_back(std::vector<mpz_class>(result.myUcr.products.size()-1, 0));
        _upDoubleCommon(plevel, elevel, er, resE);
    }
    
    if (tr.myUcr.counter != 0 && er.myUcr.counter != 0) {
        if (resT.doubleProducts.size() != resE.doubleProducts.size()) {
            std::ostringstream ost;
            ost << "Error, resT doubleProducts size is " << resT.doubleProducts.size() << " and resE doubleproducts size is " << resE.doubleProducts.size() << std::endl << "plevel " << plevel << " tlevel " << tlevel << " elevel " << elevel << std::endl;
            throw std::logic_error(ost.str());
        }
        for(int c = 0; c < resT.doubleProducts.size(); c++) {
            std::vector<mpz_class> temp;
            if (resT.doubleProducts[c].size() != resE.doubleProducts[c].size()) {
                std::ostringstream ost;
                ost << "Error, resT doubleProducts[" << c << "] size is " << resT.doubleProducts[c].size() << " and resE doubleproducts[" << c << "] size is " << resE.doubleProducts[c].size() << std::endl << "plevel " << plevel << " tlevel " << tlevel << " elevel " << elevel << std::endl;
                throw std::logic_error(ost.str());
            }
            for(int d = 0; d < resT.doubleProducts[c].size(); d++)
                //temp.push_back(resT.doubleProducts[c][d] + resE.doubleProducts[c][d]);
                temp.push_back(resT.doubleProducts.at(c).at(d) + resE.doubleProducts.at(c).at(d));
            result.doubleProducts.push_back(temp);
        }
        return result;
    }
    if (tr.myUcr.counter != 0) {
        resT.myUcr = result.myUcr;
        return resT;
    }
    resE.myUcr = result.myUcr;
    return resE;
}

std::vector<mpf_class> computeCommonality(int xverbose, cuddAdapter *a) {
    verbose = xverbose;
    std::vector<mpf_class> res(a->getNumVars());
    ucr rZero, rOne, recRes;
    rZero.counter = 0;
    rOne.counter  = 1;
    rZero.products.push_back(0);
    rOne.products.push_back(1);
    // Jumps to node zero are ignored. But jumps to one are counted.
    int maxJump = traverse(a, -1, 0, _compLevelJump);
    makePowersof2(maxJump);
    totalNodes = a->nodecount();
    divider    = totalNodes/100;
    recRes = traverse(xverbose, a, rZero, rOne, _upProducts);
    std::vector<int> p2v = a->pos2var();
    // Last component would be commonality of 1 so we do not include it
    for(int i = 0; i < recRes.products.size()-1; i++) {
        res[p2v[i]] = ((mpf_class) recRes.products[i] /recRes.counter);
    }
    return res;
}

coupleCommRes computeCoupleCommonality(int xverbose, cuddAdapter *a) {
    coupleCommRes res;
    udc rZero, rOne, recRes;
    rZero.myUcr.counter = 0;
    rOne.myUcr.counter  = 1;
    rZero.myUcr.products.push_back(0);
    rOne.myUcr.products.push_back(1);
    // Jumps to node zero are ignored. But jumps to one are counted.
    int maxJump = traverse(a, -1, 0, _compLevelJump);
    makePowersof2(maxJump);
    totalNodes = a->nodecount();
    recRes = traverse(xverbose, a, rZero, rOne, _upDoubleProducts);

    // Last component would be commonality of 1 so we do not include it
    res.first.resize(a->getNumVars());
    for(int i = 0; i < recRes.myUcr.products.size()-1; i++)
        res.first[a->varAtPos(i)] = ((mpq_class) recRes.myUcr.products[i] /recRes.myUcr.counter);
    
    int len = a->getNumVars() - 1;
    for(int x = 0; x < a->getNumVars()-1; x++) {
        std::vector<mpq_class> temp(len--);
       res.second.push_back(temp);
    }
    for(int j = 0; j < recRes.doubleProducts.size() - 1; j++) {
        for(int k = 0; k < recRes.doubleProducts[j].size() - 1; k++) {
            mpq_class prob = ((mpq_class)recRes.doubleProducts[j][k]/recRes.myUcr.counter);
            int var1 = a->varAtPos(j);
            int var2 = a->varAtPos(k+j+1);
            if (var1 < var2)
                res.second[var1][var2-var1-1] = prob;
            else
                res.second[var2][var1-var2-1] = prob;
        }
    }
    return res;
}

std::vector<mpf_class> computeCommonalityMP(int num, cuddAdapter *a, int xverbose, bool fast) {
    std::vector<mpf_class> res(a->getNumVars());
    ucr rZero, rOne, recRes;
    rZero.counter = 0;
    rOne.counter  = 1;
    rZero.products.push_back(0);
    rOne.products.push_back(1);
    // Jumps to node zero are ignored. But jumps to one are counted.
    int maxJump = traverse(a, -1, 0, _compLevelJump);
    makePowersof2(maxJump);
    totalNodes = a->nodecount();
    divider    = totalNodes/100;
    recRes = traverseMP(num, xverbose, a, rZero, rOne, _upProducts, fast);
    std::vector<int> p2v = a->pos2var();
    // Last component would be commonality of 1 so we do not include it
    for(int i = 0; i < recRes.products.size()-1; i++)
        res[p2v[i]] = ((mpf_class) recRes.products[i] /recRes.counter);
    
    return res;
}

coupleCommRes computeCoupleCommonalityMP(int num, cuddAdapter *a, int xverbose, bool fast) {
    coupleCommRes res;
    udc rZero, rOne, recRes;
    rZero.myUcr.counter = 0;
    rOne.myUcr.counter  = 1;
    rZero.myUcr.products.push_back(0);
    rOne.myUcr.products.push_back(1);
    // Jumps to node zero are ignored. But jumps to one are counted.
    int maxJump = traverse(a, -1, 0, _compLevelJump);
    makePowersof2(maxJump);
    totalNodes = a->nodecount();
    divider    = totalNodes/100;
    recRes = traverseMP(num, xverbose, a,  rZero, rOne, _upDoubleProducts, fast);
    for(int i = 0; i < recRes.myUcr.products.size()-1; i++)
        res.first.push_back((mpq_class) recRes.myUcr.products[i] /recRes.myUcr.counter);
    
    for(int j = 0; j < recRes.doubleProducts.size() - 1; j++) {
        std::vector<mpq_class> inter;
        for(int k = 0; k < recRes.doubleProducts[j].size() - 1; k++) {
            inter.push_back((mpq_class)recRes.doubleProducts[j][k]/recRes.myUcr.counter);
        }
        res.second.push_back(inter);
    }
    return res;
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

struct cmpDistribution {
    bool operator()(const Distribution& a, const Distribution& b) const {
        return a < b;
    }
};
std::map<DdNode*, Distribution> node2dist;
typedef std::map<DdNode*, std::vector<partialProduct>>  resPerformance;

std::map<int, Objective*> objectives;
Objective object;
void printProduct(std::vector<char> prod) {
    for(auto p : prod)
        std::cerr << p << ", ";
    std::cerr << std::endl;
}

void productBacktracking(int itpp, partialProduct& wp, partialProduct& pp, Objective obj, Distribution& totalDist, const Distribution& dist) {
    for(;itpp <  wp.size(); itpp++)
        if (pp[itpp] == '*') {
            wp[itpp] = 'f';
            productBacktracking(itpp+1, wp, pp, obj.setToFalse(itpp), totalDist, dist);
            wp[itpp]  = 't';
            obj = obj.setToTrue(itpp);
        }
    totalDist.addProduct(obj.getIndependent(), dist);
    return;
}

void  evaluateProducts(partialProduct pp, Objective obj, Distribution& totalDist, const Distribution& dist) {
    // Pretty little backtracking to change the asterisks for 't' and 'f' consecutively
    int ast    = 0;
    for(int qw = 0; qw < pp.size();qw++) 
        if (pp[qw] == '*')
            ast++;
        
    if (verbose > 2) {
        std::cerr << "3 Evaluating product ";
        for(int qw = 0; qw < pp.size() ; qw++) 
            std::cerr <<  qw << " = " << pp[qw] << ", ";
        std::cerr << std::endl;
    }
    else
        if (verbose > 1) {
            std::cerr << "2 Evaluating product ";
            std::cerr << " with " << std::setw(4) << ast << " asterisks. dist size " << showHuman(dist.size())
                        << " totalDist size " << showHuman(totalDist.size());
        }
    partialProduct wp = pp;
    Objective newObj = obj.simplify(pp);
    productBacktracking(0, wp, pp, newObj, totalDist, dist);
    if (verbose > 1) {
        std::cerr << " done" << std::endl;
    }
}

void processProducts(const resPerformance& input, int plevel, int limit, bool value, resPerformance& output) {
    for(auto pair : input) {
        for(partialProduct temp : pair.second) {
            if (value) temp[plevel] = 't';
            else temp[plevel] = 'f';
            for(int i = plevel+1; i < limit; i++)
                temp[i] = '*';

            output[pair.first].push_back(temp); 
        }
        //output[pair.first].resize(output[pair.first].size());
    }
}

std::map<std::tuple<int, int, int, const resPerformance&, const resPerformance&>, resPerformance> phantomCache;

int nodeCounter = 1;
long phantomCounter = -1;
resPerformance _PerformanceDistribution(int plevel, int tlevel, int elevel, const resPerformance& tr, const resPerformance& er, DdNode* node) {
    resPerformance res;
    if (verbose > 1) {
        std::cerr << "Start combination level " << plevel << " tlevel " << tlevel << " elevel " << elevel << std::endl;
        if (plevel == objectives[plevel]->getMin()) {
            std::cerr << "Start of component. Objective function is " << *objectives[plevel] << std::endl;
        }
    }
    if (!tr.empty()) {
        // If the left or child are outside this component, we need to fix this using phantom nodes
        if (tlevel > objectives[plevel]->getMax()+1) {
            
            //if (phantomCache.find(std::tuple<int, int, int, const resPerformance&, const resPerformance&>(objectives[plevel]->getMax()+1, tlevel, tlevel, tr, tr)) == phantomCache.end()) {
               resPerformance ctr = _PerformanceDistribution(objectives[plevel]->getMax()+1, tlevel, tlevel, tr, tr, (DdNode*)(phantomCounter--));
                //phantomCache[std::tuple<int, int, int, const resPerformance&, const resPerformance&>(objectives[plevel]->getMax()+1, tlevel, tlevel, tr, tr)] = ctr;
            //}
            //else
            //    ctr = phantomCache[std::tuple<int, int, int, const resPerformance&, const resPerformance&>(objectives[plevel]->getMax()+1, tlevel, tlevel, tr, tr)];
            processProducts(ctr, plevel, objectives[plevel]->getMax()+1, true, res);
        }
        else
            // Update partial product info
            processProducts(tr, plevel, tlevel, true, res);
    }
    
    if (!er.empty()) {
        if (elevel > objectives[plevel]->getMax()+1) {
            //if (phantomCache.find(std::tuple<int, int, int, const resPerformance&, const resPerformance&>(objectives[plevel]->getMax()+1, elevel, elevel, er, er)) == phantomCache.end()) {
             resPerformance   cer = _PerformanceDistribution(objectives[plevel]->getMax()+1, elevel, elevel, er, er, (DdNode*)(phantomCounter--));
                //phantomCache[std::tuple<int, int, int, const resPerformance&, const resPerformance&>(objectives[plevel]->getMax()+1, elevel, elevel, er, er)] = cer;
            //}
            //else
            //    cer = phantomCache[std::tuple<int, int, int, const resPerformance&, const resPerformance&>(objectives[plevel]->getMax()+1, elevel, elevel, er, er)];
            
            processProducts(cer, plevel, objectives[plevel]->getMax()+1, false, res);
        }
        else 
            // Update partial product info
            processProducts(er, plevel, elevel, false, res);
    }
    // If this position is the first in a component, we update the distribution evaluating the objective function
    if (plevel == objectives[plevel]->getMin()) {
        Distribution dist(objectives[plevel]->getPrecision());
        int xx    = 0;
        int total = 0;
        for(auto pair : res) {
            total += pair.second.size();
        }
        for(auto pair : res) {
            for(auto product : pair.second) {
                xx++;
                if (verbose > 1)
                    std::cerr << "Completing partial results " << std::setw(6) << xx << " of " << std::setw(6) << total
                    << std::setw(4) << (int) (100.0*xx/total) << "%" << std::endl;
                if (verbose > 2) {
                    std::cerr << "Partial product ";
                    printProduct(product);
                }
                evaluateProducts(product, *objectives[plevel], dist, node2dist[pair.first]);
            }
        }
        if (verbose > 2)
            std::cerr << "acumulated dist " << dist << "-------" << std::endl;
        
        resPerformance newRes;
        node2dist[node] = dist;
        newRes[node].push_back(emptyProduct);
        newRes[node].resize(1);
        if (verbose > 1)
            std::cerr << "End   combination level " << plevel << " tlevel " << tlevel << " elevel " << elevel << std::endl;
        return newRes;
    }
    if (verbose > 1)
        std::cerr << "End   combination level " << plevel << " tlevel " << tlevel << " elevel " << elevel << std::endl;
    return res;
}

Distribution computePerformanceDistribution(int xverbose, int ncomps, Objective obj, std::map<int, Objective*> filteredObjectives, cuddAdapter* a) {
    DdNode* one  = a->getOne();
    verbose = xverbose;
    partialProduct prodOne(a->getNumVars());
    emptyProduct = partialProduct(a->getNumVars(), ' ');
    Distribution distOne(ncomps, obj.getIndependent());
    resPerformance res, resZero, resOne;
    node2dist[one] = distOne;
    resOne[one].push_back(prodOne);
    // Now we filter the objective functions for each component
    objectives = filteredObjectives;
    object = obj;
    res = traverse(verbose, a, resZero, resOne, _PerformanceDistribution);
    Distribution d = node2dist[res.begin()->first];
    return d;
}

Distribution computePerformanceDistributionMP(int threads, bool fast, int xverbose, int ncomps, Objective obj, std::map<int, Objective*> filteredObjectives, cuddAdapter* a) {
    DdNode* one  = a->getOne();
    verbose = xverbose;
    partialProduct prodOne(a->getNumVars());
    emptyProduct = partialProduct(a->getNumVars(), ' ');
    Distribution distOne(ncomps, obj.getIndependent());
    resPerformance res, resZero, resOne;
    node2dist[one] = distOne;
    resOne[one].push_back(prodOne);
    // Now we filter the objective functions for each component
    objectives = filteredObjectives;
    object = obj;
    res = traverseMP(threads, verbose, a, resZero, resOne, _PerformanceDistribution, fast);
    Distribution d = node2dist[res.begin()->first];
    return d;
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


std::ostream& operator<<(std::ostream& os, const ucr& u) {
    os << "counter " << u.counter << std::endl;
    os << "products: ";
    for(auto x : u.products)
        os << x << " ";
    os << std::endl;
  
    return os;
}

std::ostream& operator<<(std::ostream& os, const udc& u) {
    os << "counter " << u.myUcr.counter << std::endl;
    os << "products: ";
    for(auto x : u.myUcr.products)
        os << x << " ";
    os << std::endl;
    os << "doubleProducts " << u.doubleProducts.size() << std::endl;
    for(auto vecprod : u.doubleProducts) {
        for(auto p : vecprod)
            os << p << " ";
        os << std::endl;
    }
    
    return os;
}

