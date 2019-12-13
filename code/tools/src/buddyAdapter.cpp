//
//  buddyAdapter.cpp
//  myKconf
//
//  Created by david on 15/12/14.
//  Copyright (c) 2014 david. All rights reserved.
//

#include "buddyAdapter.hpp"

using namespace buddy;


buddyAdapter::buddyAdapter() {  theBDD = bddtrue;
                                unsigned int temp = (unsigned int) get_cpu_time() % 1000000;
                                //cout << "Initializing random seed " << temp << endl;
                                srandom(temp);  }
int                 buddyAdapter::getNumVars()        { return bdd_varnum();              }
int                 buddyAdapter::getNumComponents()  { return 1;                         }
int                 buddyAdapter::getMaxComponent()   { return bdd_varnum();              }
int                 buddyAdapter::getComponent(int x) { return 0;                         }
float               buddyAdapter::getComponentEntropy() { return 0; }
int                 buddyAdapter::getCompSize(int x)  { return bdd_varnum();              }
bdd                 buddyAdapter::getBDD()            { return theBDD;                    }
bdd                 buddyAdapter::getZero()           { return bddfalse;                  }
bdd                 buddyAdapter::getOne()            { return bddtrue;                   }

void    buddyAdapter::useComponents() {};
void    buddyAdapter::setValue(std::string s , synExp* e) {
        if (e == synTrue) {
            apply(new synSymbol(s));
            return;
        }
        if (e == synFalse) {
            apply(makeNot(new synSymbol(s)));
            return;
        }
}

void    buddyAdapter::printBDD()  {
          //  std::cout << theBDD << std::endl;
         };
std::vector<int>     buddyAdapter::pos2var() {
    std::vector<int> res;
    for(int i = 0; i< bdd_varnum(); i++)
        res.push_back(bdd_level2var(i));
    return res;
}
std::vector<int>     buddyAdapter::var2pos() {
    std::vector<int> res;
    for(int i = 0; i< bdd_varnum(); i++)
        res.push_back(bdd_var2level(i));
    return res;
}
void buddyAdapter::changeOrder(std::vector<int> pos2var) {}
void   buddyAdapter::setUpVariable(const std::string& var) {
    int max = -1;
    int min = std::numeric_limits<int>::max();
    std::string minVar, maxVar;
        int pos = bdd_var2level(firstMenuBlocks[var]);
        moduleImplies(var);
        //std::cerr << "Addblock " << var << " position " << pos << std::endl;
        //addblock(var);
        if (pos < min) {
            min    = pos;
            minVar = var;
        }
        if (pos > max) {
            max    = pos;
            maxVar = var;
        }
    //std::cerr << "MinVar " << minVar << " min " << min << " max " << max << " maxVar " << maxVar << std::endl;
    //if (minVar != maxVar) {
        //std::cerr << "AddMenuBlock " << minVar << " " << maxVar << std::endl;
    //    addMenublock(minVar, maxVar);
    //}
}

void                buddyAdapter::reduce(const std::string& slow, const std::string& shigh) {
    return;
}
int      buddyAdapter::getNodes() { return totalNodes; }


void  buddyAdapter::initblocks() {
}

void buddyAdapter::init()    {
    //buddy:bdd_init(3E8, 5E7);
    buddy:bdd_init(1E4, 1E4);
    bdd_setmaxincrease(1E5);
    bdd_setcacheratio(1);
    //bdd_setvarnum(1);
    bdd_reorder_verbose(0);
    //bdd_autoreorder(BDD_REORDER_SIFT);

}

void   buddyAdapter::reorder(std::string reorderMethod)                           {
 //   if (reorderMethod = "")
        bdd_reorder(BDD_REORDER_SIFTITE);
   // if
     //   bdd_reorder(reorderMethod)
}



std::vector<std::string> buddyAdapter::giveOrder() {
    int2extLevel.clear();
    //std::cerr << "In giveOrder" << std::endl;
    std::vector<std::string> res;
    int z = 0;
    int x = 0;
    while (x < bdd_varnum()) {
        int y = bdd_level2var(x);
        std::string var = inVars.at(y);
        int2extLevel.push_back(z);
        //std::cerr << "x=" << std::setw(5) << x << std::setw(5) << y << " var " << var << std::endl;
        res.push_back(var);
        if (vars.at(var).second !=  bddfalse) {
            int2extLevel.push_back(z);
            x += 2;
        }
        else x++;
        z++;
    }
    
    return res;
}
void    buddyAdapter::newVar(std::string var, std::string type) {
    if (type == "boolean") {
        firstMenuBlocks[var] = index;
        lastMenuBlocks[var]  = index;

        bdd_setvarnum(index+1);
        vars[var] = std::pair<bdd, bdd>(bdd_ithvar(index), bddfalse);
        inVars.push_back(var);
        index++;
    }
    else
        if (type == "tristate") {
            bdd_setvarnum(index+2);
            firstMenuBlocks[var] = index;
            lastMenuBlocks[var]  = index+1;
            inVars.push_back(var);
            inVars.push_back("NO_VARIABLE");
            vars[var] = std::pair<bdd, bdd>(bdd_ithvar(index), bdd_ithvar(index+1));
            index += 2;
        }
}
void  buddyAdapter::moduleImplies(std::string var) {
      if (vars[var].second != bddfalse)
        theBDD &=   (!vars[var].first | vars[var].second) &
                    (!vars[var].first | vars.at("MODULES").first);
        //theBDD &= (!(vars[var].first & vars["MODULES"].first) | vars[var].second);
}

