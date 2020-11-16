#include "bfs.h"
#include <queue>
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

int visit[100010];
void BFSTraverse(GraphList *g){
    queue<VertexNode> que;
    for(int i = 0;i < g->numVertex;i++){
        if(visit[i]) continue;
        que.push(g->adjList[i]);
        
        while(!que.empty()){
            VertexNode now = que.front(); que.pop();
            cout<<now.value<<' ';
            visit[now.value] = 1;

            EdgeNode *ptr = now.firstedge;
            while(ptr != nullptr){
                if(!visit[ptr->adjvex]) que.push(g->adjList[ptr->adjvex]);
                ptr = ptr->next;
            }
        }
    }
}