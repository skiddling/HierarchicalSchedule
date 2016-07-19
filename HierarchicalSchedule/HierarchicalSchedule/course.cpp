#include "course.h"
#include "prefix.h"
#include "classunit.h"

Course::Course() {
}

Course::Course(string course_name, int cnum, bool visited):
	course_name_(course_name), visited_(visited){
	pre_node_ = vector<bool>(cnum, 0);
}

//Course::Course(int course_id, string course_name):
//	course_id_(course_id), course_name_(course_name){
//}