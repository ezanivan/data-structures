#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "../include/LinkedList.h"

static void test_create_and_basic_properties() {
    LinkedList *ll = llcreate(sizeof(int), NULL);

    assert(ll != NULL);
    assert(llgetCount(ll) == 0);
    assert(llgetDataSize(ll) == sizeof(int));

    lldestroy(ll);
}

static void test_insert_head_tail_and_get() {
    LinkedList *ll = llcreate(sizeof(int), NULL);
    int temp;

    assert(llinsertHead(ll, &(int){2}) == true);
    assert(llinsertHead(ll, &(int){1}) == true);
    assert(llinsertTail(ll, &(int){3}) == true);

    assert(llgetCount(ll) == 3);

    assert(llgetValueAt(ll, 0, &temp) == true);
    assert(temp == 1);

    assert(llgetValueAt(ll, 1, &temp) == true);
    assert(temp == 2);

    assert(llgetValueAt(ll, 2, &temp) == true);
    assert(temp == 3);

    lldestroy(ll);
}

static void test_insert_at_edge_cases() {
    LinkedList *ll = llcreate(sizeof(int), NULL);
    int temp;

    assert(llinsertAt(ll, 0, &(int){10}) == true); // empty insert

    assert(llinsertAt(ll, 1, &(int){20}) == true); // append at end

    assert(llinsertAt(ll, 5, &(int){30}) == false); // out of bounds

    assert(llgetCount(ll) == 2);

    assert(llgetValueAt(ll, 0, &temp) == true);
    assert(temp == 20);

    lldestroy(ll);
}

static void test_null_inputs() {
    LinkedList *ll = llcreate(sizeof(int), NULL);
    int temp = 0;

    assert(llinsertHead(NULL, &temp) == false);
    assert(llinsertTail(NULL, &temp) == false);
    assert(llinsertAt(NULL, 0, &temp) == false);

    assert(llinsertHead(ll, NULL) == false);
    assert(llinsertTail(ll, NULL) == false);
    assert(llinsertAt(ll, 0, NULL) == false);

    assert(llgetValueAt(NULL, 0, &temp) == false);
    assert(llgetValueAt(ll, 0, NULL) == false);

    assert(llremoveAt(NULL, 0) == false);

    lldestroy(ll);
}

static void test_remove_head_tail() {
    LinkedList *ll = llcreate(sizeof(int), NULL);
    int temp;

    for (int i = 1; i <= 3; i++) {
        assert(llinsertTail(ll, &i) == true);
    }

    assert(llremoveHead(ll) == true); // remove 1
    assert(llremoveTail(ll) == true); // remove 3

    assert(llgetCount(ll) == 1);

    assert(llgetValueAt(ll, 0, &temp) == true);
    assert(temp == 2);

    lldestroy(ll);
}

static void test_remove_at_edge_cases() {
    LinkedList *ll = llcreate(sizeof(int), NULL);
    int temp;

    for (int i = 0; i < 5; i++) {
        assert(llinsertTail(ll, &i) == true);
    }

    assert(llremoveAt(ll, 10) == false);
    assert(llremoveAt(ll, 2) == true);

    assert(llgetCount(ll) == 4);

    assert(llgetValueAt(ll, 2, &temp) == true);
    assert(temp == 3);

    lldestroy(ll);
}

void test_iterators()
{
    LinkedList* list = llcreate(sizeof(int), NULL);
    assert(list != NULL);

    assert(llgetIteratorHead(list) == NULL);
    assert(llgetIteratorTail(list) == NULL);

    int values[] = {10, 20, 30, 40};

    for (size_t i = 0; i < 4; i++)
    {
        assert(llinsertTail(list, &values[i]));
    }

    LinkedListIterator head = llgetIteratorHead(list);
    LinkedListIterator tail = llgetIteratorTail(list);

    assert(head != NULL);
    assert(tail != NULL);

    int out;

    assert(llgetValueAtIterator(list, &head, &out));
    assert(out == 10);

    assert(llgetValueAtIterator(list, &tail, &out));
    assert(out == 40);

    LinkedListIterator it = llgetIteratorHead(list);

    assert(it != NULL);

    assert(llgetValueAtIterator(list, &it, &out));
    assert(out == 10);

    it = llgetIteratorNext(&it);
    assert(it != NULL);
    assert(llgetValueAtIterator(list, &it, &out));
    assert(out == 20);

    it = llgetIteratorNext(&it);
    assert(it != NULL);
    assert(llgetValueAtIterator(list, &it, &out));
    assert(out == 30);

    it = llgetIteratorNext(&it);
    assert(it != NULL);
    assert(llgetValueAtIterator(list, &it, &out));
    assert(out == 40);

    it = llgetIteratorNext(&it);
    assert(it == NULL);

    for (size_t i = 0; i < 4; i++)
    {
        LinkedListIterator idxIt = llgetIteratorAt(list, i);

        assert(idxIt != NULL);
        assert(llisIteratorValid(list, &idxIt));

        assert(llgetValueAtIterator(list, &idxIt, &out));
        assert(out == values[i]);
    }

    assert(llgetIteratorAt(list, 4) == NULL);

    LinkedListIterator second = llgetIteratorAt(list, 1);

    assert(second != NULL);
    assert(llisIteratorValid(list, &second));

    assert(llremoveAt(list, 1));

    assert(!llisIteratorValid(list, &second));

    int expected[] = {10, 30, 40};

    it = llgetIteratorHead(list);

    for (size_t i = 0; i < 3; i++)
    {
        assert(it != NULL);

        assert(llgetValueAtIterator(list, &it, &out));
        assert(out == expected[i]);

        it = llgetIteratorNext(&it);
    }

    assert(it == NULL);

    lldestroy(list);
}

