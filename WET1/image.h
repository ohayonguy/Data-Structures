#ifndef IMAGE_H
#define IMAGE_H

#include <stdexcept>

#include "dict_list.h"

class SegmentAlreadyLabeled : public std::exception {};
class SegmentNotLabeled : public std::exception {};

class Image {
	typedef DictList<int, int>::ListNode ListNode;
	struct Segment {
		int label;
		ListNode* unlabeled_ptr;

		Segment(int label, ListNode* ptr) : label(label), unlabeled_ptr(ptr) {};

		bool IsLabeled() {
			return (unlabeled_ptr == nullptr);
		}
	};

	int image_id;
	int num_of_segments;
	Segment** segments;
	DictList<int, int> unlabeled_segments;

public:
	Image(int image_id, int num_of_segments) : 
		image_id(image_id),
		num_of_segments(num_of_segments),
		segments(new Segment*[num_of_segments]),
		unlabeled_segments(DictList<int, int>()) {
		for (int i = 0; i < num_of_segments; ++i) {
			ListNode* segment_node = unlabeled_segments.InsertNode(i, i);
			segments[i] = new Segment(0, segment_node);
		}
	}
	~Image() {
		for (int i = 0; i < num_of_segments; ++i) {
			delete segments[i];
			segments[i] = nullptr;
		}
		delete[] segments;
	}

	int GetID() {
		return image_id;
	}

	 /**
     * Label a given previously unlabeled segment in the image.
     * @param segmentID is the segment that should be labeled.
     * @param label is the new label for the segment.
     */
	void Label(int segmentID, int label) {
		Segment* segment = segments[segmentID];
		if (segment->IsLabeled()) {
			throw SegmentAlreadyLabeled();
		}
		segment->label = label;
		unlabeled_segments.DeleteNodeByPtr(segment->unlabeled_ptr);
		segment->unlabeled_ptr = nullptr;
	}

	 /**
     * Return the label of a given labeled segment.
     * @param segmentID is the segment to look for.
     * @return the int label of target segment.
     */
	int GetLabel(int segmentID) {
		Segment* segment = segments[segmentID];
		if (! segment->IsLabeled()) {
			throw SegmentNotLabeled();
		}
		return segment->label;
	}

	 /**
     * Remove the label of a given previously labeled segment.
     * @param segmentID is the segment to look for.
     */
	void Unlabel(int segmentID) {
		Segment* segment = segments[segmentID];
		if (! segment->IsLabeled()) {
			throw SegmentNotLabeled();
		}
		segment->unlabeled_ptr = unlabeled_segments.InsertNode(segmentID, segmentID);
	}

	 /**
     * @return the amount of unlabeled segments in the image.
     */
	int CountUnlabeled() {
		return unlabeled_segments.GetSize();
	}

	 /**
     * Look for all unlabeled segments in the image.
     * @return an integer array containing the ID's of all unlabeled segments.
     *		   the array is sorted by segmentID.
     */
	int* GetAllUnlabeled() {
		int* unlabeled = (int*)malloc(sizeof(int)*CountUnlabeled()); // has to be malloc for C interface
		if (unlabeled == nullptr) {
			throw std::bad_alloc();
		}
		for (int i = 0, j=0; i < num_of_segments; ++i) {
			if (! segments[i]->IsLabeled()) {
				unlabeled[j++] = i;
			}
		}
		return unlabeled;
	}

	 /**
     * Count segments in the image that are labeled with a given label.
     * @param label is the label to filter segments by.
     * @return amount of segments found with given label.
     */
	int CountByLabel(int label) {
		int count = 0;
		for (int i = 0; i < num_of_segments; ++i) {
			if (segments[i]->IsLabeled() && GetLabel(i) == label) {
				++count;
			}
		}
		return count;
	}

	 /**
     * Look for all segments that have a given label.
     * @param label is the label to filter segments by.
     * @return an integer array containing the ID's of all segments with given label.
     *		   the array is sorted by segmentID.
     */
	int* GetAllByLabel(int label) {
		int* segments_with_label = new int[CountByLabel(label)];
		for (int i = 0, j = 0; i < num_of_segments; ++i) {
			if (segments[i]->IsLabeled() && GetLabel(i) == label) {
				segments_with_label[j++] = i;
			}
		}
		return segments_with_label;
	}



};

#endif //IMAGE_H