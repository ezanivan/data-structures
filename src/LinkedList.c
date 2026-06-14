#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

typedef struct LinkedListNode{
    struct LinkedListNode* next;
    unsigned char data[];

} LinkedListNode;

typedef struct {
    LinkedListNode* head;
    LinkedListNode* tail;
    size_t dataSize;
    size_t count;
    void (*deconstructor)(void*);
}LinkedList;

typedef struct 
{
    struct LinkedListNode* node;
}LinkedListIterator;

LinkedListNode* createNode(const size_t dataSize){
    if (dataSize > SIZE_MAX - sizeof(LinkedListNode)) return NULL;
    if (dataSize == 0) return NULL;
    LinkedListNode* node = malloc(sizeof(LinkedListNode)+dataSize);
    if (node == NULL) return NULL;
    node->next = NULL;
    return node;
}

LinkedList* llcreate(const size_t dataSize, void (*elementDeconstructor)(void*)){
    if (dataSize > SIZE_MAX - sizeof(LinkedListNode)) return NULL;
    if (dataSize == 0) return NULL;
    LinkedList* ll  = malloc(sizeof(LinkedList));
    if (ll == NULL) return NULL;
    ll->dataSize = dataSize;
    ll->count = 0;
    ll->head = NULL;
    ll->tail = NULL;
    ll->deconstructor = elementDeconstructor;

    return ll;
}

void llclear(LinkedList* ll){
    if (ll == NULL) return;
    
    LinkedListNode* currentNode = ll->head;
    
    for (;currentNode != NULL;){
        LinkedListNode* nextNode = currentNode->next;
        if (ll->deconstructor)
        ll->deconstructor(currentNode->data);
        free(currentNode);
        currentNode = nextNode;
    }
    
    ll->head = NULL;
    ll->tail = NULL;
    ll->count = 0;
    return;
}

void lldestroy(LinkedList* ll){
    if (ll == NULL) return;
    
    LinkedListNode* currentNode = ll->head;
    
    for (;currentNode != NULL;){
        LinkedListNode* nextNode = currentNode->next;
        free(currentNode);
        currentNode = nextNode;
    }
    
    free(ll);
    return;
}

void lldestructor(void* element){
    if (element == NULL) return;

    LinkedList* ll;

    memcpy(&ll, element, sizeof(ll));

    lldestroy(ll);
};

bool llinsertTail(LinkedList* ll, const void* value){
    if (value == NULL) return false;
    if (ll == NULL) return false;
    LinkedListNode* newTail = createNode(ll->dataSize);
    if (newTail == NULL) return false;
    memcpy(newTail->data,value,ll->dataSize);
    if (ll->tail != NULL){
        ll->tail->next = newTail;
    }else{
        ll->head = newTail;
    }
    ll->tail = newTail;
    ll->count++;
    return true;
}

bool llinsertHead(LinkedList* ll, const void* value){
    if (value == NULL) return false;
    if (ll == NULL) return false;
    LinkedListNode* newHead = createNode(ll->dataSize);
    if (newHead == NULL) return false;
    memcpy(newHead->data,value,ll->dataSize);
    if (ll->head != NULL){
        newHead->next = ll->head;
    }else{
        ll->tail = newHead;
    }
    ll->head = newHead;
    ll->count++;
    return true;
}

bool llinsertAt(LinkedList* ll, const size_t index, const void* value){
    if (value == NULL) return false;
    if (ll == NULL) return false;
    if (index > ll->count) return false;
    if (index == 0) return llinsertTail(ll,value);
    if (index == ll->count) return llinsertHead(ll,value);

    LinkedListNode* newNode = createNode(ll->dataSize);
    if (newNode == NULL) return false;
    memcpy(newNode->data,value,ll->dataSize);
    
    LinkedListNode* currentNode = ll->head;
    
    for(size_t i = 0; i < index; i++){
        currentNode = currentNode->next;
    }
    newNode->next = currentNode->next;
    currentNode->next = newNode;
    ll->count++;
    return true;
}

bool llinsertAfter(LinkedList* ll, LinkedListIterator* iterator, const void* value){
    LinkedListNode* node = iterator->node;
    if (ll == NULL) return false;
    if (node == NULL) return false;
    if (value == NULL) return false;

    LinkedListNode* newNode = createNode(ll->dataSize);
    if (newNode == NULL) return false;
    
    memcpy(newNode->data,value,ll->dataSize);
    newNode->next = node->next;
    if(ll->tail == node){
        ll->tail = newNode;
    }
    node->next = newNode;
    ll->count++;
    return true;
}

bool llmoveAppend(LinkedList* outputList, LinkedList* sourceList){
    if (outputList == sourceList) return false;
    if (outputList == NULL) return false;
    if (sourceList == NULL) return false;
    if (outputList->dataSize != sourceList->dataSize) return false;
    if (sourceList->count == 0) return true;

    if (outputList->count != 0){
        outputList->tail->next = sourceList->head;
    }else{
        outputList->head = sourceList->head;
    }
    outputList->tail = sourceList->tail;

    outputList->count += sourceList->count;

    sourceList->head = NULL;
    sourceList->tail = NULL;
    sourceList->count = 0;
    return true;
}

LinkedListIterator llgetIteratorAt(const LinkedList* ll, const size_t index){
    if (ll == NULL) return (LinkedListIterator){.node = NULL};
    if (index >= ll->count) return (LinkedListIterator){.node = NULL};
    
    LinkedListNode* currentNode = ll->head;
    
    for(size_t i = 0; i < index; i++){
        currentNode = currentNode->next;
    }
    
    return (LinkedListIterator){.node = currentNode};
}