static void test_iterator_invalid_after_modification() {
    LinkedList *ll = llcreate(sizeof(int), NULL);

    for (int i = 0; i < 3; i++) {
        assert(llinsertTail(ll, &i) == true);
    }

    LinkedListIterator it = llgetIteratorHead(ll);

    assert(llremoveHead(ll) == true);

    assert(llisIteratorValid(ll, &it) == false);

    lldestroy(ll);
}

static void test_reverse() {
    LinkedList *ll = llcreate(sizeof(int), NULL);
    int temp;

    for (int i = 0; i < 4; i++) {
        assert(llinsertTail(ll, &i) == true);
    }

    llreverse(ll);

    for (int i = 0; i < 4; i++) {
        assert(llgetValueAt(ll, i, &temp) == true);
        assert(temp == 3 - i);
    }

    lldestroy(ll);
}

static void test_clear() {
    LinkedList *ll = llcreate(sizeof(int), NULL);

    for (int i = 0; i < 5; i++) {
        assert(llinsertTail(ll, &i) == true);
    }

    llclear(ll);

    assert(llgetCount(ll) == 0);

    lldestroy(ll);
}

static void test_move_append() {
    LinkedList *a = llcreate(sizeof(int), NULL);
    LinkedList *b = llcreate(sizeof(int), NULL);

    int temp;

    for (int i = 0; i < 3; i++) {
        assert(llinsertTail(a, &i) == true);
    }

    for (int i = 3; i < 6; i++) {
        assert(llinsertTail(b, &i) == true);
    }

    assert(llmoveAppend(a, b) == true);

    assert(llgetCount(a) == 6);
    assert(llgetCount(b) == 0);

    for (int i = 0; i < 6; i++) {
        assert(llgetValueAt(a, i, &temp) == true);
        assert(temp == i);
    }

    lldestroy(a);
    lldestroy(b);
}

void test_linkedlist_of_linkedlists()
{
    LinkedList* outer =
        llcreate(sizeof(LinkedList*), lldestructor);

    assert(outer != NULL);

    LinkedList* inner1 = llcreate(sizeof(int), NULL);
    LinkedList* inner2 = llcreate(sizeof(int), NULL);
    LinkedList* inner3 = llcreate(sizeof(int), NULL);

    assert(inner1);
    assert(inner2);
    assert(inner3);

    for (int i = 0; i < 5; i++)
    {
        assert(llinsertTail(inner1, &i));
    }

    for (int i = 10; i < 15; i++)
    {
        assert(llinsertTail(inner2, &i));
    }

    for (int i = 20; i < 25; i++)
    {
        assert(llinsertTail(inner3, &i));
    }

    assert(llinsertTail(outer, &inner1));
    assert(llinsertTail(outer, &inner2));
    assert(llinsertTail(outer, &inner3));

    assert(llgetCount(outer) == 3);

    LinkedListIterator outerIt = llgetIteratorHead(outer);

    int expectedStarts[] = {0, 10, 20};

    for (int i = 0; i < 3; i++)
    {
        LinkedList* retrieved = NULL;

        assert(outerIt != NULL);

        assert(
            llgetValueAtIterator(
                outer,
                &outerIt,
                &retrieved));

        assert(retrieved != NULL);

        int value;

        assert(llgetHeadValue(retrieved, &value));
        assert(value == expectedStarts[i]);

        outerIt = llgetIteratorNext(&outerIt);
    }

    assert(outerIt == NULL);

    int extra = 999;

    assert(llinsertTail(inner2, &extra));

    assert(llgetCount(inner2) == 6);

    int tailValue;

    assert(llgetTailValue(inner2, &tailValue));
    assert(tailValue == 999);

    LinkedListIterator secondOuter = llgetIteratorAt(outer, 1);

    LinkedList* retrievedInner2 = NULL;

    assert(secondOuter != NULL);

    assert(
        llgetValueAtIterator(
            outer,
            &secondOuter,
            &retrievedInner2));

    assert(retrievedInner2 == inner2);

    assert(llgetTailValue(retrievedInner2, &tailValue));
    assert(tailValue == 999);

    assert(llremoveHead(outer));

    assert(llgetCount(outer) == 2);
    
    LinkedList* remaining;

    LinkedListIterator it = llgetIteratorHead(outer);

    assert(
        llgetValueAtIterator(
            outer,
            &it,
            &remaining));

    assert(remaining == inner2);
    lldestroy(outer);
}
int main() {
    test_create_and_basic_properties();
    test_insert_head_tail_and_get();
    test_insert_at_edge_cases();
    test_null_inputs();
    test_remove_head_tail();
    test_remove_at_edge_cases();
    test_iterators();
    test_iterator_invalid_after_modification();
    test_reverse();
    test_clear();
    test_move_append();
    test_linkedlist_of_linkedlists();

    printf("All tests passed\n");
    return 0;
}