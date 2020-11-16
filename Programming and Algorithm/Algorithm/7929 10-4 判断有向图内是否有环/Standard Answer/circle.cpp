#include "circle.h"
bool visited[MAXVEX];
bool onStack[MAXVEX];
bool findcircle = false;
void DFScircle(GraphList *g, int i)
{
	EdgeNode *p;
	onStack[i] = true;
	visited[i] = true;
	p = g->adjList[i].firstedge;
	while (p && findcircle == false)
	{
		if (visited[p->adjvex] && onStack[p->adjvex]) {
			
			findcircle = true;
			return;
		}
		if (!visited[p->adjvex])
		{
			DFScircle(g, p->adjvex);           //对访问的邻接顶点递归调用
		}
		p = p->next;
	}
	onStack[i] = false;
}
void Graphcircle(GraphList *g) {
	int i;
	for (i = 0; i < g->numVertex; i++)
	{
		visited[i] = false;         //初始化所有顶点状态都是未访问过状态
	}
	for (i = 0; i < g->numVertex; i++)
	{
		if (!visited[i])             //对未访问的顶点调用DFS，若是连通图，只会执行一次
		{
			DFScircle(g, i);
		}
	}
	if (findcircle == false) {
		cout << "no circle" << endl;
	}
	else {
		cout << "has circle" << endl;
	}
}