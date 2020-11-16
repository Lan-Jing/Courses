#include <bits/stdc++.h>
using namespace std;
int n;
vector<int>a;
int main()
{
    cin>>n;
    for(int i=1,x;i<=n;i++)
    {
        scanf("%d",&x);
        a.insert(upper_bound(a.begin(),a.end(),x),x);//二分插入保证单调性
        if(i%2==1)
        {
            printf("%d\n",a[(i-1)/2]);//是奇数个就输出
        }
    }
    return 0;
}