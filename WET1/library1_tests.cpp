#include <iostream>
#include "library1.h"
//#include "dict_list.h"
#include "dict_avl.h"
/*typedef int Key;
typedef void* Value;
typedef void* DS;
typedef DictList<Key,Value>::ListNode ListNode;
using std::cout;
using std::endl;
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
    true_data_set->PrintDict();
    return 0;*/
    DictAvl<int,int> avl_dict;
    avl_dict.InsertNode(10,5);
}