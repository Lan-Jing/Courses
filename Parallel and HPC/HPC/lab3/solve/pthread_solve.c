#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int solFlag = 1;
double a, b, c;
double minus_b, frac_a, delta, x1, x2;

int threadCount, barrierCount = 0;
int* rankVec;
pthread_t* pVec;
pthread_mutex_t mutex;
pthread_cond_t cond;

void* B()
{
    minus_b = -b;
    return NULL;
}

void* A()
{
    frac_a = 1 / (2*a);
    return NULL;
}

void* D()
{
    delta = b*b - 4*a*c;
    return NULL;
}

void* X1()
{
    x1 = (minus_b + delta) * frac_a;
    return NULL;
}

void* X2()
{
    x2 = (minus_b - delta) * frac_a;
    return NULL;
}

void* thread_work(void *rank)
{
    int myRank = *((int*)rank);
    switch (myRank)
    {
        case 0: A(); break;
        case 1: B(); break;
        case 2: D(); break;
    }
    
    // a barrier to sync the threads
    pthread_mutex_lock(&mutex);
    barrierCount++;
    if(barrierCount == threadCount) {
        solFlag = delta >= 0;
        pthread_cond_broadcast(&cond);
    } else {
        while(pthread_cond_wait(&cond, &mutex));
    }
    pthread_mutex_unlock(&mutex);

    // get the solution, break if unsolvable;
    if(!solFlag)
        return NULL; 
    switch (myRank)
    {
        case 0: X1(); break;
        case 1: X2(); break;
    }
    return NULL;
}

int main() 
{
    printf("Input a, b, c for solving ax^2+bx+c = 0.\n");
    scanf("%lf %lf %lf", &a, &b, &c);
    
    threadCount = 3;
    pVec = (pthread_t*)malloc(sizeof(pthread_t) * threadCount);
    rankVec = (int*)malloc(sizeof(int) * threadCount);

    pthread_mutex_init(&mutex, NULL);
    for(int i = 0;i < threadCount;i++) {
        rankVec[i] = i;
        pthread_create(&pVec[i], NULL, thread_work, &rankVec[i]);
    }
    for(int i = 0;i < threadCount;i++) {
        pthread_join(pVec[i], NULL);
    }
    pthread_mutex_destroy(&mutex);

    if(solFlag) {
        printf("Solution of the given equation: x1=%lf, x2=%lf.\n", x1, x2);
    } else {
        printf("This equation has no solution.\n");
    }
    
    free(pVec);
    free(rankVec);
    return 0;
}