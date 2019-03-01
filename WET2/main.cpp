#define CATCH_CONFIG_RUNNER
#include <iostream>
#include "dict_avl.h"
#include "super_pixel.h"
#include "dict_avl.h"
#include <vector>
#include <map>
typedef int Key;
typedef int Value;
using std::cout;
using std::endl;

void TestCreateTreeFromInOrderList() {
	double max_size = 3000.0;
	for (int k = 1; k < max_size; k++) {

		DictAvl<Key,Value>::AvlNode* array_of_nodes = new DictAvl<Key,Value>::AvlNode[k];
		std::vector<Key> inserted_keys;
		std::vector<int> insertion_shuffle;
		for (int i = 0; i < k; i++) {
			int new_insertion = rand();
			insertion_shuffle.push_back(new_insertion);
		}
		for (int i = 0; i < k; i ++) {
			Key key = rand();
			if (std::find(inserted_keys.begin(), inserted_keys.end(), key) != inserted_keys.end()) {
				--i;
				continue;
			}
			inserted_keys.push_back(key);
			array_of_nodes[i].key = key;
			array_of_nodes[i].value = insertion_shuffle.back();
			insertion_shuffle.pop_back();
		}
		std::sort(array_of_nodes, array_of_nodes + k);
		DictAvl<Key,Value> tree = DictAvl<Key,Value>(DictAvl<Key,Value>::CreateAVLTreeFromInOrderNodesArray(array_of_nodes,k),k);
		delete[] array_of_nodes;
		if (ceil((k/max_size)*100) == (k/max_size)*100)
			std::cout<<"TestCreateTreeFromInOrderList Progress: "<<(k/max_size)*100<<"%"<<std::endl;
	}
	std::cout<<"TestCreateTreeFromInOrderList OK!"<<std::endl;
}
void TestInsertDeleteInAvlTree() {
	DictAvl<Key,Value> avl_dict;
	std::vector<Key> inserted_keys;
	std::vector<int> insertion_shuffle;
	double max_size = 3000.0;
	for (int i = 0; i < max_size; i++) {
		int new_insertion = rand();
		insertion_shuffle.push_back(new_insertion);
	}
	for (int i = 0; i < max_size; i ++) {
		Key key = rand();
		if (std::find(inserted_keys.begin(), inserted_keys.end(), key) != inserted_keys.end()) {
			--i;
			continue;
		}
		inserted_keys.push_back(key);
		avl_dict.InsertNode(key,insertion_shuffle.back());
		insertion_shuffle.pop_back();
		if (ceil((i/max_size)*100) == (i/max_size)*100)
			std::cout<<"Progress: "<<(i/max_size)*100<<"%"<<std::endl;
	}
	std::random_shuffle(inserted_keys.begin(),inserted_keys.end());
	for (int i = 0; i < max_size; i ++) {
		avl_dict.DeleteNodeByKey(inserted_keys[i]);
		if (ceil((i/max_size)*100) == (i/max_size)*100)
			std::cout<<"TestInsertDeleteInAvlTree Progress: "<<(i/max_size)*100<<"%"<<std::endl;
	}
	std::cout<<"TestInsertDeleteInAvlTree OK!"<<std::endl;
}
int RandomFunc() {
	return rand();
}
void TestMergeSuperPixels() {
	int num_of_labels = 100;
	double num_of_tests = 3000.0;
	for (int j = 1; j < num_of_tests; j++) {
		SuperPixel sp1;
		SuperPixel sp2;

		std::vector<int> sp1_labels;
		std::vector<int> sp1_scores;
		std::vector<int> sp2_labels;
		std::vector<int> sp2_scores;

		for (int i = 0; i < num_of_labels; i++) {
			int random_score = RandomFunc();
			sp1_scores.push_back(random_score);
			random_score = RandomFunc();
			sp2_scores.push_back(random_score);
		}

		for (int i = 0; i < num_of_labels; i++) {
			int random_label = RandomFunc();
			if (std::find(sp1_labels.begin(), sp1_labels.end(), random_label) != sp1_labels.end()) {
				--i;
				continue;
			}
			sp1_labels.push_back(random_label);
			sp1.SetLabelScore(random_label, sp1_scores.back());
			sp1_scores.pop_back();

		}
		for (int i = 0; i < num_of_labels; i++) {
			int random_label = RandomFunc();
			if (std::find(sp2_labels.begin(), sp2_labels.end(), random_label) != sp2_labels.end()) {
				--i;
				continue;
			}
			sp2_labels.push_back(random_label);
			sp2.SetLabelScore(random_label, sp2_scores.back());
			sp2_scores.pop_back();
		}

		SuperPixel *sp_merged = SuperPixel::MergeSuperPixels(sp1, sp2);
		delete sp_merged;

		SuperPixel sp3;
		SuperPixel sp4;
		std::vector<int> random_scores;
		for (int i = 0; i < num_of_labels; i++) {
			int random_score = RandomFunc();
			random_scores.push_back(random_score);
		}

		for (int i = 0; i < num_of_labels; i++) {
			sp3.SetLabelScore(sp2_labels[i],random_scores[i]);
			sp4.SetLabelScore(sp2_labels[i],random_scores[i]);
		}

		sp_merged = SuperPixel::MergeSuperPixels(sp3, sp4);

		for (int i = 0; i < num_of_labels; i++) {
			assert(sp_merged->GetScoreOfLabel(sp2_labels[i]) == 2 * random_scores[i]);
		}

		delete sp_merged;
		if (ceil((j/num_of_tests)*100) == (j/num_of_tests)*100)
			std::cout<<"TestMergeSuperPixels Progress: "<<(j/num_of_tests)*100<<"%"<<std::endl;
	}
	std::cout<<"TestMergeSuperPixels OK!"<<std::endl;

}
void TestGetHighestScoreLabel() {
	int num_of_labels = 100;
	double num_of_tests = 3000.0;
	for (int j = 1; j < num_of_tests; j++) {
		SuperPixel sp1;

		std::vector<int> sp1_labels;
		std::vector<int> sp1_scores;

		for (int i = 0; i < num_of_labels; i++) {
			int random_score = RandomFunc();
			sp1_scores.push_back(random_score);
		}

		int last_max_score = -1;
		for (int i = 0; i < num_of_labels; i++) {
			int random_label = RandomFunc();
			if (std::find(sp1_labels.begin(), sp1_labels.end(), random_label) != sp1_labels.end()) {
				--i;
				continue;
			}
			sp1_labels.push_back(random_label);
			sp1.SetLabelScore(random_label, sp1_scores.back());

			if (sp1_scores.back() >= last_max_score) {
				last_max_score = sp1_scores.back();
			}

			sp1_scores.pop_back();
		}
		assert(sp1.GetHighestScoredLabel() == label_of_max_score);
		if (ceil((j/num_of_tests)*100) == (j/num_of_tests)*100)
			std::cout<<"TestGetHighestScoreLabel Progress: "<<(j/num_of_tests)*100<<"%"<<std::endl;
	}
	std::cout<<"TestGetHighestScoreLabel OK!"<<std::endl;
}
void TestSuperPixel() {
	TestMergeSuperPixels();
	TestGetHighestScoreLabel();
}
int main(int argc, char* argv[]) {
	srand(time(NULL));
	std::cout<<"start!"<<std::endl;
	TestSuperPixel();
	TestCreateTreeFromInOrderList();
	TestInsertDeleteInAvlTree();
	std::cout<<"OK!"<<std::endl;
  return 0;
}