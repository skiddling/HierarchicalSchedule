#include "prefix.h"

Prefix::Prefix() {
}

Prefix::Prefix(int rows) {
	avl_time_ = vector<int>(rows, 1);
	avl_tab_[avl_time_] = 1;
	satisfied_ = 0;
}

Prefix::Prefix(vector<Course> pattern, int pre_id, int rows):pattern_(pattern), pre_id_(pre_id) {
	avl_time_ = vector<int>(rows, 0);
	satisfied_ = 0;
}