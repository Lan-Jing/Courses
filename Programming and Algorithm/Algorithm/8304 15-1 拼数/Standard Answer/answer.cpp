#include<bits/stdc++.h> 
using namespace std;
string a[30]; //使用string更方便一些(可以直接比较大小)
int main()
{
    int n;
    cin>>n;
    for(int i=1;i<=n;i++) cin>>a[i]; //读入
    for(int i=1;i<n;i++) //排序
    {
        for(int j=i+1;j<=n;j++)
        {
            if(a[j]+a[i]>a[i]+a[j]) //string类型可以直接比较大小
            {
                swap(a[j],a[i]);  //交换a[i]与a[j],同样可以用swap(a[i],a[j]);
                /*
                    举个例子:如果是12,34，由于3412>1234，所以肯定要交换一下，直接用swap就可以了
                */
            }
        }
    } //排序结束
    for(int i=1;i<=n;i++) cout<<a[i]; //由于已经排好序了，直接输出即可
    return 0;
}