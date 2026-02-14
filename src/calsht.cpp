#include "calsht.hpp"
#include "hash.hpp"
#include <algorithm>
#include <format>
#include <fstream>
#include <stdexcept>
constexpr int NUM_TIDS = 34;
const Hash<9> hash1;
const Hash<7> hash2;

constexpr std::array<std::array<uint8_t, 10>, 5> index1 = {{
    {0u, 3u, 14u, 14u, 14u, 2u, 14u, 14u, 14u, 14u},
    {0u, 2u, 14u, 14u, 14u, 1u, 14u, 14u, 14u, 14u},
    {0u, 1u, 14u, 14u, 14u, 0u, 14u, 14u, 14u, 14u},
    {0u, 0u, 14u, 14u, 14u, 0u, 14u, 14u, 14u, 14u},
    {0u, 0u, 14u, 14u, 14u, 0u, 14u, 14u, 14u, 14u},
}};

void Calsht::add1(LArr& lhs, const RArr& rhs, const int m) const
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

void Calsht::add2(LArr& lhs, const RArr& rhs, const int m) const
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
    fin.read(reinterpret_cast<char*>(first->data()), first->size() * sizeof(RArr::value_type));
  }
}

Calsht::Calsht(const std::string& dir)
#ifndef ENABLE_NYANTEN
    : mp1(1953125), mp2(78125)
#else
    : mp1(405350), mp2(43130)
#endif
{
  read_file(mp1.begin(), mp1.end(), std::filesystem::path(dir) / "index_s.bin");
  read_file(mp2.begin(), mp2.end(), std::filesystem::path(dir) / "index_h.bin");
}

int Calsht::calc_lh(const std::array<int, NUM_TIDS>& t, const int m, const bool three_player) const
{
  LArr ret = mp2[hash2(t.cbegin() + 27)];

  add1(ret, mp1[hash1(t.cbegin() + 18)], m);
  add1(ret, mp1[hash1(t.cbegin() + 9)], m);

  if (three_player) {
    add1(ret, index1[t[8]], m);
    add2(ret, index1[t[0]], m);
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

      n += t[i];
    }

#ifdef ENABLE_NYANTEN
    if (n > 14) {
      throw std::invalid_argument(std::format("Invalid sum of hand's tiles: {}", n));
    }
#endif

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
