#include<time.h>
#include<stdio.h>

time_t start,end;//time_t只能精确到秒，精度是很低的
clock_t start1,end1;//clock_t可以精确到毫秒，足矣做一些要求不高的事情了。

int main(){
    start = time(NULL);
    start1 = clock();

    for(int i = 0;i <= 10000000;i++);

    end = time(NULL);
    end1 = clock();
    printf("time=%f\n",difftime(end,start));//这里精度损失掉了，所以输出的是0
    printf("time=%f\n",(double)(end1-start1)/CLOCKS_PER_SEC);
    return 0;
}