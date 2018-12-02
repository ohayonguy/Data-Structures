//
// Created by guyoe on 30-Nov-18.
//
#include <stdlib.h>
#include "dict_avl.h"



template<class Key, class Value>
void DictAvl<Key, Value>::DeleteTree(const DictAvl::AvlNode *node_to_delete) {
    if (node_to_delete == nullptr)
        return;
    DeleteAvlTreeRecursively(node_to_delete->right_son);
    DeleteAvlTreeRecursively(node_to_delete->left_son);
    delete node_to_delete;
    return;
}



template <class Key, class Value>
void DictAvl<Key, Value>::InsertNodeRecursively(const DictAvl::AvlNode *current_node, bool which_son,
        AvlNode* father_of_node_to_insert) {
    if (current_node == nullptr) { // This means that we arrived to the point of insertion.
        node_to_insert->father = father_of_node_to_insert;
        if (father_of_node_to_insert != nullptr) {
            if (which_son == true) {// true means it's the right son
                father_of_node_to_insert->right_son = node_to_insert;
                if (father_of_node_to_insert->left_son == nullptr)
                    father_of_node_to_insert->balance_factor--;
            } else {// otherwise it's the left son
                father_of_node_to_insert->left_son = node_to_insert;
                if (father_of_node_to_insert->right_son == nullptr)
                    father_of_node_to_insert->balance_factor++;
            }
        } else { //This means it's the root (it's the first node in the tree)
            root = node_to_insert;
        }
        return;
    }
    int last_balance_factor;
    int new_balance_factor;
    last_balance_factor = current_node->balance_factor;
    if (node_to_insert.key >= current_node->key)
        InsertNodeRecursively(current_node->right_son,true,current_node);
    else {
        InsertNodeRecursively(current_node->left_son,false,current_node);
    }
    new_balance_factor = current_node->balance_factor;
    /*
     * which_son is true if the current node is the right son of the previous node. false is it's the left son.
     */
    UpdateBalanceFactorAndRoll(current_node,which_son,last_balance_factor);
}

template <class Key, class Value>
void DictAvl<Key,Value>::UpdateBalanceFactorAndRoll(DictAvl<Key, Value>::AvlNode * current_node,
                                            bool which_son,
                                            int last_balance_factor) {
    int new_balance_factor = current_node->balance_factor;
    if (new_balance_factor == last_balance_factor) //if the balance factor didn't change, there's nothing to do.
        return;
    else if (new_balance_factor == -2) {// this means we need to roll either RR or RL
        if (current_node->right_son->balance_factor <= 0) { //this means we roll RR
            RollLeft(current_node);
        } else if (current_node->right_son->balance_factor == 1) {// this means we roll RL
            RollRight(current_node->right_son);
            RollLeft(current_node);
        }
    } else if (new_balance_factor == 2) {
        if (current_node->right_son->balance_factor <= 0) { //this means we roll RR
            RollLeft(current_node->right_son);
            RollRight(current_node);
        } else if (current_node->right_son->balance_factor == 1) {// this means we roll RL
            RollRight(current_node);
        }
    } else if (current_node->father != nullptr && new_balance_factor != 0) {
        if (which_son == true) // this means that we updated the right side of the tree.
            current_node->father->balance_factor -= 1;
        else
            current_node->father->balance_factor += 1;
    }
    return;
}

template<class Key, class Value>
void DictAvl<Key, Value>::RollLeft(DictAvl::AvlNode *current_node) {
    AvlNode* right_son = current_node->right_son;
    AvlNode* current_node_right_son_left_son = right_son->left_son;
    right_son->father = current_node->father;
    current_node->father = right_son;
    current_node->right_son = current_node_right_son_left_son;
    right_son->left_son = current_node;
    current_node->balance_factor = 0;
    right_son->balance_factor += 1;
}

template <class Key, class Value>
void DictAvl<Key, Value>::RollRight(DictAvl::AvlNode *current_node) {
    AvlNode* left_son = current_node->left_son;
    AvlNode* current_node_left_son_right_son = left_son->left_son;
    left_son->father = current_node->father;
    current_node->father = left_son;
    current_node->left_son = current_node_left_son_right_son;
    left_son->right_son = current_node;
    current_node->balance_factor = 0;
    left_son->balance_factor -= 1;
}

template<class Key, class Value>
typename DictAvl<Key, Value>::AvlNode *DictAvl<Key, Value>::InsertNode(Key key, Value value) {
    node_to_insert = new AvlNode(key,value,nullptr,nullptr,nullptr,0);
    InsertNodeRecursively(root,true,nullptr);
    assert(true); // add assert to find the inserted node
    AvlNode* temp = node_to_insert;
    node_to_insert = nullptr;
    size++;
    return temp;
}

