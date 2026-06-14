/**
 * @file Stack.h
 * @brief Generic stack implementation.
 *
 * @details
 * Conventions:
 * - All functions returning bool return true on success, false on failure.
 * - NULL input and output pointers are invalid and cause failure.
 * - push and creation allocates memory 
 * - pop and destruction deallocates memory
 * - All functions have O(1) time complexity unless given a deconstructor function, then sdestroy and sclear get O(n) time complexity
 */

#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct Stack Stack;
// @param elementDeconstructor receives a pointer to a function that frees or cleans an element stored in the list,
// if passed NULL no destructor function gets called.
// The user is responsible for ensuring that the function matches the actual
// type stored in the list. Incorrect casting or mismatched types may result in undefined behavior.
Stack* screate(const size_t size,void (*elementDeconstructor)(void*));
void sdestroy(Stack* stack);
// @brief pass this as the destructor when required so that on removal there is no memory leek
void sdestructor(void* ll);
bool sreserve(Stack* stack, const size_t size);
bool strim(Stack* stack);
bool strimToSize(Stack* stack,const size_t size);
bool sclear(Stack* stack);
bool spush(Stack* stack, const void* value);
bool spop(Stack* stack, void* out);
bool peek(Stack* stack, void* out);
size_t sgetSize(Stack* stack);
size_t sgetCapacity(Stack* stack);
void* getPtr(Stack* stack, const size_t index);

#endif