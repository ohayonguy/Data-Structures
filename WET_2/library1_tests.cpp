#include <iostream>
#include "dict_avl.h"
#ifdef NDEBUG
#undef NDEBUG
#endif
#include "dict_avl.h"
#include <list>
typedef int Key;
typedef int Value;
using std::cout;
using std::endl;

void TestCreateTreeFromInOrderList() {
    DictAvl<Key,Value>* tree = nullptr;
    for (int k = 1; k < 10000; k++) {

        DictAvl<Key,Value>::AvlNode* array_of_nodes = new DictAvl<Key,Value>::AvlNode[k];
        std::vector<Key> inserted_keys;
        std::vector<int> insertion_shuffle;
        for (int i = 0; i < k; i++) {
            int new_insertion = rand();
            insertion_shuffle.push_back(new_insertion);
        }
        for (int i = 0; i < k; i ++) {
            Key key = rand();
            if (std::find(inserted_keys.begin(), inserted_keys.end(), key) != inserted_keys.end()) {
                --i;
                continue;
            }
            inserted_keys.push_back(key);
            array_of_nodes[i].key = key;
            array_of_nodes[i].value = insertion_shuffle.back();
            insertion_shuffle.pop_back();
        }
        std::sort(array_of_nodes, array_of_nodes + k);
        tree = DictAvl<Key,Value>::CreateAVLTreeFromInOrderNodesArray(array_of_nodes,k);
        delete tree;
        tree = nullptr;
        delete array_of_nodes;
        if (ceil((k/10000.0)*100) == (k/10000.0)*100)
            std::cout<<"Progress: "<<(k/10000.0)*100<<"%"<<std::endl;
    }
    std::cout<<"TestCreateTreeFromInOrderList OK!"<<std::endl;
}
void TestInsertDeleteInAvlTree() {
    DictAvl<Key,Value> avl_dict;
    std::vector<Key> inserted_keys;
    std::vector<int> insertion_shuffle;
    for (int i = 0; i < 10000; i++) {
        int new_insertion = rand();
        insertion_shuffle.push_back(new_insertion);
    }
    for (int i = 0; i < 10000; i ++) {
        Key key = rand();
        if (std::find(inserted_keys.begin(), inserted_keys.end(), key) != inserted_keys.end()) {
            --i;
            continue;
        }
        inserted_keys.push_back(key);
        avl_dict.InsertNode(key,insertion_shuffle.back());
        insertion_shuffle.pop_back();
    }
    std::random_shuffle(inserted_keys.begin(),inserted_keys.end());
    for (int i = 0; i < 10000; i ++) {
        avl_dict.DeleteNodeByKey(inserted_keys[i]);
    }
    std::cout<<"TestInsertDeleteInAvlTree OK!"<<std::endl;
}
int main() {

    srand(time(NULL));
    std::cout<<"start!"<<std::endl;
    TestCreateTreeFromInOrderList();
    TestInsertDeleteInAvlTree();
    std::cout<<"OK!"<<std::endl;
    return 0;
}