#ifndef IMAGE_DATABASE_H
#define IMAGE_DATABASE_H

#include <memory>

#include "image.h"
#include "dict_avl.h"

/*
* Thrown if trying to insert an image with an already existing ID.
*/
class ImageAlreadyExists : public std::exception {};

/*
* Thrown if trying to access an Image through a non-existent ID.
*/
class ImageDoesNotExist : public std::exception {};

using std::shared_ptr;
class ImageDatabase {
	typedef DictAvl<int, shared_ptr<Image>>::KeyNotFound KeyNotFound;
	DictAvl<int, shared_ptr<Image>> images;
	int num_of_segments;

	 /**
     * Look for an image by given id.
     * @param imageID is the id to look for.
     * @return a pointer to the image object, if found.
     */
	Image* FindImage(int imageID) {
		try {
			return images.GetValueByKey(imageID).get();
		} catch (KeyNotFound&) {
			throw ImageDoesNotExist();
		}
	}

public:
	/*
	* Constructs a new Image database.
	* @param segments is the fixed segment number each image will contain.
	*/
	ImageDatabase(int segments) : images(DictAvl<int, shared_ptr<Image>>()), num_of_segments(segments) {};

	/*
	* Destructor.
	*/
	~ImageDatabase()=default; //it's okay since shared_ptr destructor also frees the Image inside.

	/*
	* Getter for the fixed number of segments in each image.
	*/
	int GetNumOfSegments() {
		return num_of_segments;
	}

	/*
	* Adds a new image to the database.
	* @param imageID is the ID of the new image.
	*/
	void AddImage(int imageID) {
		shared_ptr<Image> new_image = shared_ptr<Image>(new Image(imageID, num_of_segments));
		try {
			images.GetValueByKey(imageID);
			throw ImageAlreadyExists();
		} catch (const KeyNotFound&) {
			images.InsertNode(imageID, new_image);
		}
	}

	/*
	* Deletes an existing image from the database.
	* @param imageID is the ID of the image to remove.
	*/
	void DeleteImage(int imageID) {
		FindImage(imageID);
		images.DeleteNodeByKey(imageID); // if previous command didn't throw an exception, image exists for sure.
	}

	/*
	* Labels a segment inside a given image.
	* @param imageID is the ID of the image which contains the segment.
	* @param segmentID is the ID of the segment to label.
	* @param label is the new label for the given segment.
	*/
	void AddLabel(int imageID, int segmentID, int label) {
		Image* image = FindImage(imageID);
		image->Label(segmentID, label);
	}

	/*
	* Returns the label of a given segment inside an image.
	* @param imageID is the ID of the image which contains the segment.
	* @param segmentID is the wanted segment's ID.
	* @returns the segment's label.
	*/
	int GetLabel(int imageID, int segmentID) {
		Image* image = FindImage(imageID);
		return image->GetLabel(segmentID);
	}

	/*
	* Deletes the label of a given segment inside an image.
	* @param imageID is the ID of the image which contains the segment.
	* @param segmentID is the wanted segment's ID.
	*/
	void DeleteLabel(int imageID, int segmentID) {
		Image* image = FindImage(imageID);
		image->Unlabel(segmentID);
	}

	/*
	* Looks for all unlabeled segments in a given image.
	* @param imageID is the ID of the wanted image.
	* @param unlabeled_segments is a pointer into which the array of unlabeled segments
	*		 will be assined. Will be assigned nullptr if no unlabeled segments are found.
	* @returns the number of unlabeled segments found in the given image.
	*/
	int GetAllUnlabeledSegments(int imageID, int** unlabeled_segments) {
		Image* image = FindImage(imageID);
		*unlabeled_segments = nullptr;
		if (image->CountUnlabeled() != 0) {
			*unlabeled_segments = image->GetAllUnlabeled();
		}
		return image->CountUnlabeled();
	}

	/*
	* Looks for all segments in the entire database which are labeled with a given label.
	* @param label is the label to filter segments by.
	* @param images is a pointer into which the array of image numbers will be assigned.
	* @param segments is a pointer into which the array of segments will be assigned.
	* @returns the amount of segments found with the given label.
	*		   images and segments will contain array's with identical lengths, where
	*		   every 2 corresponding slots will contain a segment ID found with the given
	*          label and the image ID the segment was found in.
	* 		   if no segments found with the given label, both pointers will contain nullptr.
	*/
	int GetAllSegmentsByLabel(int label, int** images, int** segments) {
		shared_ptr<Image>* all_images = this->images.GetAllValuesInOrder();
		*images = *segments = NULL;
		int total_segments = 0;
		for (int i = 0; i < this->images.GetSize(); ++i) {
			total_segments += all_images[i]->CountByLabel(label);
		}
		if (total_segments == 0) {
			delete[] all_images;
			return 0;
		}

		try {
			*images = (int*)malloc(sizeof(int)*total_segments); // has to be malloc for C interface
			if (*images == NULL) {
				throw std::bad_alloc();
			}
			*segments = (int*)malloc(sizeof(int)*total_segments);
			if (*segments == NULL) {
				throw std::bad_alloc();
			}

			int current_index = 0;
			for (int i = 0; i < this->images.GetSize(); ++i) {
				int* segments_from_image = all_images[i]->GetAllByLabel(label);
				int num_of_segments_from_image = all_images[i]->CountByLabel(label);

				for (int s = 0; s < num_of_segments_from_image; ++s) {
					(*images)[current_index] = all_images[i]->GetID();
					(*segments)[current_index] = segments_from_image[s];
					++current_index;
				}
				delete[] segments_from_image;
			}

		} catch (std::bad_alloc& e) {
			free(*images);
			free(*segments);
			*images = *segments = NULL;
			delete[] all_images;
			throw e;
		}

		delete[] all_images;
		return total_segments;
	}
};

#endif //IMAGE_DATABASE_H