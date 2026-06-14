/**
 * @file MutableList.h
 * @brief Generic Vector implementation.
 *
 * @details
 * Conventions:
 * - All functions returning bool return true on success, false on failure.
 * - NULL input and output pointers are invalid and cause failure.
 * - All functions have O(1) time complexity unless given a deconstructor function, then ldestroy and lclear get O(n) time complexity
 */

#ifndef MUTABLELIST_H
#define MUTABLELIST_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct MutableList MutableList;
//@return A MutableList pointer on succes,
// NULL on failure
// @param elementDeconstructor receives a pointer to a function that frees or cleans an element stored in the list,
// if passed NULL no destructor function gets called.
// The user is responsible for ensuring that the function matches the actual
// type stored in the list. Incorrect casting or mismatched types may result in undefined behavior.
MutableList* lcreate(const size_t typeSize, void (*elementDeconstructor)(void*));
void ldestroy(MutableList* ml);
// @brief pass this as the destructor when required so that on removal there is no memoery leek
void ldestructor(void* ml);
bool lappend(MutableList* ml, const void* value);
bool linsert(MutableList* ml, const size_t index, const void* value);
bool lremove(MutableList* ml, const size_t index);
bool lclear(MutableList* ml);
bool ljoin(MutableList* output, const MutableList* source);
bool lreserve(MutableList* ml, const size_t size);
bool ltrim(MutableList* ml);
bool ltrimToSize(MutableList* ml,const size_t size);
void* lgetPtr(const MutableList* ml, const size_t index);
bool lget(const MutableList* ml, const size_t index, void* out);
size_t lgetSize(const MutableList* ml);
size_t lgetCapacity(const MutableList* ml);

#endif