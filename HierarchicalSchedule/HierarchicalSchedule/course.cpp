#include "course.h"
#include "prefix.h"
#include "classunit.h"

Course::Course() {
}

Course::Course(int course_id, string course_name):
	course_id_(course_id), course_name_(course_name){
}