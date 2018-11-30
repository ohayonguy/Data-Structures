#include <iostream>
#include "library1.h"
#include "dict_list.h"
typedef int Key;
typedef void* Value;

void test_Add() {

};

int main() {
    DictList<Key,Value> dict_list;

    int numbers[10];
    for (int i = 0; i < 10; i++) {
        numbers[i] = i;
    }
    return 0;
}