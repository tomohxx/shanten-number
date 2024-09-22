#include <algorithm>
#include <fstream>
#include <numeric>
#ifdef CHECK_HAND
#include <sstream>
#include <stdexcept>
#endif
#include "calsht.hpp"

#ifdef THREE_PLAYER
Calsht::RVec Calsht::index1(const int n) const
{
  RVec ret(10, INT8_C(127));

  ret[0] = INT8_C(0);
  ret[1] = std::max(3 - n, INT8_C(0));
  ret[5] = std::max(2 - n, INT8_C(0));

  return ret;
}
#endif

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
  int j = m + 5;
  int sht = std::min(lhs[j] + rhs[0], lhs[0] + rhs[j]);

  for (int k = 5; k < j; ++k) {
    sht = std::min({sht, lhs[k] + rhs[j - k], lhs[j - k] + rhs[k]});
  }
  lhs[j] = sht;
}

Calsht::Iter Calsht::read_file(Iter first, Iter last, std::filesystem::path file) const
{
  std::ifstream fin(file);

  if (!fin) {
    throw std::runtime_error("Reading file does not exist: " + file.string());
  }

  int tmp;

  for (; first != last; ++first) {
    for (int j = 0; j < 10; ++j) {
      fin >> tmp;
      (*first)[j] = static_cast<int8_t>(tmp);
    }
  }
  return first;
}

void Calsht::initialize(const std::string& dir)
{
  read_file(mp1.begin(), mp1.end(), std::filesystem::path(dir) / "index_s.txt");
  read_file(mp2.begin(), mp2.end(), std::filesystem::path(dir) / "index_h.txt");
}

int Calsht::calc_lh(const int* t, const int m) const
{
#ifdef THREE_PLAYER
  LVec ret = [](const RVec& rhs) {
    return LVec(rhs.begin(), rhs.end());
  }(index1(t[0]));

  add1(ret, index1(t[8]), m);
#else
  LVec ret = [](const RVec& rhs) {
    return LVec(rhs.begin(), rhs.end());
  }(mp1[std::accumulate(t + 1, t + 9, t[0], [](int x, int y) { return 5 * x + y; })]);
#endif

  add1(ret, mp1[std::accumulate(t + 10, t + 18, t[9], [](int x, int y) { return 5 * x + y; })], m);
  add1(ret, mp1[std::accumulate(t + 19, t + 27, t[18], [](int x, int y) { return 5 * x + y; })], m);
  add2(ret, mp2[std::accumulate(t + 28, t + 34, t[27], [](int x, int y) { return 5 * x + y; })], m);
  return ret[5 + m];
}

int Calsht::calc_sp(const int* t) const
{
  int pair = 0;
  int kind = 0;

  for (int i = 0; i < K; ++i) {
#ifdef THREE_PLAYER
    if (i > 0 && i < 8) continue;
#endif
    if (t[i] > 0) {
      ++kind;
      if (t[i] >= 2) ++pair;
    }
  }
  return 7 - pair + (kind < 7 ? 7 - kind : 0);
}

int Calsht::calc_to(const int* t) const
{
  int pair = 0;
  int kind = 0;

  for (int i : {0, 8, 9, 17, 18, 26, 27, 28, 29, 30, 31, 32, 33}) {
    if (t[i] > 0) {
      ++kind;
      if (t[i] >= 2) ++pair;
    }
  }
  return 14 - kind - (pair > 0 ? 1 : 0);
}

std::tuple<int, int> Calsht::operator()(const std::vector<int>& t, const int m, const int mode) const
{
#ifdef CHECK_HAND
  if (m < 0 || m > 4) {
    throw std::runtime_error("Abnormal sum of hands's melds");
  }
  for (int i = 0; i < K; ++i) {
    if (t[i] < 0 || t[i] > 4) {
      std::stringstream ss;
      ss << "Abnormal number of hand's tiles at " << i << ": " << t[i];
      throw std::runtime_error(ss.str());
    }
  }
#endif

  std::tuple<int, int> ret{1024, 0};

  if (mode & 1) {
    if (int sht = calc_lh(t.data(), m); sht < std::get<0>(ret)) {
      ret = {sht, 1};
    }
    else if (sht == std::get<0>(ret)) {
      std::get<1>(ret) |= 1;
    }
  }

  if ((mode & 2) && m == 4) {
    if (int sht = calc_sp(t.data()); sht < std::get<0>(ret)) {
      ret = {sht, 2};
    }
    else if (sht == std::get<0>(ret)) {
      std::get<1>(ret) |= 2;
    }
  }

  if ((mode & 4) && m == 4) {
    if (int sht = calc_to(t.data()); sht < std::get<0>(ret)) {
      ret = {sht, 4};
    }
    else if (sht == std::get<0>(ret)) {
      std::get<1>(ret) |= 4;
    }
  }

  return ret;
}
