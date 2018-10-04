# ShantenNumberCalculator

## Usage
1. Include the header file "calsht.hpp".
2. Instantiate the class "Calsht".
3. Prepare an int type array of length 34 representing a hand.<br>
The n'th element stores the number of the n'th tile (n=0:1m, ..., n=8:9m, n=9:1p, ..., n=17:9p, n=18:1s, .. , n=26:9s, n=27:east, ...,n=30:north, n=31:white, n=32:green, n=33:red).
3. Call the method of "Calsht" class and calculate the shanten number. Note that each method returns a value of shanten number + 1.

  - n面子一雀頭形のシャンテン数を求めるには
    - int calc_lh(int* [手牌を表す配列], int [手牌の枚数])
  - 七対子のシャンテン数も求めるには
    - int calc_sp(int* [手牌を表す配列])
  - 国士無双のシャンテン数を求めるには
    - int calc_to(int* [手牌を表す配列])
  - 一般形のシャンテン数(上記のシャンテン数の最小値)を求めるには
    - int operator()(int* [手牌を表配列], int [手牌の枚数]])

## How to run a sample program
This progaram generates hands at random and calculate the expected value of shanten numbers.
~~~shell{
$ make sample.out
$ ./sample.out [number of tiles (ex. 13)] [number of trails (ex. 100000000)]
~~~
## Notes
- You need a compiler that supports C++11.
- Put "index_h.txt" and "index_s.txt" in the same directory as the executable file.
