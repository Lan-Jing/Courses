#include "graphlist.h"
void createGraph(GraphList* g) {
	int k, m;
	cin >> k >> m;
	g->numVertex = k;
	g->numEdges = m;
	int tempval;
	for (int i = 0; i < g->numVertex; i++) {
		cin >> tempval;
		g->adjList[i].value = tempval;
		g->adjList[i].firstedge = nullptr;
	}
	int vi, vj;
	int weight;
	for (int i = 0; i < g->numEdges; i++) {
		cin >> vi >> vj;
		cin >> weight;
		// 创建一条边表节点
		EdgeNode* edge = new EdgeNode();
		edge->adjvex = vj;
		edge->weight = weight;
		edge->next = nullptr;

		// 找到节点i的最后一个边表节点
		EdgeNode* temp = g->adjList[vi].firstedge;
		if (temp == nullptr) {
			g->adjList[vi].firstedge = edge;
		}
		else {
			EdgeNode* tempnext = temp->next;
			while (tempnext != nullptr) {
				temp = tempnext;
				tempnext = temp->next;
			}
			temp->next = edge;
		}
		
	}
}