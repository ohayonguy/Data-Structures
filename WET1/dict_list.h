//
// Created by guyoe on 29-Nov-18.
//
#include <iostream>
#ifndef WET1_DICT_LIST_H
#define WET1_DICT_LIST_H

template <class Key, class Value>
class DictList {
private:
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
    ListNode* head;
    unsigned int size;
    class KeyNotFound : public std::exception {};
public:
    DictList() : head(nullptr), size(0) {};
    ~DictList() {
        ListNode* current_node = head;
        while (current_node != nullptr) {
            ListNode* temp_current_node = current_node->next;
            delete current_node;
            current_node = temp_current_node;
        }
    }
    void Insert(const Key& key, const Value& value) {
        ListNode* new_node = new ListNode(key, value, head, nullptr);
        if (head != nullptr)
            head->previous = new_node;
        head = new_node;
        size++;
    }
    Value Find(const Key& key) {
        return FindNodeByKey(key)->value;
    }
    void Delete(const Key& key) {
        ListNode* node_to_delete = FindNodeByKey(key);
        ListNode* temp_next = node_to_delete->next;
        ListNode* temp_previous = node_to_delete->previous;
        delete node_to_delete;
        if (temp_next != nullptr)
            temp_next->previous = temp_previous;
        if (temp_previous != nullptr)
            temp_previous->next = temp_next;
        else // If we got here it means that node_to_delete was the head, so we need to update the head ptr.
            head = temp_next;
    }
private:
    /**
     * Find a node by a given key and return a pointer to that node.
     * @param key is the key to look for
     * @return list node pointer which contains the given key.
     */
    ListNode* FindNodeByKey(const Key& key) {
        ListNode* current_node = head;
        while(current_node != nullptr) {
            if(current_node->key == key) {
                return current_node;
            }
            current_node = current_node->next;
        }
        throw KeyNotFound();
    }
};

#endif //WET1_DICT_LIST_H
