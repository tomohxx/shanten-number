# ShantenNumberCalculator

## Usage
1. Include the header file "calsht.hpp".
2. Instantiate the class "Calsht".
3. Prepare an int type array of length 34 representing a hand.<br>
The n'th element stores the number of the n'th tile (n=0:1m, ..., n=8:9m, n=9:1p, ..., n=17:9p, n=18:1s, .. , n=26:9s, n=27:east, ...,n=30:north, n=31:white, n=32:green, n=33:red).
3. Call the method of "Calsht" class and calculate the shanten number. Note that each method returns a value of shanten number + 1.

## Class methods
- Calculate shanten number of legal hand.
  - int calc_lh(int* [an array representing the hand], int [number of tiles])
- Calculate shanten number of seven pairs.
  - int calc_sp(int* [an array representing the hand])
- Calculate shanten number of thirteen orphans.
  - int calc_to(int* [an array representing the hand])
- Calculate shanten number of general hand (the minimum value of the above-mentioned shanten number).
  - int operator()(int* [an array representing the hand], int [number of tiles]])

## How to run a sample program
This progaram generates hands at random and calculate the expected value of shanten numbers.
~~~shell{
$ make sample.out
$ ./sample.out [number of tiles (ex. 13)] [number of trails (ex. 100000000)]
=========================RESULT=========================
-1  0           0
0   8150        0.00815
1   622033      0.622033
2   9354929     9.35493
3   36203040    36.203
4   39873533    39.8735
5   13101192    13.1012
6   837123      0.837123
Number of Tiles         13
Total                   100000000
Time (msec.)            82466
Expected Value          3.57964
~~~
## Notes
- You need a compiler that supports C++11.
- Put "index_h.txt" and "index_s.txt" in the same directory as the executable file.
