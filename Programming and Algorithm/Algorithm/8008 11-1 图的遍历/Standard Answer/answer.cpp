#include <iostream>
#include <cstdio>
#include <vector>
using namespace std;

#define MAXL 100010

int N, M, A[MAXL];
vector<int> G[MAXL]; 

void dfs(int x, int d) {
    if(A[x]) return; 
    A[x] = d;
    for(int i=0; i<G[x].size(); i++)
        dfs(G[x][i], d);
}

int main() {
    int u, v;
    scanf("%d%d", &N, &M);
    for(int i=1; i<=M; i++) {
        scanf("%d%d", &u, &v);
        G[v].push_back(u); 
    }
    for(int i=N; i; i--) dfs(i, i); 
    for(int i=1; i<=N; i++) printf("%d ", A[i]);
    printf("\n");
    return 0;
}
