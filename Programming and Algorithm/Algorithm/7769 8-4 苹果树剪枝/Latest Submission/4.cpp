#include <iostream>
using namespace std;
#include <cstring>

struct node{
    int lchild,rchild;
}tree[110];
int N,Q,con[110][110],dp[110][110];
void buildTree(int now,int parent){
    for(int i = 1;i <= N;i++){
        if(con[now][i] && i != parent){
            if(!tree[now].lchild) tree[now].lchild = i;
            else tree[now].rchild = i;
        }
    }
    if(tree[now].lchild) buildTree(tree[now].lchild,now);
    if(tree[now].rchild) buildTree(tree[now].rchild,now);
}

void DFS(int now){
    if(!tree[now].lchild && !tree[now].rchild){
        dp[now][0] = 0;
        return ;
    }else{
        int lchild = tree[now].lchild,rchild = tree[now].rchild;
        if(lchild) DFS(lchild);
        if(rchild) DFS(rchild);
        for(int total = 0;total <= N-Q-1;total++){
            if(lchild && total) dp[now][total] = max(dp[now][total],dp[lchild][total-1] + con[now][lchild]);
            if(rchild && total) dp[now][total] = max(dp[now][total],dp[rchild][total-1] + con[now][rchild]);
            if(lchild && rchild) 
                for(int left = 0;left <= total;left++){
                    int right = total - left;
                    dp[now][total] = max(dp[now][total],
                                         dp[lchild][left] + dp[rchild][right] + con[now][lchild] + con[now][rchild]);
                }
//            cout<<now<<' '<<total<<':'<<dp[now][total]<<endl;
        }
    }
}

int main(){
    cin>>N>>Q;
    for(int i = 1;i <= N-1;i++){
        int a,b,c; cin>>a>>b>>c;
        con[a][b] = con[b][a] = c;
    }

    memset(dp,-0x3f3f3f3f,sizeof(dp));
    buildTree(1,-1);

    DFS(1);
    cout<<dp[1][N-Q-1];
    return 0;
}
