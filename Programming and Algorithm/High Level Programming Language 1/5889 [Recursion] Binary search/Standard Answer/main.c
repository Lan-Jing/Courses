#include <stdio.h>

int array[101000];


int find(int low, int upper, int query) {
  if (low > upper) { return -1; }

  int mid = (upper + low) / 2;

  if (query == array[mid]) {
    return mid;
  } else if (query < array[mid]) {
    return find(low, mid-1, query);
  } else {
    return find(mid+1, upper, query);
  }
}


int main() {
  int m, n;
  scanf("%d", &n);
  for (int i = 0; i < n; i++) { scanf("%d", &array[i]); }

  scanf("%d", &m);

  int query;
  for (int i = 0; i < m; i++) {
    scanf("%d", &query);
    printf("%d\n", find(0, n-1, query));
  }

  return 0;
}