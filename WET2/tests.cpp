#include <iostream>
#include "library.h"
#include "catch.hpp"

//#define STRESS_TEST

namespace Tests {
using std::string;
std::string StatusToString(StatusType st) {
  string a;
  switch (st) {
    case SUCCESS:a = "SUCCESS";
      break;
    case FAILURE:a = "FAILURE";
      break;
    case INVALID_INPUT:a = "INVALID_INPUT";
      break;
    case ALLOCATION_ERROR:a = "ALLOCATION_ERROR";
      break;
  }
  return a;
}


TEST_CASE("Testing Initialization of Datastructure") {
  SECTION("Checking whether 0 pixels case was handled:") {
    REQUIRE(!Init(0));
  }
  SECTION("Checking whether negative pixels case was handled:") {
    REQUIRE(!Init(-1));
  }
  SECTION("Checking creating of Data structure for pixels > 1:") {
    auto ds = Init(1);
    REQUIRE(ds);
    Quit(&ds);
  }
}
TEST_CASE("Testing AddImage Function") {
  SECTION("Checking whether null DS ptr was handled:") {
    REQUIRE(StatusToString(AddImage(NULL, 5)) == StatusToString(INVALID_INPUT));
  }
  auto ds = Init(5);
  SECTION("Checking whether non-positive imageID was handled") {
    REQUIRE(StatusToString(AddImage(ds, 0)) == StatusToString(INVALID_INPUT));
  }
  SECTION("Checking whether negative imageID was handled") {
    REQUIRE(StatusToString(AddImage(ds, -1)) == StatusToString(INVALID_INPUT));
  }
  SECTION("Checking successful addition of an image") {
    REQUIRE(StatusToString(AddImage(ds, 1)) == StatusToString(SUCCESS));
  }
  SECTION("Checking Failure on addition of an image with existing id:") {
    REQUIRE(StatusToString(AddImage(ds, 1)) == StatusToString(SUCCESS));
    REQUIRE(StatusToString(AddImage(ds, 1)) == StatusToString(FAILURE));
  }
  Quit(&ds);
}
TEST_CASE("Testing Delete_image Function:") {
  SECTION("Checking whether null DS ptr was handled:") {
    REQUIRE(StatusToString(DeleteImage(NULL, 5)) ==
        StatusToString(INVALID_INPUT));
  }
  auto ds = Init(5);
  SECTION("Checking whether non-positive imageID was handled") {
    REQUIRE(StatusToString(DeleteImage(ds, 0)) ==
        StatusToString(INVALID_INPUT));
  }
  SECTION("Checking whether negative imageID was handled") {
    REQUIRE(StatusToString(DeleteImage(ds, -1)) ==
        StatusToString(INVALID_INPUT));
  }
  SECTION(
      "Checking failure on removal of non existing image when there are no "
      "images at all") {
    REQUIRE(StatusToString(DeleteImage(ds, 1)) == StatusToString(FAILURE));
  }
  SECTION(
      "Checking failure on removal of non existing image when there are images "
      "in the DS") {
    REQUIRE(StatusToString(AddImage(ds, 2)) == StatusToString(SUCCESS));
    REQUIRE(StatusToString(DeleteImage(ds, 1)) == StatusToString(FAILURE));
  }
  SECTION("Checking success on removal of an existing image:") {
    REQUIRE(StatusToString(AddImage(ds, 1)) == StatusToString(SUCCESS));
    REQUIRE(StatusToString(DeleteImage(ds, 1)) == StatusToString(SUCCESS));
    REQUIRE(StatusToString(AddImage(ds, 1)) ==
        StatusToString(
            SUCCESS));  // should be able to add it again after removal
  }
  Quit(&ds);
}

TEST_CASE("Testing SetLabelScore function:") {
  SECTION("Checking whether null DS ptr was handled:") {
    REQUIRE(StatusToString(SetLabelScore(NULL, 5, 5, 4, 10)) ==
        StatusToString(INVALID_INPUT));
  }
  auto ds = Init(5);
  SECTION("Checking whether non-positive imageID was handled") {
    REQUIRE(StatusToString(SetLabelScore(ds, 0, 5, 4, 10)) ==
        StatusToString(INVALID_INPUT));
  }
  SECTION("Checking whether negative imageID was handled") {
    REQUIRE(StatusToString(SetLabelScore(ds, -1, 5, 4, 10)) ==
        StatusToString(INVALID_INPUT));
  }
  SECTION("Checking whether negative pixel number was handled") {
    REQUIRE(StatusToString(SetLabelScore(ds, 1, -1, 4, 10)) ==
        StatusToString(INVALID_INPUT));
  }
  SECTION("Checking whether pixel>= pixels in DS was handled") {
    REQUIRE(StatusToString(SetLabelScore(ds, 1, 6, 4, 10)) ==
        StatusToString(INVALID_INPUT));
  }
  SECTION("Checking whether zero label was handled") {
    REQUIRE(StatusToString(SetLabelScore(ds, 1, 2, 0, 10)) ==
        StatusToString(INVALID_INPUT));
  }
  SECTION("Checking whether negative label was handled") {
    REQUIRE(StatusToString(SetLabelScore(ds, 1, 2, -1, 10)) ==
        StatusToString(INVALID_INPUT));
  }
  SECTION("Checking whether  zero score was handled") {
    REQUIRE(StatusToString(SetLabelScore(ds, 1, 2, 1, 0)) ==
        StatusToString(INVALID_INPUT));
  }
  SECTION("Checking whether  negative score was handled") {
    REQUIRE(StatusToString(SetLabelScore(ds, 1, 2, 1, -1)) ==
        StatusToString(INVALID_INPUT));
  }
  SECTION("Checking failure on adding label to non-existing image") {
    REQUIRE(StatusToString(SetLabelScore(ds, 1, 2, 1, 2))
                == StatusToString(FAILURE));
  }

  SECTION("Checking success on adding label to an existing image") {
    REQUIRE(StatusToString(AddImage(ds, 1)) == StatusToString(SUCCESS));
    REQUIRE(StatusToString(SetLabelScore(ds, 1, 2, 1, 2))
                == StatusToString(SUCCESS));
  }
  SECTION(
      "Checking success on adding label to an pixel that's already labeled") {
    REQUIRE(StatusToString(AddImage(ds, 2)) == StatusToString(SUCCESS));
    REQUIRE(StatusToString(SetLabelScore(ds, 2, 2, 1, 2))
                == StatusToString(SUCCESS));
    REQUIRE(StatusToString(SetLabelScore(ds, 2, 2, 1, 5))
                == StatusToString(SUCCESS));
  }
  Quit(&ds);
}
TEST_CASE("Testing ResetLabelScore function") {
  int label;
  SECTION("Checking whether null DS ptr was handled") {
    REQUIRE(StatusToString(ResetLabelScore(NULL, 1, 1, 1)) ==
        StatusToString(INVALID_INPUT));
  }
  auto ds = Init(5);
  SECTION("Checking whether imageID < 0 was handled") {
    REQUIRE(StatusToString(ResetLabelScore(ds, -1, 1, 1)) ==
        StatusToString(INVALID_INPUT));
  }
  SECTION("Checking whether pixel >= pixels in DS was handled") {
    REQUIRE(StatusToString(AddImage(ds, 1)) == StatusToString(SUCCESS));
    REQUIRE(StatusToString(ResetLabelScore(ds, 1, 6, 1)) ==
        StatusToString(INVALID_INPUT));
  }
  SECTION("Checking whether pixel < 0  was handled") {
    REQUIRE(StatusToString(AddImage(ds, 1)) == StatusToString(SUCCESS));
    REQUIRE(StatusToString(ResetLabelScore(ds, 1, -1, 1)) ==
        StatusToString(INVALID_INPUT));
  }
  SECTION("Checking whether label = 0  was handled") {
    REQUIRE(StatusToString(AddImage(ds, 1)) == StatusToString(SUCCESS));
    REQUIRE(StatusToString(ResetLabelScore(ds, 1, 1, 0)) ==
        StatusToString(INVALID_INPUT));
  }
  SECTION("Checking whether label < 0  was handled") {
    REQUIRE(StatusToString(AddImage(ds, 1)) == StatusToString(SUCCESS));
    REQUIRE(StatusToString(ResetLabelScore(ds, 1, 1, -1)) ==
        StatusToString(INVALID_INPUT));
  }
  SECTION("Checking failure on resetting label of non-existing image") {
    REQUIRE(StatusToString(ResetLabelScore(ds, 5, 1, 1)) ==
        StatusToString(FAILURE));
  }

  SECTION(
      "Checking failure on getting label from image where the segment isn't "
      "labeled") {
    REQUIRE(StatusToString(AddImage(ds, 1)) == StatusToString(SUCCESS));
    REQUIRE(StatusToString(ResetLabelScore(ds, 1, 1, 1)) ==
        StatusToString(FAILURE));
  }
  SECTION("Checking success on resetting existing label") {
    REQUIRE(StatusToString(AddImage(ds, 2)) == StatusToString(SUCCESS));
    REQUIRE(StatusToString(SetLabelScore(ds, 2, 2, 1, 2))
                == StatusToString(SUCCESS));
    REQUIRE(StatusToString(ResetLabelScore(ds, 2, 2, 1))
                == StatusToString(SUCCESS));
  }
  Quit(&ds);
}
TEST_CASE("Testing GetHighestScoredLabel function") {
  int label = 0;
  SECTION("Checking whether null DS ptr was handled") {
    REQUIRE(StatusToString(GetHighestScoredLabel(NULL, 1, 1, &label)) ==
        StatusToString(INVALID_INPUT));
  }
  auto ds = Init(5);
  AddImage(ds, 1);
  SetLabelScore(ds, 1, 1, 1, 1);
  SECTION("Checking whether imageID < 0 was handled") {
    REQUIRE(StatusToString(GetHighestScoredLabel(ds, -1, 1, &label)) ==
        StatusToString(INVALID_INPUT));
  }
  SECTION("Checking whether pixel < 0 was handled") {
    REQUIRE(StatusToString(GetHighestScoredLabel(ds, 1, -1, &label)) ==
        StatusToString(INVALID_INPUT));
  }
  SECTION("Checking whether pixel >= pixels in DS was handled") {
    REQUIRE(StatusToString(GetHighestScoredLabel(ds, 1, 6, &label)) ==
        StatusToString(INVALID_INPUT));
  }
  SECTION("Checking whether label == NULL  was handled") {
    REQUIRE(StatusToString(GetHighestScoredLabel(ds, 1, 1, NULL)) ==
        StatusToString(INVALID_INPUT));
  }

  SECTION("Checking failure on getting label from non-existing image") {
    REQUIRE(StatusToString(GetHighestScoredLabel(ds, 2, 1, &label))
                == StatusToString(FAILURE));
  }
  SECTION(
      "Checking failure on getting label from image where the super-pixel isn't "
      "tagged at all") {
    REQUIRE(StatusToString(AddImage(ds, 2)) == StatusToString(SUCCESS));
    REQUIRE(StatusToString(GetHighestScoredLabel(ds, 2, 1, &label))
                == StatusToString(FAILURE));
  }
  SECTION("Checking successul getting of a label") {
    REQUIRE(StatusToString(AddImage(ds, 2)) == StatusToString(SUCCESS));
    REQUIRE(StatusToString(SetLabelScore(ds, 2, 2, 1, 2))
                == StatusToString(SUCCESS));
    REQUIRE(StatusToString(GetHighestScoredLabel(ds, 2, 2, &label))
                == StatusToString(SUCCESS));
    REQUIRE(label == 1);
  }
  SECTION("Checking failure on getting label=1 after resetting it") {
    REQUIRE(StatusToString(AddImage(ds, 3)) == StatusToString(SUCCESS));
    REQUIRE(StatusToString(SetLabelScore(ds, 3, 2, 1, 2))
                == StatusToString(SUCCESS));
    REQUIRE(StatusToString(ResetLabelScore(ds, 3, 2, 1))
                == StatusToString(SUCCESS));
    REQUIRE(StatusToString(GetHighestScoredLabel(ds, 3, 2, &label))
                == StatusToString(FAILURE));
  }
  SECTION(
      "Checking whether GetHighestScoredLable returns the label with higher ID when they have same score") {
    REQUIRE(StatusToString(AddImage(ds, 4)) == StatusToString(SUCCESS));
    REQUIRE(StatusToString(SetLabelScore(ds, 4, 1, 2, 5))
                == StatusToString(SUCCESS));
    REQUIRE(StatusToString(SetLabelScore(ds, 4, 1, 3, 5))
                == StatusToString(SUCCESS));
    REQUIRE(StatusToString(GetHighestScoredLabel(ds, 4, 1, &label))
                == StatusToString(SUCCESS));
    REQUIRE(label == 3);
  }
  Quit(&ds);
}
TEST_CASE("Testing MergeSuperPixels function:") {
  SECTION("Checking whether null DS ptr was handled") {
    REQUIRE(StatusToString(MergeSuperPixels(NULL, 1, 1, 2))
                == StatusToString(INVALID_INPUT));
  }
  auto ds = Init(5);
  SECTION("Checking whether imageID = 0 was handled") {
    REQUIRE(StatusToString(MergeSuperPixels(ds, 0, 1, 2))
                == StatusToString(INVALID_INPUT));
  }
  SECTION("Checking whether imageID < 0 was handled") {
    REQUIRE(StatusToString(MergeSuperPixels(ds, -1, 1, 2))
                == StatusToString(INVALID_INPUT));
  }
  AddImage(ds, 1);
  SECTION("Checking whether pixel1 < 0 was handled") {
    REQUIRE(StatusToString(MergeSuperPixels(ds, 1, -1, 2))
                == StatusToString(INVALID_INPUT));
  }
  SECTION("Checking whether pixel2 < 0 was handled") {
    REQUIRE(StatusToString(MergeSuperPixels(ds, 1, 1, -1))
                == StatusToString(INVALID_INPUT));
  }
  SECTION("Checking whether pixel1 >= pixels in DS was handled") {
    REQUIRE(StatusToString(MergeSuperPixels(ds, 1, 6, 1))
                == StatusToString(INVALID_INPUT));
  }
  SECTION("Checking whether pixel2 >= pixels in DS was handled") {
    REQUIRE(StatusToString(MergeSuperPixels(ds, 1, 1, 6))
                == StatusToString(INVALID_INPUT));
  }
  SECTION("Checking failure on merging in non-existing image") {
    REQUIRE(StatusToString(MergeSuperPixels(ds, 2, 1, 1))
                == StatusToString(FAILURE));
  }
  SECTION("Checking failure on merging when pixel1 = pixel2 image") {
    REQUIRE(StatusToString(MergeSuperPixels(ds, 1, 1, 1))
                == StatusToString(FAILURE));
  }
  SECTION(
      "Checking failure on merging when pixel1 and pixel 2 belongs same super-pixel") {
    REQUIRE(StatusToString(AddImage(ds, 2)) == StatusToString(SUCCESS));
    REQUIRE(StatusToString(MergeSuperPixels(ds, 2, 1, 2))
                == StatusToString(SUCCESS));
    REQUIRE(StatusToString(MergeSuperPixels(ds, 2, 1, 2))
                == StatusToString(FAILURE));
  }
  SECTION("Checking success on merging ") {
    int label = 0;
    REQUIRE(StatusToString(AddImage(ds, 3)) == StatusToString(SUCCESS));
    REQUIRE(StatusToString(SetLabelScore(ds, 3, 1, 1, 10))
                == StatusToString(SUCCESS));
    REQUIRE(StatusToString(SetLabelScore(ds, 3, 2, 2, 5))
                == StatusToString(SUCCESS));
    REQUIRE(StatusToString(GetHighestScoredLabel(ds, 3, 1, &label))
                == StatusToString(SUCCESS));
    REQUIRE(label == 1);
    REQUIRE(StatusToString(GetHighestScoredLabel(ds, 3, 2, &label))
                == StatusToString(SUCCESS));
    REQUIRE(label == 2);
    REQUIRE(StatusToString(MergeSuperPixels(ds, 3, 1, 2))
                == StatusToString(SUCCESS));
    //Now pixel 1 and 2 belong to same super-pixel thus should have label 1 as HighScore
    REQUIRE(StatusToString(GetHighestScoredLabel(ds, 3, 2, &label))
                == StatusToString(SUCCESS));
    REQUIRE(label == 1);
    REQUIRE(StatusToString(GetHighestScoredLabel(ds, 3, 1, &label))
                == StatusToString(SUCCESS));
    REQUIRE(label == 1);
    REQUIRE(StatusToString(ResetLabelScore(ds, 3, 1, 1))
                == StatusToString(SUCCESS));
    REQUIRE(StatusToString(GetHighestScoredLabel(ds, 3, 1, &label))
                == StatusToString(SUCCESS));
    REQUIRE(label == 2);
    REQUIRE(StatusToString(ResetLabelScore(ds, 3, 1, 2))
                == StatusToString(SUCCESS));
    //Now both label 1 and 2 in super-pixel {1,2} have a score of 0 so label 2 should count as High-score.
    REQUIRE(label == 2);
    REQUIRE(StatusToString(SetLabelScore(ds, 3, 3, 3, 5))
                == StatusToString(SUCCESS));
    REQUIRE(StatusToString(MergeSuperPixels(ds, 3, 1, 3))
                == StatusToString(SUCCESS));
    REQUIRE(StatusToString(MergeSuperPixels(ds, 3, 2, 3))
                == StatusToString(FAILURE));
    REQUIRE(StatusToString(GetHighestScoredLabel(ds, 3, 1, &label))
                == StatusToString(SUCCESS));
    REQUIRE(label == 3);
  }
  SECTION("Checking success addition of scores on pixels with same labels") {
    int label = 0;
    REQUIRE(StatusToString(AddImage(ds, 4)) == StatusToString(SUCCESS));
    REQUIRE(StatusToString(SetLabelScore(ds, 4, 1, 1, 10))
                == StatusToString(SUCCESS));
    REQUIRE(StatusToString(SetLabelScore(ds, 4, 1, 2, 14))
                == StatusToString(SUCCESS));
    REQUIRE(StatusToString(SetLabelScore(ds, 4, 2, 1, 5))
                == StatusToString(SUCCESS));
    REQUIRE(StatusToString(MergeSuperPixels(ds, 4, 1, 2))
                == StatusToString(SUCCESS));
    REQUIRE(StatusToString(GetHighestScoredLabel(ds, 4, 1, &label))
                == StatusToString(SUCCESS));
    REQUIRE(label == 1);
  }
  Quit(&ds);
}

#ifdef STRESS_TEST
TEST_CASE("Stress Addition deletion test"){
  int Images = 100000;
  int pixels = 10;
  auto ds = Init(pixels);
  for(int i = 1; i < Images; i++){
    AddImage(ds,i);
    for(int j = 1; j < pixels;j++){
      SetLabelScore(ds,i,j,j,j+1);
    }
    for(int j = 1; j < pixels-1;j+=2){
      int label=0;
      REQUIRE(StatusToString(GetHighestScoredLabel(ds, i, j, &label)) == StatusToString(SUCCESS));
      REQUIRE(label == j);
      MergeSuperPixels(ds,i,j,j+1);
      REQUIRE(StatusToString(GetHighestScoredLabel(ds, i, j, &label)) == StatusToString(SUCCESS));
      REQUIRE(label == j+1);
    }
  }
  for(int i = 0; i < 200000; i++){
    DeleteImage(ds,i);
  }
  Quit(&ds);
}
#endif

}  // namespace Tests