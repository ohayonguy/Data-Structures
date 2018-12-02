//
// Created by guyoe on 29-Nov-18.
//

#ifndef WET1_DICT_AVL_H
#define WET1_DICT_AVL_H

#include <iostream>
#include <assert.h>
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
        AvlNode(const Key& key, const Value& value, AvlNode* left_son,
                AvlNode* right_son, AvlNode* father, int balance_factor) :
                value(value),
                key(key),
                left_son(left_son),
                right_son(right_son),
                father(father),
                balance_factor(balance_factor){};
    };
    DictAvl() : root(nullptr), size(0) {
        node_to_insert = nullptr;
    };
    ~DictAvl() {
        this->DeleteTree(root);
    };
    AvlNode* InsertNode(Key key, Value value) {
        node_to_insert = new AvlNode(key,value,nullptr,nullptr,nullptr,0);
        InsertNodeRecursively(root,true,nullptr);
        assert(true); // add assert to find the inserted node
        AvlNode* temp = node_to_insert;
        node_to_insert = nullptr;
        size++;
        return temp;
    }

    //Value GetValueByKey(const Key& key);
    //void DeleteNodeByKey(const Key &key);
    //void DeleteNodeByPtr(const DictAvl* node_to_delete);
    void DeleteNodeByKey(const Key& key, AvlNode* current_node) {
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
    //int GetSize();
    //void PrintDict();
private:
    /**
     * Find a node by a given key and return a pointer to that node.
     * @param key is the key to look for
     * @return list node pointer which contains the given key.
     */
    ///DictAvl* GetNodeByKey(const Key& key);
    AvlNode* root;
    int size;
    void InsertNodeRecursively(DictAvl::AvlNode *current_node, bool which_son, AvlNode* father_of_node_to_insert){
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
        if (node_to_insert->key >= current_node->key)
            InsertNodeRecursively(current_node->right_son,true,current_node);
        else {
            InsertNodeRecursively(current_node->left_son,false,current_node);
        }
        new_balance_factor = current_node->balance_factor;
        /*
         * which_son is true if the current node is the right son of the previous node. false is it's the left son.
         */
        UpdateBalanceFactorAndRoll(current_node,which_son,last_balance_factor);
    };
    void BalanceTreeUpTheHill(AvlNode* bottom_node, AvlNode* top_node,int last_balance_factor) {
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
    AvlNode* node_to_insert;
    static void RollRight(AvlNode* current_node) {
        AvlNode* left_son = current_node->left_son;
        AvlNode* current_node_left_son_right_son = left_son->left_son;
        left_son->father = current_node->father;
        current_node->father = left_son;
        current_node->left_son = current_node_left_son_right_son;
        left_son->right_son = current_node;
        current_node->balance_factor = 0;
        left_son->balance_factor -= 1;
    }
    static void RollLeft(AvlNode* current_node) {
        AvlNode* right_son = current_node->right_son;
        AvlNode* current_node_right_son_left_son = right_son->left_son;
        right_son->father = current_node->father;
        current_node->father = right_son;
        current_node->right_son = current_node_right_son_left_son;
        right_son->left_son = current_node;
        current_node->balance_factor = 0;
        right_son->balance_factor += 1;
    }
    static void DeleteTree(const AvlNode* node_to_delete) {
        if (node_to_delete == nullptr)
            return;
        DeleteTree(node_to_delete->right_son);
        DeleteTree(node_to_delete->left_son);
        delete node_to_delete;
        return;
    }
    static void UpdateBalanceFactorAndRoll(AvlNode * current_node,
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
};

#endif //WET1_DICT_AVL_H
