/**
 * @file Queue.h
 * @brief Generic queue implementation.
 *
 * @details
 * Conventions:
 * - All functions returning bool return true on success, false on failure.
 * - NULL input and output pointers are invalid and cause failure.
 * - Enqueue and creation allocates memory 
 * - Dequeue and destruction deallocates memory
 * - All operations have O(1) time complexity
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct Queue Queue;

//@note returns NULL on failure
Queue* qcreate(size_t dataSize, void (*elementDeconstructor)(void*));
void qdestroy(Queue* q);
// @brief pass this as the destructor when required so that on removal there is no memory leek
void qdestructor(void* ll);
void qclear(Queue* q);
bool qenqueue(Queue* q, const void* value);
bool qdequeue(Queue* q, void* out);
bool qpeek(const Queue* q, void* out);
size_t qgetCount(const Queue* q);

#endif