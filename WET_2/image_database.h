#ifndef __IMAGEDATABASE__
#define __IMAGEDATABASE__

#include <stdexcept>
#include <memory>
#include "image.h"
#include "hash_table.h"

using std::shared_ptr;
class ImageDatabase {
	const int INIT_HASH_TABLE_SIZE = 50;

	int pixels;
	HashTable<shared_ptr<Image>>* images;

	Image* GetImage(int image_id);
public:
	class ImageAlreadyExists : public std::exception {};
	class ImageDoesNotExist : public std::exception {};

	/*
	*	constructor.
	*	@param pixels is the number of pixels in each image.
	*/
	ImageDatabase(int pixels);
	/*
	*	destructor.
	*/
	~ImageDatabase();

	/*
	*	adds a new image to the database.
	*	@param image_id is the new image id to add.
	*/
	void AddImage(int image_id);

	/*
	*	deletes an image from the database.
	*	@param image_id is the image id to delete.
	*/
	void DeleteImage(int image_id);

	/*
	*	adds a new label with a given score to the super pixel of a given
	*	pixel inside a given image.
	*	@param image_id is the image to which the pixel belongs.
	*	@param pixel is the pixel id. the label will be added to this pixel's super pixel.
	*	@param label is the new label to add.
	*	@param score is the score for the new label.
	*/
	void SetLabelScore(int image_id, int pixel, int label, int score);

	/*
	*	clears the score of a given label in a super pixel of a given pixel
	*	inside a given image.
	*	@param image_id is the image to which the pixel belongs.
	*	@param pixel is the pixel id. the label will be removed from this pixel's super pixel.
	*	@param label is the label to remove.
	*/
	void ResetLabelScore(int image_id, int pixel, int label);

	/*
	*	finds the highest scored label in a given pixel's super pixel inside a given image.
	*	@param image_id is the image to which the pixel belongs.
	*	@param pixel is the pixel id. in this pixel's super pixel the highest scored label will be returned.
	*	@returns the highest scored label in the super pixel.
	*	@if multiple labels have the highest score, the highest label id will be returned.
	*/
	int GetHighestScoredLabel(int image_id, int pixel);

	/*
	*	merges the super pixels of given two pixels (can not be the same super pixel)
	*	inside a given image.
	*	@param image_id is the id of the image to which the pixels belong.
	*	@param pixel1 is the id of the first pixel.
	*	@param pixel2 is the id of the second pixel.
	*/
	void MergeSuperPixels(int image_id, int pixel1, int pixel2);

};

#endif // __IMAGEDATABASE__