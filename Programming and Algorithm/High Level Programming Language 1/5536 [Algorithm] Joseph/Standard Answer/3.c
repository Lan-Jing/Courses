#include<stdio.h>
int a[1100] = {};
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    int sum = 0, now = -1, tot = 0, ans;
    while (1) {
        now = (now+1) % n;
        if (a[now] == 1) continue;
        tot++;
        if (tot % m == 0) a[now] = 1, sum++;
        if (sum == n) {
            ans = now+1;
            break;
        }
    }
    printf("%d\n", ans);
    return 0;
}