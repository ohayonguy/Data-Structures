//
// Created by guyoe on 29-Nov-18.
//

#ifndef WET1_DICT_AVL_H
#define WET1_DICT_AVL_H

#include <iostream>
#include <assert.h>
#include <algorithm>

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
        int right_height;
        int left_height;
        AvlNode(const Key& key, const Value& value, AvlNode* left_son,
                AvlNode* right_son, AvlNode* father, int balance_factor) :
                value(value),
                key(key),
                left_son(nullptr),
                right_son(nullptr),
                father(nullptr),
                balance_factor(0),
                right_height(0),
                left_height(0) {};
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
    void UpdateBalanceFactorAndRoll(AvlNode * current_node, int last_balance_factor);
    static void SwitchNodesValueAndKey(AvlNode* node1, AvlNode* node2);
    static bool CheckIfAVL(const AvlNode* root);
    static int GetTreeHeight(const AvlNode* root);
    static void PrintInOrder(const AvlNode* root);
    static void BSTInsert(AvlNode* root, AvlNode* new_node);
    static AvlNode* Roll(AvlNode* base_node);
};

template<class Key, class Value>
typename DictAvl<Key, Value>::AvlNode *DictAvl<Key, Value>::InsertNode(const Key &key, const Value &value) {
    AvlNode* new_node = new AvlNode(key,value, nullptr, nullptr, nullptr,0);
    if (this->root == nullptr) {
        this->root = new_node;
        return new_node;
    }
    BSTInsert(root,new_node);
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
    }
    return;
}

template<class Key, class Value>
void DictAvl<Key, Value>::UpdateTreeBottomToTop(DictAvl::AvlNode *bottom_node) {
    if (bottom_node == nullptr)
        return;
    if (bottom_node->father == nullptr) {
        root = Roll(bottom_node);
        root->father = nullptr;
        return;
    }
    AvlNode* bottom_node_father = bottom_node->father;
    bottom_node = Roll(bottom_node);
    int bottom_node_height = std::max(bottom_node->right_height,bottom_node->left_height);
    if (bottom_node_father->key < bottom_node->key) {
        bottom_node_father->right_son = bottom_node;
        bottom_node_father->right_height = 1 + bottom_node_height;
    } else {
        bottom_node_father->left_son = bottom_node;
        bottom_node_father->left_height = 1 + bottom_node_height;
    }
    bottom_node_father->balance_factor = bottom_node_father->left_height - bottom_node_father->right_height;
    bottom_node->father = bottom_node_father;
    UpdateTreeBottomToTop(bottom_node_father);
}

template<class Key, class Value>
typename DictAvl<Key,Value>::AvlNode* DictAvl<Key, Value>::Roll(typename DictAvl::AvlNode *base_node) {
    if (base_node == nullptr)
        return nullptr;
    if (base_node->balance_factor == -2) {
        assert(base_node->right_son != nullptr);
        if (base_node->right_son->balance_factor == -1) {
            AvlNode* return_node = base_node->right_son;
            RollLeft(base_node);
            return return_node;
        } else {
            assert(base_node->right_son->balance_factor == 1 && base_node->right_son->left_son != nullptr);
            AvlNode* return_node = base_node->right_son->left_son;
            RollRight(base_node->right_son);
            base_node->right_son = return_node;
            RollLeft(base_node);
            return return_node;
        }
    } else if (base_node->balance_factor == 2) {
        assert(base_node->left_son != nullptr);
        if (base_node->left_son->balance_factor == -1) {
            assert(base_node->left_son->right_son != nullptr);
            AvlNode* return_node = base_node->left_son->right_son;
            RollLeft(base_node->left_son);
            base_node->left_son = return_node;
            RollRight(base_node);
            return return_node;
        } else {
            assert(base_node->left_son->balance_factor == 1);
            AvlNode* return_node = base_node->left_son;
            RollRight(base_node);
            return return_node;
        }
    }
    return base_node;
}

template<class Key, class Value>
void DictAvl<Key, Value>::RollLeft(DictAvl::AvlNode *base_node) {
    assert(base_node != nullptr && base_node->right_son != nullptr);
    AvlNode* right_son = base_node->right_son;
    base_node->father = right_son;
    base_node->right_son = right_son->left_son;
    base_node->right_height = 0;
    if (right_son->left_son != nullptr)
        base_node->right_height = 1 + std::max(right_son->left_son->right_height, right_son->left_son->left_height);
    base_node->balance_factor = base_node->left_height - base_node->right_height;

    right_son->left_son = base_node;
    right_son->left_height = 1 + std::max(right_son->left_son->left_height, right_son->left_son->right_height);
    right_son->balance_factor = right_son->left_height - right_son->right_height;
}
template<class Key, class Value>
void DictAvl<Key, Value>::RollRight(DictAvl::AvlNode *base_node) {
    assert(base_node != nullptr && base_node->left_son != nullptr);
    AvlNode* left_son = base_node->left_son;
    base_node->father = left_son;
    base_node->left_son = left_son->right_son;
    base_node->left_height = 0;
    if (left_son->right_son != nullptr)
        base_node->left_height = 1 + std::max(left_son->right_son->right_height, left_son->right_son->left_height);
    base_node->balance_factor = base_node->left_height - base_node->right_height;

    left_son->right_son = base_node;
    left_son->right_height = 1 + std::max(left_son->right_son->right_height, left_son->right_son->left_height);
    left_son->balance_factor = left_son->left_height - left_son->right_height;
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
    if (abs(right_height-left_height) > 1)
        return false;

    if (CheckIfAVL(root->right_son) == false)
        return false;
    return CheckIfAVL(root->left_son); //I splitted the test for the left and right tree in order to potentially save a lot of runtime.
    // I mean, instead of: return CheckIfAVL(root->left_son) && CheckIfAVL(root->right_son)
}
#endif //WET1_DICT_AVL_H
