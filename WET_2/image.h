#ifndef __IMAGE__
#define __IMAGE__

#include <memory>
#include <stdexcept>
#include "union_find.h"
#include "super_pixel.h"

using std::shared_ptr;
class Image {
	class PixelNotFound : public std::exception {};
	class IllegalPixelMerge : public std::exception {};

	typedef int pixelID;
	UnionFind<shared_ptr<SuperPixel>>* pixels;

	/*
	*	finds the super pixel of a given pixel.
	*	@param pixel is the pixel id.
	*	@returns a pointer to the super pixel object.
	*/

	SuperPixel* GetSuperPixel(pixelID pixel);

public:
	/*
	*	constructor.
	*	@param pixels is the number of pixels in the image.
	*/
	Image(int pixels);

	/*
	*	destructor.
	*	deallocates all pixel-related memory.
	*/
	~Image();

	/*
	*	adds a label with a given score to a given pixels' superpixel.
	*	@param pixel is the pixel id. the label will be added to that pixel's SUPER PIXEL.
	*	@param label is the new label to add.
	*	@param score is the score of the label.
	*/
	void SetLabelScore(pixelID pixel, int label, int score);

	/*
	*	resets a label's score in a given pixel's super pixel.
	*	@param pixel is the pixel id. the label will be deleted from that pixel's super pixel.
	*	@param label is the label to delete.
	*/
	void ResetLabelScore(pixelID pixel, int label);

	/*
	*	find the highest scored label in a given pixel's super pixel.
	*	@param pixel is the pixel id to look for super pixel by.
	*	@returns the highest scored label of a given pixel's super pixel.
	*	if there are multiple labels with the same score - the highest label will be returned.
	*/
	int GetHighestScoredLabel(pixelID pixel);

	/*
	*	merges the super pixels to which 2 given pixels belong.
	*	if pixels belong to same super pixel - an exception will be thrown.
	*	@param pixel1 is the first pixel's id.
	*	@parram pixel2 is the second pixel's id.
	*/
	void MergeSuperPixels(pixelID pixel1, pixelID pixel2);
};


#endif // __IMAGE__