#include "library2.h"
#include "image_database.h"

void* Init(int segments) {
	try {
		if (segments <= 0) {
			return NULL;
		}
		ImageDatabase* DS = new ImageDatabase(segments);
		return (void*)DS;
	} catch (std::bad_alloc&) {
		return NULL;
	}
}

StatusType AddImage(void *DS, int imageID) {
	if (DS == NULL || imageID <= 0) {
		return INVALID_INPUT;
	}

	try {
		ImageDatabase* DB = (ImageDatabase*)DS;
		DB->AddImage(imageID);
	} catch (std::bad_alloc&) {
		return ALLOCATION_ERROR;
	} catch (ImageAlreadyExists&) {
		return FAILURE;
	}

	return SUCCESS;
}

StatusType DeleteImage(void *DS, int imageID) {
	if (DS == NULL || imageID <= 0) {
		return INVALID_INPUT;
	}

	try {
		ImageDatabase* DB = (ImageDatabase*)DS;
		DB->DeleteImage(imageID);
	} catch (std::bad_alloc&) {
		return ALLOCATION_ERROR;
	} catch (ImageDoesNotExist&) {
		return FAILURE;
	}

	return SUCCESS;
}

StatusType AddLabel(void *DS, int imageID, int segmentID, int label) {
	if (DS == NULL || segmentID < 0 || imageID <= 0 || label <= 0) {
		return INVALID_INPUT;
	}

	try {
		ImageDatabase* DB = (ImageDatabase*)DS;
		if (segmentID >= DB->GetNumOfSegments()) {
			return INVALID_INPUT;
		}
		DB->AddLabel(imageID, segmentID, label);
	} catch (std::bad_alloc&) {
		return ALLOCATION_ERROR;
	} catch (ImageDoesNotExist&) {
		return FAILURE;
	} catch (SegmentAlreadyLabeled&) {
		return FAILURE;
	}

	return SUCCESS;
}

StatusType GetLabel(void *DS, int imageID, int segmentID, int *label) {
	if (DS == NULL || segmentID < 0 || imageID <= 0 || label == NULL) {
		return INVALID_INPUT;
	}

	try {
		ImageDatabase* DB = (ImageDatabase*)DS;
		if (segmentID >= DB->GetNumOfSegments()) {
			return INVALID_INPUT;
		}
		*label = DB->GetLabel(imageID, segmentID);
	} catch (std::bad_alloc&) {
		return ALLOCATION_ERROR;
	} catch (ImageDoesNotExist&) {
		return FAILURE;
	} catch (SegmentNotLabeled&) {
		return FAILURE;
	}

	return SUCCESS;
}

StatusType DeleteLabel(void *DS, int imageID, int segmentID) {
	if (DS == NULL || segmentID < 0 || imageID <= 0) {
		return INVALID_INPUT;
	}

	try {
		ImageDatabase* DB = (ImageDatabase*)DS;
		if (segmentID >= DB->GetNumOfSegments()) {
			return INVALID_INPUT;
		}
		DB->DeleteLabel(imageID, segmentID);
	} catch(std::bad_alloc&) {
		return ALLOCATION_ERROR;
	} catch (ImageDoesNotExist&) {
		return FAILURE;
	} catch (SegmentNotLabeled&) {
		return FAILURE;
	}

	return SUCCESS;
}

StatusType GetAllUnLabeledSegments(void *DS, int imageID, int **segments, int *numOfSegments) {
	if (DS == NULL || imageID <= 0 || segments == NULL || numOfSegments == NULL) {
		return INVALID_INPUT;
	}

	try {
		ImageDatabase* DB = (ImageDatabase*)DS;
		int num_of_segments = DB->GetAllUnlabeledSegments(imageID, segments);
		if (num_of_segments == 0) {
			return FAILURE;
		}
		*numOfSegments = num_of_segments;
	} catch (std::bad_alloc&) {
		return ALLOCATION_ERROR;
	} catch (ImageDoesNotExist&) {
		*segments = NULL;
		return FAILURE;
	}

	return SUCCESS;
}

StatusType GetAllSegmentsByLabel(void *DS, int label, int **images, int **segments, int *numOfSegments) {
	if (DS == NULL || images == NULL || segments == NULL || numOfSegments == NULL || label <= 0) {
		return INVALID_INPUT;
	}

	try {
		ImageDatabase* DB = (ImageDatabase*)DS;
		*images = *segments = NULL;
		*numOfSegments = DB->GetAllSegmentsByLabel(label, images, segments);
	} catch (std::bad_alloc&) {
		return ALLOCATION_ERROR;
	}

	return SUCCESS;
}

void Quit(void** DS) {
	ImageDatabase* DB = *(ImageDatabase**)DS;
	delete DB;
	*DS = NULL;
}