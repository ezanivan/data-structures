#include "../include/LinkedList.h"
#include <stdlib.h>

typedef LinkedList Queue;

Queue* qcreate(size_t dataSize, void (*elementDeconstructor)(void*)){
    return llcreate(dataSize, elementDeconstructor);
}

void qdestroy(Queue* q){
    return lldestroy(q);
}

void qdestructor(void* element){
    lldestructor(element);
}

void qclear(Queue* q){
    return llclear(q);
}

bool qenqueue(Queue* q, const void* value){
    return llinsertTail(q,value);
}

bool qdequeue(Queue* q, void* out){
    if (!llgetHeadValue(q,out)) return false;
    return llremoveHead(q);
}

bool qpeek(const Queue* q, void* out){
    return llgetHeadValue(q,out);
}

size_t qgetCount(const Queue* q){
    return llgetCount(q);
}
