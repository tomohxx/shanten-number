#ifndef CALSHT_HPP
#define CALSHT_HPP

#include <array>
#include <tuple>
#include <vector>
#include "constant.hpp"

class Calsht{
private:
  using Vec  = std::vector<int>;
  using Iter = std::vector<Vec>::iterator;

  std::vector<Vec> mp1;
  std::vector<Vec> mp2;

  Iter itr1;
  Iter itr2;

#ifdef THREE_PLAYER
  Vec index1(int n) const;
#endif
  void add1(Vec& lhs, const Vec& rhs, int m) const;
  void add2(Vec& lhs, const Vec& rhs, int m) const;
  Iter read_file(Iter first, Iter last, const char* file_name) const;

public:
  Calsht();
  int calc_lh(const int* t, int m) const;
  int calc_sp(const int* t) const;
  int calc_to(const int* t) const;
  std::tuple<int,int> operator()(const std::vector<int>& t, int m, int mode) const;
};

#endif
