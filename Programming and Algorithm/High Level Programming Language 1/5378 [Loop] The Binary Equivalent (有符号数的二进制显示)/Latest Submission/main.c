#include<stdio.h>
#include<string.h>
#include<math.h>

int target,IsNeg;
char str[20];

int main(){
    while(scanf("%d",&target) != EOF){
        IsNeg = 0;
        int i;
        for(i = 0;i < 16;i++) str[i] = '0';
        if(target < 0) IsNeg = 1,target = -target;

        for(i = 0;i < 16;i++){
            if(target >= (1<<(15-i))) target -= (1 << (15-i)),str[i] = '1';
        }

        if(IsNeg){
            for(i = 0;i < 16;i++) str[i] = ('1' - str[i]) + '0';
            for(i = 15;i >= 0;i--){
                if(str[i]-'0' > 0){
                    str[i] = '0';
                }else{
                    str[i] += 1;break;
                }
            }
        }

        for(i = 0;i < 16;i++) printf("%c",str[i]); printf("\n");
    }
    return 0;
}