LinkedListIterator llgetIteratorHead(const LinkedList* ll){
    if (ll == NULL) return (LinkedListIterator){.node = NULL};
    return (LinkedListIterator){.node = ll->head};
}

LinkedListIterator llgetIteratorTail(const LinkedList* ll){
    if (ll == NULL) return (LinkedListIterator){.node = NULL};
    return (LinkedListIterator){.node = ll->tail};
}

LinkedListIterator llgetIteratorNext(const LinkedListIterator* iterator){
    LinkedListNode* node = iterator->node;
    if (node == NULL) return (LinkedListIterator){.node = NULL};
    return (LinkedListIterator){.node = node->next};
}

bool llgetValueAtIterator(const LinkedList* ll, const LinkedListIterator* iterator, void* out){
    const LinkedListNode* node = iterator->node;
    if (node == NULL) return false;
    if (ll == NULL) return false;
    if (out == NULL) return false;
    
    memcpy(out,node->data,ll->dataSize);
    return true;
}

bool llgetHeadValue(const LinkedList* ll, void* out){
    if (ll == NULL) return false;
    if (out == NULL) return false;
    const LinkedListNode* node = llgetIteratorHead(ll).node;
    if (node == NULL) return false;
    
    memcpy(out,node->data,ll->dataSize);
    return true;
}

bool llgetTailValue(const LinkedList* ll, void* out){
    if (ll == NULL) return false;
    if (out == NULL) return false;
    
    const LinkedListNode* node = llgetIteratorTail(ll).node;
    if (node == NULL) return false;
    
    memcpy(out,node->data,ll->dataSize);
    return true;
}

bool llgetValueAt(const LinkedList* ll, const size_t index, void* out){
    if (out == NULL) return false;
    
    const LinkedListNode* node = llgetIteratorAt(ll,index).node;
    if (node == NULL) return false;
    
    memcpy(out,node->data,ll->dataSize);
    return true;
}

bool llisIteratorValid(const LinkedList* ll, const LinkedListIterator* iterator){
    LinkedListNode* iteratorNode = iterator->node;

    LinkedListNode* currentNode = ll->head;
    for(;currentNode!=NULL;){
        if (iteratorNode == currentNode) return true;
        currentNode = currentNode->next;
    }
    return false;
}

size_t llgetCount(const LinkedList* ll){
    if (ll == NULL) return 0;
    return ll->count;
}

size_t llgetDataSize(const LinkedList* ll){
    if (ll == NULL) return 0;
    return ll->dataSize;
}

bool llremoveHead(LinkedList* ll){
    if (ll==NULL) return false;
    if (ll->head == NULL) return false;
    LinkedListNode* cachedHead = ll->head;
    ll->head = ll->head->next;
    if (ll->deconstructor != NULL)
        ll->deconstructor(cachedHead->data);
    free(cachedHead);
    ll->count--;
    if (ll->count == 0)
        ll->tail = NULL;
    return true; 
}

bool llremoveTail(LinkedList* ll){
    if (ll==NULL) return false;
    if (ll->tail == NULL) return false;
    LinkedListNode* cachedTail = ll->tail;

    if (ll->count == 1){
        
    if (ll->deconstructor != NULL)
            ll->deconstructor(ll->tail->data);
        free(ll->tail);
        ll->tail = NULL;
        ll->head = NULL;
        ll->count = 0;
        return true;
    }
    
    LinkedListNode* currentNode = ll->head;
    
    for(;currentNode->next!=ll->tail;){
        currentNode = currentNode->next;
    }

    ll->tail = currentNode;
    ll->tail->next = NULL;
    
    if (ll->deconstructor != NULL)
        ll->deconstructor(cachedTail->data);
    free(cachedTail);
    ll->count--;
    return true; 
}

bool llremoveAt(LinkedList* ll, const size_t index){
    if (ll == NULL) return false;
    if (index >= ll->count) return false;
    if (index == 0) return llremoveHead(ll);
    if (index == ll->count-1) return llremoveTail(ll);

    LinkedListNode* previousNode = ll->head;
    
    for(size_t i = 0; i < index - 1 ; i++){
        previousNode = previousNode->next;
    }
    
    LinkedListNode* nodeToRemove = previousNode->next;
    previousNode->next = nodeToRemove->next;
    
    if (ll->deconstructor != NULL)
        ll->deconstructor(nodeToRemove->data);
    free(nodeToRemove);
    ll->count--;
    return true;
}

bool llremoveAfter(LinkedList* ll,const LinkedListIterator* iterator){
    LinkedListNode* node = iterator->node;
    if (ll == NULL) return false;
    if (node == NULL) return false;
    if (node->next == NULL) return false;
    LinkedListNode* nodeToRemove = node->next;
    if (ll->tail == nodeToRemove) 
    ll->tail = node;
    node->next = nodeToRemove->next;
    
    free(nodeToRemove);
    ll->count--;
    return true;
}

bool llsetValueAtIterator(LinkedList* ll, const LinkedListIterator* iterator, const void* value){
    LinkedListNode* node = iterator->node;
    if (value == NULL) return false;
    if (ll == NULL) return false;
    if (node == NULL) return false;
    memcpy(node->data,value,ll->dataSize);
    return true;
}

void llreverse(LinkedList* ll){
    if (ll == NULL) return;
    if (ll->count < 2) return;
    LinkedListNode* previousNode = ll->head;

    LinkedListNode* currentNode = previousNode->next;
    
    ll->head->next = NULL;

    for(;currentNode!=NULL;){
        LinkedListNode* nextNode = currentNode->next;
        currentNode->next = previousNode;
        previousNode = currentNode;
        currentNode = nextNode;
    }
    
    ll->tail = ll->head;
    ll->head = previousNode;
}
