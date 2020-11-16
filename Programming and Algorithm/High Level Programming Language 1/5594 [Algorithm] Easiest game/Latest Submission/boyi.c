#include<stdio.h>

int t,N;

/*
    dalao：先手，xinpang后手

    1-2个石头：先手必胜，dalao胜利
    3个石头：后手必胜。xinpang胜利

    4个石头：拿走一个转化成后手必胜,dalao胜利。
    5个石头：拿走两个转化成后手必胜,dalao胜利。

    6个石头：转化成前面两种情况，但都是xinpang必胜
*/

int main(){
    scanf("%d",&t);;
    while(t--){
        scanf("%d",&N);
        if(N%3==0) printf("xinpang win!\n");
        else printf("dalao win!\n");
    }
    return 0;
}
