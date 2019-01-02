#include "image.h"

SuperPixel* Image::GetSuperPixel(pixelID pixel) {
	try {
		SuperPixel* super_pixel = pixels->GetData((pixels->Find(pixel))).get();
		return super_pixel;
	} catch (IllegalElement&) {
		throw PixelNotFound();
	}
}

Image::Image(int num_of_pixels) : pixels(new UnionFind<shared_ptr<SuperPixel>>(num_of_pixels)) {
	for (int i = 0; i < num_of_pixels; ++i) {
		pixels->SetData(i, shared_ptr<SuperPixel>(new SuperPixel()));
	}
}
Image::~Image() {
	delete pixels;
}

void Image::SetLabelScore(pixelID pixel, int label, int score) {
	GetSuperPixel(pixel)->SetLabelScore(label, score);
}

void Image::ResetLabelScore(pixelID pixel, int label) {
	GetSuperPixel(pixel)->ResetLabelScore(label);
}

int Image::GetHighestScoredLabel(pixelID pixel) {
	return GetSuperPixel(pixel)->GetHighestScoredLabel();
}

void Image::MergeSuperPixels(pixelID pixel1, pixelID pixel2) {
	SuperPixel* sp1 = GetSuperPixel(pixel1);
	SuperPixel* sp2 = GetSuperPixel(pixel2);
	try {
		int union_result = pixels->Union(pixels->Find(pixel1), pixels->Find(pixel2));
		SuperPixel* new_sp = SuperPixel::MergeSuperPixels(*sp1, *sp2);
		pixels->SetData(union_result, shared_ptr<SuperPixel>(new_sp));
	} catch (IllegalSet&) {
		throw IllegalPixelMerge();
	}
}
