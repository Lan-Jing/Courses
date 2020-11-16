#include<stdio.h>

int h,s;
int f[30];

int main(){
	f[0] = f[1] = 1;
	scanf("%d %d",&h,&s);
	
	int step,stage;
	for(stage = 2;stage <= h;stage++)
		for(step = 1;step <= s;step++)
			if(stage-step >= 0)
				f[stage] += f[stage-step];

	printf("%d\n",f[h]);
	return 0;
}