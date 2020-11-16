#include "graphlist.h"
void createGraph(GraphList *g){
	cin>>g->numVertex>>g->numEdges;
	for(int i = 0;i < g->numVertex;i++)
		cin>>g->adjList[i].value;
	for(int i = 0;i < g->numEdges;i++){
		int a,b,c; cin>>a>>b>>c; 
		EdgeNode *newNode = new EdgeNode;
		newNode->adjvex = b;
		newNode->weight = c;
		newNode->next = nullptr;
		
		EdgeNode *ptr = g->adjList[a].firstedge;
		while(ptr != nullptr && ptr->next != nullptr) ptr = ptr->next;
		
		if(ptr) ptr->next = newNode;
		else    g->adjList[a].firstedge = newNode;
	}
}