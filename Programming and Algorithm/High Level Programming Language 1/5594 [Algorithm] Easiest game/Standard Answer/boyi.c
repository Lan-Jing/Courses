#include <stdio.h>

int main() {
	int t, n;
	scanf("%d", &t);
	while(t--) {
		scanf("%d", &n);
		if (n %3 == 0) {
			printf("xinpang win!\n");
		} else {
			printf("dalao win!\n");
		}
	}
	return 0;
}