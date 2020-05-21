#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <numeric>
#include <vector>
#include <array>
#include <functional>
#include "judwin.hpp"

template <int N>
using array = std::array<int,N>;

template <int N, class Func>
void dealtile(int, int, int, int&, std::vector<array<N> >&, Func);
template <int N>
void deal(int, array<10>&, std::vector<array<N> >&, std::ofstream&);
template <int N, class Iter>
void calc(array<N>&, array<10>&, array<10>&, Iter);

int main()
{
  array<10> kind = {0};
  array<10> kind_h = {0};
  std::vector<array<9> > vec;
  std::vector<array<7> > vec7;
  std::ofstream fout("index_s.txt");
  std::ofstream fout2("index_h.txt");
  std::function<bool(int*)> func0 = iswh0;
  std::function<bool(int*)> func2 = iswh2;
  std::function<bool(int*)> funcs = iswhs;

  for(int i=0; i<=4; i++){
    dealtile<9>(0,3*i,4*8,kind[i],vec,func0);
  }
  for(int i=0; i<=4; i++){
    dealtile<9>(0,3*i+2,4*8,kind[5+i],vec,func2);
  }
  deal<9>(0,kind,vec,fout);

  for(int i=0; i<=4; i++){
    dealtile<7>(0,3*i,4*6,kind_h[i],vec7,funcs);
  }
  for(int i=0; i<=4; i++){
    dealtile<7>(0,3*i+2,4*6,kind_h[5+i],vec7,funcs);
  }
  deal<7>(0,kind_h,vec7,fout2);

  return 0;
}
template <int N, class Func>
void dealtile(int n, int m, int l, int& cnt, std::vector<array<N> >& vec, Func func)
{
  static array<N> hd;

  if(n>=N){
    if(func(hd.data())){
      cnt++;
      vec.push_back(hd);
    }
  }
  else{
    for(int i=(l>=m ? 0:m-l); i<=(m>=4 ? 4:m); i++){
      hd[n] = i;
      dealtile<N>(n+1,m-i,l-4,cnt,vec,func);
    }
  }
}
template <int N>
void deal(int n, array<10>& kind, std::vector<array<N> >& vec, std::ofstream& fout)
{
  static array<N> hd;
  static array<10> sht;

  if(n>=N){
    calc<N>(hd,sht,kind,vec.begin());
    for(int i=0; i<10; i++) fout << sht[i] << ' ';
    fout << '\n';
  }
  else{
    for(int i=0; i<=4; i++){
      hd[n] = i;
      deal<N>(n+1,kind,vec,fout);
    }
  }
}
template <int N, class Iter>
void calc(array<N>& t, array<10>& ret, array<10>& kind, Iter itr)
{
  int sht = 0;
  int tmp = 0;

  for(int i=0; i<10; i++){
    sht = 100;
    for(int j=0; j<kind[i]; j++){
      tmp = std::inner_product(itr->begin(), itr->end(), t.begin(), 0, std::plus<int>(), [](int x, int y){return abs(x-y)+x-y;})/2;
      sht = std::min(tmp, sht);
      ++itr;
    }
    ret[i] = sht;
  }
}
