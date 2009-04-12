#include <cstddef>
#include <cstring>
#include <cstdio>

const size_t NMAX = 1024;

class summator_t {
  int S[NMAX];
  size_t prev(size_t i) const {
    return i & (i - 1);
  }
  size_t next(size_t i) const {
    return i + i - prev(i);
  }
public:
  summator_t() {
    memset(S, 0, sizeof(S));
  }

  void add(size_t i, int a) {
    for(; i < NMAX; i = next(i))
      S[i] += a;
  }

  int sum(size_t i) const {
    int a = 0;
    for(; i; i = prev(i))
      a += S[i];
    return a;
  }
};

int main() {
  summator_t S;

  S.add(1, 1);
  S.add(2, 2);
  S.add(3, 3);
  S.add(100, 100);

  printf("S[1..3] = %d\n", S.sum(3));
  printf("S[1..1023] = %d\n", S.sum(1023));


  return 0;
}