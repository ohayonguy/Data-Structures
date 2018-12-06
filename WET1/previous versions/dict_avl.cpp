template<class Key, class Value>
void DictAvl<Key, Value>::DeleteTree(const DictAvl::AvlNode *node_to_delete) {
    if (node_to_delete == nullptr)
        return;
    DeleteTree(node_to_delete->right_son);
    DeleteTree(node_to_delete->left_son);
    delete node_to_delete;
    return;
}

template <class Key, class Value>
void DictAvl<Key, Value>::InsertNode(AvlNode *current_node, AvlNode* node_to_insert) {
    if (root == nullptr) {
        root = node_to_insert;
        return;
    }
    if (current_node == nullptr || node_to_insert == nullptr) {
        return;// This means that we arrived to the point of insertion.
        /** node_to_insert->father = father_of_node_to_insert;
         if (father_of_node_to_insert != nullptr) {
             if (father_of_node_to_insert->key <= node_to_insert->key) {// true means it's the right son. It must be null at this point.
                 assert(father_of_node_to_insert->right_son == nullptr);
                 father_of_node_to_insert->right_son = node_to_insert;
                 if (father_of_node_to_insert->left_son == nullptr)
                     father_of_node_to_insert->balance_factor--;
             } else {// otherwise it's the left son
                 assert(father_of_node_to_insert->left_son == nullptr);
                 father_of_node_to_insert->left_son = node_to_insert;
                 if (father_of_node_to_insert->right_son == nullptr)
                     father_of_node_to_insert->balance_factor++;
             }
         } else { //This means it's the root (it's the first node in the tree)
             root = node_to_insert;
         }
         size++;
         return;*/
    }
    if (node_to_insert->key > current_node->key)
        if(current_node->right_son != nullptr)
            InsertNode(current_node->right_son,node_to_insert);
        else {
            current_node->right_son = node_to_insert;
            node_to_insert->father = current_node;
            int last_balance_factor = current_node->balance_factor;
            current_node->balance_factor--;
            current_node->right_height = 1;
            size++;
            BalanceTreeUpTheHill(current_node,nullptr,last_balance_factor);
            return;
        }
    else if (node_to_insert->key < current_node->key){
        if (current_node->left_son != nullptr)
            InsertNode(current_node->left_son,node_to_insert);
        else {
            current_node->left_son = node_to_insert;
            node_to_insert->father = current_node;
            int last_balance_factor = current_node->balance_factor;
            current_node->balance_factor++;
            current_node->left_height = 1;
            size++;
            BalanceTreeUpTheHill(current_node, nullptr,last_balance_factor);
            return;
        }
    } else { //this means the key already exists. Do nothing!
        return;
    }
}

