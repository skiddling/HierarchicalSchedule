#include "course.h"
#include "prefix.h"
#include "classunit.h"

Course::Course() {
}

Course::Course(string course_name, int class_num, int cnum, int stuppper, int stulower, set<int> notinset, bool visited):
	course_name_(course_name), class_num_(class_num), stu_upper_(stuppper), stu_lower_(stulower), not_in_set_(notinset), visited_(visited){
	pre_node_ = vector<bool>(cnum, 0);
	satis_num_ = 0;
	num_of_stus_in_sex_[male] = num_of_stus_in_sex_[female] = 0;
	total_scores_in_sex_[male] = total_scores_in_sex_[female] = 0.0;
	dva_female_num_ = 5;
	dva_male_num_ = 5;
}

//Course::Course(int course_id, string course_name):
//	course_id_(course_id), course_name_(course_name){
//}