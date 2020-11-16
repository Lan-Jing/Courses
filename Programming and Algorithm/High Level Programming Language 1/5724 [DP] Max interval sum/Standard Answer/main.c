#include <stdio.h>
int main(){
	int n, tmp;
	int num[101];
	scanf("%d", &n);
	for (int i = 0; i < n; i++){
		scanf("%d", &tmp);
		num[i] = tmp;
	}
	int i, j, k;
	int best_sum = 0, besti, bestj, cur_sum;
	for (i = 0; i < n; i++)
		for (j = i; j < n; j++){
			cur_sum = 0;
			for (k = i; k <= j; k++){
				cur_sum += num[k];
			}
			if (cur_sum > best_sum){
				best_sum = cur_sum;
				besti = i;
				bestj = j;
			}
		}
			
	printf("%d\n", best_sum);
	for (int idx = besti; idx < bestj; idx++){
		printf("%d ", num[idx]);
	}
	printf("%d\n", num[bestj]);
	return 1;
}
