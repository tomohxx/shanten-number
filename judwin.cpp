#include "judwin.hpp"

bool iswh0(const int* t)
{
  int a = t[0], b = t[1];

  for (int i = 0; i < 7; ++i) {
    if (int r = a % 3; b >= r && t[i + 2] >= r) {
      a = b - r;
      b = t[i + 2] - r;
    }
    else return false;
  }

  return a % 3 == 0 && b % 3 == 0;
}

bool iswh2(int* t)
{
  int p = 0;

  for (int i = 0; i < 9; ++i) {
    p += i * t[i];
  }

  for (int i = p * 2 % 3; i < 9; i += 3) {
    if (t[i] >= 2) {
      t[i] -= 2;

      if (iswh0(t)) {
        t[i] += 2;
        return true;
      }
      else t[i] += 2;
    }
  }

  return false;
}

bool iswhs(const int* t)
{
  int head = -1;

  for (int i = 0; i < 7; ++i) {
    switch (t[i] % 3) {
    case 1: return false;
    case 2:
      if (head == -1) head = i;
      else return false;
    }
  }

  return true;
}

bool islh(int* t)
{
  int head = -1;

  for (int i = 0; i < 3; ++i) {
    switch (std::accumulate(t + 9 * i, t + 9 * i + 9, 0) % 3) {
    case 1: return false;
    case 2:
      if (head == -1) head = i;
      else return false;
    }
  }

  for (int i = 27; i < 34; ++i) {
    switch (t[i] % 3) {
    case 1: return false;
    case 2:
      if (head == -1) head = i;
      else return false;
    }
  }

  for (int i = 0; i < 3; ++i) {
    if (i == head) {
      if (!iswh2(t + 9 * i)) return false;
    }
    else {
      if (!iswh0(t + 9 * i)) return false;
    }
  }

  return true;
}

bool issp(const int* t)
{
  for (int i = 0; i < 34; ++i) {
    if (t[i] != 0 && t[i] != 2) return false;
  }

  return true;
}

bool isto(const int* t)
{
  for (const int i : {1, 2, 3, 4, 5, 6, 7, 10, 11, 12, 13, 14, 15, 16, 19, 20, 21, 22, 23, 24, 25}) {
    if (t[i] > 0) return false;
  }

  for (const int i : {0, 8, 9, 17, 18, 26, 27, 28, 29, 30, 31, 32, 33}) {
    if (t[i] == 0) return false;
  }

  return true;
}
