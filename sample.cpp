#include <iostream>
#include <iomanip>
#include <array>
#include <random>
#include <algorithm>
#include <chrono>
#include <utility>
#include "calsht.hpp"

int main(int argc, char* argv[])
{
  if(argc != 3){
    return 1;
  }

  const int T = 34;
  const int M = std::atoi(argv[1]);
  const int N = std::atoi(argv[2]);
  int num = 0;
  int n = 0;
  double ev = 0;
  std::array<int,T> hd = {0};
  std::array<int,4*T> wall = {0};
  std::array<int,8> sht = {0};
  std::mt19937 rand(std::random_device{}());
  Calsht calsht;
    
  if(!calsht) return 1;
    
  for(int i=0; i<4; ++i){
    for(int j=0; j<T; ++j){
      wall[T*i+j] = j;
    }
  }
  
  auto start = std::chrono::system_clock::now();
  
  for(int i=0; i<N; ++i){
    hd.fill(0);

    for(int j=0; j<M; ++j){
      n = rand()%(4*T-j);
      ++hd[wall[n]];
      std::swap(wall[n],wall[4*T-1-j]);
    }
    num = calsht(hd.data(),M/3);
    ++sht[num];
  }

  auto end = std::chrono::system_clock::now();

  std::cout.setf(std::ios::left,std::ios::adjustfield);

  std::cout << "=========================RESULT=========================\n";

  for(int i=0; i<8; i++){
    std::cout << std::setw(4) << (i-1) << std::setw(12) << sht[i] << std::setw(12) << 100.0*sht[i]/N << '\n';
    ev += (i-1)*sht[i];
  }
  ev /= N;

  std::cout << std::setw(24) << "Number of Tiles" << std::setw(16) << M << '\n';
  std::cout << std::setw(24) << "Total" << std::setw(16) << N << '\n';
  std::cout << std::setw(24) << "Time (msec.)" << std::setw(16) << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << '\n';
  std::cout << std::setw(24) << "Expected Value" << std::setw(24) << ev << '\n';
  
  return 0;
}
