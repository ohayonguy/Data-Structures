//
// Created by guyoe on 29-Nov-18.
//

#ifndef WET1_DICT_AVL_H
#define WET1_DICT_AVL_H
//#define NDEBUG
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
        void UpdateNode(AvlNode* father,
                AvlNode* left_son, AvlNode* right_son, int balance_factor, int left_height, int right_height) {
            this->father = father;
            this->left_son = left_son;
            this->right_son = right_son;
            this->balance_factor = balance_factor;
            this->left_height = left_height;
            this->right_height = right_height;
        }
    };
    DictAvl() : root(nullptr), size(0) {};
    ~DictAvl();
    AvlNode* InsertNode(const Key& key, const Value& value);
    Value GetValueByKey(const Key& key);
    void DeleteNodeByKey(const Key& key);
    void DeleteNodeByPtr(AvlNode* node_to_delete);
    int GetSize();
  //  void PrintDict();
    Value* GetAllValuesInOrder();
private:
    AvlNode* root;
    int size;
    void UpdateTreeBottomToTop(AvlNode* bottom_node);
    void RollRight(AvlNode*);
    void RollLeft(AvlNode*);
    static void DeleteTree(const AvlNode* node_to_delete);
    static bool CheckIfAVL(const AvlNode* root);
    static void BSTInsert(AvlNode* root, AvlNode* new_node);
    void Roll(AvlNode* base_node);
    static bool NodeInTree(const AvlNode* root, const Key& node_key_to_look_for);
    static bool NodeInTree(const AvlNode* root, const AvlNode* node_to_look_for);
    static AvlNode* GetNodeToSwitchWith(AvlNode* node_to_delete);
    static AvlNode* GetNodePtrByKey(AvlNode* root, const Key& key);
    static void SwitchNodes(AvlNode* node1, AvlNode* node2);
    int FillAllValuesInOrder(Value* values, int index, AvlNode* current_node);

};

template<class Key, class Value>
typename DictAvl<Key, Value>::AvlNode *DictAvl<Key, Value>::InsertNode(const Key &key, const Value &value) {
    AvlNode* new_node = new AvlNode(key,value);
    size++;
    if (this->root == nullptr) {
        this->root = new_node;
        return new_node;
    }
    BSTInsert(root,new_node);
    assert(NodeInTree(root,new_node));
    UpdateTreeBottomToTop(new_node);

    if (!CheckIfAVL(root)) {
        std::cout << "NOT AVL!" << std::endl;
        assert(false);
    }
    //assert(CheckIfAVL(root)); //add a test if the tree is still AVL;

    return new_node;
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

    bottom_node->left_height = 0;
    if (bottom_node->left_son != nullptr) {
        bottom_node->left_height = 1 + std::max(bottom_node->left_son->left_height, bottom_node->left_son->right_height);
    }
    bottom_node->right_height = 0;
    if (bottom_node->right_son != nullptr) {
        bottom_node->right_height = 1 + std::max(bottom_node->right_son->left_height, bottom_node->right_son->right_height);
    }
    bottom_node->balance_factor = bottom_node->left_height - bottom_node->right_height;
    Roll(bottom_node);
    assert(abs(bottom_node->balance_factor) <= 1);
    assert(abs(bottom_node->right_height-bottom_node->left_height) <= 1);
    //assert(CheckIfAVL(bottom_node));
    UpdateTreeBottomToTop(bottom_node_father);
    assert(abs(bottom_node->balance_factor)<=2);
}

template<class Key, class Value>
void DictAvl<Key, Value>::Roll(typename DictAvl::AvlNode *base_node) {
    if (base_node == nullptr)
        return;
    assert(abs(base_node->balance_factor)<=2);
    if (base_node->balance_factor == -2) {
        assert(base_node->right_son != nullptr);
        if (base_node->right_son->balance_factor <= 0) {
            RollLeft(base_node);
        } else {
            assert(base_node->right_son->balance_factor >= 0 && base_node->right_son->left_son != nullptr);
            RollRight(base_node->right_son);
            RollLeft(base_node);
        }
    } else if (base_node->balance_factor == 2) {
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
    assert(abs(base_node->balance_factor)<=1);
    return;
}

template<class Key, class Value>
void DictAvl<Key, Value>::RollLeft(DictAvl::AvlNode *base_node) {
    assert(base_node != nullptr && base_node->right_son != nullptr);
    AvlNode* father_to_update = base_node->father;
    AvlNode* right_son = base_node->right_son;
    base_node->father = right_son;
    base_node->right_son = right_son->left_son;
    if (right_son->left_son != nullptr)
        right_son->left_son->father = base_node;
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
    } else {
        root = right_son;
    }
    right_son->father = father_to_update;
}

