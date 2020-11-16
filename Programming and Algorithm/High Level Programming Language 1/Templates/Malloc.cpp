#include<cstdio>
#include<cstdlib>
#include<ctime>

int main(){
	srand(time(0));
	int **numbers = (int**)malloc(sizeof(int*) * 100);
	
	for(int i = 0;i < 100;i++)
		numbers[i] = (int*)malloc(sizeof(int) * 100);
		
	for(int i = 0;i < 100;i++)
		for(int j = 0;j < 100;j++)
			numbers[i][j] = rand()%10;
		
	for(int i = 0;i < 100;i++)
		for(int j = 0;j < 100;j++)
			if(j < 100-1) printf("%d ",numbers[i][j]);
			else printf("%d\n",numbers[i][j]);
			
	for(int i = 0;i < 100;i++) free(numbers[i]);
	
	free(numbers);
	
	return 0;
}
