#include<stdio.h>
#include<string.h>

int n;
char str_arr[1010][105];

int main(){
    scanf("%d",&n);
    int i;
    for(i = 1;i <= n;i++) scanf("%s",str_arr[i]);

    char str_temp[105],min_temp[105];

    int j,min_index;
    for(i = 1;i <= n;i++){
        memset(min_temp,'z',sizeof(min_temp));
        min_index = -1;

        for(j = i;j <= n;j++)
            if(strcmp(min_temp,str_arr[j]) > 0)
                strcpy(min_temp,str_arr[j]),min_index = j;

        strcpy(str_temp,str_arr[i]);
        strcpy(str_arr[i],min_temp);
        strcpy(str_arr[min_index],str_temp);
    }
    for(i = 1;i <= n;i++) printf("%s\n",str_arr[i]);
    return 0;
}
