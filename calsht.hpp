#ifndef CALSHT_HPP
#define CALSHT_HPP

#include <cstdint>
#include <filesystem>
#include <tuple>
#include <vector>
#include "constant.hpp"

class Calsht{
private:
  using LVec  = std::vector<int32_t>;
  using RVec  = std::vector<int8_t>;
  using Iter = std::vector<RVec>::iterator;

  std::vector<RVec> mp1;
  std::vector<RVec> mp2;

#ifdef THREE_PLAYER
  Vec index1(int n) const;
#endif
  void add1(LVec& lhs, const RVec& rhs, int m) const;
  void add2(LVec& lhs, const RVec& rhs, int m) const;
  Iter read_file(Iter first, Iter last, std::filesystem::path file) const;

public:
  Calsht() : mp1(1953125, RVec(10)), mp2(78125, RVec(10)) {}
  void initialize(const std::string& dir);
  int calc_lh(const int* t, int m) const;
  int calc_sp(const int* t) const;
  int calc_to(const int* t) const;
  std::tuple<int,int> operator()(const std::vector<int>& t, int m, int mode) const;
};

#endif