template<class Key, class Value>
void DictAvl<Key, Value>::RollRight(DictAvl::AvlNode *base_node) {
    //assert(base_node != nullptr && base_node->left_son != nullptr);
    AvlNode* father_to_update = base_node->father;
    AvlNode* left_son = base_node->left_son;
    base_node->father = left_son;
    base_node->left_son = left_son->right_son;
    if (left_son->right_son != nullptr)
        left_son->right_son->father = base_node;
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
    } else {
        root = left_son;
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
    if (abs(root->right_height-root->left_height) > 1 || abs(root->balance_factor) > 1) {
        std::cout<<root->key<<std::endl;
        std::cout<<"BF or heights problem"<<std::endl;
        return false;
    }
    if (CheckIfAVL(root->right_son) == false)
        return false;
    return CheckIfAVL(root->left_son); //I splitted the test for the left and right tree in order to potentially
    // save a lot of runtime.
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
Value* DictAvl<Key, Value>::GetAllValuesInOrder() {
    Value* values = new Value[GetSize()];
    FillAllValuesInOrder(values, 0, root);
    return values;
}

template<class Key, class Value>
int DictAvl<Key, Value>::FillAllValuesInOrder(Value* values, int index, AvlNode* current_node) {
    if (current_node == nullptr) {
        return index;
    }
    index = FillAllValuesInOrder(values, index, current_node->left_son);
    values[index++] = current_node->value;
    index = FillAllValuesInOrder(values, index, current_node->right_son);
    return index;
}

template<class Key, class Value>
void DictAvl<Key, Value>::DeleteNodeByPtr(AvlNode* node_to_delete) {
    if (node_to_delete == nullptr)
        return;
    size--;
    AvlNode* node_to_switch_with = GetNodeToSwitchWith(node_to_delete);
    if (node_to_delete == root) {
            root = node_to_switch_with;
    }
    if (node_to_switch_with == nullptr) {
        node_to_switch_with = node_to_delete;
    }
    AvlNode* node_to_update;

    SwitchNodes(node_to_switch_with,node_to_delete);
    assert(node_to_switch_with != nullptr);
    if (node_to_switch_with->right_son == node_to_delete) {
        assert(node_to_switch_with->left_son == nullptr);
        assert(node_to_delete->father != nullptr);
        node_to_switch_with->right_son = node_to_delete->right_son;
        if (node_to_switch_with->right_son != nullptr)
            node_to_switch_with->right_son->father = node_to_switch_with;
        node_to_switch_with->left_son = node_to_delete->left_son;
        if (node_to_switch_with->left_son != nullptr)
            node_to_switch_with->left_son->father = node_to_switch_with;
        node_to_update= node_to_switch_with;

    } else {
        //assert(node_to_switch_with->left_son != nullptr);
        assert(node_to_delete != nullptr);
        if (node_to_delete->father != nullptr) {
            if (node_to_delete->father->right_son == node_to_delete) {
                node_to_delete->father->right_son = node_to_delete->left_son;
            } else{
                node_to_delete->father->left_son = node_to_delete->left_son;
            }
        }
        if (node_to_delete->left_son != nullptr)
            node_to_delete->left_son->father = node_to_delete->father;
        node_to_update = node_to_delete->father;
    }
    delete node_to_delete;
    UpdateTreeBottomToTop(node_to_update);
}

template<class Key, class Value>
typename DictAvl<Key,Value>::AvlNode *DictAvl<Key, Value>::GetNodeToSwitchWith(typename
                                                                                DictAvl::AvlNode* node_to_delete) {
    assert(node_to_delete != nullptr);
    if (node_to_delete->left_son == nullptr) {
        return node_to_delete->right_son;
    }
    AvlNode* max_son = node_to_delete->left_son;
    while (max_son->right_son != nullptr)
        max_son = max_son->right_son;
    return max_son;
}

template<class Key, class Value>
void DictAvl<Key, Value>::DeleteNodeByKey(const Key &key) {
    assert(NodeInTree(root,key));
    DeleteNodeByPtr(GetNodePtrByKey(root,key));
    assert(!NodeInTree(root,key));
    assert(CheckIfAVL(root));
}

template<class Key, class Value>
typename DictAvl<Key,Value>::AvlNode *DictAvl<Key, Value>::GetNodePtrByKey(AvlNode* root, const Key& key) {
    if (root == nullptr)
        throw KeyNotFound();
    if (root->key < key)
        return GetNodePtrByKey(root->right_son, key);
    else if (root->key > key)
        return GetNodePtrByKey(root->left_son, key);
    else
        return root;
}

template<class Key, class Value>
int DictAvl<Key, Value>::GetSize() {
    return size;
}
/*
template<class Key, class Value>
void DictAvl<Key, Value>::PrintDict() {
    std::vector<Value> all_values;
    GetAllValuesInOrder(root,all_values);
    for (int i = 0; i < all_values.size(); i++) {
        std::cout<<all_values[i]<<std::endl;
    }
}*/

template<class Key, class Value>
Value DictAvl<Key, Value>::GetValueByKey(const Key &key) {
    return GetNodePtrByKey(root,key)->value;
}

template<class Key, class Value>
void DictAvl<Key,Value>::SwitchNodes(AvlNode* node1, AvlNode* node2) {
    if (node1 == nullptr || node2 == nullptr)
        return;
    if (node1 == node2)
        return;
    AvlNode node1_copy(*node1);
    AvlNode node2_copy(*node2);
    if (node2_copy.father == node1) {
        if (node1_copy.left_son == node2)
            node2->UpdateNode(node1_copy.father,node1,node1_copy.right_son,
                              node1_copy.balance_factor,node1_copy.left_height,node1_copy.right_height);
        else
            node2->UpdateNode(node1_copy.father,node1_copy.left_son,node1,
                              node1_copy.balance_factor,node1_copy.left_height,node1_copy.right_height);
        node1->UpdateNode(node2,node2_copy.left_son,node2_copy.right_son,
                          node2_copy.balance_factor,node2_copy.left_height,node2_copy.right_height);
    } else if (node1_copy.father == node2) {
        if (node2_copy.left_son == node1)
            node1->UpdateNode(node2_copy.father,node2,node2_copy.right_son,
                              node2_copy.balance_factor,node2_copy.left_height,node2_copy.right_height);
        else
            node1->UpdateNode(node2_copy.father,node2_copy.left_son,node2,
                              node2_copy.balance_factor,node2_copy.left_height,node2_copy.right_height);
        node2->UpdateNode(node1,node1_copy.left_son,node1_copy.right_son,
                          node1_copy.balance_factor,node1_copy.left_height,node1_copy.right_height);
    } else {
        node2->UpdateNode(node1_copy.father,node1_copy.left_son,node1_copy.right_son,
                          node1_copy.balance_factor,node1_copy.left_height,node1_copy.right_height);
        node1->UpdateNode(node2_copy.father,node2_copy.left_son,node2_copy.right_son,
                          node2_copy.balance_factor,node2_copy.left_height,node2_copy.right_height);
    }
    if (node2->left_son != nullptr && node2->left_son != node1)
        node2->left_son->father = node2;
    if (node2->right_son != nullptr && node2->right_son != node1)
        node2->right_son->father = node2;
    if (node2->father != nullptr && node2->father != node1) {
        if (node2->father->left_son == node1)
            node2->father->left_son = node2;
        else
            node2->father->right_son = node2;
    }
    if (node1->left_son != nullptr && node1->left_son != node2)
        node1->left_son->father = node1;
    if (node1->right_son != nullptr && node1->right_son != node2)
        node1->right_son->father = node1;
    if (node1->father != nullptr && node1->father != node2) {
        if (node1->father->left_son == node2)
            node1->father->left_son = node1;
        else
            node1->father->right_son = node1;
    }
}

template<class Key, class Value>
bool DictAvl<Key, Value>::NodeInTree(const DictAvl::AvlNode *root, const Key &node_key_to_look_for) {
    if (root == nullptr)
        return false;
    else if (root->key == node_key_to_look_for)
        return true;
    else if (root->key < node_key_to_look_for)
        return NodeInTree(root->right_son, node_key_to_look_for);
    else
        return NodeInTree(root->left_son, node_key_to_look_for);
}

#endif //WET1_DICT_AVL_H