SemanticsBuddy    buddyAdapter::getVar(std::string var) {
    if (vars.find(var) != vars.end())
        return SemanticsBuddy(vars[var].first, vars[var].second);
    
    if (storage.find(var) != storage.end())
        return SemanticsBuddy(storage[var], bddfalse);
    std::cerr << "Error looking for variables or BDD named " << var << std::endl;
    exit(-1);
}

void   buddyAdapter::namevar(std::string name, int index) {
    
}

bool buddyAdapter::apply(std::string s, synExp * exp) {
    SemanticsBuddy sm = process(exp);
    if (storage.find(s) != storage.end())
        storage[s] &= sm.main();
    else
        storage[s] = sm.main();
    return false;
}

bool buddyAdapter::apply(synExp *s) {
    SemanticsBuddy sm = process(s);
     theBDD &= sm.main();
    return false;
}

SemanticsBuddy buddyAdapter::process(synExp* exp) {
    //std::cerr << "process " << exp << std::endl;
    switch (exp->get_type()) {
        case synExp_Const : {
            if (exp == synTrue)   {
                return  SemanticsBuddy(bddtrue, bddfalse);
            }
            if (exp == synFalse) {
                return SemanticsBuddy(bddfalse, bddfalse);
            }
            if (exp == synModule) {
                return SemanticsBuddy(bddtrue,  bddtrue);
            }
            std::cerr << "Error in const synExp " << exp << std::endl; break;
        }
        case synExp_Symbol : {
            std::string var = exp->getSymbol();
            std::string t   = symbolTable::getSymbol(var)->getVartype();
            //if (t == "boolean" || t == "tristate") {
                return  SemanticsBuddy(getVar(var));
            //}
            //else {
            //    std::cerr << "Variable of type " << t << std::endl;
            //    return SemanticsBuddy(bddtrue, bddfalse);
            //}
        }
        case synExp_Compound :
        {
            switch (exp->get_op()) {
                case synNot        : {
                    SemanticsBuddy c = process(exp->first());
                    const bdd x  = c.main();
                    const bdd xm = c.module();
                    return  SemanticsBuddy(!x | xm, xm);
                    }
                case synAnd        : {
                    SemanticsBuddy c = process(exp->first());
                    SemanticsBuddy d = process(exp->second());
                    const bdd x  = c.main();
                    const bdd xm = c.module();
                    const bdd y  = d.main();
                    const bdd ym = d.module();
                    return  SemanticsBuddy(x & y, x & bdd_ite(y, xm | ym, bddfalse));
                }
                case synOr         : {
                    SemanticsBuddy c = process(exp->first());
                    SemanticsBuddy d = process(exp->second());
                    const bdd x  = c.main();
                    const bdd xm = c.module();
                    const bdd y  = d.main();
                    const bdd ym = d.module();
                    return SemanticsBuddy(x | y, (xm & !y) | (ym & !x) | xm & ym);
                }
                case synImplies    : {
                    SemanticsBuddy c = process(exp->first());
                    SemanticsBuddy d = process(exp->second());
                    const bdd x  = c.main();
                    const bdd xm = c.module();
                    const bdd y  = d.main();
                    const bdd ym = d.module();
                    return  SemanticsBuddy(bdd_ite(y,
                                                    bdd_ite(ym, !x | xm, bddtrue),
                                                    !x & !xm),
                                            bddfalse);
                }
                case synIfThenElse : {
                    SemanticsBuddy condPart = process(exp->first());
                    SemanticsBuddy thenPart = process(exp->second());
                    SemanticsBuddy elsePart = process(exp->third());
                    
                    return SemanticsBuddy(bdd_ite(condPart.main(), thenPart.main(), elsePart.main()),
                                           bdd_ite(condPart.main(), thenPart.module(), elsePart.module()));
                }
                case synEqual      : {
                    SemanticsBuddy c = process(exp->first());
                    SemanticsBuddy d = process(exp->second());
                    const bdd x  = c.main();
                    const bdd xm = c.module();
                    const bdd y  = d.main();
                    const bdd ym = d.module();
                    return SemanticsBuddy( ((x & y) | (!x & !y)) & ((xm & ym) | (!xm & !ym)),
                                           bddfalse);
                }
            }
        }
        case synExp_XOR : {
            std::cerr << "Error in synXOR not implemented " << exp << std::endl; break;
        }
        case synExp_String : {
            std::cerr   << "What is a string doing here? *" << exp->get_string() << "* "
                        << std::endl;
            return SemanticsBuddy();
        }
            
    }
    return SemanticsBuddy(bddfalse, bddfalse);
}





const int buddyAdapter::nodecount(std::string s) {
    return bdd_nodecount(storage[s]);
}

const int buddyAdapter::nodecount() {
        return bdd_nodecount(theBDD);
}

void    buddyAdapter::saveBDD(const std::string id, const std::string& base, const std::string &suffix)          {
    std::string  varfilename;
    if (suffix == "")
        varfilename = base + ".buddy";
    else
        varfilename = base + "-" + suffix + ".buddy";
    bdd_save(fopen(varfilename.c_str(), "w"), storage[id]);
}

void    buddyAdapter::saveBDD(const std::string& base, const std::string &suffix)   {
    std::string  varfilename;
    if (suffix == "")
        varfilename = base + ".buddy";
    else
        varfilename = base + "-" + suffix + ".buddy";
    bdd_save(fopen(varfilename.c_str(), "w"), theBDD);
}

void    buddyAdapter::readBDD(const std::string id, const std::string& base, const std::string &suffix) {
    std::cerr << "Unimplemented" << std::endl;
}

void    buddyAdapter::readBDD(const std::string& base, const std::string &suffix) {
    std::cerr << "Unimplemented" << std::endl;
}

bool    buddyAdapter::sameBDD(std::string s1, std::string s2) {
    return storage[s1] == storage[s2];
}


