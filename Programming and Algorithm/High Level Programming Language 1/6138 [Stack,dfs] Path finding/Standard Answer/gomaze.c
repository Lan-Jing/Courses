#include <stdio.h>
#include <stdlib.h>

typedef struct{
	int x;
	int y;
	int v;
}position;

typedef struct{
	position* elem;
	int size;
	int top;
}STACK;

int movex[5]={0,0,1,0,-1};
int movey[5]={0,-1,0,1,0};

int InitStack(STACK* s, int n){
	s->elem=(position*)malloc(n*sizeof(position));
	if (s->elem==NULL) return 0;
	s->size=n;
	s->top=-1;
}

int DestroyStack(STACK* s){
	free(s->elem);
	s->elem=NULL;
	s->top=-1;
	s->size=0;
	return 1;
}

int Push(STACK* s, position e){
	if (s->top == s-> size) return 0;
	s->top++;
	s->elem[s->top] = e;
	return 1;
}

int Pop(STACK* s, position* e){
	if (s->top == -1) return 0;
	*e = s->elem[s->top];
	s->top--;
	return 1;
}

void go(int maze[7][12], int x0, int y0, int xx, int yy){
	int x,y,x1,y1,v,size;
	STACK* s = (STACK*)malloc(sizeof(STACK));

	position e;
	position res[200];

	InitStack(s,200);
	e.x=x0; e.y=y0; e.v=0;
	Push(s,e);
	maze[x0][y0]=2;
	while(s->top != -1){
		Pop(s,&e);
		x=e.x;
		y=e.y;
		v=e.v+1;
		if(e.v>0){
			maze[x+movex[e.v]][y+movey[e.v]]=0;
		}
		
		while(v<=4){
			x1=x+movex[v];
			y1=y+movey[v];
			if (x1==xx&&y1==yy){
				size = 0;
				while(s->top != -1){
					Pop(s, &res[size++]);
				}
				size--;
				while(size >= 0){
					printf("%d %d\n", res[size].x, res[size].y);
					size--;
				}
				printf("%d %d\n", x, y);
				printf("%d %d\n", xx, yy);
				v++;
			}
			else if (x1>0 && x1<6 && y1>0 && y1<11 && !maze[x1][y1]){
				e.x=x;
				e.y=y;
				e.v=v;
				Push(s,e);
				x=x1;
				y=y1;
				v=1;
				maze[x][y]=2;
			}
			else{
				v++;
			}
		}
	}
	DestroyStack(s);
	free(s);
}