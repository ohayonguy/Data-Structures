//
// Created by guyoe on 30-Nov-18.
//
#include "library1.h"
#include "dict_list.h"

typedef int Key;
typedef void* Value;
void *Init() {
    return new DictList<Key,Value>();
}


StatusType Add(void *DS, int key, void* value, void** node) {
    if (DS == NULL || value == NULL)
        return INVALID_INPUT;
    DictList<Key,Value>* dict_list = (DictList<Key,Value>*)DS;
    try {
        *node = (void*) dict_list->InsertNode(key, value);
    } catch(std::bad_alloc&) {
        return ALLOCATION_ERROR;
    } catch(...) {
        throw; //TODO: not sure if to return this if other failures happened. Maybe just throw?
    }
    return SUCCESS;
}


StatusType Find(void *DS, int key, void** value) {
    if (value == NULL || DS == NULL)
        return INVALID_INPUT;
    DictList<Key,Value>* dict_list = (DictList<Key,Value>*)DS;
    try {
        *value = dict_list->GetValueByKey(key);
    } catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    } catch (DictList<Key,Value>::KeyNotFound&) {
        return FAILURE;
    } catch (...) {
        throw;
    }
    return SUCCESS;
};


StatusType Delete(void *DS, int key) {
    if (DS == NULL)
        return INVALID_INPUT;
    DictList<Key,Value>* dict_list = (DictList<Key,Value>*)DS;
    try {
        dict_list->DeleteNodeByKey(key);
    } catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    } catch (DictList<Key,Value>::KeyNotFound&) {
        return FAILURE;
    } catch (...) {
        throw;
    }
    return SUCCESS;
}


StatusType DeleteByPointer(void *DS, void* p) {
    if (DS == NULL || p == NULL)
        return INVALID_INPUT;
    DictList<Key,Value>* dict_list = (DictList<Key,Value>*)DS;
    try {
        dict_list->DeleteNodeByPtr((DictList<Key,Value>::ListNode*)p);
    } catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    } catch (...) {
        throw;
    }
    return SUCCESS;
}


StatusType Size(void *DS, int *n) {
    if (DS == NULL || n == NULL)
        return INVALID_INPUT;
    DictList<Key,Value>* dict_list = (DictList<Key,Value>*)DS;
    try {
        *n = dict_list->GetSize();
    } catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    } catch (...) {
        throw;
    }
    return SUCCESS;
}


void Quit(void** DS) {
    if (DS == NULL || *DS == NULL)
        return;
    delete *((DictList<Key,Value>**)DS);
    *DS = NULL;
    return;
}