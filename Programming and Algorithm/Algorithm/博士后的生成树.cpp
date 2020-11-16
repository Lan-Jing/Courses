#include <iostream>
using namespace std;
#include <algorithm>

int n, m, Union[310];
int find(int x){ return Union[x] == x ? x : find(Union[x]); }
struct edge{
    int u,v,c;
    bool operator < (const edge &other) const {
        return this->c < other.c;
    }
}E[310*310/2];

int main() {
    cin>>n>>m;
    for(int i = 1;i <= n;i++) Union[i] = i;
    for(int i = 1;i <= m;i++){
        cin>>E[i].u>>E[i].v>>E[i].c;
    }
    sort(E+1,E+1+m);
    int i = 1,ans = 0,setN = n;
    while(setN > 1){
        int nowU = E[i].u, nowV = E[i].v;
        int fatherU = find(nowU);
        int fatherV = find(nowV);
//        cout<<fatherU<<' '<<fatherV<<' '<<n<<endl;
        if(fatherU != fatherV){
            setN--;
            Union[fatherU] = fatherV;
            ans = max(ans,E[i].c);
        }
        i++;
    }
    cout<<n-1<<' '<<ans;
    return 0;
}