template <class Key, class Value>
void DictAvl<Key,Value>::UpdateBalanceFactorAndRoll(DictAvl<Key, Value>::AvlNode * current_node,
                                                    int last_balance_factor) {
    int new_balance_factor = current_node->balance_factor;
    if (new_balance_factor == last_balance_factor) //if the balance factor didn't change, there's nothing to do.
        return;
    else if (new_balance_factor == -2) {// this means we need to roll either RR or RL
        assert(abs(current_node->right_son->balance_factor) == 1); //either -1 or 1
        if (current_node->right_son->balance_factor == -1) //this means we roll RR
            RollLeft(current_node);
        else {// this means we roll RL
            RollRight(current_node->right_son);
            RollLeft(current_node);
        }
    } else if (new_balance_factor == 2) {
        assert(abs(current_node->left_son->balance_factor) == 1); //either -1 or 1
        if (current_node->left_son->balance_factor == -1) { //this means we roll LR
            RollLeft(current_node->left_son);
            RollRight(current_node);
        } else// this means we roll LL
            RollRight(current_node);
    } else if (current_node->father != nullptr && current_node->balance_factor != 0) {
        assert(current_node->father->right_son == current_node || current_node->father->left_son == current_node);
        assert(abs(new_balance_factor-last_balance_factor) == 1); // two changes in the balance factor is not okay here.
        if (current_node->father->right_son == current_node) { // this means that we updated the right side of the tree.
            current_node->father->balance_factor--;
            current_node->father->right_height++;
        } else {
            current_node->father->balance_factor++;
            current_node->father->left_height++;
        }
        //assert(abs(current_node->father->left_height-current_node->father->right_height) <= 1); //just to check it's still AVL.
    }
    return;
}
template<class Key, class Value>
void DictAvl<Key, Value>::RollLeft(DictAvl::AvlNode *current_node) {
    if (current_node == nullptr)
        return;
    if (current_node->right_son == nullptr)
        return;
    if (current_node->father == nullptr)
        root = current_node->right_son;
    AvlNode* right_son = current_node->right_son;
    current_node->right_son = right_son->left_son;

    current_node->right_height = 0;
    if (current_node->right_son != nullptr)
        current_node->right_height = 1 + std::max(current_node->right_son->right_height, current_node->right_son->left_height);
    current_node->balance_factor = current_node->left_height - current_node->right_height;
    current_node->father = right_son;

    right_son->left_son = current_node;
    right_son->left_height = 1 + std::max(right_son->left_son->right_height, right_son->left_son->left_height);
    right_son->father = current_node->father;
    right_son->balance_factor = right_son->left_height - right_son->right_height;
}
/*template<class Key, class Value>
void DictAvl<Key, Value>::RollLeft(DictAvl::AvlNode *current_node) {
    std::cout<<"RollLeft"<<std::endl;
    std::cout<<"FirstPrint:"<<std::endl;
    PrintInOrder(current_node);
    if (current_node == nullptr || current_node->right_son == nullptr)
        return;
    AvlNode* right_son = current_node->right_son;
    AvlNode* current_node_right_son_left_son = right_son->left_son;

    if (current_node->father != nullptr) {
        if (current_node->father->right_son == current_node)
            current_node->father->right_son = right_son;
        else
            current_node->father->left_son = right_son;
    }
    right_son->father = current_node->father;
    if (right_son->father == nullptr)
        root = right_son;

    right_son->left_son = current_node;
    current_node->father = right_son;
    current_node->right_son = current_node_right_son_left_son;
    current_node->balance_factor = 0;
    right_son->balance_factor ++;
    if (current_node_right_son_left_son != nullptr)
        current_node->right_height = 1 + std::max(current_node_right_son_left_son->right_height,
                current_node_right_son_left_son->left_height);
    else
        current_node->right_height = 0;
    right_son->left_height = 1 + std::max(current_node->right_height, current_node->left_height);
    std::cout<<"SecondPrint:"<<std::endl;
    PrintInOrder(right_son);
}
*/
template <class Key, class Value>
void DictAvl<Key, Value>::RollRight(DictAvl::AvlNode *current_node) {
    std::cout<<std::endl;
    std::cout<<"RollRight"<<std::endl;
    std::cout<<"FirstPrint:"<<std::endl;
    PrintInOrder(current_node);
    if (current_node == nullptr)
        return;
    if (current_node->left_son == nullptr)
        return;
    if (current_node->father == nullptr)
        root = current_node->left_son;
    AvlNode* left_son = current_node->left_son;
    current_node->left_son = left_son->right_son;

    current_node->left_height = 0;
    if (current_node->left_son != nullptr)
        current_node->left_height = 1 + std::max(current_node->left_son->right_height, current_node->left_son->left_height);
    current_node->balance_factor = current_node->left_height - current_node->right_height;
    current_node->father = left_son;

    left_son->right_son = current_node;
    left_son->right_height = 1 + std::max(left_son->right_son->right_height, left_son->right_son->left_height);
    left_son->father = current_node->father;
    left_son->balance_factor = left_son->left_height - left_son->right_height;
    std::cout<<"SecondPrint:"<<std::endl;
    PrintInOrder(left_son);
    std::cout<<std::endl;
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
        assert(next_son->right_son == nullptr);

        //switch between the current node and the biggest node of the left sub tree of the current node
        SwitchNodesValueAndKey(next_son, current_node);
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
    AvlNode *climbing_node = bottom_node;
    while (climbing_node->father != top_node && climbing_node->father != nullptr) {
        int last_balance_factor_next_father = climbing_node->father->balance_factor;
        UpdateBalanceFactorAndRoll(climbing_node, last_balance_factor);
        last_balance_factor = last_balance_factor_next_father;
        climbing_node = climbing_node->father;
    }
    UpdateBalanceFactorAndRoll(climbing_node, last_balance_factor);
    return;
}

template<class Key, class Value>
DictAvl<Key, Value>::~DictAvl() {
    DeleteTree(root);
}

template<class Key, class Value>
void DictAvl<Key, Value>::SwitchNodesValueAndKey(DictAvl::AvlNode *node1, DictAvl::AvlNode *node2) {
    assert(node1 != nullptr && node2 != nullptr);
    Key temp_key = node1->key;
    Value temp_value = node1->value;
    node1->key = node2->key;
    node1->value = node2->value;
    node2->key = temp_key;
    node2->value = temp_value;
}

template<class Key, class Value>
typename DictAvl<Key,Value>::AvlNode *DictAvl<Key, Value>::InsertNode(const Key &key, const Value &value) {
    AvlNode* new_node = new AvlNode(key,value, nullptr, nullptr, nullptr,0);
    InsertNode(root,new_node);
    assert(true); //add a check that the node exists inside.
    assert(CheckIfAVL(root)); //add a test if the tree is still AVL;
    return new_node;
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

template<class Key, class Value>
int DictAvl<Key, Value>::GetTreeHeight(const DictAvl::AvlNode *root) {
    if (root == nullptr)
        return -1;
    return std::max(1 + GetTreeHeight(root->right_son),1 + GetTreeHeight(root->left_son));
}

template<class Key, class Value>
void DictAvl<Key, Value>::PrintInOrder(const DictAvl::AvlNode *root) {
    if (root == nullptr)
        return;
    PrintInOrder(root->left_son);
    std::cout<<root->key<<std::endl;
    PrintInOrder(root->right_son);
}

template<class Key, class Value>
void DictAvl<Key, Value>::PrintInOrder(const DictAvl *avl) {
    PrintInOrder(avl->root);
};