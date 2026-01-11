# Shanten Number

このプログラムに使用しているアルゴリズムはシャンテン数を正確に計算できることが証明されています.

[Read this in English.](README.md)

## シャンテン数

シャンテン数はテンパイまでに必要な最小のツモ回数のことです.

## 前提条件

本プロジェクトは並列テーブル構築に[Intel Threading Building Blocks (TBB)](https://github.com/oneapi-src/oneTBB)を必要とします。ビルド前に、お使いのシステムにTBBがインストールされていることを確認してください。

**インストール手順:**

- **macOS (Homebrew):**
  ```
  $ brew install tbb
  ```

- **Ubuntu/Debian:**
  ```
  $ sudo apt-get install libtbb-dev
  ```

- **Fedora/RHEL:**
  ```
  $ sudo dnf install tbb-devel
  ```

## ビルド

### デバッグモード

```
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Debug
$ make
```

### リリースモード

```
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ make
```
> **NOTE:** C++20以上に対応したコンパイラが必要です。Intel TBBがインストールされていることが必須です ([前提条件](#前提条件)を参照)。

### コンパイルオプション

#### `-DENBALE_NYANTEN`

テーブルの検索アルゴリズムにCryolite氏のnyanten[^1][^2]で使用されている最小完全ハッシュ関数を使用します. このオプションを有効にすることでテーブルのサイズを小さくできます. ただし, シャンテン数を計算できる手牌の枚数が14枚以下に制限されます.

[^1]: https://github.com/Cryolite/nyanten
[^2]: https://www.slideshare.net/slideshow/a-fast-and-space-efficient-algorithm-for-calculating-deficient-numbers-a-k-a-shanten-numbers-pptx/269706666

#### `-DFIX_RANDOM_SEED`

サンプルプログラムで使用する乱数のシードを固定します.

### テーブルの作成

シャンテン数計算に必要なパラメータテーブルを構築します. `index_h.bin`, `index_s.bin`を作成します.

```
$ ./mkind
```

## 使用方法

1. 手牌を表す`std::array<int, 34>`配列を用意します.
   - `n`番目の要素が`n`番目の牌の枚数を格納します.

   |        | 1       | 2       | 3       | 4       | 5       | 6       | 7       | 8       | 9       |
   | :----- | :------ | :------ | :------ | :------ | :------ | :------ | :------ | :------ | :------ |
   | マンズ | 0 (1m)  | 1 (2m)  | 2 (3m)  | 3 (4m)  | 4 (5m)  | 5 (6m)  | 6 (7m)  | 7 (8m)  | 8 (9m)  |
   | ピンズ | 9 (1p)  | 10 (2p) | 11 (3p) | 12 (4p) | 13 (5p) | 14 (6p) | 15 (7p) | 16 (8p) | 17 (9p) |
   | ソーズ | 18 (1s) | 19 (2s) | 20 (3s) | 21 (4s) | 22 (5s) | 23 (6s) | 24 (7s) | 25 (8s) | 26 (9s) |
   | 字牌   | 27 (東) | 28 (南) | 29 (西) | 30 (北) | 31 (白) | 32 (発) | 33 (中) |         |         |

   - 例えば123m245779p13555zのような手牌の場合, 以下の配列を定義します.

   ```cpp
   std::array<int, 34> hand = {
       1, 1, 1, 0, 0, 0, 0, 0, 0, // Manzu
       0, 1, 0, 1, 1, 0, 2, 0, 1, // Pinzu
       0, 0, 0, 0, 0, 0, 0, 0, 0, // Souzu
       1, 0, 1, 0, 3, 0, 0        // Jihai
   };
   ```

1. シャンテン数を計算します. 各メソッドは**シャンテン数+1**の値を返します.
   - 一般形(`m`面子一雀頭):
   ```cpp
   int Calsht::calc_lh(const std::array<int, 34>& t, int m, bool three_player = false) const
   ```

   > **NOTE:** 通常, `m`には手牌の枚数を3で割った値を代入します.

   - 七対子:
   ```cpp
   int Calsht::calc_sp(const std::array<int, 34>& t, bool three_player = false) const
   ```
   - 国士無双:
   ```cpp
   int Calsht::calc_to(const std::array<int, 34>& t) const
   ```
   - 標準形:
   ```cpp
   std::tuple<int, int> Calsht::operator()(const std::array<int, 34>& t,
                                           int m,
                                           int mode,
                                           bool check_hand = false,
                                           bool three_player = false) const
   ```
   > **NOTE:** `mode`にはどのあがりパターンに対してシャンテン数を計算するかを指定します. 一般形の場合は1, 七対子の場合は2, 国士無双の場合は4です. 複数のあがりパターンに対してシャンテン数を計算する場合はそれらの論理和を指定します.

   > **NOTE:** このメソッドはシャンテン数の最小値とそのあがりパターンのタプルを返します. あがりパターンは`mode`と同じように表されます.

   > **NOTE:** `check_hand`を`true`にすると手牌のバリデーションを行います. `three_player`を`true`にすると三人麻雀でのシャンテン数を計算します.

   例として, 先に定義した手牌のシャンテン数を計算します.

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
   出力:
   ```
   3
   1
   ```

## サンプルプログラム

手牌をランダムに生成し, シャンテン数ごとの出現率とシャンテン数の期待値を計算します.

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

## ライセンス

GNU Lesser General Public License v3.0.
