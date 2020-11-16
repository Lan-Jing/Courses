#include <stdio.h>

long long fastPower(long long num, long long time, long long mod) {
  long long ans = 1, base = num;
  while (time != 0) {
    int bit = time & 1;
    if (bit != 0) {
      ans = (ans * base) % mod;
    }
    base = (base * base) % mod;
    time = time >> 1;
  }
  return ans % mod;
}

int main() {
  long long a, b, p;
  scanf("%lld%lld%lld", &a, &b, &p);

  printf("%lld\n", fastPower(a, b, p));
  return 0;
}