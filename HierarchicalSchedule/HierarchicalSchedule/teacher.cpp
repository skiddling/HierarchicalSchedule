#include "teacher.h"

Teacher::Teacher(int teacher_id, string teacher_name, vector<Course> courses, map<Course, vector<int> > course_table) :
	teacher_id_(teacher_id), teacher_name_(teacher_name), courses_(courses), course_table_(course_table) {
}