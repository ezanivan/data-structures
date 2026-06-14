#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct MutableList {
    size_t capacity;
    size_t count;
    size_t typeSize;
    char* items;
    void (*deconstructor)(void*);
} MutableList;

MutableList* lcreate(const size_t typeSize, void (*elementDeconstructor)(void*)){
    if (typeSize > SIZE_MAX) return NULL;
    if (typeSize == 0) return NULL;
    MutableList* ml = malloc(sizeof(MutableList));
    if (ml == NULL) return NULL;
    ml->capacity = 10;
    ml->count = 0;
    ml->typeSize = typeSize;
    ml->items = malloc(typeSize * ml->capacity);
    ml->deconstructor = elementDeconstructor;
    if (ml->items == NULL){
        free(ml);
        return NULL;
    }
    return ml;
}

void ldestroy(MutableList* ml){
    if (ml == NULL) return;
    if (ml->deconstructor != NULL){
        for(size_t i = 0; i < ml->count; i++){
            ml->deconstructor(ml->items + ml->typeSize * i);
        }
    }
    free(ml->items);
    free(ml);
}

void ldestructor(void* element){
    if (element == NULL) return;

    MutableList* ml;

    memcpy(&ml, element, sizeof(ml));

    ldestroy(ml);
};

bool extend(MutableList* ml){
    if (ml == NULL) return false;
    void* new_ptr = realloc(ml->items, (ml->capacity << 1) * ml->typeSize);
    if (new_ptr == NULL){
        return false;
    }
    ml->items = new_ptr;
    ml->capacity *= 2;
    return true;
}

bool extendToSize(MutableList* ml, const size_t size){
    if (ml == NULL) return false;
    if (size <= ml->capacity) return false;
    void* new_ptr = realloc(ml->items, size * ml->typeSize);
    if (new_ptr == NULL){
        return false;
    }
    ml->items = new_ptr;
    ml->capacity = size;
    return true;
}

bool lappend(MutableList* ml, const void* value){
    if (ml == NULL) return false;
    if (value == NULL) return false;
    if (ml->capacity == ml->count){
        if (!extend(ml)) return false;
    }
    memcpy(ml->items+ml->count*ml->typeSize,value,ml->typeSize);
    ml->count++;
    return true;
}

bool linsert(MutableList* ml, const size_t index, const void* value){
    if (ml == NULL) return false;
    if (value == NULL) return false; 
    if (index > ml->count) return false;

    if (ml->capacity == ml->count){
        if (!extend(ml)) return false;
    }
    memmove(ml->items + ((index + 1) * ml->typeSize), ml->items + ((index) * ml->typeSize), (ml->count-index) * ml->typeSize);
    memcpy(ml->items + (index * ml->typeSize), value, ml->typeSize);
    ml->count++;
    return true;
}

bool ljoin(MutableList* output, const MutableList* source){
    if (output == NULL) return false;
    if (source == NULL) return false;
    if (output->typeSize != source->typeSize) return false;

    if(output->count + source->count > output->capacity<<1){
        if(!extendToSize(output,output->count+source->count)) return false;
    }else{
        if(!extend(output)) return false;
    }
    memcpy(output->items + output->count * output->typeSize, source->items, source->count * source->typeSize);
    output->count += source->count;
    return true;
}

bool lremove(MutableList* ml, const size_t index){
    if (ml == NULL) return false;
    if (index >= ml->count) return false;
    if (ml->deconstructor != NULL){
        ml->deconstructor(ml->items + ml->typeSize * index);
    }
    memmove(ml->items + index * ml->typeSize,ml->items + (index+1) * ml->typeSize, (ml->count - index - 1) * ml->typeSize);
    ml->count--;
    return true;
}

bool lget(const MutableList* ml, const size_t index, void* out){
    if (ml == NULL) return false;
    if (out == NULL) return false;
    if (index >= ml->count) return false;
    memcpy(out,ml->items + index * ml->typeSize, ml->typeSize);
    return true;
}

void* lgetPtr(const MutableList* ml, const size_t index){
    if (ml == NULL) return NULL;
    if (index >= ml->count) return NULL;

    return ml->items + index * ml->typeSize;
}


bool lreserve(MutableList* ml, const size_t size){
    return extendToSize(ml,size);
}

bool ltrim(MutableList* ml){
    if (ml == NULL) return false;
    if (ml->count == 0) return false;
    void* new_ptr = realloc(ml->items,ml->count*ml->typeSize);
    if (new_ptr == NULL) return false;
    ml->items = new_ptr;
    ml->capacity = ml->count;
    return true;
}

bool ltrimToSize(MutableList* ml,const size_t size){
    if (ml == NULL) return false;
    if (size == 0) return false;
    if (size < ml->count) return false;
    void* new_ptr = realloc(ml->items,size*ml->typeSize);
    if (new_ptr == NULL) return false;
    ml->items = new_ptr;
    ml->capacity = size;
    return true;
}

bool lclear(MutableList* ml){
    if (ml == NULL) return false;
    if (ml->deconstructor != NULL){
        for(size_t i = 0; i < ml->count; i++){
            ml->deconstructor(ml->items + ml->typeSize * i);
        }
    }
    ml->count = 0;
    return ltrimToSize(ml,10);
}

size_t lgetSize(const MutableList* ml){
    if (ml == NULL) return 0;
    return ml->count;
}

size_t lgetCapacity(const MutableList* ml){
    if (ml == NULL) return 0;
    return ml->capacity;
}