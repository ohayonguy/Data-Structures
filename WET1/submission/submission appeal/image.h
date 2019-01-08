#ifndef IMAGE_H
#define IMAGE_H

#include <stdexcept>

#include "dict_list.h"

/*
* thrown if trying to label a labeled segment.
*/
class SegmentAlreadyLabeled : public std::exception {};

/*
* thrown if trying to get an unlabeled segment's label.
*/
class SegmentNotLabeled : public std::exception {};

/*
* class for representing an Image in the database.
* each Image contains a fixed number of labeled/unlabeled segments.
*/
class Image {
	typedef DictList<int, int>::ListNode ListNode;

	/*
	* struct to represent a single segment inside an image.
	* label - the segment's current label (if labeled).
	* unlabeled_ptr - a pointer to indicate whether the segment is labeled:
	*				  a labeled segment will have ptr = nullptr.
	*				  an unlabeled segment will have ptr point at a ListNode
	*				  containing the segment's ID.
	*/
	struct Segment {
		int label;
		ListNode* unlabeled_ptr; // nullptr iff segment is labeled

		/*
		* creates a new segment.
		* @param label is the label of the new segment.
		* @param ptr is the node pointer to assign inside the segment.
		*/
		Segment(int label, ListNode* ptr) : label(label), unlabeled_ptr(ptr) {};

		/*
		* checks whether the segment is labeled.
		* @return true iff the segment is labeled.
		*/
		bool IsLabeled() {
			return (unlabeled_ptr == nullptr);
		}
	};

	int image_id;
	int num_of_segments;
	Segment** segments;
	DictList<int, int> unlabeled_segments;

public:
	/*
	* constructs a new image.
	* @param image_id is the id for the new image.
	* @param num_of_segment is the fixed segment number the image contains.
	*/
	Image(int image_id, int num_of_segments) : 
		image_id(image_id),
		num_of_segments(num_of_segments),
		segments(new Segment*[num_of_segments]),
		unlabeled_segments(DictList<int, int>()) {
		for (int i = 0; i < num_of_segments; ++i) {
			segments[i] = nullptr; // in case of future bad alloc
		}

		try {

			for (int i = 0; i < num_of_segments; ++i) {
				ListNode* segment_node = unlabeled_segments.InsertNode(i, i);
				segments[i] = new Segment(0, segment_node);
			}
		} catch (std::bad_alloc& e) {
			for (int i = 0; i < num_of_segments; ++i) {
				delete segments[i];
			}
			delete[] segments;
			throw e;
		}
	}
	/*
	* image destructor.
	*/
	~Image() {
		for (int i = 0; i < num_of_segments; ++i) {
			delete segments[i];
			segments[i] = nullptr;
		}
		delete[] segments;
	}

	/*
	* ID field getter.
	*/
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
		int* unlabeled_values = nullptr;
		try {
			unlabeled_values = unlabeled_segments.GetAllValues(); // allocated by new
		} catch (std::bad_alloc& e) { // in case GetAllValuesInOrder throws
			free(unlabeled);
			throw e;
		}
		for (int i = 0; i < CountUnlabeled(); ++i) {
			unlabeled[i] = unlabeled_values[i];
		}
		delete[] unlabeled_values; // DELETED - fixed the bug by changing delete to delete[]
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