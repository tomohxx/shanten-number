#ifndef CALSHT_HPP
#define CALSHT_HPP

#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>

class Calsht{
private:
  using Vec  = std::vector<int>;
  using Iter = std::vector<Vec>::iterator;
  
  std::vector<Vec> mp1;
  std::vector<Vec> mp2;
  
  Iter itr1;
  Iter itr2;
  
  void add(Vec& lhs, const Vec& rhs) const;
  void add(Vec& lhs, const Vec& rhs, int j) const;
  Iter read_file(Iter first, Iter last, const char* filename) const;
  
public:
  Calsht();
  int calc_lh(const int* t, int m) const;
  int calc_sp(const int* t) const;
  int calc_to(const int* t) const;
  int operator()(const int* t, int m) const;
  bool operator!() const;
};

#endif
