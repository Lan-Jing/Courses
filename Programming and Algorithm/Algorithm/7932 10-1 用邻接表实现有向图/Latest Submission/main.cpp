#include "graphlist.h"
void printGraph(GraphList *g) {
	int i = 0;
	while (g->adjList[i].firstedge != nullptr && i < MAXVEX) {
		cout << g->adjList[i].value << endl;
		EdgeNode *e = g->adjList[i].firstedge;
		while (e!=nullptr) {
			cout << e->adjvex << ":"<<e->weight<<" ";
			e = e->next;
		}
		i++;
		cout << endl;
	}
}
int main() {
	GraphList g;
	createGraph(&g);
	printGraph(&g);
	return 0;
}