template<class Key, class Value>
void DictAvl<Key, Value>::DeleteNodeByKey(const Key& key, AvlNode* current_node) {
    if (current_node == nullptr)
        return;
    if (current_node->father == nullptr) { //the node doesn't have a father, which means the tree contains only one node.
        delete current_node;
        root = nullptr;
        size = 0;
        return;
    }

    // keep the last balance factor of the current_node because it's changed by UpdateBalanceFactorAndRoll.
    int last_balance_factor = current_node->balance_factor;

    if (current_node->key < key)
        DeleteNodeByKey(key, current_node->right_son);
    else if (current_node->key > key)
        DeleteNodeByKey(key, current_node->left_son);

    /*
     * This is the main recursion breakpoint.
     */
    else { //if we found the node.
        AvlNode *next_son = current_node->left_son;
        while (next_son->right_son != nullptr) { //find the biggest node in the left tree of the current node.
            next_son = next_son->right_son;
        }

        //switch between the current node and the biggest node of the left sub tree of the current node
        SwitchNodesValueAndKey(&next_son, &current_node);
        /* before:
         *                  current_node
         *                /               \
         *            node                 node
         *                \               /    \
         *                 next_son    ...      ...
         * after:
         *                    next_son
         *                 /            \
         *            node                 node
         *           /     \               /    \
         *        ...      current_node  ...     ...
         *
         */

        /*
         * check if the biggest node of the left sub tree of the current node is the left son of the current node.
         * This is a special case in which we just need to delete the left son of the current node, and then update
         * the balance factor accordingly. That's how it looks like:
         *
         * before:
         *                  current_node
         *                /               \
         *            next_son             node
         *                                /    \
         *                             ...      ...
         * after:
         *                     next_son
         *                /               \
         *            current_node         node
         *                                /    \
         *                             ...      ...
         *
         */
        if (current_node->left_son == next_son) {
            current_node->left_son = next_son->left_son;
            current_node->balance_factor--;
        }
        /*
         * th
         */
        else {

            /*
             * This update looks like that:
             * before:
             *                  current_node
             *                /               \
             *              node1             ...
             *            /      \
             *          ...      node2
             *                     \
             *                      next_son
             *                      /
             *                  node3
             * after:
             *                  current_node
             *                /               \
             *              node1             ...
             *            /      \
             *          ...      node2
             *                     \
             *                      node3
             * we took out next_son
             */
            AvlNode *next_son_father = next_son->father;
            next_son_father->right_son = next_son->left_son;

            int last_balance_factor_inside = next_son_father->balance_factor;
            next_son_father->balance_factor++;
            // balance the sub tree of the deleted node after it was replaced
            BalanceTreeUpTheHill(next_son_father, current_node, last_balance_factor_inside);
        }
        delete next_son;
        size--;
        return;
    }

    //if nothing changed we can save a lot of runtime because this condition is checked recursively.
    if (last_balance_factor == current_node->balance_factor)
        return;

    // balance the whole tree from current_node (the updated node) and up.
    BalanceTreeUpTheHill(current_node,nullptr,last_balance_factor);
    return;
}

template<class Key, class Value>
void DictAvl<Key, Value>::BalanceTreeUpTheHill(AvlNode* bottom_node, AvlNode* top_node,int last_balance_factor) {
    AvlNode *going_up_to_the_root = bottom_node;
    while (going_up_to_the_root->father != top_node && going_up_to_the_root->father != nullptr) {
        int last_balance_factor_next_father = going_up_to_the_root->father->balance_factor;
        if (going_up_to_the_root->father->left_son == going_up_to_the_root)
            UpdateBalanceFactorAndRoll(going_up_to_the_root, false, last_balance_factor);
        else
            UpdateBalanceFactorAndRoll(going_up_to_the_root, true, last_balance_factor);
        last_balance_factor = last_balance_factor_next_father;
        going_up_to_the_root = going_up_to_the_root->father;
    }
    UpdateBalanceFactorAndRoll(going_up_to_the_root, false, last_balance_factor);
    return;
}

template<class Key, class Value>
DictAvl<Key, Value>::~DictAvl() {

}





