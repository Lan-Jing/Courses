#include<iostream>
using namespace std;
#include<cstdio>

int main(){
    int m,s,t;
    while(scanf("%d%d%d",&m,&s,&t) != EOF){
        int dis = 0,ti = 0;
        int flag = 0;

        while(m >= 10){
            m-=10,dis+=60,ti++;
            if(dis >= s){
                printf("Yes\n%d\n",ti);
                flag = 1;
                break;
            }
        }
        if(flag) continue;
//        cout<<ti<<endl;

        if((s-dis-1)/17+1 > (9-m)/4+2){
            dis += 60,ti += (9-m)/4+2,m = 0;
            if(dis >= s){
                printf("Yes\n%d\n",ti);
                continue;
            }
        }
//        cout<<ti<<endl;
        while(ti < t){
            dis += 17,ti++;
            if(dis >= s){
                printf("Yes\n%d\n",ti);
                flag = 1;
                break;
            }
        }
        if(!flag) printf("No\n%d\n",dis);
    }
    return 0;
}