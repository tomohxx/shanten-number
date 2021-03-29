# Shanten Number Calculator
このプログラムに使用しているアルゴリズムはシャンテン数を正確に計算できることが証明されています.

[Read this in English.](README.md)

## シャンテン数
シャンテン数はテンパイまでに必要な最小のツモ回数のことです.

## 使用方法
1. 手牌を表す`std::vector<int>`配列を用意します.
- `n`番目の要素が`n`番目の牌の枚数を格納します.

||1|2|3|4|5|6|7|8|9|
|:--|:--|:--|:--|:--|:--|:--|:--|:--|:--|
|マンズ|0|1|2|3|4|5|6|7|8|
|ピンズ|9|10|11|12|13|14|15|16|17|
|ソーズ|18|19|20|21|22|23|24|25|26|
|字牌|27 (東)|28 (南)|29 (西)|30 (北)|31 (白)|32 (発)|33 (中)|||
    
- 例えば123m245779p13555zのような手牌の場合, 以下の配列を定義します.

```cpp
std::vector<int> hand = {
  1,1,1,0,0,0,0,0,0, //Manzu
  0,1,0,1,1,0,2,0,1, //Pinzu
  0,0,0,0,0,0,0,0,0, //Souzu
  1,0,1,0,3,0,0 //Jihai
};
```

2. シャンテン数を計算します. 各メソッドはシャンテン数+1の値を返します.
- 一般形(`m`面子一雀頭):
```cpp
int Calsht::calc_lh(int* hand, int m)
```

> **NOTE:** 通常, `m`には手牌の枚数を3で割った値を代入します.

- 七対子:
```cpp
int Calsht::calc_sp(int* hand)
```
- 国士無双:
```cpp
int Calsht::calc_to(int* hand)
```
- 標準形:
```cpp
std::tuple<int,int> Calsht::operator()(const std::vector<int>& hand, int m, int mode)
```
> **NOTE:** `mode`にはどのあがりパターンに対してシャンテン数を計算するかを指定します. 一般形の場合は1, 七対子の場合は2, 国士無双の場合は4です. 複数のあがりパターンに対してシャンテン数を計算する場合はそれらの論理和を指定します.

> **NOTE:** このメソッドはシャンテン数の最小値とそのあがりパターンのタプルを返します. あがりパターンは`mode`と同じように表されます.

例として, 先に定義した手牌のシャンテン数を計算します.

```cpp
#include <iostream>
#include <vector>
#include "calsht.hpp"

int main()
{
  Calsht calsht;

  // Set the location of shanten tables
  calsht.initialize(".");

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
出力:
```
3
1
```

## サンプルプログラム
手牌をランダムに生成し, シャンテン数ごとの出現率とシャンテン数の期待値を計算します.

### ビルド
- デバッグモード
```
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Debug
$ make
```

- リリースモード
```
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ make
```
> **NOTE:** C++17以上に対応したコンパイラが必要です.

### 実行
```
$ ./sample [手牌の枚数(e.g. 14)] [局数(e.g. 100000000)]
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

## 三人麻雀モード
`THREE_PLAYER`を有効にします.

例:
```
$ cmake .. -DCMAKE_BUILD_TYPE=Release -DTHREE_PLAYER=on
```

## テーブルの構築(不要)
シャンテン数計算に必要なパラメータテーブルを構築します. index_h.txt, index_s.txtを作成します.

```
$ ./mkind1
```

## ライセンス
GNU General Public License v3.0.
