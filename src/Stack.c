#include <stdlib.h>
#include "../include/MutableList.h"

typedef MutableList Stack;


Stack* screate(const size_t size, void (*elementDeconstructor)(void*)){
    Stack* stack = lcreate(size,elementDeconstructor);
    return stack;
}

void sdestructor(void* element){
    ldestructor(element);
}

void sdestroy(Stack* stack){
    ldestroy(stack);
}

bool sreserve(Stack* stack, const size_t size){
    return lreserve(stack,size);
}

bool strim(Stack* stack){
    return ltrim(stack);
}

bool strimToSize(Stack* stack,const size_t size){
    return ltrimToSize(stack,size);
}

bool sclear(Stack* stack){
    return lclear(stack);
}

bool spush(Stack* stack, const void* value){
    return lappend(stack,value);
}

bool spop(Stack* stack, void* out){
    size_t last_index = lgetSize(stack)-1;
    if (!lget(stack,last_index,out)){
        return false;
    }
    return lremove(stack,last_index);
}

bool peek(Stack* stack, void* out){
    size_t last_index = lgetSize(stack)-1;
    return lget(stack,last_index,out);       
}

size_t sgetSize(Stack* stack){
    return lgetSize(stack);
}

size_t sgetCapacity(Stack* stack){
    return lgetCapacity(stack);
}

void* getPtr(Stack* stack, const size_t index){
    return lgetPtr(stack,index);
}