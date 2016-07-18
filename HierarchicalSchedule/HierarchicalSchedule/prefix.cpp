#include "prefix.h"

Prefix::Prefix() {
}

Prefix::Prefix(int rows) {
	avl_time_ = vector<int>(rows, 1);
	used_time_ = vector<int>(rows, 0);
}

Prefix::Prefix(vector<Course> pre_pat) {
	pre_pat_ = pre_pat;
}