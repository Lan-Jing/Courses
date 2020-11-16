#include<stdio.h>
#include<string.h>

int t,n,m;
struct node{
    int left,right;
}list[500010];

int main(){
    scanf("%d",&t);
    while(t--){
        scanf("%d %d",&n,&m);
        memset(list,0,sizeof(list));
        int i;
        list[0].right = 1,list[0].left = -1;
        for(i = 1;i <= n;i++){
            list[i].left = i-1,list[i].right = i+1;
            if(i == n) list[i].right = -1,list[i+1].left = i;
        }
        for(i = 1;i <= m;i++){
            int code,tar,loc;
            scanf("%d %d %d",&code,&tar,&loc);
            if(code == 1){
                list[list[tar].right].left = list[tar].left;
                list[list[tar].left].right = list[tar].right;

                list[tar].left = list[loc].left;
                list[list[loc].left].right = tar;
                list[tar].right = loc;
                list[loc].left = tar;
            }else{
                list[list[tar].right].left = list[tar].left;
                list[list[tar].left].right = list[tar].right;

                list[tar].right = list[loc].right;
                list[list[loc].right].left = tar;
                list[tar].left = loc;
                list[loc].right = tar;
            }
        }
        for(i = 0;i != -1;i = list[i].right)
            if(i) printf("%d ",i);
        printf("\n");
    }
    return 0;
}
