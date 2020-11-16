#include<stdio.h>
#include<string.h>

char stack[200],top;
char str[200],check[7]={' ','(',')','[',']','{','}'};

int main(){
	scanf("%s",str);
	int ls = strlen(str);

	int i;
	for(i = 0;i < ls;i++){
		int flag = 0;
		int j;
		for(j = 1;j < 7;j++)
			if(str[i] == check[j]){
				flag = j; break;
			}

		if(!flag) continue;

		if(str[i] == ')' || str[i] == ']' || str[i] == '}'){
			if(!top || stack[top] != check[flag-1]){
				printf("No\n");
				return 0;
			}else
				top--;
		}else
			stack[++top] = str[i];
	}

	if(top) printf("No\n");
	else printf("Yes\n");
	
	return 0;
}