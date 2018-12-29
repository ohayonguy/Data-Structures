#include "image_database.h"

Image* ImageDatabase::GetImage(int image_id) {
	try {
		return images->Find(image_id).get();
	} catch (KeyDoesNotExist&) {
		throw ImageDoesNotExist();
	}
}

ImageDatabase::ImageDatabase(int pixels) : pixels(pixels), images(new HashTable<shared_ptr<Image>>(INIT_HASH_TABLE_SIZE)) {}
ImageDatabase::~ImageDatabase() {
	delete images;
}

void ImageDatabase::AddImage(int image_id) {
	try {
		GetImage(image_id);
		throw ImageAlreadyExists();
	} catch (ImageDoesNotExist&) {
		Image* new_img = new Image(pixels);
		images->Insert(image_id, shared_ptr<Image>(new_img));
	}
}

void ImageDatabase::DeleteImage(int image_id) {
	GetImage(image_id);
	images->Delete(image_id); // no exception should be thrown since if we got here, image exists.
}

void ImageDatabase::SetLabelScore(int image_id, int pixel, int label, int score) {
	GetImage(image_id)->SetLabelScore(pixel, label, score);
}

void ImageDatabase::ResetLabelScore(int image_id, int pixel, int label) {
	GetImage(image_id)->ResetLabelScore(pixel, label);
}

int ImageDatabase::GetHighestScoredLabel(int image_id, int pixel) {
	GetImage(image_id)->GetHighestScoredLabel(pixel);
}

void ImageDatabase::MergeSuperPixels(int image_id, int pixel1, int pixel2) {
	GetImage(image_id)->MergeSuperPixels(pixel1, pixel2);
}