/**
    AvlNode *node_to_delete_father = current_node->father;
    AvlNode *updated_node = node_to_delete_father;
    AvlNode *node_to_delete = current_node;
    if (node_to_delete->left_son == nullptr) {
        if (node_to_delete_father->left_son == node_to_delete) {//the node_to_delete is to the left of it's father.
            node_to_delete_father->left_son = node_to_delete->right_son;
            node_to_delete_father->balance_factor--;
        } else {//the node_to_delete is to the right of it's father.
            node_to_delete_father->right_son = node_to_delete->right_son;
            node_to_delete_father->balance_factor++;
        }
        size--;
        delete node_to_delete;
    } else {
        AvlNode *max_node_left_tree_of_node_to_delete = GetMaxNode(node_to_delete->left_son);
        SwitchNodes(&max_node_left_tree_of_node_to_delete,
                    &node_to_delete); //replace node_to_delete with the max in it's left sub tree.
        // now node_to_delete is located in the location of the max node that was in it's left sub tree before.
        if (node_to_delete->father->left_son == node_to_delete) { // node_to_delete is the son of the updated node.
            node_to_delete->father->left_son = node_to_delete->left_son;
            node_to_delete->father->balance_factor--;
        } else { // node_to_delete is not the son of the updated node.
            node_to_delete->father->right_son = node_to_delete->left_son; //node_to_delete can't have a right son because it's location is the farther to the right.
            node_to_delete->father->balance_factor++;
        }
        size--;
        delete node_to_delete;
    }
}
while(stabilize_tree_father != nullptr) {
    if (stabilize_tree_father->father != nullptr) {
        if (stabilize_tree_father->father->right_son == stabilize_tree_father)
            UpdateBalanceFactorAndRoll(stabilize_tree_father, true, last_balance_factor);
    } else
        UpdateBalanceFactorAndRoll(stabilize_tree_father,false,last_balance_factor);
    stabilize_tree_father = stabilize_tree_father->father;
}

}

template<class Key, class Value>
void DictAvl<Key, Value>::DeleteNodeByKeyRecursively(const Key& key, const AvlNode* current_node) {
if (node_to_delete == nullptr)
    return;
if (node_to_delete->father == nullptr) { //this means the tree contains only node_to_delete. So after deletion we will get a clean tree
    delete node_to_delete;
    root = nullptr;
    return;
}
if (current_node == nullptr)
    return;
int last_balance_factor = current_node->balance_factor;
if (current_node->key == node_to_delete->key) {
    AvlNode *max_left_son_of_node_to_delete = FindMaxNodeInTree(current_node->left_son);
    if (max_left_son_of_node_to_delete == nullptr) {
        if (node_to_delete->father->left_son == node_to_delete) {
            node_to_delete->father->left_son = node_to_delete->right_son;
            node_to_delete->father->balance_factor--;
        } else {
            node_to_delete->father->right_son = node_to_delete->right_son;
            node_to_delete->father->balance_factor++;
        }
    } else {
        SwitchNodes(max_left_son_of_node_to_delete,node_to_delete);
        updated_node = node_to_delete;
        node_to_delete = max_left_son_of_node_to_delete;
        if (node_to_delete->father == max_left_son_of_node_to_delete) {
            delete node_to_delete;
        } else {
            node_to_delete->father->right_son = node_to_delete->left_son;
            node_to_delete->father->balance_factor++;
            delete node_to_delete;
        }

    }
} else if (current_node->key > node_to_delete->key) {

}
Key key = node_to_delete->key;


    AvlNode* node_to_delete_replacement_son = node_to_delete->left_son;
    if (node_to_delete_replacement_son == nullptr) {//node_to_delete doesn't have a left son.

    }
    AvlNode* node_to_delete_replacement_father = node_to_delete->father;
    AvlNode* node_to_delete_replacement = node_to_delete;
    while (node_to_delete_replacement_son != nullptr) {
        node_to_delete_replacement = node_to_delete_replacement_son;
        node_to_delete_replacement_father = node_to_delete_replacement->father;
        node_to_delete_replacement_son = node_to_delete_replacement->right_son;
    }
    node_to_delete->key = node_to_delete_replacement->key;
    node_to_delete->value = node_to_delete_replacement->value;


    AvlNode* node_to_delete_replacement_copy = new AvlNode(*node_to_delete_replacement);
    node_to_delete_replacement_copy->balance_factor = node_to_delete->balance_factor;
    node_to_delete_replacement_copy->left_son = node_to_delete->left_son;
    node_to_delete_replacement_copy->right_son = node_to_delete->right_son;
    node_to_delete_replacement_copy->father = node_to_delete->father;
    if (node_to_delete->father->left_son == node_to_delete) { //node to delete is the left son of it's father
        node_to_delete->father->left_son = node_to_delete_replacement_copy;
    } else { //node to delete is the right son of it's father
        node_to_delete->father->right_son = node_to_delete_replacement_copy;
    }
    *node_to_delete =
    if (node_to_delete_replacement->left_son != nullptr) {
        node_to_delete_replacement->left_son->father = node_to_delete_replacement_father;
        if (node_to_delete_replacement_father != nullptr) {
            node_to_delete_replacement_father->right_son = node_to_delete_replacement->left_son;
            node_to_delete_replacement_father->balance_factor++;
        }
    }
    AvlNode* temp = node_to_delete;
    *node_to_delete = *node_to_delete_replacement;
    node_to_delete_replacement->balance_factor = temp->balance_factor;
    node_to_delete_replacement->
    node_to_delete->father->left_son == nullptr;
    node_to_delete->father->balance_factor --;
    delete node_to_delete;
    if (node_to_delete == node_to_delete->father->left_son) {

    }
}

}

template<class Key, class Value>
void DictAvl::DeleteNodeByPtr(const AvlNode* node_to_delete) {
if (node_to_delete == nullptr)
    return;
AvlNode* node_to_delete_father = node_to_delete->father;
DeleteNodeByPtrRecursively(node_to_delete,true)



if (node_to_delete_father)
AvlNode* temp_previous = node_to_delete->previous;
delete node_to_delete;
if (temp_next != nullptr)
    temp_next->previous = temp_previous;
if (temp_previous != nullptr)
    temp_previous->next = temp_next;
else // If we got here it means that node_to_delete was the head, so we need to update the head ptr.
    head = temp_next;
}
**/