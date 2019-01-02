#include "image_database.h"
#include "library.h"

/* Description:   Initiates the data structure.
 * Input:         pixels - The number of pixels in every image
 * Output:        DS - A pointer to the data structure.
 * Return Values: A pointer to a new instance of the data structure - as a void* pointer.
 */
void * Init(int pixels) {
	if (pixels <= 0) {
		return NULL;
	}
	try {
		ImageDatabase* DS = new ImageDatabase(pixels);
		return (void*)DS;
	} catch(std::bad_alloc&) {
		return NULL;
	}
}

/* Description:   Adds a new image to the data structure.
 * Input:         DS - A pointer to the data structure.
 *                image - The image to add.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL or imageID <= 0.
 *                FAILURE - If the image is already in the DS.
 *                SUCCESS - Otherwise.
 */
StatusType AddImage(void *DS, int imageID) {
	if (DS == NULL || imageID <= 0) {
		return INVALID_INPUT;
	}

	try {
		ImageDatabase* DB = (ImageDatabase*)DS;
		DB->AddImage(imageID);
		return SUCCESS;
	} catch (std::bad_alloc&) {
		return ALLOCATION_ERROR;
	} catch (ImageDatabase::ImageAlreadyExists&) {
		return FAILURE;
	}
}

/* Description:   Deletes an image from the data structure.
 * Input:         DS - A pointer to the data structure.
 *                image - The image to remove.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL or imageID <= 0.
 *                FAILURE - If the image does not exist in the DS.
 *                SUCCESS - Otherwise.
 */
StatusType DeleteImage(void *DS, int imageID) {
	if (DS == NULL || imageID <= 0) {
		return INVALID_INPUT;
	}

	try {
		ImageDatabase* DB = (ImageDatabase*)DS;
		DB->DeleteImage(imageID);
		return SUCCESS;
	} catch (std::bad_alloc&) {
		return ALLOCATION_ERROR;
	} catch (ImageDatabase::ImageDoesNotExist&) {
		return FAILURE;
	}
}

/* Description:   Tags a super pixel in an image.
 * Input:         DS - A pointer to the data structure.
 *                imageID - The image to tag.
 * 				  pixel - A pixel which belongs to the super pixel to tag.
 * 				  label - The label of the pixel.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL pixel >= pixels or pixel < 0 or imageID <= 0 or label < 0 or score <= 0.
 *                FAILURE - If the image does not exist in the DS, or the pixel is already tagged.
 *                SUCCESS - Otherwise.
 */
StatusType SetLabelScore(void *DS, int imageID, int pixel, int label, int score) {
	if (DS == NULL || imageID <= 0 || pixel < 0 || label <= 0 || score <= 0) {
		return INVALID_INPUT;
	}

	try {
		ImageDatabase* DB = (ImageDatabase*)DS;
		if (pixel >= DB->GetPixels()) {
			return INVALID_INPUT;
		}
		DB->SetLabelScore(imageID, pixel, label, score);
		return SUCCESS;
	} catch (std::bad_alloc&) {
		return ALLOCATION_ERROR;
	} catch (ImageDatabase::ImageDoesNotExist&) {
		return FAILURE;
	}
}

/* Description:   Deletes the a label of a super pixel in an image.
 * Input:         DS - A pointer to the data structure.
 *                imageID - The image to delete the label from.
 * 				  pixel - A pixel which belongs to the super pixel to reset the label.
 * 				  label - The label to reset.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL pixel >= pixels or pixel < 0 or imageID < 0 or label < 0.
 *                FAILURE - If the image does not exist in the DS.
 *                SUCCESS - Otherwise.
 */
StatusType ResetLabelScore(void *DS, int imageID, int pixel, int label) {
	if (DS == NULL || imageID <= 0 || pixel < 0 || label <= 0) {
		return INVALID_INPUT;
	}

	try {
		ImageDatabase* DB = (ImageDatabase*)DS;
		if (pixel >= DB->GetPixels()) {
			return INVALID_INPUT;
		}
		DB->ResetLabelScore(imageID, pixel, label);
		return SUCCESS;
	} catch (std::bad_alloc&) {
		return ALLOCATION_ERROR;
	} catch (ImageDatabase::ImageDoesNotExist&) {
		return FAILURE;
	} catch (SuperPixel::LabelDoesNotExist&) {
		return FAILURE;
	}
}

/* Description:   Gets the highest scored label of a the super pixel containing the given pixel in an image.
 * Input:         DS - A pointer to the data structure.
 *                imageID - The image to get the label from.
 * 				  pixel - The pixel of the super pixel to get the label from.
 * 				  label - A pointer to a variable which will hold the label.
 * Output:        The highest scored label in the super pixel containing pixel.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL pixel >= pixels or pixel < 0 or imageID < 0 or label == NULL.
 *                FAILURE - If the image does not exist in the DS, or the super pixel is not tagged at all.
 *                SUCCESS - Otherwise.
 */
StatusType GetHighestScoredLabel(void *DS, int imageID, int pixel, int *label) {
	if (DS == NULL || imageID <= 0 || pixel < 0 || label == NULL) {
		return INVALID_INPUT;
	}

	try {
		ImageDatabase* DB = (ImageDatabase*)DS;
		if (pixel >= DB->GetPixels()) {
			return INVALID_INPUT;
		}
		*label = DB->GetHighestScoredLabel(imageID, pixel);
		return SUCCESS;
	} catch (std::bad_alloc&) {
		return ALLOCATION_ERROR;
	} catch (ImageDatabase::ImageDoesNotExist&) {
		return FAILURE;
	} catch (SuperPixel::SuperPixelNotLabeled&) {
		return FAILURE;
	}
}

/* Description:   Merges two super pixels in to one super pixel
 * Input:         DS - A pointer to the data structure.
 *                imageID - The image to get the segments from.
 *				  pixel1 - The pixel of the first super pixel to merge.
 *				  pixel2 - The pixel of the second super pixel to merge.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL imageID <= 0 pixel1 < 0 pixel1 >= pixels pixel2 < 0 pixel2 >= pixels.
 *                FAILURE - If the image does not exist in the DS or if the two pixel belong to the same super pixel.
 *                SUCCESS - Otherwise.
 */
StatusType MergeSuperPixels(void *DS, int imageID, int pixel1, int pixel2) {
	if (DS == NULL || imageID <= 0 || pixel1 < 0 || pixel2 < 0) {
		return INVALID_INPUT;
	}

	try {
		ImageDatabase* DB = (ImageDatabase*)DS;
		if (pixel1 >= DB->GetPixels() || pixel2 >= DB->GetPixels()) {
			return INVALID_INPUT;
		}
		DB->MergeSuperPixels(imageID, pixel1, pixel2);
		return SUCCESS;
	} catch (std::bad_alloc&) {
		return ALLOCATION_ERROR;
	} catch (ImageDatabase::ImageDoesNotExist&) {
		return FAILURE;
	} catch (Image::IllegalPixelMerge&) {
		return FAILURE;
	}
}

/* Description:   Quits and deletes the database.
 *                DS should be set to NULL.
 * Input:         DS - A pointer to the data structure.
 * Output:        None.
 * Return Values: None.
 */
void Quit(void** DS) {
	ImageDatabase* DB = *((ImageDatabase**)DS);
	delete DB;
	*DS = NULL;
}