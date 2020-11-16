#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;
const int INF = 9999999;
int main()
{
    int n,g[205][205];
    cin >> n;
    for(int i = 1;i <= n;i++)
        for(int j = 1;j <= n;j++)
        {
            if(i == j)
                g[i][j] = 0;
            else
                g[i][j] = INF;
        }
    for (int i = 1;i <= n-1;i++)
        for(int j = i+1;j <= n;j++)
            {
                cin >> g[i][j];
            }
    //dijstkra
    int dis[205],book[205];
    memset(book,0,sizeof(book));
    for(int i = 1;i <= n;i++)
        dis[i] = g[1][i];
    dis[1] = 0;
    book[1] = 1;
    for(int i = 2;i <= n;i++)
    {
        int mind = INF,u;
        for(int j = 1;j <= n;j++)
        {
            if(!book[j] && dis[j] < mind)
            {
                mind = dis[j];
                u = j;
            } 
        }
        book[u] = 1;
        for(int j = 1;j <= n;j++)
        {
            dis[j] = min(dis[j],dis[u]+g[u][j]);
        }
    }
    cout << dis[n];
    return 0;
}
