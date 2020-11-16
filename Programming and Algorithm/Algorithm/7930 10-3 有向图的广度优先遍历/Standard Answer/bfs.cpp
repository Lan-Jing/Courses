#include "bfs.h"
bool visited[MAXVEX];
void BFSTraverse(GraphList* g)
{
	int i;
	EdgeNode *p;
	queue<int> q;
	for (i = 0; i < g->numVertex; i++)
	{
		visited[i] = false;
	}
	for (i = 0; i < g->numVertex; i++)
	{
		if (!visited[i])
		{
			visited[i] = true;
			cout << g->adjList[i].value << " ";   // 打印顶点
			//EnQueue(&q, i);
			q.push(i);
			while (!q.empty())
			{
				int m;
				m = q.front();
				q.pop();
				p = g->adjList[m].firstedge;     // 找到当前顶点边表链表头指针
					while (p)
					{
						if (!visited[p->adjvex])
						{
							visited[p->adjvex] = true;
							cout << g->adjList[p->adjvex].value<<" ";
							q.push(p->adjvex);
						}
						p = p->next;
					}
			}
		}
	}
}