//
// Created by guyoe on 29-Nov-18.
//

#ifndef WET1_DICT_LIST_H
#define WET1_DICT_LIST_H
#include <iostream>
template <class Key, class Value>
class DictList {
public:
    /**
     * An inherited exception class to specify that a key is not found in the dictionary.
     */
    class KeyNotFound : public std::exception {};
    /**
     * A struct which contains all the needed values of one node in the dictionary (key, value, a pointer to the next
     * node, and a pointer to the last node).
     */
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
    /**
     * Constructor.
     */
    DictList() : head(nullptr), size(0) {};
    /**
     * Destructor.
     */
    ~DictList() {
        ListNode* current_node = head;
        while (current_node != nullptr) {
            ListNode* temp_current_node = current_node->next;
            delete current_node;
            current_node = temp_current_node;
        }
    }
    /**
     * Inserts a new node to the DictList, with the given key and value.
     * @param key is the key of the new node.
     * @param value is the value of the new node.
     * @return a pointer to the new node inserted.
     * Throws std::badalloc if memory allocation fails.
     */
    ListNode* InsertNode(const Key &key, const Value &value) {
        ListNode* new_node = new ListNode(key, value, head, nullptr);
        if (head != nullptr)
            head->previous = new_node;
        head = new_node;
        size++;
        return new_node;
    }
    /**
     * Searches for the node with the given key, and returns it's value.
     * @param key is the key of the node to look for.
     * @return the value of the node.
     * Throws KeyNotFound if the key is not in the DictList.
     */
    Value GetValueByKey(const Key& key) {
        return GetNodeByKey(key)->value;
    }
    /**
     * Search for the node to delete by the given key, and then delete the node by using DeleteNodeByPtr.
     * @param key the key of the node that needs to be deleted.
     * Throws KeyNotFound if the key is not in the DictList.
     */
    void DeleteNodeByKey(const Key &key) {
        ListNode* node_to_delete = GetNodeByKey(key);
        this->DeleteNodeByPtr(node_to_delete);
    }
    /**
     * deletes a list node by the given pointer. Updates the list accordingly.
     * @param node_to_delete the node to delete.
     */
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
        --size;
    }
    /**
     * @return the size of the dictionary (number of nodes).
     */
    int GetSize() {
        return size;
    }
    /**
     * @return an array with all the values within all the nodes in the dictionray.
     */
    Value* GetAllValues() {
      Value* values = new Value[GetSize()];
      ListNode* current_node = head;
      int index = 0;

      while (current_node != nullptr) {
        values[index++] = current_node->value;
        current_node = current_node->next;
      }

      return values;
    }
private:
    /**
     * Find a node by a given key and return a pointer to that node.
     * @param key is the key to look for
     * @return list node pointer which contains the given key.
     * Throws KeyNotFound if the key is not found in the dictionary.
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
    /**
     * Contains the head of the list.
     */
    ListNode* head;
    /**
     * Contains the number of nodes in the dictionary.
     */
    int size;
};

#endif //WET1_DICT_LIST_H