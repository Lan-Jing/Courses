#include<stdio.h>

int gox[4]={-1,1,0,0},
	goy[4]={0,0,1,-1};
int stack[200],top;
int sx[200],sy[200],topp;
int vis[7][12],flag;

void print(int ey,int ex){
	int i;
	for(i = top;i >= 0;i--){
		sx[i] = ex,sy[i] = ey;
		ex -= gox[stack[i]],
		ey -= goy[stack[i]];
	}
	for(i = 0;i <= top;i++)
		printf("%d %d\n",sy[i],sx[i]);
}

void go(int maze[][12],int sy,int sx,int ey,int ex){
	if(flag) return ;
	if(sy == ey && sx == ex){
		print(ey,ex);
		flag = 1;
		return ;
	}

	vis[sy][sx] = 1;

	int i;
	for(i = 0; i < 4;i++){
		int nowy = sy + goy[i],
			nowx = sx + gox[i];
		if(!maze[nowy][nowx] && 0 <= nowx && nowx <= 11 && 0 <= nowy && nowy <= 7 && (!vis[nowy][nowx])){
			stack[++top] = i;
			go(maze,nowy,nowx,ey,ex);
			top--;
		}
	}

	vis[sy][sx] = 0;
}