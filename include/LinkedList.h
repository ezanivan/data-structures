/**
 * @file LinkedList.h
 * @brief Generic singly linked list implementation.
 *
 * @details
 * Conventions:
 * - All functions returning bool return true on success, false on failure.
 * - NULL input and output pointers are invalid and cause failure.
 * - All insertions of elements memory and the creation of the list allocates memory.
 * - All removal of elements and the destruction of the list deallocates memory.
 */

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct LinkedList LinkedList;

/**
 * - Functions that return Iterators, in case of failure, return an NULL iterator (returns error when passed as an argument).
 * - A NULL iterator is (LinkedList){NULL}
 * - Any iterator referring to a removed element becomes invalid and must not be used, even if it was passed by value or copied earlier. if used causes undefined behavior.
*/
// @brief stores the location of an LinkedList element
typedef struct LinkedListIterator* LinkedListIterator;

// @return returns a pointer to a LinkedList on success,
// NULL on failure
// @param elementDeconstructor receives a pointer to a function that frees or cleans an element stored in the list,
// if passed NULL no destructor function gets called.
// The user is responsible for ensuring that the function matches the actual
// type stored in the list. Incorrect casting or mismatched types may result in undefined behavior.
LinkedList* llcreate(const size_t dataSize, void (*elementDeconstructor)(void*));
// @brief frees all the elements and the list itself
// @warning using the list pointer after free is undefined behavior
void lldestroy(LinkedList* ll);
// @brief pass this as the destructor when required so that on removal there is no memory leek
void lldestructor(void* ll);
void llclear(LinkedList* ll);
bool llinsertHead(LinkedList* ll, const void* value);
bool llinsertTail(LinkedList* ll, const void* value);
//@note operation is O(n)
bool llinsertAt(LinkedList* ll, const size_t index, const void* value);
bool llinsertAfter(LinkedList* ll, LinkedListIterator* iterator, const void* value);
//@note operation is O(1)
//lists must be equal in data size and pointers must not be the same
//@brief appends all elements from source list to the end of output list
//source list becomes empty but still valid and retains its original dataSize
bool llmoveAppend(LinkedList* outputList, LinkedList* sourceList);
//@note operation is O(1)
LinkedListIterator llgetIteratorAt(const LinkedList* ll, const size_t index);
LinkedListIterator llgetIteratorHead(const LinkedList* ll);
LinkedListIterator llgetIteratorTail(const LinkedList* ll);
LinkedListIterator llgetIteratorNext(const LinkedListIterator* iterator);
bool llgetValueAtIterator(const LinkedList* ll, const LinkedListIterator* iterator, void* out);
bool llgetHeadValue(const LinkedList* ll, void* out);
bool llgetTailValue(const LinkedList* ll, void* out);
//@note O(n) in worst case scenario
bool llisIteratorValid(const LinkedList* ll, const LinkedListIterator* iterator);
//@note operation is O(1)
bool llgetValueAt(const LinkedList* ll, const size_t index, void* out);
//@note O(1) time complexity
size_t llgetCount(const LinkedList* ll);
size_t llgetDataSize(const LinkedList* ll);
bool llremoveHead(LinkedList* ll);
//@note O(n) time complexity in all cases
bool llremoveTail(LinkedList* ll);
//@note operation is O(1)
bool llremoveAt(LinkedList* ll, const size_t index);
bool llremoveAfter(LinkedList* ll, const LinkedListIterator* iterator);
bool llsetValueAtIterator(LinkedList* ll, const LinkedListIterator* iterator, const void* value);
void llreverse(LinkedList* ll);

#endif