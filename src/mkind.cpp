#include <algorithm>
#include <execution>
#include <fstream>
#include <iostream>
#include <numeric>
#include <type_traits>
#include <utility>
#include <vector>
#ifndef ENABLE_NYANTEN
#define ENABLE_NYANTEN false
#endif
#ifndef ENABLE_PARALLEL
#define ENABLE_PARALLEL false
#endif
#define NUM_TILE (ENABLE_NYANTEN ? std::min(m, 4) : 4)
constexpr int MAX_TILES = 14;
constexpr int MAX_SHT = 14;
constexpr int NUM_MELDS = 4;
constexpr std::conditional<ENABLE_PARALLEL,
                           std::execution::parallel_policy,
                           std::execution::sequenced_policy>::type policy{};
using Hand = std::vector<uint8_t>;
using Hands = std::vector<std::pair<Hand, unsigned int>>;
using Meld = std::vector<int>;
using Melds = std::vector<Meld>;

inline bool is_valid_target(const Hand& hand)
{
  return std::all_of(hand.begin(), hand.end(), [](const int x) { return x <= 4; });
}

inline uint8_t calc_distance(const Hand& current, const Hand& target)
{
  return std::inner_product(
      target.begin(),
      target.end(),
      current.begin(),
      0,
      std::plus<int>(),
      [](const auto& x, const auto& y) { return std::max(x - y, 0); });
}

inline void add_meld(Hand& target, const Meld& meld)
{
  std::for_each(meld.begin(), meld.end(), [&target](const int x) { ++target[x]; });
}

inline void remove_meld(Hand& target, const Meld& meld)
{
  std::for_each(meld.begin(), meld.end(), [&target](const int x) { --target[x]; });
}

void dfs(const Hand& current,
         Hand& target,
         const int m,
         const int min_mid,
         const Melds& melds,
         std::vector<uint8_t>& sht)
{
  for (int tid = 0; tid < static_cast<int>(current.size()); ++tid) {
    target[tid] += 2;

    if (is_valid_target(target)) {
      const auto distance = calc_distance(current, target);

      sht[m + 5] = std::min(sht[m + 5], distance);
    }

    target[tid] -= 2;
  }

  if (m >= NUM_MELDS) return;

  for (std::size_t mid = min_mid; mid < melds.size(); ++mid) {
    add_meld(target, melds[mid]);

    if (is_valid_target(target)) {
      const auto distance = calc_distance(current, target);

      sht[m + 1] = std::min(sht[m + 1], distance);

      if (distance < sht[NUM_MELDS + 5]) {
        dfs(current, target, m + 1, mid, melds, sht);
      }
    }

    remove_meld(target, melds[mid]);
  }
}

void deal(const int n, const int m, Hand& hand, Hands& hands)
{
  if (n >= static_cast<int>(hand.size())) {
    hands.push_back(std::make_pair(hand, hands.size()));
  }
  else {
    for (int i = 0; i <= NUM_TILE; ++i) {
      hand[n] = i;
      deal(n + 1, m - i, hand, hands);
    }
  }
}

int main()
{
  {
    std::ofstream fout("index_s.bin", std::ios_base::out | std::ios_base::binary);

    if (!fout) {
      std::cerr << "Failed to open \"index_s.bin\"" << std::endl;
      return 1;
    }

    Hand hand(9);
    Melds melds;
    Hands hands;

    hands.reserve(1953125); // 5^9

    for (int tid = 0; tid < static_cast<int>(hand.size()); ++tid) {
      melds.emplace_back(Meld{tid, tid, tid});
    }

    for (int tid = 0; tid < static_cast<int>(hand.size()) - 2; ++tid) {
      melds.emplace_back(Meld{tid, tid + 1, tid + 2});
    }

    deal(0, MAX_TILES, hand, hands);

    std::vector<std::vector<uint8_t>> dists(hands.size(), std::vector<uint8_t>(10, MAX_SHT));

    std::for_each(policy, hands.begin(), hands.end(),
                  [&melds, &dists](const auto& hand_hash) {
                    Hand target(9);
                    auto& dist = dists[hand_hash.second];

                    dist[0] = 0u;
                    dfs(hand_hash.first, target, 0, 0, melds, dist);
                  });

    std::for_each(dists.begin(), dists.end(),
                  [&fout](const auto& dist) {
                    fout.write(reinterpret_cast<const char*>(dist.data()), dist.size() * sizeof(uint8_t));
                  });
  }

  {
    std::ofstream fout("index_h.bin", std::ios_base::out | std::ios_base::binary);

    if (!fout) {
      std::cerr << "Failed to open \"index_h.bin\"" << std::endl;
      return 1;
    }

    Hand hand(7);
    Melds melds;
    Hands hands;

    hands.reserve(78125); // 5^7

    for (int tid = 0; tid < static_cast<int>(hand.size()); ++tid) {
      melds.emplace_back(Meld{tid, tid, tid});
    }

    deal(0, MAX_TILES, hand, hands);

    std::vector<std::vector<uint8_t>> dists(hands.size(), std::vector<uint8_t>(10, MAX_SHT));

    std::for_each(policy, hands.begin(), hands.end(),
                  [&melds, &dists](const auto& hand_hash) {
                    Hand target(7);
                    auto& dist = dists[hand_hash.second];

                    dist[0] = 0u;
                    dfs(hand_hash.first, target, 0, 0, melds, dist);
                  });

    std::for_each(dists.begin(), dists.end(),
                  [&fout](const auto& dist) {
                    fout.write(reinterpret_cast<const char*>(dist.data()), dist.size() * sizeof(uint8_t));
                  });
  }

  return 0;
}
