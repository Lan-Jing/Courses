#include<stdio.h>
#include<string.h>
#include<stdlib.h>

char operator_stk[1010]; int optop;
float number_stk[1010]; int numtop;
char str[1010]; int sl;

void push_num(float x){
//	printf("%lf\n",x);
	number_stk[++numtop] = x;
}
double pop_num(){
	return number_stk[numtop--];
}
void push_operator(char x){
	operator_stk[++optop] = x;
}
char pop_operator(){
	return operator_stk[optop--];
}
void cal(){
	char operator = pop_operator();
	float na = pop_num(),nb = pop_num();
	switch(operator){
		case '*': push_num(na*nb); break;
		case '/': push_num(nb/na); break;
		case '+': push_num(na+nb); break;
		case '-': push_num(nb-na); break;
	}
}

int main(){
	scanf("%s",str); sl = strlen(str);

	int now = 0; int flag = 0;
	for(int i = 0;i < sl;i++){
		if('1' <= str[i] && str[i] <= '9'){
			printf("%c",str[i]);

			flag = 1;
			now = 10*now + str[i]-'0';
		}else{
			if(flag){
				flag = 0;
				push_num((float)now);
				now = 0;
			}
			if(!optop || str[i] == '(') push_operator(str[i]);
			else{
				if(str[i] == ')'){
					while(operator_stk[optop] != '(') 
						printf("%c",operator_stk[optop]),cal();
					pop_operator();
				}else if(str[i] == '+' || str[i] == '-'){
					while(operator_stk[optop] != '(' && optop)
						printf("%c",operator_stk[optop]),cal();
					push_operator(str[i]);
				}else{
					while(operator_stk[optop] != '(' && operator_stk[optop] != '+' && operator_stk[optop] != '-'
                        && optop)
						printf("%c",operator_stk[optop]),cal();
					push_operator(str[i]);
				}
			}
		}
	}
	while(optop)
		printf("%c",operator_stk[optop]),cal();
	printf("\n%.4f\n",number_stk[numtop]);
	return 0;
}