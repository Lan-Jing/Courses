#include <stdio.h>

int globalVar, callTime;

void fineFunc() {
    int globalVar = 1, localVar2 = 2;

    localVar2 *= globalVar;
    return ;
}

void flawFunc() {
    globalVar++;

    if(callTime == 5) {
        callTime++;
        fineFunc();
    } else {
        callTime++;
        flawFunc();
    }
}

void flawFunc1() {
    static int flawInt = 0;
    int *intp = malloc(sizeof(int));

    flawInt++;
    free(intp);
}


int main() {
    fineFunc();
    flawFunc();
    flawFunc1();
    return 0;
}