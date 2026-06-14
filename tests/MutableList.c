#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "../include/MutableList.h"

void test_create_and_append() {
    int temp;
    bool status;

    MutableList* list = lcreate(sizeof(int), NULL);

    assert(lgetSize(list) == 0);

    for(int i = 0; i < 5; i++) {
        assert(lappend(list, &i) == true);
    }

    assert(lgetSize(list) == 5);

    status = lget(list, 0, &temp);
    assert(status == true);
    assert(temp == 0);

    status = lget(list, 4, &temp);
    assert(status == true);
    assert(temp == 4);

    temp = 999;

    status = lget(list, -1, &temp);
    assert(status == false);
    assert(temp == 999);

    status = lget(list, 5, &temp);
    assert(status == false);
    assert(temp == 999);

    status = lget(list, 0, NULL);
    assert(status == false);

    status = lget(NULL, 0, &temp);
    assert(status == false);

    assert(lappend(NULL, &temp) == false);
    assert(lappend(list, NULL) == false);

    assert(lgetSize(list) == 5);

    temp = 5;

    assert(lappend(list, &temp) == true);

    temp = 0;

    status = lget(list, 5, &temp);

    assert(status == true);
    assert(temp == 5);

    assert(lgetSize(list) == 6);

    ldestroy(list);
}

void test_insert() {
    int temp;
    bool status;

    MutableList* list = lcreate(sizeof(int), NULL);

    for(int i = 0; i < 6; i++) {
        assert(lappend(list, &i) == true);
    }

    status = linsert(NULL, 2, &temp);
    assert(status == false);

    status = linsert(list, -1, &temp);
    assert(status == false);

    status = linsert(list, 2, NULL);
    assert(status == false);

    status = linsert(list, 7, &temp);
    assert(status == false);

    assert(lgetSize(list) == 6);

    temp = 6;

    status = linsert(list, 6, &temp);
    assert(status == true);

    temp = 0;

    assert(lget(list, 6, &temp) == true);
    assert(temp == 6);

    temp = 7;

    status = linsert(list, 2, &temp);
    assert(status == true);

    temp = 0;

    assert(lget(list, 2, &temp) == true);
    assert(temp == 7);

    assert(lgetSize(list) == 8);

    ldestroy(list);
}

void test_remove() {
    int temp;
    bool status;

    MutableList* list = lcreate(sizeof(int), NULL);

    for(int i = 0; i < 8; i++) {
        assert(lappend(list, &i) == true);
    }

    status = lremove(NULL, 2);
    assert(status == false);

    status = lremove(list, -1);
    assert(status == false);

    status = lremove(list, 8);
    assert(status == false);

    status = lremove(list, 2);
    assert(status == true);

    assert(lget(list, 2, &temp) == true);
    assert(temp == 3);

    assert(lgetSize(list) == 7);

    ldestroy(list);
}

void test_capacity_functions() {
    MutableList* list = lcreate(sizeof(int), NULL);

    for(int i = 0; i < 7; i++) {
        assert(lappend(list, &i) == true);
    }

    assert(lgetCapacity(list) == 10);

    ltrimToSize(list, 0);
    assert(lgetCapacity(list) == 10);

    ltrimToSize(list, 5);
    assert(lgetCapacity(list) == 10);

    ltrimToSize(list, 11);
    assert(lgetCapacity(list) == 11);

    ltrimToSize(list, 9);
    assert(lgetCapacity(list) == 9);

    ltrim(list);
    assert(lgetCapacity(list) == 7);

    lreserve(list, 15);
    assert(lgetCapacity(list) == 15);

    ldestroy(list);
}

void test_join() {
    int temp;

    MutableList* list = lcreate(sizeof(int), NULL);
    MutableList* list2 = lcreate(sizeof(int), NULL);

    for(int i = 0; i < 6; i++) {
        assert(lappend(list, &i) == true);

        temp = i + 6;

        assert(lappend(list2, &temp) == true);
    }

    assert(ljoin(list, list2) == true);

    assert(lgetSize(list) == 12);

    for(int i = 0; i < 12; i++) {
        assert(lget(list, i, &temp) == true);
        assert(temp == i);
    }

    ldestroy(list);
    ldestroy(list2);
}

void test_get_ptr() {
    MutableList* list = lcreate(sizeof(int), NULL);

    for(int i = 0; i < 5; i++) {
        assert(lappend(list, &i) == true);
    }

    int value = *(int*)lgetPtr(list, 2);

    assert(value == 2);

    ldestroy(list);
}


void test_nested_mutable_list() {
    MutableList* outer = lcreate(sizeof(MutableList*), ldestructor);
    assert(outer != NULL);

    MutableList* a = lcreate(sizeof(int), NULL);
    MutableList* b = lcreate(sizeof(int), NULL);
    MutableList* c = lcreate(sizeof(int), NULL);

    assert(a && b && c);

    int v;

    v = 1; lappend(a, &v);
    v = 2; lappend(a, &v);

    v = 10; lappend(b, &v);
    v = 20; lappend(b, &v);
    v = 30; lappend(b, &v);

    v = 100; lappend(c, &v);

    assert(lappend(outer, &a));
    assert(lappend(outer, &b));
    assert(lappend(outer, &c));

    assert(lgetSize(outer) == 3);

    v = 40; lappend(b, &v);
    assert(lgetSize(b) == 4);

    assert(lremove(a, 0));
    assert(lgetSize(a) == 1);

    MutableList* b_ptr = NULL;
    assert(lget(outer, 1, &b_ptr));

    int* value = (int*)lgetPtr(b_ptr, 2);
    assert(value != NULL);
    assert(*value == 30);

    for (size_t i = 0; i < lgetSize(outer); i++) {
        MutableList* inner = NULL;
        assert(lget(outer, i, &inner));
        assert(inner != NULL);
        assert(lgetSize(inner) > 0);
    }

    ldestroy(outer);
}

int main() {

    test_create_and_append();
    test_insert();
    test_remove();
    test_capacity_functions();
    test_join();
    test_get_ptr();
    //test_nested_mutable_list();

    printf("All tests passed\n");

    return 0;
}