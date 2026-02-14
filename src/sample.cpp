#include "calsht.hpp"
#include <algorithm>
#include <array>
#include <cstdlib>
#include <format>
#include <iostream>
#include <random>
#include <utility>
#ifndef FIX_RANDOM_SEED
#define SEED (std::random_device{}())
#else
#define SEED (0)
#endif

int main(int argc, char* argv[])
{
  if (argc != 4) {
    std::cerr << std::format("Usage: {} [Number of tiles] "
                             "[Number of hands] [Whether three player]\n",
                             argv[0]);
    return 1;
  }

  constexpr int NUM_TIDS = 34;
  constexpr int MAX_SHT = 8;
  constexpr int MODE = 7;
  const int NUM_TILES = std::atoi(argv[1]);
  const int NUM_HANDS = std::atoi(argv[2]);
  const bool THREE_PLAYER = std::atoi(argv[3]);
  std::array<int, NUM_TIDS> hand{};
  std::vector<int> wall;
  std::array<int, MAX_SHT> sht{};
  std::mt19937_64 rand(SEED);
  Calsht calsht(INDEX_FILE_PATH);

  wall.reserve(NUM_TIDS * 4);

  for (int i = 0; i < NUM_TIDS; ++i) {
    if (THREE_PLAYER && i > 0 && i < 8) continue;

    for (int j = 0; j < 4; ++j) {
      wall.push_back(i);
    }
  }

  for (int i = 0; i < NUM_HANDS; ++i) {
    std::fill(hand.begin(), hand.end(), 0);

    for (int j = 0; j < NUM_TILES; ++j) {
      const int n = rand() % (wall.size() - j);

      ++hand[wall[n]];
      std::swap(wall[n], wall[wall.size() - j - 1]);
    }

    const auto [num, mode] = calsht(hand, NUM_TILES / 3, MODE, false, THREE_PLAYER);

    ++sht[num];
  }

  double ev = 0.;

  for (int i = 0; i < MAX_SHT; i++) {
    ev += (i - 1) * sht[i];
    std::cout << std::format("{:4d}{:12d}{:12f}\n",
                             i - 1,
                             sht[i],
                             sht[i] * 100. / NUM_HANDS);
  }

  std::cout << std::format("{:<24s}{:<16d}\n", "Number of Tiles", NUM_TILES);
  std::cout << std::format("{:<24s}{:<16d}\n", "Number of Hands", NUM_HANDS);
  std::cout << std::format("{:<24s}{:<24f}\n", "Expected Value", ev / NUM_HANDS);

  return 0;
}
