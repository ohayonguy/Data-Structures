//
// Created by guyoe on 29-Nov-18.
//

#ifndef WET1_DICT_AVL_H
#define WET1_DICT_AVL_H

#include <iostream>
#include <assert.h>
#include <algorithm>
#include <vector>

template <class Key, class Value>
class DictAvl {
public:
    class KeyNotFound : public std::exception {};
    struct AvlNode {
        Value value;
        Key key;
        AvlNode* left_son;
        AvlNode* right_son;
        AvlNode* father;
        int balance_factor;
        int left_height;
        int right_height;
        AvlNode(const Key& key, const Value& value) :
                value(value),
                key(key),
                left_son(nullptr),
                right_son(nullptr),
                father(nullptr),
                balance_factor(0),
                left_height(0),
                right_height(0) {};
    };
    DictAvl() : root(nullptr), size(0) {};
    ~DictAvl();

    AvlNode* InsertNode(const Key& key, const Value& value);
    //Value GetValueByKey(const Key& key);
    //void DeleteNodeByKey(const Key &key);
    //void DeleteNodeByPtr(const DictAvl* node_to_delete);
    void DeleteNodeByKey(const Key& key, AvlNode* current_node);
    //int GetSize();
    //void PrintDict();
    static void PrintInOrder(const DictAvl* avl);
    int GetTreeHeight(const AvlNode* root);
private:
    void InsertNode(AvlNode *current_node, AvlNode* node_to_insert);
    /**
     * Find a node by a given key and return a pointer to that node.
     * @param key is the key to look for
     * @return list node pointer which contains the given key.
     */
    ///DictAvl* GetNodeByKey(const Key& key);
    AvlNode* root;
    int size;
    void UpdateTreeBottomToTop(AvlNode* bottom_node);
    static void RollRight(AvlNode*);
    static void RollLeft(AvlNode*);
    static void DeleteTree(const AvlNode* node_to_delete);
    static bool CheckIfAVL(const AvlNode* root);
    static void PrintInOrder(const AvlNode* root, std::vector<Key>* result);
    static void BSTInsert(AvlNode* root, AvlNode* new_node);
    static void Roll(AvlNode* base_node);
    static bool NodeInTree(const AvlNode* root, const AvlNode* node_to_look_for);
};

template<class Key, class Value>
typename DictAvl<Key, Value>::AvlNode *DictAvl<Key, Value>::InsertNode(const Key &key, const Value &value) {
    AvlNode* new_node = new AvlNode(key,value);
    if (this->root == nullptr) {
        this->root = new_node;
        return new_node;
    }
    BSTInsert(root,new_node);
    size++;
    assert(NodeInTree(root,new_node));
    UpdateTreeBottomToTop(new_node);
    assert(CheckIfAVL(root)); //add a test if the tree is still AVL;
}

template<class Key, class Value>
void DictAvl<Key, Value>::BSTInsert(AvlNode* root, DictAvl::AvlNode *new_node) {
    assert(root != nullptr && new_node != nullptr);
    if (root->key < new_node->key) {
        if (root->right_son == nullptr) {
            root->right_son = new_node;
            new_node->father = root;
        }
        else
            BSTInsert(root->right_son, new_node);
    } else if (root->key > new_node->key) {
        if (root->left_son == nullptr) {
            root->left_son = new_node;
            new_node->father = root;
        }
        else
            BSTInsert(root->left_son, new_node);
    } else {
        assert(false); // the key is already inserted!
    }
}

template<class Key, class Value>
void DictAvl<Key, Value>::UpdateTreeBottomToTop(DictAvl::AvlNode *bottom_node) {
    if (bottom_node == nullptr)
        return; //nothing to update
    assert(abs(bottom_node->balance_factor)<=2);
    AvlNode* bottom_node_father = bottom_node->father;
    //int bottom_node_height = std::max(bottom_node->right_height,bottom_node->left_height);
    //std::cout<<" BOTTOM NODE HEIGHT: before = "<<bottom_node_height;
    Roll(bottom_node);
    assert(abs(bottom_node->balance_factor) <= 1);
    assert(abs(bottom_node->right_height-bottom_node->left_height) <= 1);
    int bottom_node_height = std::max(bottom_node->right_height,bottom_node->left_height);;
    assert(CheckIfAVL(bottom_node));
    UpdateTreeBottomToTop(bottom_node_father);
    assert(abs(bottom_node->balance_factor)<=2);
    //assert(abs(bottom_node_father->balance_factor)<=2);
}

template<class Key, class Value>
void DictAvl<Key, Value>::Roll(typename DictAvl::AvlNode *base_node) {
    if (base_node == nullptr)
        return;
    assert(abs(base_node->balance_factor)<=2);
    bool rolled = false;
    if (base_node->balance_factor == -2) {
        rolled = true;
        assert(base_node->right_son != nullptr);
        if (base_node->right_son->balance_factor == -1) {
            RollLeft(base_node);
        } else {
            assert(base_node->right_son->balance_factor >= 0 && base_node->right_son->left_son != nullptr);
            RollRight(base_node->right_son);
            RollLeft(base_node);
        }
    } else if (base_node->balance_factor == 2) {
        rolled = true;
        assert(base_node->left_son != nullptr);
        if (base_node->left_son->balance_factor == -1) {
            assert(base_node->left_son->right_son != nullptr);
            RollLeft(base_node->left_son);
            RollRight(base_node);
        } else {
            assert(base_node->left_son->balance_factor >= 0);
            RollRight(base_node);
        }
    }
    /*if(abs(base_node->balance_factor) > 1) {
        std::cout<<rolled<<std::endl;
        PrintInOrder(base_node);
    }*/
    //assert(abs(base_node->balance_factor)<=1);
    return;
}

