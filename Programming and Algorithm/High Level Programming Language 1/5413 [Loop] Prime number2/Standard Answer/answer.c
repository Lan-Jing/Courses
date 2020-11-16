#include <stdio.h>
#include <math.h>
int main(void){
	int n, prime, i;
	scanf("%d", &n);
	i = 0;
	prime = 2;
	while(1){
		if(i >= n){
			break;
		}
		int j, is_prime;
		is_prime = 1;
		for(j = 2; j<= sqrt(prime); j++){
			if(prime % j == 0){
				is_prime = 0;
				break;
			}
		}
		if(is_prime){
			i++;
			if(i != n){
				printf("%d ", prime);
			}else{
				printf("%d\n", prime);
			}
			
		}
		prime++;
	}
} 