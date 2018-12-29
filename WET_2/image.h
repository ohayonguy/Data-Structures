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

	SuperPixel* GetSuperPixel(pixelID pixel);

public:
	Image(int pixels);
	~Image();

	void SetLabelScore(pixelID pixel, int label, int score);
	void ResetLabelScore(pixelID pixel, int label);
	int GetHighestScoredLabel(pixelID pixel);
	void MergeSuperPixels(pixelID pixel1, pixelID pixel2);
};


#endif // __IMAGE__