//
// Created by guyoe on 29-Nov-18.
//

#ifndef WET1_DICT_LIST_H
#define WET1_DICT_LIST_H
#include <iostream>
#include <vector>
template <class Key, class Value>
class DictList {
public:
    class KeyNotFound : public std::exception {};
    struct ListNode {
        Value value;
        Key key;
        ListNode* next;
        ListNode* previous;
        ListNode(const Key& key, const Value& value, ListNode* next,
                 ListNode* previous) :
                value(value),
                key(key),
                next(next),
                previous(previous){};
    };
    DictList() : head(nullptr), size(0) {};
    ~DictList() {
        ListNode* current_node = head;
        while (current_node != nullptr) {
            ListNode* temp_current_node = current_node->next;
            delete current_node;
            current_node = temp_current_node;
        }
    }
    ListNode* InsertNode(const Key &key, const Value &value) {
        ListNode* new_node = new ListNode(key, value, head, nullptr);
        if (head != nullptr)
            head->previous = new_node;
        head = new_node;
        size++;
        return new_node;
    }
    Value GetValueByKey(const Key& key) {
        return GetNodeByKey(key)->value;
    }
    void DeleteNodeByKey(const Key &key) {
        ListNode* node_to_delete = GetNodeByKey(key);
        this->DeleteNodeByPtr(node_to_delete);
    }
    void DeleteNodeByPtr(const ListNode* node_to_delete) {
        if (node_to_delete == nullptr)
            return;
        ListNode* temp_next = node_to_delete->next;
        ListNode* temp_previous = node_to_delete->previous;
        delete node_to_delete;
        if (temp_next != nullptr)
            temp_next->previous = temp_previous;
        if (temp_previous != nullptr)
            temp_previous->next = temp_next;
        else // If we got here it means that node_to_delete was the head, so we need to update the head ptr.
            head = temp_next;
        size--;
    }
    int GetSize() {
        return size;
    }
    void PrintDict() {
        ListNode* current = head;
        while (current != nullptr) {
            std::cout << *((int*)(current->value)) << std::endl;
            current = current->next;
        }
    }
    static void GetAllValuesInOrder(const ListNode* head,const std::vector<Value>* result);
private:
    /**
     * Find a node by a given key and return a pointer to that node.
     * @param key is the key to look for
     * @return list node pointer which contains the given key.
     */
    ListNode* GetNodeByKey(const Key& key) {
        ListNode* current_node = head;
        while(current_node != nullptr) {
            if(current_node->key == key) {
                return current_node;
            }
            current_node = current_node->next;
        }
        throw KeyNotFound();
    }
    ListNode* head;
    int size;
};

template<class Key, class Value>
void DictList<Key, Value>::GetAllValuesInOrder(const DictList::ListNode *head,const std::vector<Value> *result) {
    ListNode* next_node = head;
    while (next_node != nullptr) {
        result->push_back(next_node->value);
        next_node = next_node->next;
    }
}

#endif //WET1_DICT_LIST_H
