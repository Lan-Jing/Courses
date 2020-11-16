#include <stdlib.h>
#include <stdio.h>

typedef struct stack_tag{
	int* elem;
	int size;
	int top;
}STACK;

int InitStack(STACK* s, int n){
	s->elem=(int*)malloc(n*sizeof(int)); //为数据元素分配内存
	if (s->elem==NULL) return 0;
	s->size=n;
	s->top=-1;
}

int DestroyStack(STACK* s){
	free(s->elem); //释放内存,并设置为NULL
	s->elem=NULL;
	s->top=-1;
	s->size=0;
	return 1;
}

int Push(STACK* s, int e){
	if (s->top == s-> size) return 0;
	s->top++;
	s->elem[s->top] = e;
	return 1;
}

int Pop(STACK* s, int* e){
	if (s->top == -1) return 0;
	*e = s->elem[s->top];
	s->top--;
	return 1;
}

int conversion(STACK* s, int n){
	if (n == 0){
		printf("%d\n", n);
		return 1;
	}
	int e;
	InitStack(s, 30);
	while(n){
		Push(s, n % 8);
		n = n / 8;
	}
	while(s->top != -1){
		Pop(s, &e);
		printf("%d", e);
	}
	printf("\n");
	DestroyStack(s);
	return 1;
}

int main(){
	int num = 0;
	int n = 0;
	STACK* s;
	scanf("%d", &num);
	while(num--){
		scanf("%d", &n);
		conversion(s, n);
	}
	s = NULL;
	return 1;
}