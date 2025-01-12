#ifndef CALSHT_HPP
#define CALSHT_HPP

#include <filesystem>
#include <tuple>
#include <vector>
#ifndef ENABLE_NYANTEN
#define ENABLE_NYANTEN false
#endif

class Calsht {
private:
  using LVec = std::vector<uint8_t>;
  using RVec = std::vector<uint8_t>;
  using Iter = std::vector<RVec>::iterator;

  std::vector<RVec> mp1;
  std::vector<RVec> mp2;

  RVec index1(int n) const;
  void add1(LVec& lhs, const RVec& rhs, int m) const;
  void add2(LVec& lhs, const RVec& rhs, int m) const;
  void read_file(Iter first, Iter last, std::filesystem::path file) const;

public:
  Calsht()
      : mp1(ENABLE_NYANTEN ? 405350 : 1953125, RVec(10)),
        mp2(ENABLE_NYANTEN ? 43130 : 78125, RVec(10)) {}
  void initialize(const std::string& dir);
  int calc_lh(const std::vector<int>& t, int m, bool three_player = false) const;
  int calc_sp(const std::vector<int>& t, bool three_player = false) const;
  int calc_to(const std::vector<int>& t) const;
  std::tuple<int, int> operator()(const std::vector<int>& t,
                                  int m,
                                  int mode,
                                  bool check_hand = false,
                                  bool three_player = false) const;
};

#endif
