#ifndef MUTABLELIST_H
#define MUTABLELIST_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct MutableList MutableList;
MutableList* lcreate(const size_t typeSize);
void ldestroy(MutableList* ml);
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