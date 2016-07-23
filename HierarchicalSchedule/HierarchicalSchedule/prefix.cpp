#include "prefix.h"

Prefix::Prefix() {
}

Prefix::Prefix(int rows) {
	avl_time_ = vector<int>(rows, 1);
	avl_tab_[avl_time_] = 1;
}

Prefix::Prefix(vector<Course> pattern):pattern_(pattern) {
}