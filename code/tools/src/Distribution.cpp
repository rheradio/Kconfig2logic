#include "Distribution.hpp"
Distribution::Distribution() {};
Distribution::Distribution(int xPrecision) : precision(xPrecision) {

    //mpz_class two = 2;
    //mpz_class one = 1;
    //if (precision >= 0)
    //    mpz_pow_ui(twoRaisedToPrecision.get_mpz_t(), two.get_mpz_t(), precision);
    //else
    //    mpz_div_2exp(twoRaisedToPrecision.get_mpz_t(), one.get_mpz_t(), -precision);
    if (precision >= 0)
        twoRaisedToPrecision = pow(2, precision);
    else
        twoRaisedToPrecision = pow(2, -precision);
    
    if (twoRaisedToPrecision <= 0) {
        std::ostringstream ost;
        ost << "drawerType is not big enough to hold 2^" << precision << std::endl
            << "sizeof(drawerType) = " << sizeof(drawerType) << " sizeof(int) = " << sizeof(int)
        << " sizeof(unsigned long int) " << sizeof(unsigned long int) << std::endl;
        throw std::logic_error(ost.str());
    }
        
}

Distribution::Distribution(int xPrecision, double xconstant) : precision(xPrecision) {
//    mpz_class two = 2;
//    mpz_class one = 1;
//    if (precision >= 0)
//        mpz_pow_ui(twoRaisedToPrecision.get_mpz_t(), two.get_mpz_t(), precision);
//    else
//        mpz_div_2exp(twoRaisedToPrecision.get_mpz_t(), one.get_mpz_t(), -precision);
    if (precision >= 0)
        twoRaisedToPrecision = pow(2, precision);
    else
        twoRaisedToPrecision = pow(2, -precision);
    
    if (twoRaisedToPrecision <= 0) {
        std::ostringstream ost;
        ost << "drawerType is not big enough to hold 2^" << precision << std::endl
            << "sizeof(drawerType) = " << sizeof(drawerType) << " sizeof(int) = " << sizeof(int)
        << " sizeof(unsigned long int) " << sizeof(unsigned long int) << std::endl;
        throw std::logic_error(ost.str());
    }
  _map[getDrawer(xconstant)]  = 1;
}

void Distribution::accumulate(const Distribution& one, const Distribution& two) {
    int c = 0;
    for(auto onePair : one._map)
        for(auto twoPair : two._map) {
            if (c++ % 1000000 == 0) std::cerr << ".";
            _map[(onePair.first+twoPair.first)/2] += onePair.second*twoPair.second;
        }
}

int Distribution::size() const { return int(_map.size()); }
void    Distribution::addProduct(double d) {
   _map[getDrawer(d)]++;
}

void Distribution::addProduct(double d, const Distribution& dist) {
    for(auto p : dist._map) {
       drawerType  drawer = getDrawer(dist.getValue(p.first)+d);
      _map[drawer] += p.second;
  }
}
double Distribution::getValue(drawerType drawer) const {
//    mpq_class a = drawer;
//    mpq_class b = twoRaisedToPrecision;
//    mpq_class temp =  a/b;
//    return  mpq_get_d(temp.get_mpq_t());
    if (precision >= 0)
        return drawer / twoRaisedToPrecision;
    else
        return drawer*twoRaisedToPrecision;
}
drawerType Distribution::getDrawer(double xconstant) {
//    drawerType result;
//    mpf_class temp = (mpq_class) xconstant * twoRaisedToPrecision;
//    mpz_set_f(result.get_mpz_t(), temp.get_mpf_t());
//    return result;
     if (precision >= 0)
         return xconstant*twoRaisedToPrecision;
     else
         return xconstant/twoRaisedToPrecision;
}

std::ostream& operator<<(std::ostream& os, const Distribution& dist) {
  if (dist.precision != 0)
     os << "precision " << dist.precision << std::endl;
  for(auto p : dist._map) 
    if (dist.precision != 0) {
//       mpf_class res;
//       mpf_class f = p.first;
//       mpf_div_2exp(res.get_mpf_t(), f.get_mpf_t(), dist.precision);
//       os << std::setw(10) << p.first << " (" << mpf_get_si(res.get_mpf_t()) << ")" <<  "\t" << p.second << std::endl;
        os << std::setw(10) << p.first << " (" << dist.getValue(p.first) << ") \t" << p.second << std::endl;
    }
    else
       os << std::setw(10) << p.first <<  "\t" << p.second << std::endl;
  
  return os;
}

mpz_class Distribution::countProducts() {
  mpz_class total;
  for(auto p : _map)
    total +=  p.second;
  
  return total;
}

bool Distribution::operator<(const Distribution& b) const {
     if (precision != b.precision)
       return precision < b.precision;

     disType::const_iterator ita, itb;
     ita = this->_map.begin();
     itb = b._map.begin();
     while (ita != this->_map.end() && itb != b._map.end() && ita->first == itb->first && ita->second == itb->second) {
        ita++;
        itb++;
     }
     if (ita == this->_map.end()) {
        if (itb == b._map.end())
	  return false;

        return true;
     }
     if (itb == b._map.end())
       return false;

     if  (ita->first !=  itb->first)
         return ita->first < itb->first;

     return ita->second < itb ->second; 
}

std::map<drawerType, mpz_class> Distribution::sort() {
    std::map<drawerType, mpz_class> res;
    for(auto p : _map)
        res[p.first] = p.second;
    return res;
}
