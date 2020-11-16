#include "dfs.h"
bool visited[MAXVEX];
void DFS(GraphList *g, int i)
{
	EdgeNode *p;
	visited[i] = true;
	cout << g->adjList[i].value <<" ";   //打印顶点
	p = g->adjList[i].firstedge;
	while (p)
	{
		if (!visited[p->adjvex])
		{
			DFS(g, p->adjvex);           //对访问的邻接顶点递归调用
		}
		p = p->next;
	}
}
void DFSTraverse(GraphList *g) {
	int i;
	for (i = 0; i < g->numVertex; i++)
	{
		visited[i] = false;         //初始化所有顶点状态都是未访问过状态
	}
	for (i = 0; i < g->numVertex; i++)
	{
		if (!visited[i])             //对未访问的顶点调用DFS，若是连通图，只会执行一次
		{
			DFS(g, i);
		}
	}
}