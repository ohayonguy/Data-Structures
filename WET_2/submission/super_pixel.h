//
// Created by guyoe on 29-Dec-18.
//

#ifndef WET_2_SUPER_PIXEL_H
#define WET_2_SUPER_PIXEL_H

#include <iostream>
#include <assert.h>
#include "dict_avl.h"

#include <stdexcept>


class SuperPixel {
private:
    //MEMBERS
    DictAvl<int, int> labels_and_scores;
    //METHODS
    static const DictAvl<int,int>::AvlNode* MergeSortedAvlNodesArrays(const DictAvl<int,int>::AvlNode* arr1, int size1,
                                                                      const DictAvl<int,int>::AvlNode* arr2, int size2,
                                                                      int* final_size);
public:
    //MEMBERS

    //METHODS
    class LabelAlreadyExists : public std::exception {};
    class LabelDoesNotExist : public std::exception {};
    class SuperPixelNotLabeled : public std::exception {};
    class LabelNotBiggerThanZero : public std::exception {};
    class ScoreNotBiggerThanZero : public std::exception {};
    /*
    *	constructor.
    */
    SuperPixel() = default;
    SuperPixel(DictAvl<int,int>::AvlNode* labels_and_scores, int size) :
            labels_and_scores(DictAvl<int,int>(labels_and_scores,size)){};
    /*
    *	creates a new label inside the super pixel.
    *	throws LabelAlreadyExists if label exists.
    */
    void SetLabelScore(int label, int score);
    /*
    *	removes a givel label from the super pixel.
    *	throws LabelDoesNotExist if label not found.
    */
    void ResetLabelScore(int label);
    /*
    *	returns the label with the highest score.
    *	throws SuperPixelNotLabeled if no labels exist inside.
    */
    int GetHighestScoredLabel() const;
    /*
    *	returns a new super pixel that is a merge result of 2 given ones.
    *	(inside it should use GetLabelsAndScoresInorder on both super pixels, then merge the arrays
    *	according the the assignment instructions, then call CreateSuperPixelFromArrays)
    */
    static SuperPixel* MergeSuperPixels(const SuperPixel& sp1, const SuperPixel& sp2);
    /**
     * Returns the size of the AVL tree contained in the super pixel.
     * @return
     */
    int GetSize() const;

    //DEBUG
    int GetScoreOfLabel(int label);
};


#endif //WET_2_SUPER_PIXEL_H
