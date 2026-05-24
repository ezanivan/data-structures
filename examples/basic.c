#include "../include/MutableList.h"
#include <stdio.h>
int main(){

    MutableList* myList = lcreate(sizeof(int));
    int temp = 0;
    lappend(myList,&temp);
    temp = 1;
    lappend(myList,&temp);
    temp = 2;
    lappend(myList,&temp);
    temp = 3;
    lappend(myList,&temp);
    temp = 4;
    lappend(myList,&temp);
    temp = 5;
    lappend(myList,&temp);

    lremove(myList,4);

    for(size_t i = 0; i < lgetSize(myList); i++){
        lget(myList,i,&temp);
        printf("%d", temp);
    }
    

    return 0;
}