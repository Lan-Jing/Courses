#include "graphlist.h"
/*void createGraph(GraphList *g){
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
}*/

int visit[10010];
void DFS(GraphList *g,VertexNode now){
    cout<<now.value<<' ';
    visit[now.value] = 1;
    EdgeNode *ptr = now.firstedge;
    while(ptr != nullptr){
        int next = ptr->adjvex;
        if(visit[next] == 0) DFS(g,g->adjList[next]);
        ptr = ptr->next;
    }
}

void DFSTraverse(GraphList *g){
    VertexNode begin;
    for(int i = 0;i < g->numVertex;i++){
        if(!visit[i]){
            begin = g->adjList[i]; 
            DFS(g,begin);
        }
    }
}