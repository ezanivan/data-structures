#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/Queue.h"

void test_fifo_order(void)
{
    Queue* q = qcreate(sizeof(int), NULL);
    assert(q != NULL);

    int a = 1, b = 2, c = 3;

    assert(qenqueue(q, &a));
    assert(qenqueue(q, &b));
    assert(qenqueue(q, &c));
    assert(qgetCount(q) == 3);

    int out = 0;

    assert(qdequeue(q, &out)); assert(out == 1);
    assert(qdequeue(q, &out)); assert(out == 2);
    assert(qdequeue(q, &out)); assert(out == 3);

    assert(qgetCount(q) == 0);

    qdestroy(q);
}

void test_dequeue_empty(void)
{
    Queue* q = qcreate(sizeof(int), NULL);
    assert(q != NULL);

    int out = 123;
    assert(qdequeue(q, &out) == false);

    qdestroy(q);
}


void test_null_safety(void)
{
    Queue* q = qcreate(sizeof(int), NULL);
    assert(q != NULL);

    int val = 10;

    assert(qenqueue(NULL, &val) == false);
    assert(qdequeue(NULL, &val) == false);
    assert(qgetCount(NULL) == 0);

    assert(qenqueue(q, NULL) == false);

    qdestroy(q);
    qdestroy(NULL);
}

void test_interleaved_ops(void)
{
    Queue* q = qcreate(sizeof(int), NULL);
    assert(q != NULL);

    int a = 1, b = 2, c = 3, d = 4;
    int out;

    assert(qenqueue(q, &a));
    assert(qenqueue(q, &b));

    assert(qdequeue(q, &out)); assert(out == 1);

    assert(qenqueue(q, &c));
    assert(qenqueue(q, &d));

    assert(qdequeue(q, &out)); assert(out == 2);
    assert(qdequeue(q, &out)); assert(out == 3);
    assert(qdequeue(q, &out)); assert(out == 4);

    assert(qgetCount(q) == 0);

    qdestroy(q);
}

void test_stress_large_queue(void)
{
    Queue* q = qcreate(sizeof(int), NULL);
    assert(q != NULL);

    const int N = 10000;

    for (int i = 0; i < N; i++) {
        assert(qenqueue(q, &i));
    }

    assert(qgetCount(q) == N);

    for (int i = 0; i < N; i++) {
        int out = -1;
        assert(qdequeue(q, &out));
        assert(out == i);
    }

    assert(qgetCount(q) == 0);

    qdestroy(q);
}

void test_queue_of_pointers(void)
{
    Queue* outer = qcreate(sizeof(Queue*), NULL);
    assert(outer != NULL);

    Queue* inner1 = qcreate(sizeof(int), NULL);
    Queue* inner2 = qcreate(sizeof(int), NULL);

    int a = 10, b = 20;

    assert(qenqueue(inner1, &a));
    assert(qenqueue(inner2, &b));

    assert(qenqueue(outer, &inner1));
    assert(qenqueue(outer, &inner2));

    Queue* out = NULL;

    assert(qdequeue(outer, &out));
    assert(out == inner1);

    int value = 0;
    assert(qdequeue(inner1, &value));
    assert(value == 10);

    assert(qdequeue(outer, &out));
    assert(out == inner2);

    assert(qdequeue(inner2, &value));
    assert(value == 20);

    qdestroy(inner1);
    qdestroy(inner2);
    qdestroy(outer);
}

void test_destroy_safety(void)
{
    Queue* q = qcreate(sizeof(int), NULL);
    assert(q != NULL);

    int a = 5;
    assert(qenqueue(q, &a));

    qdestroy(q);
}


int main(void)
{
    test_fifo_order();
    test_dequeue_empty();
    test_null_safety();
    test_interleaved_ops();
    test_stress_large_queue();
    test_queue_of_pointers();
    test_destroy_safety();

    printf("All tests passed\n");

    return 0;
}