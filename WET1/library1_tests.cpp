#include <iostream>
#include "library1.h"
#include "dict_list.h"
#ifdef NDEBUG
#undef NDEBUG
#endif
#include "dict_avl.h"
#include <list>
typedef int Key;
typedef int Value;
//typedef void* DS;
//typedef DictList<Key,Value>::ListNode ListNode;
using std::cout;
using std::endl;

/*
void test_Add(DS dataset,int size, int* numbers) {
    StatusType result;
    ListNode** list_node = new ListNode*;
    for (int i = 0; i < size; i++) {
        result = Add((void*)dataset,i, &numbers[i],(void**)list_node);
        if (result != SUCCESS){
            throw std::runtime_error("Failed to add element" + std::to_string(i));
        }
    }
};
void test_Find(DS dataset,Key key_to_find, int value_of_key) {
    StatusType result;
    Value* value;
    result = Find(dataset,key_to_find,value);
    if (result != SUCCESS){
        throw std::runtime_error("Failed to find");
    }
};

*/
int main() {

    /*DS dataset = Init();
    if (dataset == NULL || dataset == nullptr) {
        cout << "Failed to init" << endl;
        throw std::runtime_error("Failed to init dataset");
    }
    int size = 10;
    int numbers[size];
    for (int i = 0; i < size; i++) {
        numbers[i] = i;
    }
    test_Add(dataset,size,numbers);
    DictList<Key,Value>* true_data_set = (DictList<Key,Value>*)dataset;
    true_data_set->PrintDict();*/
    std::cout<<"start!"<<std::endl;
    DictAvl<Key,Value> avl_dict;
    std::vector<Key> inserted_keys;
    for (int i = 0; i < 10000; i ++) {
        std::cout<<i<<std::endl;
        Key key = rand();
        inserted_keys.push_back(key);
        //std::cout<<i<<std::endl;
        avl_dict.InsertNode(key,i);
    }
    for (int i = 0; i < 10000; i ++) {
        std::cout<<i<<std::endl;
        //std::cout<<i<<std::endl;
        avl_dict.DeleteNode(inserted_keys.back());
        inserted_keys.pop_back();
    }
//    DictAvl<Key,Value>::PrintInOrder(&avl_dict);
    //TODO: the root is not correct. somewhere I need to update it after every insertion.
    std::cout<<"OK!"<<std::endl;
    return 0;
}