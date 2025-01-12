#ifndef HASH_HPP
#define HASH_HPP

#include <numeric>
#include <vector>

template <int Length>
struct DefaultHash {
  int operator()(std::vector<int>::const_iterator iter) const
  {
    return std::accumulate(iter + 1,
                           iter + Length,
                           *iter,
                           [](const int& x, const int& y) {
                             return 5 * x + y;
                           });
  }
};

template <int Length>
struct NyantenHash {
  static constexpr int MAX_TILES = 14;
  std::array<std::array<std::array<int, 5>, MAX_TILES + 1>, Length> table{};

  NyantenHash()
  {
    std::array<std::array<int, MAX_TILES + 1>, Length + 1> temp_table{};

    temp_table[Length].fill(1);

    for (int i = Length - 1; i >= 0; --i) {
      for (int n = 0; n <= std::min(4 * i, MAX_TILES); ++n) {
        for (int c = 0; c <= std::min(MAX_TILES - n, 4); ++c) {
          temp_table[i][n] += temp_table[i + 1][n + c];
        }
      }
    }

    for (int i = 0; i < Length; ++i) {
      for (int n = 0; n <= MAX_TILES; ++n) {
        for (int c = 0; c <= std::min(n, 4); ++c) {
          for (int cc = 0; cc < c; ++cc) {
            table[i][n][c] += temp_table[i + 1][n - c + cc];
          }
        }
      }
    }
  }

  int operator()(std::vector<int>::const_iterator iter) const
  {
    int h = 0;
    int n = 0;

    for (int i = 0; i < Length; ++i) {
      const int c = *iter++;

      n += c;
      h += table[i][n][c];
    }

    return h;
  }
};

#endif
