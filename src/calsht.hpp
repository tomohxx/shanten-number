#ifndef CALSHT_HPP
#define CALSHT_HPP

#include <array>
#include <cstdint>
#include <filesystem>
#include <string>
#include <tuple>
#include <vector>

class Calsht {
private:
  using LArr = std::array<uint8_t, 10>;
  using RArr = std::array<uint8_t, 10>;
  using Iter = std::vector<RArr>::iterator;

  std::vector<RArr> mp1;
  std::vector<RArr> mp2;

  void add1(LArr& lhs, const RArr& rhs, int m) const;
  void add2(LArr& lhs, const RArr& rhs, int m) const;
  void read_file(Iter first, Iter last, std::filesystem::path file) const;

public:
  Calsht(const std::string& dir);
  int calc_lh(const std::array<int, 34>& t, int m, bool three_player = false) const;
  int calc_sp(const std::array<int, 34>& t, bool three_player = false) const;
  int calc_to(const std::array<int, 34>& t) const;
  std::tuple<int, int> operator()(const std::array<int, 34>& t,
                                  int m,
                                  int mode,
                                  bool check_hand = false,
                                  bool three_player = false) const;
};

#endif
