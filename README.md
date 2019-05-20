# ShantenNumberCalculator
A tool for calculating Shanten number in Japanes mahjong

[Read this in Japanes (日本語).](README.ja.md)

## Usage
1. Prepare an int-type array of length 34 representing a hand.
    - The n th element stores the number of n th tiles.

        ||1|2|3|4|5|6|7|8|9|
        |:--|:--|:--|:--|:--|:--|:--|:--|:--|:--|
        |Manzu|0|1|2|3|4|5|6|7|8|
        |Pinzu|9|10|11|12|13|14|15|16|17|
        |Souzu|18|19|20|21|22|23|24|25|26|
        |Jihai|27 (East)|28 (South)|29 (West)|30 (North)|31 (White)|32 (Green)|33 (Red)|||

    - For example, if you have Manzu tiles (1, 2, 3), Pinzu tiles (2, 4, 5, 7, 7, 9), and Jihai tiles (East, West, White, White, White), define the following array.

        ~~~cpp
        int hand[34] = {
            1,1,1,0,0,0,0,0,0, //Manzu
            0,1,0,1,1,0,2,0,1, //Pinzu
            0,0,0,0,0,0,0,0,0, //Souzu
            1,0,1,0,3,0,0 //Jihai
        };
        ~~~

2. Calculate the Shanten number.
    - Calculate Shanten number of a hand composed of _n_ tile groups and a pair.
        - Normally, substitute the value obtained by dividing the number of tiles by 3 into _n_.
        ~~~cpp
        int Calsht::calc_lh(int* hand, int n)
        ~~~
    - Calculate Shanten number of Seven Pairs.
        ~~~cpp
        int Calsht::calc_sp(int* hand)
        ~~~
    - Calculate Shanten number of Thirteen Orphans.
        ~~~cpp
        int Calsht::calc_to(int* hand)
        ~~~
    - Calculate the minimun value of the Shanten number above.
        ~~~cpp
        int Calsht::operator()(int* hand, int n)
        ~~~
    - Note that each method returns a value of Shanten number + 1.

## Sample Program
- Randomly genearte hands and calculate the appearance rate per Shanten number and the expected value of Shanten number.
- It requires a compiler compatiable with C++11 or higher. 

~~~shell
$ make sample.out
$ ./sample.out [number of tiles (e.g. 14)] [number of trials (e.g. 100000000)]
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
~~~
