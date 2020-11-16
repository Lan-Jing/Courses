#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define maxQueSize 1000
#define maxQueNum 10
#define randLimit (1 << 16)

struct message {
    int number, sender;
};

struct messQue {
    int head, tail;
    struct message que[maxQueSize];
    omp_lock_t headLock;
    omp_lock_t tailLock;
};

void init_que(struct messQue *que) {
    que->head = que->tail = 0;
    omp_init_lock(&que->headLock);
    omp_init_lock(&que->tailLock);
}

int send(struct messQue *targetQue, int tid, int ttid) {
    int sendRes;
    omp_set_lock(&targetQue->tailLock);
    // full queue
    if(targetQue->head != targetQue->tail && targetQue->head % maxQueSize == targetQue->tail % maxQueSize)
        sendRes = 0;
    else {
        // construct new message;
        struct message newMessage;
        newMessage.number = rand() % randLimit, newMessage.sender = tid;
        // push it into the queue
        targetQue->que[targetQue->tail % maxQueSize] = newMessage;
        targetQue->tail++;
        printf("Thread %d sent %d to thread %d\n", newMessage.sender, newMessage.number, ttid);
        // set return value
        sendRes = newMessage.number;
    }
    omp_unset_lock(&targetQue->tailLock);
    return sendRes;
}

int recv(struct messQue *localQue, int tid) {
    int recvRes;
    omp_set_lock(&localQue->headLock);
    // empty queue
    if(localQue->head == localQue->tail)
        recvRes = -1;
    else {
        // get message from head of the queue
        struct message oldMessage;
        oldMessage = localQue->que[localQue->head % maxQueSize];
        localQue->head++;
        printf("Thread %d received %d from thread %d\n", tid, oldMessage.number, oldMessage.sender);
        // set return value
        recvRes = oldMessage.number;    
    }
    omp_unset_lock(&localQue->headLock);
    return recvRes;
}

int main(int argc, char *argv[]) {
    // only a single pair of producer and consumer;
    int threadCount = argc < 2 ? 8 : strtol(argv[1], NULL, 10);
    // message queues for threads
    struct messQue* ques[maxQueNum];
    for(int i = 0;i < maxQueNum;i++) {
        ques[i] = (struct messQue*)malloc(sizeof(struct messQue));
        init_que(ques[i]);
    }
    // setup random seed
    srand(time(NULL));
    // end signal
    int endSignal = 0;

    #pragma omp parallel num_threads(threadCount) \
    default(none), shared(threadCount, ques, endSignal)
    {
        int tid = omp_get_thread_num();
        printf("Thread %d is running\n", tid);
        if(tid >= threadCount / 2) {
            // producer task
            for(int i = 0;i < 5;i++) {
                // randomly send messages
                int target = rand() % (threadCount/2);
                send(ques[target], tid, target);
            }
            #pragma omp atomic
            endSignal++;
        } else {
            // consumer task
            // at least one producer is working or, local queue not empty
            while(endSignal < threadCount/2 || ques[tid]->tail != ques[tid]->head) {
                recv(ques[tid], tid);
            }
        }
    }
    // destroy locks and queues
    for(int i = 0;i < maxQueNum;i++) {
        omp_destroy_lock(&ques[i]->headLock);
        omp_destroy_lock(&ques[i]->tailLock);
        free(ques[i]);
    }
    return 0;
}