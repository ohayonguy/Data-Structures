#ifndef IMAGE_DATABASE_H
#define IMAGE_DATABASE_H

#include <memory>

#include "image.h"
#include "dict_avl.h"

class ImageAlreadyExists : public std::exception {};
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
	ImageDatabase(int segments) : images(DictAvl<int, shared_ptr<Image>>()), num_of_segments(segments) {};
	~ImageDatabase()=default; //it's okay since shared_ptr destructor also frees the Image inside.

	int GetNumOfSegments() {
		return num_of_segments;
	}

	void AddImage(int imageID) {
		shared_ptr<Image> new_image = shared_ptr<Image>(new Image(imageID, num_of_segments));
		try {
			images.GetValueByKey(imageID);
			throw ImageAlreadyExists();
		} catch (const KeyNotFound&) {
			images.InsertNode(imageID, new_image);
		}
	}

	void DeleteImage(int imageID) {
		FindImage(imageID);
		images.DeleteNodeByKey(imageID); // if previous command didn't throw an exception, image exists for sure.
	}

	void AddLabel(int imageID, int segmentID, int label) {
		Image* image = FindImage(imageID);
		image->Label(segmentID, label);
	}

	int GetLabel(int imageID, int segmentID) {
		Image* image = FindImage(imageID);
		return image->GetLabel(segmentID);
	}

	void DeleteLabel(int imageID, int segmentID) {
		Image* image = FindImage(imageID);
		image->Unlabel(segmentID);
	}

	int GetAllUnlabeledSegments(int imageID, int** unlabeled_segments) {
		Image* image = FindImage(imageID);
		*unlabeled_segments = nullptr;
		if (image->CountUnlabeled() != 0) {
			*unlabeled_segments = image->GetAllUnlabeled();
		}
		return image->CountUnlabeled();
	}

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