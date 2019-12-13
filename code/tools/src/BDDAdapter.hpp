//
//  BDDAdapter.h
//  myKconf
//
//  Created by david on 15/12/14.
//  Copyright (c) 2014 david. All rights reserved.
//

#ifndef __myKconf__BDDAdapter__
#define __myKconf__BDDAdapter__

#include <fstream>
#include <vector>
#include <set>
#include <gmp.h>
#include <gmpxx.h>

#include "Components.hpp"
#include "synExp.hpp"
#include "MenuBlock.hpp"

class BDDAdapter {
public:
    //virtual         T                   getBDD()                                                    = 0;
    //virtual         T                   getZero()                                                   = 0;
    //virtual         T                   getOne()                                                    = 0;
    virtual         int                 getNumVars()                                                = 0;
    virtual         int                 getNumComponents()                                          = 0;
    virtual         int                 getMaxComponent()                                           = 0;
    virtual         float               getComponentEntropy() = 0;

    virtual         int                 getComponent(int x)                                         = 0;
    virtual         int                 getCompSize(int x)                                          = 0;

    virtual         std::vector<int>    pos2var()                                                   = 0;
    virtual         std::vector<int>    var2pos()                                                   = 0;
    virtual         void                changeOrder(std::vector<int> pos2var)                       = 0;
    virtual         void                useComponents()                                             = 0;
    virtual         void                useComponents(std::vector<int> var2pos, std::vector<int> pos2var)                                             = 0;
    virtual         void                setValue(std::string, synExp*)                              = 0;
    virtual         void                setName(const std::string& name) {};
    virtual         void                reduce(const std::string& slow, const std::string& shigh)   = 0;
    virtual         int                 getNodes()                                                  = 0;
    virtual         void                setUpVariable(const std::string& var)                       = 0;
    virtual         void                printBDD()                                                  = 0;
    virtual         void                init()                                                      = 0;
    virtual         void                reorder(std::string reorderMethod)                          = 0;
    virtual         void                namevar(std::string name, int index)                        = 0;
    virtual         std::vector<std::string>    giveOrder()                                         = 0;
    virtual         void                newVar(std::string var, std::string type)                   = 0;
    virtual         void                newVar(std::string var, std::string type, int pos)          = 0;
    virtual         void                moduleImplies(std::string var)                              = 0;
    virtual         bool                apply(synExp * s)                                           = 0;
    virtual         bool                apply(std::string s, synExp * exp)                          = 0;
    virtual const   int                 nodecount()                                                 = 0;
    virtual const   int                 nodecount(std::string s)                                    = 0;
    virtual         void                saveBDD(const std::string& b, const std::string &s)         = 0;
    virtual         void                saveBDD(const std::string id, const std::string& b, const std::string &s)  = 0;
    virtual         void                readBDD(const std::string& b, const std::string &s)         = 0;
    virtual         void                readBDD(const std::string id, const std::string& b, const std::string &s)         = 0;
    virtual         bool                sameBDD(std::string s1, std::string s2)                     = 0;
    virtual         void                existentialQ(const std::set<std::string>& v)                = 0;
    virtual         void                destroyInternal(const std::set<std::string>& v)             = 0;
    virtual         std::set<std::string>   checkOneSolution()                                      = 0;
    virtual         void                syncOrder()                                                 = 0;
    virtual         void                checkOrder()                                                = 0;
    virtual         void                setXORBlocks(synExp* s)                                     = 0;
    virtual         void                relateVars(std::set<int>)			                        = 0;
    virtual	        void		        setRelatedVars()					                        = 0;
    virtual                             ~BDDAdapter(){};


};

#endif /* defined(__myKconf__BDDAdapter__) */
