//
// Created by guyoe on 07-Dec-18.
//

#include "library1.h"
#ifdef AVL_LIB
#include "dict_avl.h"

typedef int Key;
typedef void* Value;
void *Init() {
    DictAvl<Key, Value>* new_dict = nullptr;
    try {
        new_dict = new DictAvl<Key, Value>();
    } catch (std::bad_alloc&) {
        return NULL;
    } catch (...) {
        delete new_dict;
        return NULL;
    }
    return new_dict;
}


StatusType Add(void *DS, int key, void* value, void** node) {
    if (DS == NULL || value == NULL)
        return INVALID_INPUT;
    DictAvl<Key,Value>* dict_avl = (DictAvl<Key,Value>*)DS;
    try {
        *node = (void*)dict_avl->InsertNode(key, value);
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
    DictAvl<Key,Value>* dict_avl = (DictAvl<Key,Value>*)DS;
    try {
        *value = dict_avl->GetValueByKey(key);
    } catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    } catch (DictAvl<Key,Value>::KeyNotFound&) {
        return FAILURE;
    } catch (...) {
        throw;
    }
    return SUCCESS;
};


StatusType Delete(void *DS, int key) {
    if (DS == NULL)
        return INVALID_INPUT;
    DictAvl<Key,Value>* dict_avl = (DictAvl<Key,Value>*)DS;
    try {
        dict_avl->DeleteNodeByKey(key);
    } catch (std::bad_alloc&) {
        return ALLOCATION_ERROR;
    } catch (DictAvl<Key,Value>::KeyNotFound&) {
        return FAILURE;
    } catch (...) {
        throw;
    }
    return SUCCESS;
}


StatusType DeleteByPointer(void *DS, void* p) {
    if (DS == NULL || p == NULL)
        return INVALID_INPUT;
    DictAvl<Key,Value>* dict_avl = (DictAvl<Key,Value>*)DS;
    try {
        dict_avl->DeleteNodeByPtr((DictAvl<Key,Value>::AvlNode*)p);
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
    DictAvl<Key,Value>* dict_avl = (DictAvl<Key,Value>*)DS;
    try {
        *n = dict_avl->GetSize();
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
    delete *((DictAvl<Key,Value>**)DS);
    *DS = NULL;
    return;
}
#endif