template<class Key, class Value>
void DictAvl<Key, Value>::RollLeft(DictAvl::AvlNode *base_node) {
    assert(base_node != nullptr && base_node->right_son != nullptr);
    AvlNode* father_to_update = base_node->father;
    AvlNode* right_son = base_node->right_son;
    base_node->father = right_son;
    base_node->right_son = right_son->left_son;
    base_node->right_height = 0;
    if (base_node->right_son != nullptr)
        base_node->right_height = 1 + std::max(base_node->right_son->right_height, base_node->right_son->left_height);
    base_node->balance_factor = base_node->left_height - base_node->right_height;

    right_son->left_son = base_node;
    right_son->left_height = 0;
    if (right_son->left_son != nullptr)
        right_son->left_height = 1 + std::max(right_son->left_son->left_height, right_son->left_son->right_height);
    right_son->balance_factor = right_son->left_height - right_son->right_height;

    if (father_to_update != nullptr) {
        if (father_to_update->right_son == base_node) {
            father_to_update->right_height = 1 + std::max(right_son->left_height, right_son->right_height);
            father_to_update->right_son = right_son;
        } else {
            father_to_update->left_height = 1 + std::max(right_son->left_height, right_son->right_height);
            father_to_update->left_son = right_son;
        }
        father_to_update->balance_factor = father_to_update->left_height - father_to_update->right_height;
    }
    right_son->father = father_to_update;
}
template<class Key, class Value>
void DictAvl<Key, Value>::RollRight(DictAvl::AvlNode *base_node) {
    assert(base_node != nullptr && base_node->left_son != nullptr);
    AvlNode* father_to_update = base_node->father;
    AvlNode* left_son = base_node->left_son;
    base_node->father = left_son;
    base_node->left_son = left_son->right_son;
    base_node->left_height = 0;
    if (base_node->left_son != nullptr)
        base_node->left_height = 1 + std::max(base_node->left_son->right_height, base_node->left_son->left_height);
    base_node->balance_factor = base_node->left_height - base_node->right_height;

    left_son->right_son = base_node;
    left_son->right_height = 0;
    if (left_son->right_son != nullptr)
        left_son->right_height = 1 + std::max(left_son->right_son->left_height, left_son->right_son->right_height);
    left_son->balance_factor = left_son->left_height - left_son->right_height;

    if (father_to_update != nullptr) {
        if (father_to_update->left_son == base_node) {
            father_to_update->left_height = 1 + std::max(left_son->left_height, left_son->right_height);
            father_to_update->left_son = left_son;
        } else {
            father_to_update->right_height = 1 + std::max(left_son->left_height, left_son->right_height);
            father_to_update->right_son = left_son;
        }
        father_to_update->balance_factor = father_to_update->left_height - father_to_update->right_height;
    }
    left_son->father = father_to_update;
}

template<class Key, class Value>
DictAvl<Key, Value>::~DictAvl() {
    DeleteTree(root);
}

template<class Key, class Value>
void DictAvl<Key, Value>::DeleteTree(const DictAvl::AvlNode *node_to_delete) {
    if (node_to_delete == nullptr)
        return;
    DeleteTree(node_to_delete->right_son);
    DeleteTree(node_to_delete->left_son);
    delete node_to_delete;
    return;
}


template<class Key, class Value>
bool DictAvl<Key, Value>::CheckIfAVL(const DictAvl::AvlNode *root) {
    if (root == nullptr)
        return true;
    int right_height = root->right_height;
    int left_height = root->left_height;
    if (abs(right_height-left_height) > 1 || abs(root->balance_factor) > 1)
        return false;

    if (CheckIfAVL(root->right_son) == false)
        return false;
    return CheckIfAVL(root->left_son); //I splitted the test for the left and right tree in order to potentially save a lot of runtime.
    // I mean, instead of: return CheckIfAVL(root->left_son) && CheckIfAVL(root->right_son)
}

template<class Key, class Value>
bool DictAvl<Key, Value>::NodeInTree(const DictAvl::AvlNode *root, const DictAvl::AvlNode* node_to_look_for) {
    if (root == nullptr || node_to_look_for == nullptr)
        return false;
    if (root->key < node_to_look_for->key) {
        if (root->right_son == node_to_look_for)
            return true;
        return NodeInTree(root->right_son, node_to_look_for);
    } else if (root->key > node_to_look_for->key) {
        if (root->left_son == node_to_look_for)
            return true;
        return NodeInTree(root->left_son, node_to_look_for);
    }
    return false;
}

template<class Key, class Value>
void DictAvl<Key, Value>::PrintInOrder(const DictAvl::AvlNode *root, std::vector<Key>* result) {
    if (root == nullptr)
        return;
    PrintInOrder(root->left_son,result);
    result->push_back(root->key);
    std::cout<<"Key:"<<root->key<<"| BF:"<<root->balance_factor<<"| RH:"<<root->right_height<<"| LH:"<<root->left_height<<std::endl;
    PrintInOrder(root->right_son,result);
}

#endif //WET1_DICT_AVL_H
