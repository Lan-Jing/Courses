#include<stdio.h>
#include<string.h>

char a[105],b[105],c[105];
char na[105],nb[105];
int la,lb,lc;

int max(int a,int b){
    return a > b ? a : b;
}

void print(){
    int flag = 0;

    int i;
    for(i = lc-1;i >= 0;i--){
        if(c[i] != '0') flag = 1,printf("%c",c[i]);
        else if(flag) printf("%c",c[i]);
    }
    printf("\n");
}

int main(){
    memset(na,'0',sizeof(na)),memset(nb,'0',sizeof(nb));

    scanf("%s",a),scanf("%s",b);
    la = strlen(a),lb = strlen(b);

//  printf("%d %d\n",la,lb);

    int i;
    for(i = 0;i < la;i++) na[i] = a[la-1-i];
    for(i = 0;i < lb;i++) nb[i] = b[lb-1-i];
/*
    for(i = 0;i < la;i++) printf("%c",na[i]); printf("\n");
    for(i = 0;i < lb;i++) printf("%c",nb[i]); printf("\n");
*/

    int p = 0;
    for(i = 0;i < max(la,lb);i++){
        c[i] = (p + (na[i]-'0') + (nb[i]-'0'))%10 + '0';
        p = (p + (na[i]-'0') + (nb[i]-'0'))/10;
    }
    if(p) c[max(la,lb)] = '1',lc = max(la,lb)+1;
    else lc = max(la,lb);

    print();

    return 0;
}
