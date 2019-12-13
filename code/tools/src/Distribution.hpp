#ifndef _DISTRIBUTION_HPP_
#define _DISTRIBUTION_HPP_
#include <map>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <gmpxx.h>
#include <unordered_map>

//typedef int drawerType ;
typedef long drawerType ;
typedef std::unordered_map<drawerType, mpz_class> disType;
//typedef std::map<drawerType, mpz_class> disType;

class Distribution {
public:

    Distribution();
    Distribution(int precision);
    Distribution(int xPrecision, double xconstant);
    // Adds a product extending another distribution.
    void         accumulate(const Distribution& one, const Distribution& two);
    void         addProduct(double d, const Distribution& dist);
    void         addProduct(double d);
    drawerType   getDrawer(double xconstant);
    double       getValue(drawerType drawer) const;
    friend std::ostream& operator<<(std::ostream& os, const  Distribution& dist);
    bool operator<(const Distribution& b) const;
    mpz_class    countProducts();
    int          size() const;
    std::map<drawerType, mpz_class> sort();
    
private:
    drawerType   twoRaisedToPrecision;
    disType      _map;
    int          precision;
    
};
#endif // _DISTRIBUTION_HPP_
