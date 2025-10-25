# Shanten Number

The algorithm used in this program has been proven to be able to calculate the shanten number accurately.

[Read this in Japanese (日本語).](README.ja.md)

## What is Shanten number in mahjong?

Shanten number is the minimul number of tile exchanges for "tempai".

## Build

### Debug mode

```
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Debug
$ make
```

### Release mode

```
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ make
```
> **NOTE:** It requires a compiler compatiable with C++20 or higher.

### Compile options

#### `-DENBALE_NYANTEN`

It enables the table search algorithm to use the minimal perfect hash function used in Cryolite's nyanten[^1][^2]. Enabling this option can reduce the size of tables. However, the number of tiles in a hand that can be calculated shanten number is limited to 14 or less.

[^1]: https://github.com/Cryolite/nyanten
[^2]: https://www.slideshare.net/slideshow/a-fast-and-space-efficient-algorithm-for-calculating-deficient-numbers-a-k-a-shanten-numbers-pptx/269706666

#### `-DFIX_RANDOM_SEED`

It fixes the random seed used in the sample program.

#### Building tables

Build tables of parameters required for calculating shanten number. Create files `index_h.bin` and `index_s.bin`.

```
$ ./mkind
```

## Usage

1. Prepare a `std::array<int, 34>` array representing a hand.
   - The `n` th element stores the number of `n` th tiles.

   |         | 1           | 2            | 3           | 4            | 5            | 6            | 7          | 8       | 9       |
   | :------ | :---------- | :----------- | :---------- | :----------- | :----------- | :----------- | :--------- | :------ | :------ |
   | *Manzu* | 0 (1m)      | 1 (2m)       | 2 (3m)      | 3 (4m)       | 4 (5m)       | 5 (6m)       | 6 (7m)     | 7 (8m)  | 8 (9m)  |
   | *Pinzu* | 9 (1p)      | 10 (2p)      | 11 (3p)     | 12 (4p)      | 13 (5p)      | 14 (6p)      | 15 (7p)    | 16 (8p) | 17 (9p) |
   | *Souzu* | 18 (1s)     | 19 (2s)      | 20 (3s)     | 21 (4s)      | 22 (5s)      | 23 (6s)      | 24 (7s)    | 25 (8s) | 26 (9s) |
   | *Jihai* | 27 (*East*) | 28 (*South*) | 29 (*West*) | 30 (*North*) | 31 (*White*) | 32 (*Green*) | 33 (*Red*) |         |         |

   - For example, if you have *manzu* tiles (1, 2, 3), *pinzu* tiles (2, 4, 5, 7, 7, 9), and *jihai* tiles (*East*, *West*, *White*, *White*, *White*), define the following array.

   ```cpp
   std::array<int, 34> hand = {
       1, 1, 1, 0, 0, 0, 0, 0, 0, // Manzu
       0, 1, 0, 1, 1, 0, 2, 0, 1, // Pinzu
       0, 0, 0, 0, 0, 0, 0, 0, 0, // Souzu
       1, 0, 1, 0, 3, 0, 0        // Jihai
   };
   ```

2. Calculate the shanten number. Each method returns a value of **shanten number + 1**.
   - General Form (`m` melds and a pair):
   ```cpp
   int Calsht::calc_lh(const std::array<int, 34>& t, int m, bool three_player = false) const
   ```

   > **NOTE:** Normally, substitute the value obtained by dividing the number of tiles by 3 into `m`.

   - Seven Pairs:
   ```cpp
   int Calsht::calc_sp(const std::array<int, 34>& t, bool three_player = false) const
   ```
   - Thirteen Orphans:
   ```cpp
   int Calsht::calc_to(const std::array<int, 34>& t) const
   ```
   - Normal Form:
   ```cpp
   std::tuple<int, int> Calsht::operator()(const std::array<int, 34>& t,
                                           int m,
                                           int mode,
                                           bool check_hand = false,
                                           bool three_player = false) const
   ```
   > **NOTE:** `mode` specifies for which winning pattern calculate shanten number. When the pattern is "General Form", `mode` is 1, when "Seven Pairs": 2, "Thirteen Orphans": 4. When calculating the shanten number for multiple winning patterns, specify the logical sum of them.

   > **NOTE:** This method returns a tuple of the minimum shunten number and its winnig pattern. The winning pattern is represented in the same way as `mode`.

   > **NOTE:** If you set `check_hand` to `true`, the hand will be validated. If you set `three_player` to `true`, it will calculate the number of shanten in three-player mahjong.

   For example, calculate the shanten number of the hand defined above. The source code is as follows:

   ```cpp
   #include "calsht.hpp"
   #include <array>
   #include <filesystem>
   #include <iostream>

   int main()
   {
     Calsht calsht;

     // Set the location of shanten tables
     calsht.initialize(std::filesystem::current_path());

     std::array<int, 34> hand = {
         1, 1, 1, 0, 0, 0, 0, 0, 0, // manzu
         0, 1, 0, 1, 1, 0, 2, 0, 1, // pinzu
         0, 0, 0, 0, 0, 0, 0, 0, 0, // souzu
         1, 0, 1, 0, 3, 0, 0        // jihai
     };

     const auto [sht, mode] = calsht(hand, 4, 7);

     std::cout << sht << std::endl;
     std::cout << mode << std::endl;

     return 0;
   }
   ```
   Output:
   ```
   3
   1
   ```

## Sample Program

- Randomly genearte hands and calculate the appearance rate per shanten number and the expected value of shanten number.

```
$ ./sample 14 100000000 0
  -1         278    0.000278
   0       69553    0.069553
   1     2334287    2.334287
   2    19502040   19.502040
   3    43925782   43.925782
   4    28516861   28.516861
   5     5496101    5.496101
   6      155098    0.155098
Number of Tiles         14
Number of Hands         100000000
Expected Value          3.155940
```

## License

GNU Lesser General Public License v3.0.
