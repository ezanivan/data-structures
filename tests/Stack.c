#include <stdio.h>
#include <assert.h>
#include <stdio.h>
#include "../include/Stack.h"

void test_create() {
    Stack* stack = screate(sizeof(int), NULL);

    assert(stack != NULL);
    assert(sgetSize(stack) == 0);

    sdestroy(stack);
}

void test_push_and_peek() {
    Stack* stack = screate(sizeof(int), NULL);

    int value;
    bool status;

    for(int i = 0; i < 5; i++) {
        assert(spush(stack, &i) == true);
    }

    assert(sgetSize(stack) == 5);

    status = peek(stack, &value);

    assert(status == true);
    assert(value == 4);

    sdestroy(stack);
}

void test_pop() {
    Stack* stack = screate(sizeof(int), NULL);

    int value;
    bool status;

    for(int i = 0; i < 5; i++) {
        assert(spush(stack, &i) == true);
    }

    for(int i = 4; i >= 0; i--) {
        value = -1;

        status = spop(stack, &value);

        assert(status == true);
        assert(value == i);
    }

    assert(sgetSize(stack) == 0);

    sdestroy(stack);
}

void test_invalid_push() {
    Stack* stack = screate(sizeof(int), NULL);

    int value = 10;

    assert(spush(NULL, &value) == false);
    assert(spush(stack, NULL) == false);

    sdestroy(stack);
}

void test_invalid_pop() {
    Stack* stack = screate(sizeof(int), NULL);

    int value = 999;

    assert(spop(NULL, &value) == false);
    assert(spop(stack, &value) == false);
    assert(value == 999);

    assert(spop(stack, NULL) == false);

    sdestroy(stack);
}

void test_invalid_peek() {
    Stack* stack = screate(sizeof(int), NULL);

    int value = 999;

    assert(peek(NULL, &value) == false);
    assert(peek(stack, &value) == false);
    assert(value == 999);

    assert(peek(stack, NULL) == false);

    sdestroy(stack);
}

void test_copy_behavior() {
    Stack* stack = screate(sizeof(int), NULL);

    int value = 42;

    assert(spush(stack, &value) == true);

    value = 0;

    assert(peek(stack, &value) == true);
    assert(value == 42);

    sdestroy(stack);
}

void test_capacity_functions() {
    Stack* stack = screate(sizeof(int), NULL);

    for(int i = 0; i < 7; i++) {
        assert(spush(stack, &i) == true);
    }

    assert(sgetCapacity(stack) == 10);

    strimToSize(stack, 0);
    assert(sgetCapacity(stack) == 10);

    strimToSize(stack, 5);
    assert(sgetCapacity(stack) == 10);

    strimToSize(stack, 11);
    assert(sgetCapacity(stack) == 11);

    strimToSize(stack, 9);
    assert(sgetCapacity(stack) == 9);

    strim(stack);
    assert(sgetCapacity(stack) == 7);

    sreserve(stack, 15);
    assert(sgetCapacity(stack) == 15);

    sdestroy(stack);
}

void test_clear() {
    Stack* stack = screate(sizeof(int), NULL);

    for(int i = 0; i < 5; i++) {
        assert(spush(stack, &i) == true);
    }

    assert(sgetSize(stack) == 5);

    assert(sclear(stack) == true);

    assert(sgetSize(stack) == 0);

    int value = 999;

    assert(spop(stack, &value) == false);
    assert(value == 999);

    sdestroy(stack);
}

void test_get_ptr() {
    Stack* stack = screate(sizeof(int), NULL);

    for(int i = 0; i < 5; i++) {
        assert(spush(stack, &i) == true);
    }

    int value = *(int*)getPtr(stack, 2);

    assert(value == 2);

    sdestroy(stack);
}

void test_large_push_pop() {
    Stack* stack = screate(sizeof(int), NULL);

    for(int i = 0; i < 100000; i++) {
        assert(spush(stack, &i) == true);
    }

    assert(sgetSize(stack) == 100000);

    int value;

    for(int i = 99999; i >= 0; i--) {
        assert(spop(stack, &value) == true);
        assert(value == i);
    }

    assert(sgetSize(stack) == 0);

    sdestroy(stack);
}

void test_stack_of_stacks(void) {

    Stack* outer = screate(sizeof(Stack*), sdestructor);
    assert(outer);

    Stack* s1 = screate(sizeof(int), NULL);
    Stack* s2 = screate(sizeof(int), NULL);
    Stack* s3 = screate(sizeof(int), NULL);

    assert(s1 && s2 && s3);

    for (int i = 0; i < 5; i++) {
        assert(spush(s1, &i));
    }
    for (int i = 100; i < 103; i++) {
        assert(spush(s2, &i));
    }

    for (int i = 50; i < 52; i++) {
        assert(spush(s3, &i));
    }

    assert(spush(outer, &s1));
    assert(spush(outer, &s2));
    assert(spush(outer, &s3));

    assert(sgetSize(outer) == 3);

    Stack* top = NULL;
    assert(peek(outer, &top));
    assert(top == s3);

    int extra = 999;
    assert(spush(top, &extra));
    assert(sgetSize(top) == 3);

    int popped = 0;
    assert(spop(top, &popped));
    assert(popped == 999);
    assert(sgetSize(top) == 2);

    Stack* poppedStack = NULL;
    assert(spop(outer, &poppedStack));
    assert(poppedStack == s3);
    assert(sgetSize(outer) == 2);

    assert(sclear(outer));
    assert(sgetSize(outer) == 0);

    sdestroy(outer);
}

int main() {

    test_create();
    test_push_and_peek();
    test_pop();

    test_invalid_push();
    test_invalid_pop();
    test_invalid_peek();

    test_copy_behavior();

    test_capacity_functions();

    test_clear();

    test_get_ptr();

    test_large_push_pop();

    test_stack_of_stacks();
    
    printf("All tests passed\n");

    return 0;
}