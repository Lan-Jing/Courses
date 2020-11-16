#include "circle.h"
#include <cstring>
/*
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
*/

int visit[10001];
int flag;
void DFS(GraphList *g, VertexNode now) {
  if (!flag) return;
  visit[now.value] = 1;
  EdgeNode *ptr = now.firstedge;
  while (ptr != nullptr) {
    if (visit[ptr->adjvex] == 0) {  // the next node unreached.
      DFS(g, g->adjList[ptr->adjvex]);
    } else if (visit[ptr->adjvex] == 1) {
      flag = 0;  // DFS from the next node unfinished
      return;    // so the edge form a circle.
    }
    ptr = ptr->next;
  }
  visit[now.value] =
      2;  // all subsequent DFS processes completed, mark the current node 2
}

void Graphcircle(GraphList *g) {
  for (int i = 0; i < g->numVertex; i++) {
    if (visit[i] == 2) continue;
    flag = 1;
    DFS(g, g->adjList[i]);
    if (!flag) break;
  }
  if (!flag)
    cout << "has circle";
  else
    cout << "no circle";
}