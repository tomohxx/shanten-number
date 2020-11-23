#ifndef CALSHT_HPP
#define CALSHT_HPP

#include <array>
#include <filesystem>
#include <tuple>
#include <vector>
#include "constant.hpp"

class Calsht{
private:
  using Vec  = std::vector<int>;
  using Iter = std::vector<Vec>::iterator;

  std::vector<Vec> mp1;
  std::vector<Vec> mp2;

#ifdef THREE_PLAYER
  Vec index1(int n) const;
#endif
  void add1(Vec& lhs, const Vec& rhs, int m) const;
  void add2(Vec& lhs, const Vec& rhs, int m) const;
  Iter read_file(Iter first, Iter last, std::filesystem::path file) const;

public:
  Calsht() : mp1(std::vector<Vec>(1953125,Vec(10))), mp2(std::vector<Vec>(78125,Vec(10))) {}
  void initialize(std::filesystem::path dir);
  int calc_lh(const int* t, int m) const;
  int calc_sp(const int* t) const;
  int calc_to(const int* t) const;
  std::tuple<int,int> operator()(const std::vector<int>& t, int m, int mode) const;
};

#endif
