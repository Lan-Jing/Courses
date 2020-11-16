#include "Object.h"
void TestObjects(){
    Object *t1 = new Object(1);
    Object *t2 = new Object(2);
    Object *t3 = new Object(3);
    delete t3;
    delete t2;
    Object *t4 = new Object(4);
    delete t4;
    delete t1;
}