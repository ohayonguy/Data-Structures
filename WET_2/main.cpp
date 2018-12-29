#include "image.h"
#include <iostream>
#include <assert.h>

using std::cout;
using std::endl;
int main() {
	cout << "hi" << endl;

	Image img = Image(6000);
	for (int i = 1; i < 5000; ++i) {
		img.SetLabelScore(i, 2*i+1, 3*i+1);
		img.SetLabelScore(i, 5*i+1, 3*i+1);
	}
	for (int i = 1; i < 5000; ++i) {
		assert(img.GetHighestScoredLabel(i) == 5*i+1);
	}
	for (int i = 1; i < 5000; ++i) {
		img.ResetLabelScore(i, 5*i+1);
	}
	img.MergeSuperPixels(3, 5);
	assert(img.GetHighestScoredLabel(3) == img.GetHighestScoredLabel(5));
	assert(img.GetHighestScoredLabel(3) == 11);
	img.SetLabelScore(5, 8, 50);
	assert(img.GetHighestScoredLabel(3) == 8);
	img.SetLabelScore(6, 8, 40);
	img.MergeSuperPixels(6, 3);
	assert(img.GetHighestScoredLabel(3) == 8);
	cout << "bye" << endl;

	return 0;
}