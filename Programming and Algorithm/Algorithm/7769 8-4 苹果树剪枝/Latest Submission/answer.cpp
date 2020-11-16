#include<bits/stdc++.h>
#define ll long long
using namespace std;
int n,m,f[101][101],d[101][101];
vector<int> v[101];
int dfs(int x,int y,int fa)
{
    vector<int> child;
    int ans = 0;
    if(f[x][y]!=-1)
        return f[x][y];
    if(x!=1&&v[x].size()==1||y==0)
    {
        f[x][y] = ans;
        return ans;
    }
    else if(v[x].size()==2&&x!=1||x==1&&v[x].size()==1)
    {
        for(int i = 0;i<v[x].size();i++)
            if(v[x][i]!=fa)
                child.push_back(v[x][i]);
        ans = d[x][child[0]]+dfs(child[0],y-1,x);
        f[x][y] = ans;
        return ans;
    }
    else
    {
        for(int i = 0;i<v[x].size();i++)
            if(v[x][i]!=fa)
                child.push_back(v[x][i]);
        if(y>=2)
            for(int i = 0;i<y-1;i++)
                ans = max(ans,d[x][child[0]]+d[x][child[1]]+dfs(child[0],i,x)+dfs(child[1],y-i-2,x));
        if(y>=1)
        {
            ans = max(ans,d[x][child[0]]+dfs(child[0],y-1,x));
            ans = max(ans,d[x][child[1]]+dfs(child[1],y-1,x));
        }            
        f[x][y] = ans;
        return ans;
    }
}
int main()
{
    memset(d,-1,sizeof(d));
    memset(f,-1,sizeof(f));
    int a,b,c;
    cin>>n>>m;
    for(int i = 0;i<n-1;i++)
    {
        cin>>a>>b>>c;
        v[a].push_back(b);
        v[b].push_back(a);
        d[a][b] = c;
        d[b][a] = c;
    }
    cout<<dfs(1,m,0);
    return 0;
}