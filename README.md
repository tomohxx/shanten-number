# Shanten Number Calculator
The algorithm used in this program has been proven to be able to calculate the shanten number accurately.

[Read this in Japanese (日本語).](README.ja.md)

## What is Shanten number in mahjong ?
Shanten number is the minimul number of tile exchanges for "tempai".

## Usage
1. Prepare a `std::vector<int>` array representing a hand.
- The `n` th element stores the number of `n` th tiles.

||1|2|3|4|5|6|7|8|9|
|:--|:--|:--|:--|:--|:--|:--|:--|:--|:--|
|*Manzu*|0|1|2|3|4|5|6|7|8|
|*Pinzu*|9|10|11|12|13|14|15|16|17|
|*Souzu*|18|19|20|21|22|23|24|25|26|
|*Jihai*|27 (*East*)|28 (*South*)|29 (*West*)|30 (*North*)|31 (*White*)|32 (*Green*)|33 (*Red*)|||

- For example, if you have *manzu* tiles (1, 2, 3), *pinzu* tiles (2, 4, 5, 7, 7, 9), and *jihai* tiles (*East*, *West*, *White*, *White*, *White*), define the following array.

```cpp
std::vector<int> hand = {
  1,1,1,0,0,0,0,0,0, //Manzu
  0,1,0,1,1,0,2,0,1, //Pinzu
  0,0,0,0,0,0,0,0,0, //Souzu
  1,0,1,0,3,0,0 //Jihai
};
```

2. Calculate the Shanten number. Each method returns a value of Shanten number + 1.
- General Form (`m` tile groups and a pair):
```cpp
int Calsht::calc_lh(int* hand, int m)
```

> **NOTE:** Normally, substitute the value obtained by dividing the number of tiles by 3 into `m`.

- Seven Pairs:
```cpp
int Calsht::calc_sp(int* hand)
```
- Thirteen Orphans:
```cpp
int Calsht::calc_to(int* hand)
```
- Normal Form:
```cpp
std::tuple<int,int> Calsht::operator()(const std::vector<int>& hand, int m, int mode)
```
> **NOTE:** `mode` specifies for which winning pattern calculate shanten number. When the pattern is "General Form", `mode` is 1, when "Seven Pairs": 2, "Thirteen Orphans": 4. When calculating the Shanten number for multiple winning patterns, specify the logical sum of them.

> **NOTE:** This method returns a tuple of the minimum shunten number and its winnig pattern. The winning pattern is represented in the same way as `mode`.

For example, calculate the shanten number of the hand defined above. The source code is as follows:

```cpp
#include <iostream>
#include <vector>
#include "calsht.hpp"

int main()
{
  Calsht calsht;
  std::vector<int> hand = {
    1,1,1,0,0,0,0,0,0,// manzu
    0,1,0,1,1,0,2,0,1,// pinzu
    0,0,0,0,0,0,0,0,0,// souzu
    1,0,1,0,3,0,0// jihai
  };

  auto [sht, mode] = calsht(hand, 4, 7);

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
- Randomly genearte hands and calculate the appearance rate per Shanten number and the expected value of Shanten number.

### Build
- Debug mode
```
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Debug
$ make
```

- Release mode
```
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ make
```
> **NOTE:** It requires a compiler compatiable with C++17 or higher.

### Execute
```
$ ./sample [number of tiles (e.g. 14)] [number of rounds (e.g. 100000000)]
=========================RESULT=========================
-1  315         0.000315
0   69900       0.0699
1   2333875     2.33387
2   19496567    19.4966
3   43932086    43.9321
4   28515676    28.5157
5   5496180     5.49618
6   155401      0.155401
Number of Tiles         14
Total                   100000000
Time (msec.)            99710
Expected Value          3.15599
```

## Three Player Mahjong mode
Enable `THREE_PLAYER`.

Example:
```
$ cmake .. -DCMAKE_BUILD_TYPE=Release -DTHREE_PLAYER=on
```

## Building tables (Unneeded)
- Build tables of parameters required for calculating Shanten number. Make "index_h.txt" and "index_s.txt".

```
$ ./mkind1
```
