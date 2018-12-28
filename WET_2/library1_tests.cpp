#include <iostream>
#include "dict_avl.h"
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
    srand(time(NULL));
    std::cout<<"start!"<<std::endl;
    DictAvl<Key,Value> avl_dict;
    std::vector<Key> inserted_keys;
    std::vector<int> insertion_shuffle;
    int max = 0;
    for (int i = 0; i < 10000; i++) {
        int new_insertion = rand();
        insertion_shuffle.push_back(new_insertion);
        if (new_insertion > max)
            max = new_insertion;
    }
    for (int i = 0; i < 10000; i ++) {
        std::cout<<insertion_shuffle.back()<<std::endl;
        Key key = rand();

        if (std::find(inserted_keys.begin(), inserted_keys.end(), key) != inserted_keys.end()) {
            --i;
            continue;
        }

        inserted_keys.push_back(key);
        //std::cout<<i<<std::endl;
        avl_dict.InsertNode(key,insertion_shuffle.back());
        insertion_shuffle.pop_back();
    }
    std::random_shuffle(inserted_keys.begin(),inserted_keys.end());
    for (int i = 0; i < 10000; i ++) {
        std::cout<<10000 - i << " - " << inserted_keys[i] <<std::endl;
        avl_dict.DeleteNodeByKey(inserted_keys[i]);
    }
//    DictAvl<Key,Value>::PrintInOrder(&avl_dict);
    //TODO: the root is not correct. somewhere I need to update it after every insertion.
    std::cout<<"OK!"<<std::endl;
    return 0;
}