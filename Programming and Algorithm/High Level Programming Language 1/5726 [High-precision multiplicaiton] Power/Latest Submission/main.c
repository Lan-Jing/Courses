#include<stdio.h>
#include<string.h>

char a[1000],c[1000];

int main(){
	memset(a,'0',sizeof(a));
	a[1] = '1';

	int n,la = 1;
	scanf("%d",&n);
	int i;	
	for(i = 1;i <= n;i++){
		int j,cc = 0;
		memset(c,'0',sizeof(c));
		for(j = 1;j <= la;j++){
			c[j] = ((a[j]-'0')*3+cc)%10 + '0';
			cc = ((a[j]-'0')*3+cc)/10;
		}
		if(cc) c[la+1] = cc+'0',la++;
		strcpy(a,c);
	}
	
	int flag = 0;
	for(i = 999;i >= 1;i--){
		if(a[i] != '0'){
			printf("%c",a[i]),flag = 1;
		}else if(flag) printf("0");
	}
	printf("\n");
	return 0;
}