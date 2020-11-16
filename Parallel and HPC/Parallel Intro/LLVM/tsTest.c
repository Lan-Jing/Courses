#include <stdio.h>
#include <pthread.h>

#define threadCount 8

pthread_mutex_t sumMutex;
void sum(int *times) {
    // for(int i = 0;i < times[1];i++)
    //     times[0] += i;
    // thread-unsafe operation;

    for(int i = 0;i < times[1];i++) {
        printf("sub-thread is executing sum: %d\n", times[0]);
        pthread_mutex_lock(&sumMutex);
        times[0] += i;
        pthread_mutex_unlock(&sumMutex);
    } 
    // thread-safe operation using mutex;
}

int main() {
    pthread_t threadPool[threadCount]; // make 8 threads to run;
    int para[2] = {0, 10};             // para[0]: sum, para[1]: times;
    pthread_mutex_init(&sumMutex, NULL);
    for(int i = 0;i < threadCount;i++) 
        pthread_create(&threadPool[i], NULL, sum, (void*)para); 

    for(int i = 0;i < para[1] * 5;i++) {
        para[0] += i;
        printf("Main thread is executing sum: %d\n", para[0]);
    }
    // also unsafe ?

    for(int i = 0;i < threadCount;i++)
        pthread_join(threadPool[i], NULL);
    pthread_mutex_destroy(&sumMutex);

    printf("%d\n", para[0]);

    return 0;
}

// should be 50*49/2 + 10*9/2 * 8 = 360 + 1225 = 1585