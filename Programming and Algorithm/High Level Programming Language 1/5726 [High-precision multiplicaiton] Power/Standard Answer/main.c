#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include <stdio.h>

int main() {
    int n = 0;
    char sum[1001] = {0};
    scanf("%d", &n);

    if (n == 0) {
        printf("1\n");
    } else {

        int i = 0, j = 0;
        sum[0] = '3';
        for (i = 1; i < n; i++) {
            int t = 0, k = 0;
            while (sum[k] != '\0' || t > 0) {
                int temp = 0;
                if (sum[k] != '\0') temp = (sum[k] - '0') * 3;
                temp += t;
                sum[k] = (temp % 10) + '0';
                t = temp / 10;
                k++;
            }
        }

        for (i = strlen(sum) - 1; i >= 0; i--) {
            putchar(sum[i]);
        }
        printf("\n");
    }
    return 0;
}