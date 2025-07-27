#include "calsht.hpp"
#include "hash.hpp"
#include <algorithm>
#include <format>
#include <fstream>
#include <stdexcept>
#include <type_traits>
constexpr int NUM_TIDS = 34;
const std::conditional<ENABLE_NYANTEN, NyantenHash<9>, DefaultHash<9>>::type hash1;
const std::conditional<ENABLE_NYANTEN, NyantenHash<7>, DefaultHash<7>>::type hash2;

Calsht::RVec Calsht::index1(const int n) const
{
  RVec ret(10, 14u);

  ret[0] = 0u;
  ret[1] = std::max(3u - n, 0u);
  ret[5] = std::max(2u - n, 0u);

  return ret;
}

void Calsht::add1(LVec& lhs, const RVec& rhs, const int m) const
{
  for (int j = m + 5; j >= 5; --j) {
    int sht = std::min(lhs[j] + rhs[0], lhs[0] + rhs[j]);

    for (int k = 5; k < j; ++k) {
      sht = std::min({sht, lhs[k] + rhs[j - k], lhs[j - k] + rhs[k]});
    }

    lhs[j] = sht;
  }

  for (int j = m; j >= 0; --j) {
    int sht = lhs[j] + rhs[0];

    for (int k = 0; k < j; ++k) {
      sht = std::min(sht, lhs[k] + rhs[j - k]);
    }

    lhs[j] = sht;
  }
}

void Calsht::add2(LVec& lhs, const RVec& rhs, const int m) const
{
  const int j = m + 5;
  int sht = std::min(lhs[j] + rhs[0], lhs[0] + rhs[j]);

  for (int k = 5; k < j; ++k) {
    sht = std::min({sht, lhs[k] + rhs[j - k], lhs[j - k] + rhs[k]});
  }

  lhs[j] = sht;
}

void Calsht::read_file(Iter first, Iter last, std::filesystem::path file) const
{
  std::ifstream fin(file, std::ios_base::in | std::ios_base::binary);

  if (!fin) {
    throw std::runtime_error(std::format("Reading file does not exist: {}", file.string()));
  }

  for (; first != last; ++first) {
    fin.read(reinterpret_cast<char*>(first->data()), first->size() * sizeof(RVec::value_type));
  }
}

void Calsht::initialize(const std::string& dir)
{
  read_file(mp1.begin(), mp1.end(), std::filesystem::path(dir) / "index_s.bin");
  read_file(mp2.begin(), mp2.end(), std::filesystem::path(dir) / "index_h.bin");
}

int Calsht::calc_lh(const std::array<int, NUM_TIDS>& t, const int m, const bool three_player) const
{
  LVec ret = mp2[hash2(t.cbegin() + 27)];

  add1(ret, mp1[hash1(t.cbegin() + 18)], m);
  add1(ret, mp1[hash1(t.cbegin() + 9)], m);

  if (three_player) {
    add1(ret, index1(t[8]), m);
    add2(ret, index1(t[0]), m);
  }
  else {
    add2(ret, mp1[hash1(t.cbegin())], m);
  }

  return ret[m + 5];
}

int Calsht::calc_sp(const std::array<int, NUM_TIDS>& t, const bool three_player) const
{
  int pair = 0;
  int kind = 0;

  for (int i = 0; i < NUM_TIDS; ++i) {
    if (three_player && i > 0 && i < 8) continue;

    if (t[i] > 0) {
      ++kind;
      if (t[i] >= 2) ++pair;
    }
  }

  return 7 - pair + (kind < 7 ? 7 - kind : 0);
}

int Calsht::calc_to(const std::array<int, NUM_TIDS>& t) const
{
  int pair = 0;
  int kind = 0;

  for (const int i : {0, 8, 9, 17, 18, 26, 27, 28, 29, 30, 31, 32, 33}) {
    if (t[i] > 0) {
      ++kind;
      if (t[i] >= 2) ++pair;
    }
  }

  return 14 - kind - (pair > 0 ? 1 : 0);
}

std::tuple<int, int> Calsht::operator()(const std::array<int, NUM_TIDS>& t,
                                        const int m,
                                        const int mode,
                                        const bool check_hand,
                                        const bool three_player) const
{
  if (check_hand) {
    int n = 0;

    for (int i = 0; i < NUM_TIDS; ++i) {
      if (t[i] < 0 || t[i] > 4) {
        throw std::invalid_argument(std::format("Invalid number of hand's tiles at {}: {}", i, t[i]));
      }

      ++n;
    }

    if (ENABLE_NYANTEN && n > 14) {
      throw std::invalid_argument(std::format("Invalid sum of hand's tiles: {}", n));
    }

    if (m < 0 || m > 4) {
      throw std::invalid_argument(std::format("Invalid sum of hands's melds: {}", m));
    }

    if (mode < 0 || mode > 7) {
      throw std::invalid_argument(std::format("Invalid caluculation mode: {}", mode));
    }
  }

  std::tuple<int, int> ret{1024, 0};

  if (mode & 1) {
    if (int sht = calc_lh(t, m, three_player); sht < std::get<0>(ret)) {
      ret = {sht, 1};
    }
    else if (sht == std::get<0>(ret)) {
      std::get<1>(ret) |= 1;
    }
  }

  if ((mode & 2) && m == 4) {
    if (int sht = calc_sp(t, three_player); sht < std::get<0>(ret)) {
      ret = {sht, 2};
    }
    else if (sht == std::get<0>(ret)) {
      std::get<1>(ret) |= 2;
    }
  }

  if ((mode & 4) && m == 4) {
    if (int sht = calc_to(t); sht < std::get<0>(ret)) {
      ret = {sht, 4};
    }
    else if (sht == std::get<0>(ret)) {
      std::get<1>(ret) |= 4;
    }
  }

  return ret;
}
