#include<iostream>
using namespace std;
#include<cstdio>
#include<cstring>

int N,num[200];
void preorder(int now){
    if(now > N) return ;
    cout<<num[now]<<endl;
    preorder(2*now);
    preorder(2*now+1);
}
void inorder(int now){
    if(now > N) return ;
    inorder(2*now);
    cout<<num[now]<<endl;
    inorder(2*now+1);
}
void postorder(int now){
    if(now > N) return ; 
    postorder(2*now);
    postorder(2*now+1);
    cout<<num[now]<<endl;
}

int main(){
    cin>>N;
    for(int i = 1;i <= N;i++) cin>>num[i];
    printf("Preorder\n");
    preorder(1);
    printf("Inorder\n");
    inorder(1);
    printf("Postorder\n");
    postorder(1);
}