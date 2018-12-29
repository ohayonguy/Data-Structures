//
// Created by guyoe on 29-Dec-18.
//
#include <iostream>
#include "super_pixel.h"
using std::cout;
using std::endl;

void SuperPixel::SetLabelScore(int label, int score) {
    if (label <= 0)
        throw LabelNotBiggerThanZero();
    if (score <= 0)
        throw ScoreNotBiggerThanZero();
    try {
        this->labels_and_scores.InsertNode(label,score);
    } catch (DictAvl<int,int>::KeyAlreadyExists&) {
        throw LabelAlreadyExists();
    } catch (...) {
        throw;
    }
}

int SuperPixel::GetHighestScoredLabel() const {
    int label_of_max_score;
    try {
        label_of_max_score = labels_and_scores.GetMaximumRank();
    } catch (DictAvl<int, int>::DictAvlIsNotInitialized&) {
        throw SuperPixelNotLabeled();
    } catch (...) {
        throw;
    }
    return label_of_max_score;
}

void SuperPixel::ResetLabelScore(int label) {
    try {
        labels_and_scores.DeleteNodeByKey(label);
    } catch (DictAvl<int,int>::KeyNotFound&) {
        throw LabelDoesNotExist();
    } catch (...) {
        throw;
    }
}

SuperPixel* SuperPixel::MergeSuperPixels(const SuperPixel &sp1, const SuperPixel &sp2) {
    DictAvl<int, int>::AvlNode* sp1_inorder_array = sp1.labels_and_scores.GetAllAvlNodesInOrder();
    DictAvl<int, int>::AvlNode *sp2_inorder_array;
    int sp1_size = sp1.GetSize();
    try {
        sp2_inorder_array = sp2.labels_and_scores.GetAllAvlNodesInOrder();
    } catch (...) {
        delete[] sp1_inorder_array;
        throw;
    }
    int sp2_size = sp2.GetSize();
    int merged_array_size;
    const DictAvl<int, int>::AvlNode *merged_array;
    try {
        merged_array = MergeSortedAvlNodesArrays(sp1_inorder_array, sp1_size,
                                                 sp2_inorder_array, sp2_size,
                                                 &merged_array_size);
    } catch (...) {
        delete[] sp1_inorder_array;
        delete[] sp2_inorder_array;
        throw;
    }

    delete[] sp1_inorder_array;
    delete[] sp2_inorder_array;

    DictAvl<int,int>::AvlNode* merged_avl = DictAvl<int,int>::CreateAVLTreeFromInOrderNodesArray(merged_array,
                                                                              merged_array_size);
    delete[] merged_array;
    SuperPixel* merged_super_pixel = new SuperPixel(merged_avl,merged_array_size);
    return merged_super_pixel;
}

int SuperPixel::GetSize() const {
    return labels_and_scores.GetSize();
}

const DictAvl<int, int>::AvlNode *
SuperPixel::MergeSortedAvlNodesArrays(const DictAvl<int, int>::AvlNode *arr1, int size1,
                                      const DictAvl<int, int>::AvlNode *arr2, int size2, int* final_size) {
    if (size1+size2 <= 0)
        return nullptr;

    DictAvl<int, int>::AvlNode* merged_array = new DictAvl<int, int>::AvlNode[size1+size2]; // All the nodes will be
    //constructed with the default constructor of AvlNode.

    int i = 0, j = 0, k = 0;
    while (i < size1 && j < size2)
    {
        if (arr1[i] < arr2[j])
            merged_array[k++] = arr1[i++];
        else if (arr1[i] == arr2[j]) {
            merged_array[k] = arr2[j];
            merged_array[k].value += arr1[i].value; // Addition of the scores.
            k++;
            i++;
            j++;
        } else {
            merged_array[k++] = arr2[j++];
        }
    }

    // Store remaining elements of first array
    while (i < size1)
        merged_array[k++] = arr1[i++];

    // Store remaining elements of second array
    while (j < size2)
        merged_array[k++] = arr2[j++];
    *final_size = k;
    return merged_array;
}

int SuperPixel::GetScoreOfLabel(int label) {
    return labels_and_scores.GetValueByKey(label);
}
