#include "calsht.hpp"

void Calsht::add(Vec& lhs, const Vec& rhs) const
{
  for(int j=9; j>=5; --j){
    int sht = std::min(lhs[j]+rhs[0], lhs[0]+rhs[j]);
    
    for(int k=5; k<j; ++k){
      sht = std::min({sht, lhs[k]+rhs[j-k], lhs[j-k]+rhs[k]});
    }
    lhs[j] = sht;
  }
  for(int j=4; j>=0; --j){
    int sht = lhs[j]+rhs[0];
    
    for(int k=0; k<j; ++k){
      sht = std::min(sht, lhs[k]+rhs[j-k]);
    }
    lhs[j] = sht;
  }
}
  
void Calsht::add(Vec& lhs, const Vec& rhs, const int j) const
{
  int sht = std::min(lhs[j]+rhs[0], lhs[0]+rhs[j]);
  
  for(int k=5; k<j; ++k){
    sht = std::min({sht, lhs[k]+rhs[j-k], lhs[j-k]+rhs[k]});
  }
  lhs[j] = sht;
}

Calsht::Iter Calsht::read_file(Iter first, Iter last, const char* filename) const
{
  std::ifstream fin(filename);
  Vec vec(10);
  
  if(fin.is_open()){
    while(first != last){
      for(int j=0; j<10; ++j) fin >> vec[j];
      *first++ = vec;
    }
  }
  return first;
}

Calsht::Calsht() : mp1(std::vector<Vec>(1953125,Vec(10))), mp2(std::vector<Vec>(78125,Vec(10))), itr1(mp1.begin()), itr2(mp2.begin())
{
  itr1 = read_file(mp1.begin(),mp1.end(),"index_s.txt");
  itr2 = read_file(mp2.begin(),mp2.end(),"index_h.txt");
}

bool Calsht::operator!() const
{
  return mp1.empty() || mp2.empty() || itr1==mp1.begin() || itr2==mp2.begin();
}

int Calsht::calc_lh(const int* t, const int m) const
{
  Vec ret = mp1[std::accumulate(t+1, t+9, t[0], [](int x, int y){return 5*x+y;})];

  add(ret, mp1[std::accumulate(t+10, t+18, t[9], [](int x, int y){return 5*x+y;})]);
  add(ret, mp1[std::accumulate(t+19, t+27, t[18], [](int x, int y){return 5*x+y;})]);
  add(ret, mp2[std::accumulate(t+28, t+34, t[27], [](int x, int y){return 5*x+y;})], 5+m/3);
  return (ret[5+m/3]+m/3*3+2-m)/2;
}
  
int Calsht::calc_sp(const int* t) const
{
  int pair = 0, kind = 0;

  for(int i=0; i<34; ++i){
    if(t[i]>0){
      ++kind;
      if(t[i]>=2) ++pair;
    }
  }
  return 7-pair+(kind<7 ? 7-kind:0);
}
  
int Calsht::calc_to(const int* t) const
{
  int pair = 0, kind = 0;

  for(int i : {0,8,9,17,18,26,27,28,29,30,31,32,33}){
    if(t[i]>0){
      ++kind;
      if(t[i]>=2) ++pair;
    }
  }
  return 14-kind-(pair>0 ? 1:0);
}

int Calsht::operator()(const int* t, const int m) const
{
  return m>12 ? std::min({calc_lh(t,m),calc_sp(t),calc_to(t)}) : calc_lh(t,m);
}