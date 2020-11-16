#include<bits/stdc++.h>
using namespace std;
set<int>s;
int a[105];
int main()
{
    int n;
    cin>>n;
    for(int i=0;i<n;i++)
    {
        cin>>a[i];
        s.insert(a[i]);
    }
    cout<<s.size()<<endl;
    while(!s.empty())
    {
        cout<<*s.begin()<<" ";           
        s.erase(s.begin()); 
    }
    return 0;
 }